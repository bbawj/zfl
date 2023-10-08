#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

void handle_connection(int sock) {}

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
        printf("Received response:\n%s", buf);

        close(client_socket);
    }
}
