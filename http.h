#ifndef HTTP_H_
#define HTTP_H_

#include "sb.h"
#ifdef SERVER
#include <sys/socket.h>
#else
#include "zephyr/net/socket.h"
#endif

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

const char GET[] = "GET";
const char POST[] = "POST";
const char CONTENT_LENGTH[] = "Content-Length";

// Client endpoints
const char TRAIN_ENDPOINT[] = "/train";
const char READY_ENDPOINT[] = "/ready";

// Server endpoints
const char ID_ENDPOINT[] = "/id";
const char TRAINING_LABELS_ENDPOINT[] = "/train-labels";
const char TRAINING_IMAGES_ENDPOINT[] = "/train-images";
const char RESULTS_ENDPOINT[] = "/results";
const char SHUTDOWN_ENDPOINT[] = "/shutdown";

typedef struct {
    char *method;
    char *url;

    size_t response_code;
    char *response_message;

    size_t n_query;
    char **query_keys;
    char **query_vals;

    size_t n_headers;
    char **header_keys;
    char **header_vals;
} Http;

bool is_char(const char *p, const char *c, size_t len) {
    return strncmp(p, c, len) == 0;
}

/**
 * Finds the first occurrence of c of length len in p
 * @return number of bytes to first match
 * @return -1 if no match is found
 */
int find_first_match(const char *p, size_t size, const char *c, size_t len) {
    size_t processed = 0;
    while (processed < size && *p != '\0' && !is_char(p, c, len)) {
        ++p;
        ++processed;
    }
    if (is_char(p, c, len)) {
        return processed;
    } else {
        return -1;
    }
}

char *substr(const char *p, size_t start, size_t len) {
    char *sub = malloc(len + 1);
    assert(sub);

    memcpy(sub, &p[start], len);
    sub[len] = '\0';
    return sub;
}

int parse_required_res(Http *h, const char *payload, size_t len) {
    const char *p = payload;
    int processed = 0;
    int code_len = find_first_match(p, len, " ", 1);
    if (code_len <= 0) {
        return -1;
    }
    h->response_code = atoi(substr(p, 0, code_len));
    p += code_len + 1;
    processed += code_len + 1;
    len -= code_len + 1;

    int message_len = find_first_match(p, len, " ", 1);
    if (message_len <= 0) {
        return -1;
    }
    h->response_message = substr(p, 0, message_len);
    p += message_len + 1;
    processed += message_len + 1;
    len -= message_len + 1;

    int ret = find_first_match(p, len, "\r\n", 2);
    if (ret == -1) {
        return -1;
    }
    processed += ret + 2;
    return processed;
}

int parse_required_req(Http *h, const char *payload, size_t len) {
    const char *p = payload;
    int processed = 0;
    int method_len = find_first_match(p, len, " ", 1);
    if (method_len <= 0) {
        // printf("invalid method: %s\n", payload);
        return -1;
    }
    h->method = substr(p, 0, method_len);
    p += method_len + 1;
    processed += method_len + 1;
    len -= method_len + 1;

    int url_len = find_first_match(p, len, " ", 1);
    if (url_len <= 0) {
        // printf("invalid url: %s\n", payload);
        return -1;
    }
    h->url = substr(p, 0, url_len);
    p += url_len + 1;
    processed += url_len + 1;
    len -= url_len + 1;

    int ret = find_first_match(p, len, "\r\n", 2);
    if (ret == -1) {
        // printf("no blank line: %s\n", payload);
        return -1;
    }
    processed += ret + 2;
    return processed;
}

