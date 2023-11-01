#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(zflclient, LOG_LEVEL_DBG);

#include "../../zflserver/src/http.h"
#include "common.h"
#include "cson.h"
#include "train.h"

#define BIND_PORT 8080
#define PEER_PORT 8080

#define RANDOM_NN 0

Trainer TRAINER = {0};

const unsigned char *train_labels = NULL;
const unsigned char *train_images = NULL;
int train_labels_size = 0;
int train_images_size = 0;

static int ID = 0;

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

static int recvsb(int sock, StringBuilder *sb) {
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

int init_model(char *data, size_t len) {
#if RANDOM_NN
    init_nn(NULL, NULL);
#else
    float **initial_weights = malloc(sizeof(float *) * (ARCH_COUNT - 1));
    assert(initial_weights);
    float **initial_bias = malloc(sizeof(float *) * (ARCH_COUNT - 1));
    assert(initial_bias);
    parse_weights_json(data, len, initial_weights, initial_bias, false);
    init_nn(&TRAINER.model, initial_weights, initial_bias);
    for (int i = 0; i < ARCH_COUNT - 1; ++i) {
        free(initial_weights[i]);
        free(initial_bias[i]);
    }
    free(initial_weights);
    free(initial_bias);
#endif /* if RANDOM_NN */

    TRAINER.model_ready = true;
    LOG_INF("NN init!\n");

    return 0;
}

void handle_incoming_connection(void *sock, void *x, void *y) {
    int client = *((int *)sock);
    char buf[256];
    int len = zsock_recv(client, buf, sizeof(buf), 0);
    if (len <= 0) {
        goto clean;
    }
    LOG_INF("received %d bytes\n", len);
    Http h = {0};
    int ret = parse_header(&h, buf, len);
    if (ret <= 0) {
        LOG_ERR("invalid HTTP header");
        goto clean;
    }
    parse_query_param(&h);
    if (strncmp(h.url, TRAIN_ENDPOINT, strlen(TRAIN_ENDPOINT)) == 0) {
        char *content_len_str = get_header(&h, CONTENT_LENGTH);
        if (content_len_str == NULL) {
            LOG_ERR("no training data received\n");
            goto clean;
        }
        char *endptr;
        size_t content_len = strtol(content_len_str, &endptr, 10);
        char *content = recv_size(client, content_len);
        if (content == NULL) {
            goto clean;
        }
        init_model(content, content_len);
        if (TRAINER.samples_ready && TRAINER.model_ready) {
            LOG_INF("Starting training");
            train(&TRAINER);
        } else {
            LOG_ERR("Unable to start training on client id %d. Trainer "
                    "state sample: %d, model: %d",
                    ID, TRAINER.samples_ready, TRAINER.model_ready);
        }
    } else if (strncmp(payload, ready_endpoint, strlen(ready_endpoint)) == 0) {
        char *ready =
            TRAINER.samples_ready && TRAINER.model_ready ? "true" : "false";
        sendall(client, ready, strlen(ready));
    }
clean:
    sb_free(&sb);
    ret = zsock_close(client);
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

char *get_request(const char *endpoint, int *received) {
    char req[256];
    snprintf(req, sizeof(req), "%s %s \r\n\r\n", GET, endpoint);
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
    *received = recvsb(serv, &sb);
    char *content = sb_string(&sb);
    free(sb.data);
    LOG_INF("Received %d bytes\n", *received);
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
    LOG_INF("Client ID is %d", ID);
    return ID;
}

char *req_images(int *n) {
    static char image_resource[100];
    snprintf(image_resource, sizeof(image_resource), "%s?id=%d \r\n",
             request_images, ID);
    return get_request(image_resource, n);
}

char *req_labels(int *n) {
    static char label_resource[100];
    snprintf(label_resource, sizeof(label_resource), "%s?id=%d \r\n",
             request_labels, ID);
    return get_request(label_resource, n);
}

int send_weights() {
    int serv = connect_main_server();
    if (serv < 0) {
        zsock_close(serv);
        return serv;
    }
    StringBuilder sb;
    sb_init(&sb, 1024);
    sb_appendf(&sb, "%s%d \r\n", results_endpoint, ID);
    char *weights = weights_to_string(&sb, &TRAINER);
    int ret = sendall(serv, weights, sb.size);
    if (ret < 0) {
        LOG_ERR("could not send to socket: %s\n", strerror(errno));
    }
    zsock_close(serv);
    sb_free(&sb);
    free(weights);
    return ret;
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

    TRAINER.model_ready = false;
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
    Mat m;
    init_train_set(&m, img_data, label_data, n_images);
    LOG_INF("train set init!");
    TRAINER.n_images = image_bytes;
    TRAINER.samples_ready = true;
    free(img_data);
    free(label_data);

    start_server();

    return 0;
}

SHELL_CMD_ARG_REGISTER(run, NULL, "Run with IPv4 address", run, 2, 0);

int main(void) { LOG_INF("Ready"); }
