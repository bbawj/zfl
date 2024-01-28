#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(zflclient, LOG_LEVEL_DBG);

#include <zephyr/debug/thread_analyzer.h>
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/shell/shell.h>

#define NN_IMPLEMENTATION
#define CSON_IMPLEMENTATION
#define SB_IMPLEMENTATION
#define COMMON_IMPLEMENTATION
#define TRAIN_IMPLEMENTATION
#include "../../common.h"
#include "../../http.h"
#include "train.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIND_PORT 8080
#define PEER_PORT 8080
#define CONFIG_NET_CONFIG_PEER_IPV4_ADDR "192.0.2.2"

#define RANDOM_NN 0

Trainer TRAINER = {0};
int NUM_EPOCHS = 0;
int BATCH_SIZE = 0;

int train_labels_size = 0;
int train_images_size = 0;

static int ID = 0;

static int sendall(int sock, const char *buf, size_t len) {
    while (len > 0) {
        ssize_t out_len = zsock_send(sock, buf, len, 0);

        if (out_len < 0) {
            return out_len;
        }
        buf = (const char *)buf + out_len;
        len -= out_len;
    }

    return 0;
}

int recvall(int sock, StringBuilder *sb) {
    char buf[256];
    size_t total = 0;
    ssize_t r = zsock_recv(sock, buf, sizeof(buf), 0);
    while (r > 0) {
        total += r;
        sb_append(sb, buf, r);
        r = zsock_recv(sock, buf, sizeof(buf), 0);
    }
    if (r < 0) {
        LOG_ERR("Failed to receive data %s\n", strerror(errno));
        return r;
    }
    return total;
}

int init_model(Token *weights) {
    LOG_INF("Initializing Model, random: %d", RANDOM_NN);
#if RANDOM_NN
    init_nn(NULL, NULL);
#else
    float **initial_weights = malloc(sizeof(float *) * (ARCH_COUNT - 1));
    assert(initial_weights);
    float **initial_bias = malloc(sizeof(float *) * (ARCH_COUNT - 1));
    assert(initial_bias);
    for (int i = 0; i < ARCH_COUNT - 1; ++i) {
        initial_weights[i] = malloc(sizeof(float) * ARCH[i] * ARCH[i + 1]);
        initial_bias[i] = malloc(sizeof(float) * ARCH[i + 1]);
    }
    LOG_INF("Parsing json");
    parse_weights_json(weights, initial_weights, initial_bias, false);
    LOG_INF("Initing nn");
    init_nn(&TRAINER.model, initial_weights, initial_bias);
    for (int i = 0; i < ARCH_COUNT - 1; ++i) {
        free(initial_weights[i]);
        free(initial_bias[i]);
    }
    free(initial_weights);
    free(initial_bias);
#endif /* if RANDOM_NN */

    LOG_INF("NN init!\n");
    return 0;
}

int connect_main_server() {
    int serv;
    int ret;
    struct sockaddr_in addr4;
    addr4.sin_family = AF_INET;
    addr4.sin_port = htons(PEER_PORT);
    LOG_INF("Attempting to connect to %s %d\n",
            CONFIG_NET_CONFIG_PEER_IPV4_ADDR, PEER_PORT);
    zsock_inet_pton(AF_INET, CONFIG_NET_CONFIG_PEER_IPV4_ADDR, &addr4.sin_addr);
    serv = zsock_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serv == -1) {
        LOG_ERR("could not create server socket because %s\n", strerror(errno));
        return -1;
    }
    ret = zsock_connect(serv, (struct sockaddr *)&addr4, sizeof(addr4));
    if (ret < 0) {
        LOG_ERR("could not connect to server socket: %s\n", strerror(errno));
        return ret;
    }
    LOG_INF("Successfully connected to server\n");
    return serv;
}

