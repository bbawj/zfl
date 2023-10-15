#include "errno.h"
#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define PEER_PORT 8080

#define CONFIG_NET_CONFIG_PEER_IPV4_ADDR "192.0.2.1"

int send_weights(int client_socket) {
    const char *initial_weights_file = "./initial_weights.json";
    FILE *fd = fopen(initial_weights_file, "r");
    if (fd == NULL) {
        printf("ERROR: could not open file %s because: %s",
               initial_weights_file, strerror(errno));
        return -1;
    }
    size_t total_sent = 0;
    char buf[1024];
    int ret = fread(buf, sizeof(*buf), sizeof(buf), fd);
    while (ret > 0) {
        const char *data = buf;
        while (ret) {
            int sent = send(client_socket, data, ret, 0);
            if (sent == -1) {
                printf("Error sending data to peer, errno: %s\n",
                       strerror(errno));
                break;
            }
            ret -= sent;
            data += sent;
            total_sent += sent;
        }
        ret = fread(buf, sizeof(*buf), sizeof(buf), fd);
    }
    printf("Successfully sent %zu bytes of data to client\n", total_sent);
    fclose(fd);
    close(client_socket);

    return ret;
}

int connect_client() {
    int client;
    int ret;
    struct sockaddr_in addr4;
    addr4.sin_family = AF_INET;
    addr4.sin_port = htons(PEER_PORT);
    printf("Attempting to connect to %s %d\n", CONFIG_NET_CONFIG_PEER_IPV4_ADDR,
           PEER_PORT);
    inet_pton(AF_INET, CONFIG_NET_CONFIG_PEER_IPV4_ADDR, &addr4.sin_addr);
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
    printf("Successfully connected to client\n");
    ret = send_weights(client);
    if (ret < 0) {
        printf("ERROR: could not send to socket: %s\n", strerror(errno));
        return ret;
    }
    close(client);
    return 0;
}

int main(void) {
    int sock;
    struct sockaddr_in name;

    /* Create the socket. */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* Give the socket a name. */
    name.sin_family = AF_INET;
    name.sin_port = htons(PORT);
    name.sin_addr.s_addr = inet_addr("192.0.2.2");
    if (bind(sock, (struct sockaddr *)&name, sizeof(name)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(sock, 3) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("HTTP server waits for a connection on "
           "port %d...\n",
           PORT);

    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t client_size = sizeof(client_addr);
    while (1) {
        client_socket =
            accept(sock, (struct sockaddr *)&client_addr, &client_size);
        if (client_socket == -1) {
            perror("accept");
        }
        char addr_str[INET_ADDRSTRLEN];
        inet_ntop(client_addr.sin_family, &client_addr.sin_addr, addr_str,
                  sizeof(addr_str));
        printf("Client connected from %s\n", addr_str);
        // TODO: do work
        char buf[256];
        int ret = recv(client_socket, buf, sizeof(buf), 0);
        if (ret == 0) {
            printf("The client has closed the connection\n");
        } else if (ret < 0) {
            perror("recv");
        }
        const char *data = buf;
        if (strncmp(data, "GET ", 4) == 0) {
            data += 4;
            // TODO: use another endpoint?
            if (strncmp(data, "/ ", 2) == 0) {
                send_weights(client_socket);
            } else {
                printf("ERROR: unknown resource requested %s", data);
            }
        } else if (strncmp(data, "POST ", 5) == 0) {
            // TODO: FedAvg
        }
        printf("Received response:\n%s", buf);

        close(client_socket);
    }
}
