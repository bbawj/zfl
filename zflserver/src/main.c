#include "raylib.h"
#define IS_SERVER
#define SB_IMPLEMENTATION
#define CSON_IMPLEMENTATION
#define NN_IMPLEMENTATION
#define COMMON_IMPLEMENTATION
#include "../../common.h"
#include "../../da.h"
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

pthread_mutex_t bytes_transferred_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t round_mutex = PTHREAD_MUTEX_INITIALIZER;
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
    bool started;
    bool averaging;
    size_t round_number;
    size_t participants;
    size_t responded;
    float **weights;
    float **biases;
    Mat test_set;
    NN nn;
} Round;

#define LOG_LENGTH_MAX 100

typedef struct {
    size_t num_rounds;
    size_t clients_per_round;
    size_t bytes_transferred;
    size_t total_training_time;
    float *accuracies;
    float *loss;
    char logs[LOG_LENGTH_MAX][1024];
    size_t log_len;
    size_t clients_ready;

    Client *clients;
    Round current_round;
} Server;

static Server SERVER = {0};

void increment_bytes_transferred(size_t bytes) {
    pthread_mutex_lock(&bytes_transferred_mutex);
    SERVER.bytes_transferred += bytes;
    pthread_mutex_unlock(&bytes_transferred_mutex);
}

#define CURRENT_LOG_LEVEL 1

typedef enum { DEBUG = 0, INFO, ERROR } LOG_LEVEL;

void log_append(LOG_LEVEL level, char *msg, ...) {
    if (CURRENT_LOG_LEVEL > level)
        return;
    char buf[1024];
    va_list args;
    va_start(args, msg);
    int rc = vsnprintf(buf, sizeof(buf), msg, args);
    if (rc < 0) {
        printf("ERROR: failed to append to log because %s\n", strerror(errno));
    }
    va_end(args);
    printf("%s", buf);
    if (SERVER.log_len == LOG_LENGTH_MAX) {
        memmove(&SERVER.logs, &SERVER.logs[1],
                sizeof(SERVER.logs[0]) * (LOG_LENGTH_MAX - 1));
        SERVER.log_len--;
    }
    snprintf(SERVER.logs[SERVER.log_len++], sizeof(SERVER.logs[0]), "%s", buf);
}

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
    printf("INFO: Attempting to connect to %s %d\n", addr, PEER_PORT);
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
        printf("INFO: Successfully sent %zu bytes of data to client\n",
               total_sent);
    }
    fclose(fd);
    return total_sent;
}