int send_weights(size_t round_number, int64_t training_time) {
    StringBuilder sb;
    sb_init(&sb, 1024);
    sb_appendf(&sb, "{\"round\": %zu, \"training_time\": %ld, \"weights\": ",
               round_number, training_time);
    weights_to_string(&sb, &TRAINER.model);
    sb_append(&sb, "}", 1);
    char req[256];
    int req_len = snprintf(req, sizeof(req),
                           "POST %s?id=%d \r\nContent-Length: %zu\r\n\r\n",
                           RESULTS_ENDPOINT, ID, sb.size);

    int ret = 0;
    int serv = -1;
    int retries = 3;
    for (int i = 0; i < retries; ++i) {
        LOG_INF("Sending local weights, current tries: %d", i + 1);
        serv = connect_main_server();
        if (serv < 0) {
            continue;
        }
        int ret = sendall(serv, req, req_len);
        if (ret < 0) {
            LOG_ERR("could not send req to socket: %s", strerror(errno));
            if (errno == ENOTCONN) {
                zsock_close(serv);
                continue;
            }
            break;
        }
        ret = sendall(serv, sb.data, sb.size);
        if (ret < 0) {
            LOG_ERR("could not send weights to socket: %s", strerror(errno));
            zsock_close(serv);
            continue;
        }
        break;
    }
    zsock_close(serv);
    sb_free(&sb);
    return ret;
}

void handle_incoming_connection(void *sock, void *x, void *y) {
    int client = *((int *)sock);
    Http h = {0};
    StringBuilder sb = {0};
    sb_init(&sb, 1024);

    int required_len = recv_required_req(&h, &sb, client);
    if (required_len < 0) {
        LOG_ERR("invalid HTTP required header: %.*s", sb.size, sb.data);
        goto clean;
    }
    int optional_len = recv_optional_headers(&h, &sb, client, required_len);
    if (optional_len < 0) {
        LOG_ERR("invalid HTTP optional headers: %.*s", sb.size, sb.data);
        goto clean;
    }

    parse_query_param(&h);
    LOG_INF("HTTP request\nMethod: %s, url: %s\n", h.method, h.url);

    if (strncmp(h.url, TRAIN_ENDPOINT, strlen(TRAIN_ENDPOINT)) == 0) {
        char *content_len_str = get_header(&h, CONTENT_LENGTH);
        if (content_len_str == NULL) {
            LOG_ERR("no training data received\n");
            goto clean;
        }
        char *endptr;
        size_t content_len = strtol(content_len_str, &endptr, 10);
        size_t offset = required_len + optional_len;
        int ret = recv_content(&sb, client, offset, content_len);
        if (ret == -1) {
            LOG_ERR("Could not recv all %zu bytes of content, required: %d, "
                    "optional: %d, current: %zu",
                    content_len, required_len, optional_len, sb.size);
            goto clean;
        }
        char *content = substr(sb.data, offset, content_len);

        Payload p = {0};
        deserialize_training_data(content, content_len, &p);
        init_model(p.weights);

        LOG_INF("Starting training");
        int64_t start = k_uptime_get();
        train(&TRAINER);
        int64_t end = k_uptime_get();

        send_weights(p.round_number, (end - start) / 1000);

        free_tokens(p.json);
        free(content);
    } else if (strncmp(h.url, READY_ENDPOINT, strlen(READY_ENDPOINT)) == 0) {
        char *ready = TRAINER.samples_ready ? "true" : "false";
        sendall(client, ready, strlen(ready));
    } else {
        LOG_ERR("404 resource not found: %s\n", h.url);
    }
clean:
    http_free(&h);
    zsock_close(client);
    sb_free(&sb);
    free(sock);
}

#define THREAD_STACK_SIZE 1024
#define THREAD_PRIORITY 5
struct k_thread thread_data;

K_THREAD_STACK_DEFINE(server_stack_area, THREAD_STACK_SIZE);
k_tid_t handle_incoming_connection_async(int *sock) {
    LOG_INF("Attempting to create server thread\n");
    k_tid_t tid = k_thread_create(&thread_data, server_stack_area,
                                  K_THREAD_STACK_SIZEOF(server_stack_area),
                                  handle_incoming_connection, sock, NULL, NULL,
                                  THREAD_PRIORITY, 0, K_NO_WAIT);
    return tid;
}

