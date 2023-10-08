#include "sb.h"
#include "train.h"

#include <stdlib.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(zflserver, LOG_LEVEL_DBG);
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/fs/ext2.h>
#include <zephyr/fs/fs.h>
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>

#define BIND_PORT 8080
#define PEER_PORT 8080

#if !defined(CONFIG_NET_CONFIG_PEER_IPV4_ADDR)
#define CONFIG_NET_CONFIG_PEER_IPV4_ADDR ""
#endif

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
        char buf[256];
        ssize_t r = zsock_recv(client, buf, sizeof(buf), 0);
        while (r > 0) {
            sb_append(&sb, buf, r);
            r = zsock_recv(client, buf, sizeof(buf), 0);
        }
        if (r == 0) {
            goto close_client;
        } else if (r < 0) {
            LOG_ERR("Failed to receive data %s\n", strerror(errno));
            goto close_client;
        }
        // TODO: parse HTTP request
        char *payload = sb_string(&sb);
        LOG_INF("SERVER: %zu byte payload received %s\n", r, payload);
        const char *data = content;
        size_t len = sizeof(content);
        while (len) {
            int sent_len = zsock_send(client, data, len, 0);
            if (sent_len == -1) {
                LOG_ERR("Error sending data to peer, errno: %s\n",
                        strerror(errno));
                break;
            }
            data += sent_len;
            len -= sent_len;
        }
        int ret;
    close_client:
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

K_THREAD_STACK_DEFINE(server_stack_area, THREAD_STACK_SIZE);
k_tid_t start_async_server() {
    struct k_thread thread_data;
    k_tid_t tid =
        k_thread_create(&thread_data, server_stack_area,
                        K_THREAD_STACK_SIZEOF(server_stack_area), start_server,
                        NULL, NULL, NULL, THREAD_PRIORITY, 0, K_NO_WAIT);
    return tid;
}

int send_weights() {
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
        LOG_ERR("ERROR: could not create server socket because %s\n",
                strerror(errno));
        return -1;
    }
    ret = zsock_connect(serv, (struct sockaddr *)&addr4, sizeof(addr4));
    if (ret < 0) {
        LOG_ERR("ERROR: could not connect to socket: %s\n", strerror(errno));
        return ret;
    }
    LOG_INF("Successfully connected to server\n");
    ret = sendall(serv, content, sizeof(content));
    if (ret < 0) {
        LOG_ERR("ERROR: could not send to socket: %s\n", strerror(errno));
        return ret;
    }
    zsock_close(serv);
    return 0;
}

int main(void) {
    k_tid_t tid = start_async_server();
    if (tid == NULL) {
        LOG_ERR("Failed to create thread\n");
        return -1;
    }
    printf("Model loaded!\n");
    train();
    return 0;
}
