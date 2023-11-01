#include "../../sb.h"
#define CSON_IMPLEMENTATION
#define NN_IMPLEMENTATION
#define COMMON_IMPLEMENTATION
#include "../../zflclient/src/common.h"
#include "http.h"
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
#include <unistd.h>

#define PORT 8080
#define PEER_PORT 8080

pthread_mutex_t accum_mutex = PTHREAD_MUTEX_INITIALIZER;
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
Round CURRENT_ROUND = {0};

static int sendall(int sock, const char *buf, size_t len) {
    while (len) {
        ssize_t out_len = send(sock, buf, len, 0);

        if (out_len < 0) {
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
    pthread_mutex_lock(&accum_mutex);
    int id = ++CLIENTS.n;
    pthread_mutex_unlock(&accum_mutex);
    printf("Current ID is %d\n", id);
    char id_str[20];
    snprintf(id_str, sizeof(id_str), "%d", id);
    char res[256];
    int len =
        snprintf(res, sizeof(res), "200 OK \r\nContent-Length: %zu\r\n\r\n%s",
                 strlen(id_str), id_str);
    int sent = send(client_socket, res, len, 0);
    CLIENTS.client = realloc(CLIENTS.client, sizeof(Client) * CLIENTS.n);
    CLIENTS.client[id - 1] = (Client){.id = id, .ipaddr = addr, .ready = false};
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

int send_weights(int client_socket) {
    const char *initial_weights_file = "./initial_weights.json";
    return send_file(client_socket, initial_weights_file);
}

void *handle_results(int id, char *results, size_t len) {
    for (int i = 0; i < CLIENTS.n; ++i) {
        Client *c = &CLIENTS.client[i];
        if (c->id == id) {
            pthread_mutex_lock(&result_mutex);

            parse_weights_json(results, len, CURRENT_ROUND.weights,
                               CURRENT_ROUND.biases,
                               CURRENT_ROUND.responded == 0);

            ++CURRENT_ROUND.responded;

            // TODO: allow certain fraction of clients to dropoff
            if (CURRENT_ROUND.responded == CURRENT_ROUND.participants) {
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
            }

            init_nn(&CURRENT_ROUND.nn, CURRENT_ROUND.weights,
                    CURRENT_ROUND.biases);

            pthread_mutex_unlock(&result_mutex);
            printf("INFO: final model accuracy is %f\n",
                   accuracy(CURRENT_ROUND.nn, CURRENT_ROUND.test_set));

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
    char buf[256];
    int len = recv(info->sock, buf, sizeof(buf), 0);
    if (len <= 0) {
        goto clean;
    }
    printf("INFO: Received %d bytes\n", len);
    Http h = {0};
    int ret = parse_header(&h, buf, len);
    parse_query_param(&h);
    printf("INFO: HTTP request\nMethod: %s, url: %s\n", h.method, h.url);
    for (int i = 0; i < h.n_query; ++i) {
        printf("Query %d. Key: %s, Val: %s\n", i, h.query_keys[i],
               h.query_vals[i]);
    }
    for (int i = 0; i < h.n_headers; ++i) {
        printf("header %d. Key: %s, Val: %s\n", i, h.header_keys[i],
               h.header_vals[i]);
    }
    // return NULL;

    if (ret == -1) {
        printf("ERROR: invalid HTTP request\n");
        return NULL;
    }
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
        // TODO: FedAvg
        if (strncmp(h.url, RESULTS_ENDPOINT, strlen(RESULTS_ENDPOINT))) {
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
            char *content = recv_size(info->sock, content_len);
            if (content != NULL) {
                // TODO: create new thread
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
            if (strncmp(buf, "true", strlen("true")) == 0) {
                c->ready = true;
                ++ready;
            } else {
                c->ready = false;
            }
        clean:
            close(sock);
        }

        if (ready == 1) {
            ++CURRENT_ROUND.round_number;
            for (int i = 0; i < CLIENTS.n; ++i) {
                Client c = CLIENTS.client[i];
                if (c.ready) {
                    ++CURRENT_ROUND.participants;
                    int sock = connect_client(c.ipaddr);
                    char req[256];
                    snprintf(req, sizeof(req), "%s %s \r\n\r\n", POST,
                             TRAIN_ENDPOINT);
                    sendall(sock, req, strlen(req));
                    send_weights(sock);
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
    init_train_set(&CURRENT_ROUND.test_set, images.data, labels.data, 10000);
    printf("INFO: test set loaded!\n");

    CURRENT_ROUND.weights = malloc(sizeof(float *) * (ARCH_COUNT - 1));
    assert(CURRENT_ROUND.weights);
    CURRENT_ROUND.biases = malloc(sizeof(float *) * (ARCH_COUNT - 1));
    assert(CURRENT_ROUND.biases);

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
        char *addr_str = malloc(INET_ADDRSTRLEN);
        inet_ntop(client_addr.sin_family, &client_addr.sin_addr, addr_str,
                  sizeof(addr_str));
        printf("Client connected from %s\n", addr_str);

        ConnectionInfo *con = malloc(sizeof(ConnectionInfo));
        con->sock = client_socket;
        con->addr = addr_str;
        pthread_t th;
        pthread_create(&th, NULL, handle_incoming_connection, con);
    }
}
