#include <stdio.h>
#include <string.h>
#include <zephyr/net/socket.h>

#define BIND_PORT 8080

int main(void)
{
	const char *content = "HTTP/1.0 200 OK\r\nContent-Type: text/plain; "
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
		int client = zsock_accept(serv, (struct sockaddr *)&client_addr, &client_addr_len);
		if (client < 0) {
			printf("Error in accept: %d - continuing\n", errno);
			continue;
		}
		zsock_inet_ntop(client_addr.sin_family, &client_addr.sin_addr, addr_str,
				sizeof(addr_str));
		printf("Connection #%d from %s\n", counter++, addr_str);

		int sent_len = zsock_send(client, content, strlen(content), 0);

		if (sent_len == -1) {
			fprintf(stderr, "Error sending data to peer, errno: %s\n", strerror(errno));
			break;
		}
	}
	return 1;
}