int parse_optional(Http *h, const char *payload, size_t len) {
    const char *p = payload;
    int processed = 0;

    while (len > 0) {
        int header_len = find_first_match(p, len, "\r\n", 2);
        processed += header_len + 2;
        if (header_len == 0) {
            // No more headers
            break;
        } else if (header_len > 0) {
            char *header = substr(p, 0, header_len);

            int key_len = find_first_match(header, header_len, ": ", 2);
            if (key_len == -1) {
                return -1;
            }
            char *header_key = substr(header, 0, key_len);
            char *header_val =
                substr(header, key_len + 2, header_len - key_len - 2);
            ++h->n_headers;
            h->header_keys =
                realloc(h->header_keys, sizeof(*h->header_keys) * h->n_headers);
            h->header_vals =
                realloc(h->header_vals, sizeof(*h->header_vals) * h->n_headers);
            h->header_keys[h->n_headers - 1] = header_key;
            h->header_vals[h->n_headers - 1] = header_val;

            p += header_len + 2;
            len -= header_len + 2;
            free(header);
        } else {
            // Cannot find required blank line
            return -1;
        }
    }

    return processed;
}

int parse_query_param(Http *h) {
    const char *ptr = h->url;
    size_t len = strlen(h->url);
    bool found = false;
    int to_skip = find_first_match(ptr, len, "?", 1);
    if (to_skip == -1) {
        return -1;
    }
    ptr += to_skip + 1;
    len -= to_skip + 1;
    while (len > 0) {
        int param_len = find_first_match(ptr, len, "&", 1);
        if (param_len == -1) {
            param_len = len;
        }
        if (param_len > 0) {
            char *param = substr(ptr, 0, param_len);
            int key_len = find_first_match(param, len, "=", 1);
            if (key_len == -1 || key_len == param_len) {
                return -1;
            }
            char *key = substr(param, 0, key_len);
            char *val = substr(param, key_len + 1, param_len - key_len);
            ++h->n_query;
            h->query_keys =
                realloc(h->query_keys, sizeof(*h->query_keys) * h->n_query);
            h->query_vals =
                realloc(h->query_vals, sizeof(*h->query_vals) * h->n_query);
            h->query_keys[h->n_query - 1] = key;
            h->query_vals[h->n_query - 1] = val;

            found = true;
            free(param);
            ptr += param_len;
            len -= param_len;
        } else {
            break;
        }
    }
    return found;
}

char *get_header(Http *h, const char *key) {
    for (size_t i = 0; i < h->n_headers; ++i) {
        const char *cur = h->header_keys[i];
        if (strncmp(cur, key, strlen(key)) == 0) {
            return h->header_vals[i];
        }
    }
    return NULL;
}

char *get_param(Http *h, const char *key) {
    for (size_t i = 0; i < h->n_query; ++i) {
        const char *cur = h->query_keys[i];
        if (strncmp(cur, key, strlen(key)) == 0) {
            return h->query_vals[i];
        }
    }
    return NULL;
}

// Receive all data on the socket until we are able to parse required
// HTTP header
int recv_required_req(Http *h, StringBuilder *sb, int sock) {
    char buf[100];
    int processed = 0;
    const int max_required = 256;
    int required_len = 0;
    while (processed < max_required &&
           (required_len = parse_required_req(h, sb->data, sb->size)) <= 0) {
        int len = recv(sock, buf, sizeof(buf), 0);
        if (len <= 0) {
            printf("ERROR: failed to recv because: %s\n", strerror(errno));
            return -1;
        }
        processed += len;
        sb_append(sb, buf, len);
    }
    return required_len;
}

// Receive all data on the socket until we are able to parse required
// blank line or timeout
int recv_optional_headers(Http *h, StringBuilder *sb, int sock, size_t offset) {
    char buf[256];
    int optional_len = 0;
    while ((optional_len =
                parse_optional(h, sb->data + offset, sb->size - offset)) <= 0) {
        int len = recv(sock, buf, sizeof(buf), 0);
        if (len <= 0) {
            printf("ERROR: failed to recv because: %s\n", strerror(errno));
            return -1;
        }
        sb_append(sb, buf, len);
    }
    return optional_len;
}

// Recv until we are able to obtain all the content
int recv_content(StringBuilder *sb, int sock, size_t offset, size_t len) {
    char buf[256];
    while (sb->size - offset < len) {
        int len = recv(sock, buf, sizeof(buf), 0);
        if (len <= 0) {
            printf("ERROR: failed to recv because: %s\n", strerror(errno));
            return -1;
        }
        sb_append(sb, buf, len);
    }
    return 0;
}
#endif // !HTTP_H_