int send_id(int client_socket, char *addr) {
    pthread_mutex_lock(&client_mutex);
    int id = da_len(SERVER.clients) + 1;
    printf("INFO: ID of client %s is %d\n", addr, id);
    char id_str[20];
    snprintf(id_str, sizeof(id_str), "%d", id);
    char res[256];
    int len = snprintf(res, sizeof(res), "%s", id_str);
    Client c = (Client){.id = id, .ipaddr = strdup(addr), .ready = false};
    da_put(SERVER.clients, c);
    pthread_mutex_unlock(&client_mutex);

    int sent = send(client_socket, res, len, 0);
    increment_bytes_transferred(sent);
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
    Payload p = {0};
    if (deserialize_training_data(results, len, &p) == -1) {
        log_append(ERROR, "invalid json response for results\n");
        goto clean;
    }
    pthread_mutex_lock(&round_mutex);
    if (p.round_number != SERVER.current_round.round_number) {
        pthread_mutex_unlock(&round_mutex);
        log_append(DEBUG, "Client %d responded for round %zu, skipping...\n",
                   id, p.round_number);
        goto clean;
    }
    pthread_mutex_unlock(&round_mutex);

    pthread_mutex_lock(&result_mutex);
    if (SERVER.current_round.averaging) {
        pthread_mutex_unlock(&result_mutex);
        log_append(DEBUG,
                   "INFO: Client %d responded for round %zu but server already "
                   "averaging\n",
                   id, p.round_number);
        goto clean;
    }

    SERVER.total_training_time += p.training_time;

    parse_weights_json(p.weights, SERVER.current_round.weights,
                       SERVER.current_round.biases,
                       SERVER.current_round.responded != 0);

    ++SERVER.current_round.responded;
    // TODO: allow certain fraction of clients to dropoff
    SERVER.current_round.averaging =
        SERVER.current_round.responded == SERVER.current_round.participants;
    pthread_mutex_unlock(&result_mutex);

    if (SERVER.current_round.averaging) {
        log_append(INFO,
                   "INFO: round %zu all clients responded, starting "
                   "FedAvg\n",
                   SERVER.current_round.round_number);
        for (size_t i = 0; i < ARCH_COUNT - 1; ++i) {
            for (size_t j = 0; j < ARCH[i]; ++j) {
                for (size_t k = 0; k < ARCH[i + 1]; ++k) {
                    SERVER.current_round.weights[i][j * ARCH[i + 1] + k] /=
                        SERVER.current_round.participants;
                }
            }
            for (size_t j = 0; j < ARCH[i + 1]; ++j) {
                SERVER.current_round.biases[i][j] /=
                    SERVER.current_round.participants;
            }
        }
        init_nn(&SERVER.current_round.nn, SERVER.current_round.weights,
                SERVER.current_round.biases);
        float acc =
            accuracy(SERVER.current_round.nn, SERVER.current_round.test_set);
        float loss = nn_cross_entropy(SERVER.current_round.nn,
                                      SERVER.current_round.test_set);
        log_append(
            INFO,
            "INFO: Final Model. Accuracy against test set: %f, Loss: %f\n", acc,
            loss);
        da_put(SERVER.accuracies, acc);
        da_put(SERVER.loss, loss);

        pthread_mutex_lock(&round_mutex);
        SERVER.current_round.started = false;
        SERVER.current_round.averaging = false;
        pthread_mutex_unlock(&round_mutex);
    }
clean:
    free_tokens(p.json);
    free(results);
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
    size_t id = strtol(id_str, &endptr, 10);
    if (errno != 0) {
        printf("ERROR: id %zu could not be converted to size_t\n", id);
        return -1;
    }
    if (id < 1 || id > da_len(SERVER.clients)) {
        printf("ERROR: id %zu is invalid, there only %zu clients\n", id,
               da_len(SERVER.clients));
        return -1;
    }
    return id;
}

void *handle_incoming_connection(void *con) {
    ConnectionInfo *info = (ConnectionInfo *)con;
    Http h = {0};
    StringBuilder sb = {0};
    sb_init(&sb, 1024);
    size_t total_recv = 0;

    int required_len = recv_required_req(&h, &sb, info->sock);
    if (required_len < 0)
        goto clean;
    total_recv += required_len;
    int optional_len = recv_optional_headers(&h, &sb, info->sock, required_len);
    if (optional_len < 0)
        goto clean;
    total_recv += optional_len;

    parse_query_param(&h);
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

            total_recv += ret;

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
    increment_bytes_transferred(total_recv);
    close(info->sock);
    free(con);
    return 0;
}

void *start_round() {
    int delay = 10;
    while (1) {
        if (SERVER.current_round.round_number == SERVER.num_rounds) {
            printf("INFO: all %zu rounds completed. Final accuracies:\n",
                   SERVER.current_round.round_number);
            for (size_t i = 0; i < da_len(SERVER.accuracies); ++i) {
                printf("Round %lu. Accuracy: %f. Loss: %f\n", i + 1,
                       SERVER.accuracies[i], SERVER.loss[i]);
            }
            break;
        }
        log_append(INFO, "INFO: waiting for %d seconds before starting...\n",
                   delay);
        sleep(delay);

        pthread_mutex_lock(&round_mutex);
        if (SERVER.current_round.started) {
            log_append(DEBUG,
                       "INFO: round %zu already started nothing to do...\n",
                       SERVER.current_round.round_number);
            pthread_mutex_unlock(&round_mutex);
            continue;
        }
        pthread_mutex_unlock(&round_mutex);

        log_append(DEBUG, "INFO: checking client ready status\n");
        SERVER.clients_ready = 0;
        for (size_t i = 0; i < da_len(SERVER.clients); ++i) {
            Client *c = &SERVER.clients[i];
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
                ++SERVER.clients_ready;
            } else {
                c->ready = false;
            }
        clean:
            close(sock);
        }

        if (SERVER.clients_ready == SERVER.clients_per_round) {
            pthread_mutex_lock(&round_mutex);
            SERVER.current_round.started = true;
            pthread_mutex_unlock(&round_mutex);

            ++SERVER.current_round.round_number;
            log_append(INFO,
                       "INFO: %zu clients are ready, starting round %zu\n",
                       SERVER.clients_ready, SERVER.current_round.round_number);
            for (size_t i = 0; i < da_len(SERVER.clients); ++i) {
                Client c = SERVER.clients[i];
                if (c.ready) {
                    ++SERVER.current_round.participants;
                    int sock = connect_client(c.ipaddr);
                    char req[256];
                    StringBuilder sb;
                    sb_init(&sb, 1024);
                    sb_appendf(&sb, "{\"round\": %zu, \"weights\": ",
                               SERVER.current_round.round_number);
                    weights_to_string(&sb, &SERVER.current_round.nn);
                    sb_append(&sb, "}", 1);
                    int len = snprintf(
                        req, sizeof(req),
                        "POST /train \r\nContent-Length: %zu\r\n\r\n", sb.size);
                    printf("INFO: sending POST /train request\n");
                    sendall(sock, req, len);
                    sendall(sock, sb.data, sb.size);

                    increment_bytes_transferred(sb.size + len);
                    sb_free(&sb);
                    close(sock);
                }
            }
        }
    }
    return NULL;
}

