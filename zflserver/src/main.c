#define SERVER
#define SB_IMPLEMENTATION
#define CSON_IMPLEMENTATION
#define NN_IMPLEMENTATION
#define COMMON_IMPLEMENTATION
#include "../../common.h"
#include "../../http.h"
#include "../../zflclient/src/train.h"
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

#define PORT 8080
#define PEER_PORT 8080
#define READ_TIMEOUT 5
#define CLIENTS_PER_ROUND 2

pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t result_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t shutdown_mutex = PTHREAD_MUTEX_INITIALIZER;
bool should_shutdown = false;

typedef struct {
    int sock;
    char *addr;
} ConnectionInfo;

typedef struct {
    int id;
    char *ipaddr;
    bool ready;
} Client;

typedef struct {
    int n;
    Client *client;
} Clients;

typedef struct {
    int client_id;
    char *bytes;
    size_t size;
} Result;

typedef struct {
    size_t round_number;
    size_t participants;
    size_t responded;
    float **weights;
    float **biases;
    Mat test_set;
    NN nn;
} Round;

static Clients CLIENTS = {0};
static Round CURRENT_ROUND = {0};

static int sendall(int sock, const char *buf, size_t len) {
    while (len) {
        ssize_t out_len = send(sock, buf, len, 0);

        if (out_len < 0) {
            printf("ERROR: failed to send because: %s\n", strerror(errno));
            return out_len;
        }
        buf = (const char *)buf + out_len;
        len -= out_len;
    }

    return 0;
}

int connect_client(char *addr) {
    int client;
    int ret;
    struct sockaddr_in addr4;
    addr4.sin_family = AF_INET;
    addr4.sin_port = htons(PEER_PORT);
    printf("Attempting to connect to %s %d\n", addr, PEER_PORT);
    if (inet_pton(AF_INET, addr, &addr4.sin_addr) <= 0) {
        printf("ERROR: invalid ip address %s\n", addr);
        return -1;
    }
    client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client == -1) {
        printf("ERROR: could not create server socket because %s\n",
               strerror(errno));
        return -1;
    }
    ret = connect(client, (struct sockaddr *)&addr4, sizeof(addr4));
    if (ret < 0) {
        printf("ERROR: could not connect to socket: %s\n", strerror(errno));
        return ret;
    }
    printf("INFO: Successfully connected to client\n");
    return client;
}

int send_file(int client_socket, const char *file_path) {
    size_t total_sent = 0;
    char buf[1024];
    FILE *fd = fopen(file_path, "r");
    if (fd == NULL) {
        printf("ERROR: could not open file %s because: %s", file_path,
               strerror(errno));
        return -1;
    }
    int ret = fread(buf, sizeof(*buf), sizeof(buf), fd);
    while (ret > 0) {
        const char *data = buf;
        while (ret) {
            int sent = send(client_socket, data, ret, 0);
            if (sent == -1) {
                printf("Error sending data to peer, errno: %s\n",
                       strerror(errno));
                return -1;
            }
            ret -= sent;
            data += sent;
            total_sent += sent;
        }
        ret = fread(buf, sizeof(*buf), sizeof(buf), fd);
    }
    if (total_sent > 0) {
        printf("Successfully sent %zu bytes of data to client\n", total_sent);
    }
    fclose(fd);
    return total_sent;
}

int send_id(int client_socket, char *addr) {
    pthread_mutex_lock(&client_mutex);
    int id = ++CLIENTS.n;
    printf("ID of client %s is %d\n", addr, id);
    char id_str[20];
    snprintf(id_str, sizeof(id_str), "%d", id);
    char res[256];
    int len = snprintf(res, sizeof(res), "%s", id_str);
    CLIENTS.client = realloc(CLIENTS.client, sizeof(Client) * CLIENTS.n);
    CLIENTS.client[id - 1].id = id;
    CLIENTS.client[id - 1].ipaddr = strdup(addr);
    CLIENTS.client[id - 1].ready = false;
    pthread_mutex_unlock(&client_mutex);

    int sent = send(client_socket, res, len, 0);
    return sent;
}

