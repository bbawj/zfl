#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(zflclient, LOG_LEVEL_DBG);

#include "train.h"

#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>

#include "sb.h"

#include "cson.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/fs/ext2.h>
#include <zephyr/fs/fs.h>
#include <zephyr/net/socket.h>

#define BIND_PORT 8080
#define PEER_PORT 8080

#define RANDOM_NN 0

const unsigned char *train_labels = NULL;
const unsigned char *train_images = NULL;
int train_labels_size = 0;
int train_images_size = 0;

static int ID = 0;

static const char request_id[] = "GET /id ";
static const char request_images[] = "GET /train-images";
static const char request_labels[] = "GET /train-labels";
static const char request_weights[] = "GET /weights ";

static const char content[] = "HTTP/1.0 200 OK\r\nContent-Type: text/plain; "
                              "charset=utf-8\r\n\nPlain-text example.";

static int sendall(int sock, const char *buf, size_t len) {
    while (len) {
        ssize_t out_len = zsock_send(sock, buf, len, 0);

        if (out_len < 0) {
            return out_len;
        }
        buf = (const char *)buf + out_len;
        len -= out_len;
    }

    return 0;
}

static int recvall(int sock, StringBuilder *sb) {
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

        StringBuilder sb = {0};
        sb_init(&sb, 1024);
        int ret = recvall(client, &sb);
        if (ret >= 0) {
            char *payload = sb_string(&sb);
            LOG_INF("SERVER: %d byte payload received %s\n", ret, payload);
            // TODO: parse HTTP request
        }
        ret = zsock_close(client);
        if (ret == 0) {
            LOG_INF("Connection from %s closed\n", addr_str);
        } else {
            LOG_ERR("Got error %s while closing the "
                    "socket\n",
                    strerror(errno));
        }
    }
}

#define THREAD_STACK_SIZE 1024
#define THREAD_PRIORITY 5
struct k_thread thread_data;

K_THREAD_STACK_DEFINE(server_stack_area, THREAD_STACK_SIZE);
k_tid_t start_async_server() {
    LOG_INF("Attempting to create server thread\n");
    k_tid_t tid =
        k_thread_create(&thread_data, server_stack_area,
                        K_THREAD_STACK_SIZEOF(server_stack_area), start_server,
                        NULL, NULL, NULL, THREAD_PRIORITY, 0, K_NO_WAIT);
    return tid;
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

char *get_request(const char *req, int *received) {
    LOG_INF("Making get request to %s", req);
    int serv = connect_main_server();
    if (serv < 0) {
        zsock_close(serv);
        return NULL;
    }

    int ret = sendall(serv, req, strlen(req));
    if (ret < 0) {
        LOG_ERR("could not send to socket: %s\n", strerror(errno));
        return NULL;
    }
    StringBuilder sb = {0};
    sb_init(&sb, 1024);
    *received = recvall(serv, &sb);
    char *content = sb_string(&sb);
    free(sb.data);
    LOG_INF("Received %d bytes\n", *received);
    zsock_close(serv);
    return content;
}

int req_id() {
    int received = 0;
    char *id_string = get_request(request_id, &received);
    ID = atoi(id_string);
    LOG_INF("Client ID is %d", ID);
    return ID;
}

void req_images() {
    int received = 0;
    static char image_resource[100];
    snprintf(image_resource, sizeof(image_resource), "%s-%d ", request_images,
             ID);
    train_images = get_request(image_resource, &received);
    train_images_size = received;
}

void req_labels() {
    int received = 0;
    static char label_resource[100];
    snprintf(label_resource, sizeof(label_resource), "%s-%d ", request_labels,
             ID);
    train_labels = get_request(label_resource, &received);
    train_labels_size = received;
}

int req_weights() {
    LOG_INF("Starting training");
#if RANDOM_NN
    train(NULL, NULL);
#else
    int received = 0;
    char *content = get_request(request_weights, &received);

    LOG_INF("Starting CSON...\n");
    Cson c = {0};
    c.b = content;
    c.size = received;
    c.cap = received;
    c.cur = 0;

    Token *json = parse_json(&c);
    Token *weights = json->child;
    Token *biases = weights->next;

    float **initial_weights = malloc(sizeof(float *) * (ARCH_COUNT - 1));
    assert(initial_weights);
    float **initial_bias = malloc(sizeof(float *) * (ARCH_COUNT - 1));
    assert(initial_bias);
    for (int i = 0; i < ARCH_COUNT - 1; ++i) {
        initial_weights[i] = malloc(sizeof(float) * ARCH[i] * ARCH[i + 1]);
        initial_bias[i] = malloc(sizeof(float) * ARCH[i + 1]);
    }

    for (int i = 0; i < ARCH_COUNT - 1; ++i) {
        Token *row = weights->child;
        for (int j = 0; j < ARCH[i]; ++j) {
            Token *col = row->child;
            for (int k = 0; k < ARCH[i + 1]; ++k) {
                initial_weights[i][j * ARCH[i + 1] + k] =
                    strtof(col->text, NULL);
                col = col->next;
            }
            row = row->next;
        }
        weights = weights->next->next;
        Token *bias = biases->child;
        for (int j = 0; j < ARCH[i + 1]; ++j) {
            initial_bias[i][j] = strtof(bias->text, NULL);
            bias = bias->next;
        }

        if (biases->next != NULL) {
            biases = biases->next->next;
        }
    }

    // pretty_print(json, 0);
    train(initial_weights, initial_bias);
    for (int i = 0; i < ARCH_COUNT - 1; ++i) {
        free(initial_weights[i]);
        free(initial_bias[i]);
    }
    free(initial_weights);
    free(initial_bias);

    free_tokens(json);
    free(content);
#endif /* if RANDOM_NN */
    LOG_INF("Training completed!\n");

    return 0;
}

int send_weights() {
    int serv = connect_main_server();
    if (serv < 0) {
        zsock_close(serv);
        return serv;
    }
    int ret = sendall(serv, content, sizeof(content));
    if (ret < 0) {
        LOG_ERR("could not send to socket: %s\n", strerror(errno));
        return ret;
    }
    zsock_close(serv);
    return 0;
}

int run(const struct shell *sh, size_t argc, char **argv) {
    // k_tid_t tid = start_async_server();
    // if (tid == NULL) {
    //     LOG_ERR("Failed to create thread\n");
    //     return -1;
    // }
    if (argc < 2) {
        LOG_ERR("ipv4 address not given");
        return -1;
    }
    char *addr_str = argv[1];
    struct in_addr addr;
    zsock_inet_pton(AF_INET, addr_str, &addr);
    if (!net_if_ipv4_addr_add(net_if_get_default(), &addr, NET_ADDR_MANUAL,
                              UINT32_MAX)) {
        LOG_ERR("failed to add %s to interface", addr_str);
        return -1;
    }

    req_id();
    req_images();
    req_labels();
    if (train_images == NULL || train_labels == NULL) {
        LOG_ERR("train_labels or train_labels is NULL");
        return -1;
    }
    req_weights();
    // k_thread_join(tid, K_FOREVER);
    // printf("Model loaded!\n");
    // train();
    return 0;
}

SHELL_CMD_ARG_REGISTER(run, NULL, "Run with IPv4 address", run, 2, 0);

int main(void) { LOG_INF("Ready"); }