void *accept_conns(void *fd) {
    int sock = *(int *)fd;
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t client_size = sizeof(client_addr);

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

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage:\n ./server [num_rounds] [clients_per_round]>\n");
        return -1;
    }
    SERVER.num_rounds = atoi(argv[1]);
    SERVER.clients_per_round = atoi(argv[2]);
    SERVER.accuracies = NULL;
    SERVER.clients = NULL;
    SERVER.log_len = 0;

    printf("INFO: Greetings from server!\n");

    StringBuilder images = {0};
    sb_init(&images, 1024);
    printf("INFO: opening test image!\n");
    FILE *test_image = fopen("../data/test-images", "r");
    assert(test_image);
    char buf[1024];
    while (!feof(test_image)) {
        int ret = fread(buf, sizeof(*buf), sizeof(buf), test_image);
        sb_append(&images, buf, ret);
        if (ferror(test_image) != 0) {
            printf("ERROR: failed to read test image because: %s\n",
                   strerror(errno));
        }
    }
    StringBuilder labels = {0};
    sb_init(&labels, 1024);
    printf("INFO: opening test label!\n");
    FILE *test_labels = fopen("../data/test-labels", "r");
    assert(test_labels);
    while (!feof(test_labels)) {
        int ret = fread(buf, sizeof(*buf), sizeof(buf), test_labels);
        sb_append(&labels, buf, ret);
        if (ferror(test_labels) != 0) {
            printf("ERROR: failed to read file test label because: %s\n",
                   strerror(errno));
        }
    }
    printf("INFO: image: %zu, labels: %zu\n", images.size, labels.size);
    assert(images.size / IMG_SIZE == labels.size);

    printf("INFO: init test set!\n");
    SERVER.current_round.test_set =
        init_train_set(images.data, labels.data, 10000);
    printf("INFO: test set loaded!\n");

    SERVER.current_round.weights = malloc(sizeof(float *) * (ARCH_COUNT - 1));
    assert(SERVER.current_round.weights);
    SERVER.current_round.biases = malloc(sizeof(float *) * (ARCH_COUNT - 1));
    assert(SERVER.current_round.biases);
    for (int i = 0; i < ARCH_COUNT - 1; ++i) {
        SERVER.current_round.weights[i] =
            malloc(sizeof(float) * ARCH[i] * ARCH[i + 1]);
        SERVER.current_round.biases[i] = malloc(sizeof(float) * ARCH[i + 1]);
    }

    // TODO: add optional arg for intial model file
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
    //                    SERVER.current_round.weights, CURRENT_ROUND.biases,
    //                    false);
    // init_nn(&SERVER.current_round.nn, CURRENT_ROUND.weights,
    // CURRENT_ROUND.biases);
    srand(time(NULL));
    init_nn(&SERVER.current_round.nn, NULL, NULL);

    int *sock = malloc(sizeof(int));
    struct sockaddr_in name;
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock < 0) {
        printf("ERROR: could not create socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    const int enable = 1;
    if (setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        printf("ERROR: could not set socket opt: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (setsockopt(*sock, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) {
        printf("ERROR: could not set socket opt: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Give the socket a name. */
    name.sin_family = AF_INET;
    name.sin_port = htons(PORT);
    inet_pton(AF_INET, "192.0.2.2", &name.sin_addr);
    if (bind(*sock, (struct sockaddr *)&name, sizeof(name)) < 0) {
        printf("ERROR: could not bind socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (listen(*sock, 100) == -1) {
        printf("ERROR: could not listen on socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("HTTP server waits for a connection on "
           "port %d...\n",
           PORT);

    pthread_t th;
    pthread_create(&th, NULL, start_round, NULL);

    pthread_t listener;
    pthread_create(&listener, NULL, accept_conns, sock);

    // Start UI
    int window_width = 1600;
    int window_height = 900;
    InitWindow(window_width, window_height, "zfl");

    const int panel_font_size = 20;
    const int panel_start = 10;
    const int panel_margin = 10;
    const int panel_element_spacing = 20;

    while (!WindowShouldClose()) {
        window_width = GetScreenWidth();
        window_height = GetScreenHeight();
        int panel_width = 0.4 * window_width;

        BeginDrawing();
        ClearBackground(BLACK);

        BeginScissorMode(0, 0, panel_width, window_height);
        DrawText("Welcome to server UI", panel_start, panel_margin,
                 panel_font_size, RAYWHITE);
        char stats[100];
        snprintf(stats, sizeof(stats), "Current Round: %zu",
                 SERVER.current_round.round_number);
        DrawText(stats, panel_start, panel_start + panel_element_spacing,
                 panel_font_size, RAYWHITE);
        snprintf(stats, sizeof(stats), "Bytes Transferred: %zu",
                 SERVER.bytes_transferred);
        DrawText(stats, panel_start, panel_start + 2 * panel_element_spacing,
                 panel_font_size, RAYWHITE);
        snprintf(stats, sizeof(stats), "Clients ready: %zu",
                 SERVER.clients_ready);
        DrawText(stats, panel_start, panel_start + 3 * panel_element_spacing,
                 panel_font_size, RAYWHITE);
        snprintf(stats, sizeof(stats), "Total training time: %zu",
                 SERVER.total_training_time);
        DrawText(stats, panel_start, panel_start + 4 * panel_element_spacing,
                 panel_font_size, RAYWHITE);

        DrawText("Logs:", panel_start, panel_start + 6 * panel_element_spacing,
                 panel_font_size, RAYWHITE);
        for (int i = SERVER.log_len - 1, j = 1; i >= 0; --i, ++j) {
            DrawText(SERVER.logs[i], panel_start,
                     panel_start + (6 + j) * panel_element_spacing,
                     panel_font_size, RAYWHITE);
        }
        EndScissorMode();

        int chart_margin = 10;
        int chart_start_x = panel_width + chart_margin;
        int chart_start_y = chart_margin + panel_font_size;
        int chart_height = window_height - chart_start_y - panel_font_size;
        int chart_width = window_width - chart_start_x - chart_margin;

        DrawText("Accuracies on test set:", chart_start_x, panel_margin,
                 panel_font_size, RAYWHITE);
        DrawLine(chart_start_x, chart_start_y, chart_start_x,
                 chart_height + chart_start_y, RAYWHITE);
        DrawLine(chart_start_x, chart_height + chart_start_y,
                 chart_start_x + chart_width, chart_height + chart_start_y,
                 RAYWHITE);

        if (SERVER.accuracies) {
            int spacing = chart_width / da_len(SERVER.accuracies);
            for (size_t i = 0; i < da_len(SERVER.accuracies) - 1; ++i) {
                Vector2 start =
                    (Vector2){.x = (float)i * spacing + chart_start_x,
                              .y = chart_height * (1 - SERVER.accuracies[i]) +
                                   chart_start_y};
                Vector2 end = (Vector2){
                    .x = (float)(i + 1) * spacing + chart_start_x,
                    .y = chart_height * (1 - SERVER.accuracies[i + 1]) +
                         chart_start_y};
                DrawLineV(start, end, GREEN);
            }
        }
        EndDrawing();
    }

    CloseWindow();
}