int send_training_labels(int client_socket, int id) {
    char labels[100];
    snprintf(labels, sizeof(labels), "../data/train-labels-%d", id);
    return send_file(client_socket, labels);
}

int send_training_images(int client_socket, int id) {
    char images[100];
    snprintf(images, sizeof(images), "../data/train-images-%d", id);
    return send_file(client_socket, images);
}

void *handle_results(int id, char *results, size_t len) {
    printf("INFO: Client %d responded with results size %zu\n", id, len);
    for (int i = 0; i < CLIENTS.n; ++i) {
        Client *c = &CLIENTS.client[i];
        if (c->id == id) {
            pthread_mutex_lock(&result_mutex);

            parse_weights_json(results, len, CURRENT_ROUND.weights,
                               CURRENT_ROUND.biases,
                               CURRENT_ROUND.responded != 0);

            ++CURRENT_ROUND.responded;
            bool run_averaging =
                CURRENT_ROUND.responded == CURRENT_ROUND.participants;

            pthread_mutex_unlock(&result_mutex);

            // TODO: allow certain fraction of clients to dropoff
            if (run_averaging) {
                printf("INFO: all clients responded, starting "
                       "FedAvg\n");
                for (size_t i = 0; i < ARCH_COUNT - 1; ++i) {
                    for (size_t j = 0; j < ARCH[i]; ++j) {
                        for (size_t k = 0; k < ARCH[i + 1]; ++k) {
                            CURRENT_ROUND.weights[i][j * ARCH[i + 1] + k] /=
                                CURRENT_ROUND.participants;
                        }
                    }
                    for (size_t j = 0; j < ARCH[i + 1]; ++j) {
                        CURRENT_ROUND.biases[i][j] /=
                            CURRENT_ROUND.participants;
                    }
                }
                init_nn(&CURRENT_ROUND.nn, CURRENT_ROUND.weights,
                        CURRENT_ROUND.biases);
                printf("INFO: final model accuracy against test set is %f\n",
                       accuracy(CURRENT_ROUND.nn, CURRENT_ROUND.test_set));
            }
            break;
        }
    }
    return NULL;
}

int extract_id(Http *h) {
    char *id_str = get_param(h, "id");
    if (id_str == NULL) {
        printf("ERROR: id %s is invalid\n", id_str);
        return -1;
    }
    char *endptr;
    errno = 0;
    int id = strtol(id_str, &endptr, 10);
    if (errno != 0) {
        printf("ERROR: id %d could not be converted to int\n", id);
        return -1;
    }
    if (id < 1 || id > CLIENTS.n) {
        printf("ERROR: id %d is invalid, there only %d clients\n", id,
               CLIENTS.n);
        return -1;
    }
    return id;
}