void start_server() {
    int serv;
    static int counter;
    struct sockaddr_in bind_addr;

    serv = zsock_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serv == -1) {
        LOG_ERR("Could not create server socket because %s\n", strerror(errno));
        return;
    }
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind_addr.sin_port = htons(BIND_PORT);
    if (zsock_bind(serv, (struct sockaddr *)&bind_addr, sizeof(bind_addr)) ==
        -1) {
        LOG_ERR("Could not bind server socket because %s\n", strerror(errno));
        return;
    }

    if (zsock_listen(serv, 5) == -1) {
        LOG_ERR("Could not listen on server socket because %s\n",
                strerror(errno));
        return;
    }

    LOG_INF("HTTP server waits for a connection on "
            "port %d...\n",
            BIND_PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        char addr_str[INET_ADDRSTRLEN];
        int client = zsock_accept(serv, (struct sockaddr *)&client_addr,
                                  &client_addr_len);
        if (client < 0) {
            LOG_ERR("Error in accept: %s - continuing\n", strerror(errno));
            continue;
        }
        zsock_inet_ntop(client_addr.sin_family, &client_addr.sin_addr, addr_str,
                        sizeof(addr_str));
        LOG_INF("Connection #%d from %s\n", counter++, addr_str);
        int *sock = malloc(sizeof(int));
        *sock = client;
        handle_incoming_connection_async(sock);
    }
}

char *get_request(const char *endpoint, int *received) {
    char req[256];
    int len = snprintf(req, sizeof(req), "%s %s \r\n\r\n", GET, endpoint);
    LOG_INF("Making get request to %s", req);
    int serv = connect_main_server();
    if (serv < 0) {
        LOG_ERR("could not connect to server: %s\n", strerror(errno));
        zsock_close(serv);
        return NULL;
    }

    int ret = sendall(serv, req, len);
    if (ret < 0) {
        LOG_ERR("could not send to server: %s\n", strerror(errno));
        zsock_close(serv);
        return NULL;
    }
    StringBuilder sb = {0};
    sb_init(&sb, 1024);
    *received = recvall(serv, &sb);
    char *content = sb_string(&sb);
    sb_free(&sb);
    zsock_close(serv);
    return content;
}

int req_id() {
    int received = 0;
    char *id_string = get_request(ID_ENDPOINT, &received);
    if (id_string == NULL) {
        return -1;
    }
    ID = atoi(id_string);
    free(id_string);
    LOG_INF("Client ID is %d", ID);
    return received;
}

char *req_images(int *n) {
    static char image_resource[100];
    snprintf(image_resource, sizeof(image_resource), "%s?id=%d",
             TRAINING_IMAGES_ENDPOINT, ID);
    return get_request(image_resource, n);
}

char *req_labels(int *n) {
    static char label_resource[100];
    snprintf(label_resource, sizeof(label_resource), "%s?id=%d",
             TRAINING_LABELS_ENDPOINT, ID);
    return get_request(label_resource, n);
}

int run(const struct shell *sh, size_t argc, char **argv) {
    if (argc < 2) {
        LOG_ERR("ipv4 address not given");
        return -1;
    }
    char *addr_str = argv[1];
    LOG_INF("instance ipaddr is %s", addr_str);
    struct in_addr addr;
    zsock_inet_pton(AF_INET, addr_str, &addr);
    if (!net_if_ipv4_addr_add(net_if_get_default(), &addr, NET_ADDR_MANUAL,
                              UINT32_MAX)) {
        LOG_ERR("failed to add %s to interface", addr_str);
        return -1;
    }

    NUM_EPOCHS = atoi(argv[2]);
    if (NUM_EPOCHS == 0) {
        LOG_ERR("num epochs: %s is invalid", argv[2]);
        return -1;
    }

    BATCH_SIZE = atoi(argv[3]);
    if (BATCH_SIZE == 0) {
        LOG_ERR("batch size: %s is invalid", argv[3]);
        return -1;
    }

    TRAINER.samples_ready = false;
    TRAINER.samples = (Mat){0};
    TRAINER.model = (NN){0};

    int image_bytes, n_labels;
    req_id();
    char *img_data = req_images(&image_bytes);
    char *label_data = req_labels(&n_labels);
    int n_images = image_bytes / IMG_SIZE;
    if (n_images != n_labels) {
        LOG_ERR("n_images is %d but n_labels is %d, mismatch!", n_images,
                n_labels);
        return -1;
    }

    LOG_INF("n_images is %d, n_labels is %d", n_images, n_labels);
    TRAINER.samples = init_train_set(img_data, label_data, n_images);
    LOG_INF("train set init!");
    TRAINER.n_images = n_images;
    TRAINER.samples_ready = true;
    free(img_data);
    free(label_data);

    start_server();

    return 0;
}

SHELL_CMD_ARG_REGISTER(run, NULL, "Run with IPv4 address", run, 4, 0);

int main(void) { LOG_INF("Ready"); }
