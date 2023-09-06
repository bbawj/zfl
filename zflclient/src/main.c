/*
 * Copyright (c) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
// #include <zephyr/logging/log.h>
// LOG_MODULE_REGISTER(net_http_client_sample, LOG_LEVEL_DBG);

#include <zephyr/net/net_ip.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/http/client.h>

#define HTTP_PORT  8080
#define HTTPS_PORT 4443

#if defined(CONFIG_NET_CONFIG_PEER_IPV6_ADDR)
#define SERVER_ADDR6 CONFIG_NET_CONFIG_PEER_IPV6_ADDR
#else
#define SERVER_ADDR6 ""
#endif

#if defined(CONFIG_NET_CONFIG_PEER_IPV4_ADDR)
#define SERVER_ADDR4 CONFIG_NET_CONFIG_PEER_IPV4_ADDR
#else
#define SERVER_ADDR4 ""
#endif

#define MAX_RECV_BUF_LEN 512

static uint8_t recv_buf_ipv4[MAX_RECV_BUF_LEN];
static uint8_t recv_buf_ipv6[MAX_RECV_BUF_LEN];

static int setup_socket(sa_family_t family, const char *server, int port, int *sock,
			struct sockaddr *addr, socklen_t addr_len)
{
	const char *family_str = family == AF_INET ? "IPv4" : "IPv6";
	int ret = 0;

	memset(addr, 0, addr_len);

	if (family == AF_INET) {
		net_sin(addr)->sin_family = AF_INET;
		net_sin(addr)->sin_port = htons(port);
		zsock_inet_pton(family, server, &net_sin(addr)->sin_addr);
	} else {
		net_sin6(addr)->sin6_family = AF_INET6;
		net_sin6(addr)->sin6_port = htons(port);
		zsock_inet_pton(family, server, &net_sin6(addr)->sin6_addr);
	}

	*sock = zsock_socket(family, SOCK_STREAM, IPPROTO_TCP);

	if (*sock < 0) {
		printf("Failed to create %s HTTP socket (%d)\n", family_str, -errno);
	}

	return ret;
}

static void response_cb(struct http_response *rsp, enum http_final_call final_data, void *user_data)
{
	if (final_data == HTTP_DATA_MORE) {
		printf("Partial data received (%zd bytes)\n", rsp->data_len);
	} else if (final_data == HTTP_DATA_FINAL) {
		printf("All the data received (%zd bytes)\n", rsp->data_len);
	}

	printf("Response to %s\n", (const char *)user_data);
	printf("Response status %s\n", rsp->http_status);
}

static int connect_socket(sa_family_t family, const char *server, int port, int *sock,
			  struct sockaddr *addr, socklen_t addr_len)
{
	int ret;

	ret = setup_socket(family, server, port, sock, addr, addr_len);
	if (ret < 0 || *sock < 0) {
		return -1;
	}

	ret = zsock_connect(*sock, addr, addr_len);
	if (ret < 0) {
		printf("Cannot connect to %s remote (%d)\n", family == AF_INET ? "IPv4" : "IPv6",
		       -errno);
		ret = -errno;
	}

	return ret;
}

static int run_queries(void)
{
	struct sockaddr_in6 addr6;
	struct sockaddr_in addr4;
	int sock4 = -1, sock6 = -1;
	int32_t timeout = 3 * MSEC_PER_SEC;
	int ret = 0;
	int port = HTTP_PORT;

	if (IS_ENABLED(CONFIG_NET_IPV4)) {
		(void)connect_socket(AF_INET, SERVER_ADDR4, port, &sock4, (struct sockaddr *)&addr4,
				     sizeof(addr4));
	}

	if (IS_ENABLED(CONFIG_NET_IPV6)) {
		(void)connect_socket(AF_INET6, SERVER_ADDR6, port, &sock6,
				     (struct sockaddr *)&addr6, sizeof(addr6));
	}

	if (sock4 < 0 && sock6 < 0) {
		printf("Cannot create HTTP connection.\n");
		return -ECONNABORTED;
	}

	if (sock4 >= 0 && IS_ENABLED(CONFIG_NET_IPV4)) {
		struct http_request req;

		memset(&req, 0, sizeof(req));

		req.method = HTTP_GET;
		req.url = "/";
		req.host = SERVER_ADDR4;
		req.protocol = "HTTP/1.1";
		req.response = response_cb;
		req.recv_buf = recv_buf_ipv4;
		req.recv_buf_len = sizeof(recv_buf_ipv4);

		ret = http_client_req(sock4, &req, timeout, "IPv4 GET");

		zsock_close(sock4);
	}

	if (sock6 >= 0 && IS_ENABLED(CONFIG_NET_IPV6)) {
		struct http_request req;

		memset(&req, 0, sizeof(req));

		req.method = HTTP_GET;
		req.url = "/";
		req.host = SERVER_ADDR6;
		req.protocol = "HTTP/1.1";
		req.response = response_cb;
		req.recv_buf = recv_buf_ipv6;
		req.recv_buf_len = sizeof(recv_buf_ipv6);

		ret = http_client_req(sock6, &req, timeout, "IPv6 GET");

		zsock_close(sock6);
	}

	return ret;
}

int main(void)
{
	int ret;

	printk("Beginning queries\n");
	ret = run_queries();
	if (ret < 0) {
		exit(1);
	}

	k_sleep(K_FOREVER);

	exit(0);
	return 0;
}