void *handle_incoming_connection(void *con) {
    ConnectionInfo *info = (ConnectionInfo *)con;
    Http h = {0};
    StringBuilder sb = {0};
    sb_init(&sb, 1024);

    int required_len = recv_required_req(&h, &sb, info->sock);
    if (required_len < 0)
        goto clean;
    int optional_len = recv_optional_headers(&h, &sb, info->sock, required_len);
    if (optional_len < 0)
        goto clean;

    parse_query_param(&h);
    printf("INFO: HTTP request\nMethod: %s, url: %s\n", h.method, h.url);
    for (size_t i = 0; i < h.n_query; ++i) {
        printf("Query %zu. Key: %s, Val: %s\n", i, h.query_keys[i],
               h.query_vals[i]);
    }
    for (size_t i = 0; i < h.n_headers; ++i) {
        printf("header %zu. Key: %s, Val: %s\n", i, h.header_keys[i],
               h.header_vals[i]);
    }
    // return NULL;
    printf("INFO: %s request for: %s\n", h.method, h.url);
    if (strncmp(h.method, GET, strlen(GET)) == 0) {
        if (strncmp(h.url, ID_ENDPOINT, strlen(ID_ENDPOINT)) == 0) {
            send_id(info->sock, info->addr);
        } else if (strncmp(h.url, TRAINING_IMAGES_ENDPOINT,
                           strlen(TRAINING_IMAGES_ENDPOINT)) == 0) {
            int id = extract_id(&h);
            if (id == -1) {
                goto clean;
            }
            send_training_images(info->sock, id);
        } else if (strncmp(h.url, TRAINING_LABELS_ENDPOINT,
                           strlen(TRAINING_LABELS_ENDPOINT)) == 0) {
            int id = extract_id(&h);
            if (id == -1) {
                goto clean;
            }
            send_training_labels(info->sock, id);
        } else {
            printf("INFO: 404 not found: %s\n", h.url);
        }
    } else if (strncmp(h.method, POST, strlen(POST)) == 0) {
        if (strncmp(h.url, RESULTS_ENDPOINT, strlen(RESULTS_ENDPOINT)) == 0) {
            int id = extract_id(&h);
            if (id == -1) {
                goto clean;
            }

            char *content_len_str = get_header(&h, CONTENT_LENGTH);
            if (content_len_str == NULL) {
                printf("ERROR: No results sent to %s\n", RESULTS_ENDPOINT);
                goto clean;
            }
            char *endptr;
            size_t content_len = strtol(content_len_str, &endptr, 10);

            size_t offset = required_len + optional_len;
            int ret = recv_content(&sb, info->sock, offset, content_len);
            if (ret == -1)
                goto clean;
            char *content = substr(sb.data, offset, content_len);
            if (content != NULL) {
                handle_results(id, content, content_len);
            }
        } else if (strncmp(h.url, SHUTDOWN_ENDPOINT,
                           strlen(SHUTDOWN_ENDPOINT)) == 0) {
            pthread_mutex_lock(&shutdown_mutex);
            should_shutdown = true;
            pthread_mutex_unlock(&shutdown_mutex);
        } else {
            printf("INFO: 404 resource not found: %s\n", h.url);
        }
    } else {
        printf("INFO: 405 method not supported: %s\n", h.method);
    }
clean:
    close(info->sock);
    free(con);
    return 0;
}

void *start_round() {
    int delay = 10;
    while (1) {
        printf("INFO: waiting for %d seconds before starting...\n", delay);
        sleep(delay);
        printf("INFO: checking client ready status\n");
        size_t ready = 0;
        for (int i = 0; i < CLIENTS.n; ++i) {
            Client *c = &CLIENTS.client[i];
            int sock = connect_client(c->ipaddr);
            char req[256];
            snprintf(req, sizeof(req), "%s %s \r\n\r\n", GET, READY_ENDPOINT);
            sendall(sock, req, strlen(req));
            char buf[10];
            if (recv(sock, buf, sizeof(buf), 0) <= 0) {
                printf("ERROR: no ready payload received from %s\n", c->ipaddr);
                c->ready = false;
                goto clean;
            }
            printf("INFO: Client %s ready: %s\n", c->ipaddr, buf);
            if (strncmp(buf, "true", strlen("true")) == 0) {
                c->ready = true;
                ++ready;
            } else {
                c->ready = false;
            }
        clean:
            close(sock);
        }

        if (ready == CLIENTS_PER_ROUND) {
            ++CURRENT_ROUND.round_number;
            printf("INFO: %zu clients are ready, starting round %zu\n", ready,
                   CURRENT_ROUND.round_number);
            for (int i = 0; i < CLIENTS.n; ++i) {
                Client c = CLIENTS.client[i];
                if (c.ready) {
                    ++CURRENT_ROUND.participants;
                    int sock = connect_client(c.ipaddr);
                    char req[256];
                    StringBuilder sb;
                    sb_init(&sb, 1024);
                    size_t weights_len =
                        weights_to_string(&sb, &CURRENT_ROUND.nn);
                    int len =
                        snprintf(req, sizeof(req),
                                 "POST /train \r\nContent-Length: %zu\r\n\r\n",
                                 weights_len);
                    printf("INFO: sending req %s\n", req);
                    sendall(sock, req, len);
                    sendall(sock, sb.data, weights_len);
                    sb_free(&sb);
                    close(sock);
                }
            }
            // TODO: sigtimedwait
            break;
        }
    }
    return NULL;
}

