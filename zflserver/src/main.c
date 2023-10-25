#include "errno.h"
#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define PEER_PORT 8080

#define CONFIG_NET_CONFIG_PEER_IPV4_ADDR "192.0.2.1"

pthread_mutex_t accum_mutex = PTHREAD_MUTEX_INITIALIZER;

static const char *GET = "GET ";
static const char *ID_ENDPOINT = "/id ";
static const char *WEIGHTS_ENDPOINT = "/weights ";
static const char *TRAINING_LABELS_ENDPOINT = "/train-labels-";
static const char *TRAINING_IMAGES_ENDPOINT = "/train-images-";
static int TOTAL_CLIENTS = 0;

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
    if (ret < 0) {
        printf("ERROR: could not send to socket: %s\n", strerror(errno));
        return ret;
    }
    close(client);
    return 0;
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

int send_id(int client_socket) {
    pthread_mutex_lock(&accum_mutex);
    ++TOTAL_CLIENTS;
    int id = TOTAL_CLIENTS;
    pthread_mutex_unlock(&accum_mutex);
    printf("Current ID is %d\n", TOTAL_CLIENTS);
    char id_string[100];
    snprintf(id_string, 100, "%d", id);
    int sent = send(client_socket, id_string, 1, 0);
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

void *handle_incoming_connection(void *sock) {
    char buf[256];
    int client_socket = *((int *)sock);
    int ret = recv(client_socket, buf, sizeof(buf), 0);
    if (ret == 0) {
        printf("The client has closed the connection\n");
    } else if (ret < 0) {
        perror("recv");
    }
    printf("Request: %s\n", buf);
    const char *data = buf;
    if (strncmp(data, GET, strlen(GET)) == 0) {
        data += strlen(GET);
        if (strncmp(data, ID_ENDPOINT, strlen(ID_ENDPOINT)) == 0) {
            send_id(client_socket);
        } else if (strncmp(data, TRAINING_IMAGES_ENDPOINT,
                           strlen(TRAINING_IMAGES_ENDPOINT)) == 0) {
            data += strlen(TRAINING_IMAGES_ENDPOINT);
            char id_string = data[0];
            int id = atoi(&id_string);
            send_training_images(client_socket, id);
        } else if (strncmp(data, TRAINING_LABELS_ENDPOINT,
                           strlen(TRAINING_LABELS_ENDPOINT)) == 0) {
            data += strlen(TRAINING_LABELS_ENDPOINT);
            char id_string = data[0];
            int id = atoi(&id_string);
            send_training_labels(client_socket, id);
        } else if (strncmp(data, WEIGHTS_ENDPOINT, strlen(WEIGHTS_ENDPOINT)) ==
                   0) {
            send_weights(client_socket);
        } else {
            printf("ERROR: unknown resource requested %s\n", data);
        }
    } else if (strncmp(data, "POST ", 5) == 0) {
        // TODO: FedAvg
    }

    free(sock);
    close(client_socket);
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
    const int enable = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    /* Give the socket a name. */
    name.sin_family = AF_INET;
    name.sin_port = htons(PORT);
    inet_pton(AF_INET, "192.0.2.2", &name.sin_addr);
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

        int *sock = malloc(sizeof(client_socket));
        *sock = client_socket;
        pthread_t th;
        pthread_create(&th, NULL, handle_incoming_connection, sock);
    }
}
