#include <stdio.h>
#include <string.h>
#include <zephyr/net/socket.h>

#define BIND_PORT 8080

int main(void)
{
	static const char content[] = "HTTP/1.0 200 OK\r\nContent-Type: text/plain; "
				      "charset=utf-8\r\n\nPlain-text example.";
	int serv;
	static int counter;
	struct sockaddr_in bind_addr;

	serv = zsock_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv == -1) {
		fprintf(stderr, "ERROR: could not create server socket because %s\n",
			strerror(errno));
		return -1;
	}
	bind_addr.sin_family = AF_INET;
	bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind_addr.sin_port = htons(BIND_PORT);
	if (zsock_bind(serv, (struct sockaddr *)&bind_addr, sizeof(bind_addr)) == -1) {
		fprintf(stderr, "ERROR: could not bind server socket because %s\n",
			strerror(errno));
		return -1;
	}

	if (zsock_listen(serv, 5) == -1) {
		fprintf(stderr, "ERROR: could not listen on server socket because %s\n",
			strerror(errno));
		return -1;
	}

	printf("Single-threaded dumb HTTP server waits for a connection on "
	       "port %d...\n",
	       BIND_PORT);

	while (1) {
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		char addr_str[INET_ADDRSTRLEN];
		char buf[100];
		int client = zsock_accept(serv, (struct sockaddr *)&client_addr, &client_addr_len);
		if (client < 0) {
			printf("Error in accept: %s - continuing\n", strerror(errno));
			continue;
		}
		zsock_inet_ntop(client_addr.sin_family, &client_addr.sin_addr, addr_str,
				sizeof(addr_str));
		printf("Connection #%d from %s\n", counter++, addr_str);

		ssize_t r = zsock_recv(client, buf, sizeof(buf), 0);
		buf[r] = '\0';
		if (r == 0) {
			goto close_client;
		} else if (r < 0) {
			fprintf(stderr, "ERROR: failed to receive data %s\n", strerror(errno));
			goto close_client;
		}
		// TODO: parse HTTP request
		printf("SERVER: %zu byte payload received %s\n", r, buf);
		const char *data = content;
		size_t len = sizeof(content);
		while (len) {
			int sent_len = zsock_send(client, data, len, 0);
			if (sent_len == -1) {
				printf("Error sending data to peer, errno: %s\n", strerror(errno));
				break;
			}
			data += sent_len;
			len -= sent_len;
		}
		int ret;
close_client:
		ret = zsock_close(client);
		if (ret == 0) {
			printf("Connection from %s closed\n", addr_str);
		} else {
			printf("Got error %s while closing the "
			       "socket\n", strerror(errno));
		}
	}
	exit(0);
}