int main(void) {
    printf("Greetings from server!\n");
    CLIENTS.n = 0;
    CLIENTS.client = NULL;

    StringBuilder images = {0};
    sb_init(&images, 1024);
    printf("Opening test image!\n");
    FILE *test_image = fopen("../data/test-images", "r");
    assert(test_image);
    char buf[1024];
    while (!feof(test_image)) {
        int ret = fread(buf, sizeof(*buf), sizeof(buf), test_image);
        sb_append(&images, buf, ret);
        if (ferror(test_image) != 0) {
            printf("Error reading file %s\n", strerror(errno));
        }
    }
    StringBuilder labels = {0};
    sb_init(&labels, 1024);
    printf("Opening test label!\n");
    FILE *test_labels = fopen("../data/test-labels", "r");
    assert(test_labels);
    while (!feof(test_labels)) {
        int ret = fread(buf, sizeof(*buf), sizeof(buf), test_labels);
        sb_append(&labels, buf, ret);
        if (ferror(test_labels) != 0) {
            printf("Error reading file %s\n", strerror(errno));
        }
    }
    printf("Image: %zu, Labels: %zu\n", images.size, labels.size);
    assert(images.size / IMG_SIZE == labels.size);

    printf("INFO: init test set!\n");
    CURRENT_ROUND.test_set = init_train_set(images.data, labels.data, 10000);
    printf("INFO: test set loaded!\n");

    CURRENT_ROUND.weights = malloc(sizeof(float *) * (ARCH_COUNT - 1));
    assert(CURRENT_ROUND.weights);
    CURRENT_ROUND.biases = malloc(sizeof(float *) * (ARCH_COUNT - 1));
    assert(CURRENT_ROUND.biases);

    // StringBuilder initial_model;
    // sb_init(&initial_model, 1024);
    // FILE *init = fopen("./initial_weights.json", "r");
    // assert(init);
    // while (!feof(init)) {
    //     int ret = fread(buf, sizeof(*buf), sizeof(buf), init);
    //     sb_append(&initial_model, buf, ret);
    //     if (ferror(init) != 0) {
    //         printf("Error reading file %s\n", strerror(errno));
    //     }
    // }
    //
    // parse_weights_json(initial_model.data, initial_model.size,
    //                    CURRENT_ROUND.weights, CURRENT_ROUND.biases, false);
    // init_nn(&CURRENT_ROUND.nn, CURRENT_ROUND.weights, CURRENT_ROUND.biases);
    init_nn(&CURRENT_ROUND.nn, NULL, NULL);

    int sock;
    struct sockaddr_in name;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("ERROR: could not create socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    const int enable = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        printf("ERROR: could not set socket opt: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) {
        printf("ERROR: could not set socket opt: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Give the socket a name. */
    name.sin_family = AF_INET;
    name.sin_port = htons(PORT);
    inet_pton(AF_INET, "192.0.2.2", &name.sin_addr);
    if (bind(sock, (struct sockaddr *)&name, sizeof(name)) < 0) {
        printf("ERROR: could not bind socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (listen(sock, 100) == -1) {
        printf("ERROR: could not listen on socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("HTTP server waits for a connection on "
           "port %d...\n",
           PORT);

    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t client_size = sizeof(client_addr);

    pthread_t th;
    pthread_create(&th, NULL, start_round, NULL);

    while (1) {
        pthread_mutex_lock(&shutdown_mutex);
        if (should_shutdown) {
            printf("INFO: shutting down\n");
            return 0;
        }
        pthread_mutex_unlock(&shutdown_mutex);

        client_socket =
            accept(sock, (struct sockaddr *)&client_addr, &client_size);
        if (client_socket == -1) {
            printf("ERROR: accept because %s\n", strerror(errno));
        }

        struct timeval tv;
        tv.tv_sec = READ_TIMEOUT;
        tv.tv_usec = 0;
        setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv,
                   sizeof(tv));

        ConnectionInfo *con = malloc(sizeof(ConnectionInfo));
        con->sock = client_socket;
        con->addr = inet_ntoa(client_addr.sin_addr);
        printf("INFO: Client connected from %s\n", con->addr);
        pthread_t th;
        pthread_create(&th, NULL, handle_incoming_connection, con);
    }
}
