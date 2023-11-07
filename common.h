#ifndef COMMON_H_
#define COMMON_H_

#include "cson.h"
#include "nn.h"
#include "sb.h"

#define IMG_HEIGHT 28
#define IMG_WIDTH 28
#define IMG_SIZE (IMG_HEIGHT * IMG_WIDTH)
#define HIDDEN_LAYER 16
#define OUTPUT 10
#define ARCH_COUNT 3

Mat init_train_set(char *img_data, char *label_data, int n_images);
int parse_weights_json(char *bytes, size_t len, float **initial_weights,
                       float **initial_bias, bool accum);
int insert_weights_to_nn(NN nn, float **initial_weights, float **initial_bias);
void init_nn(NN *nn, float **initial_weights, float **initial_bias);
float accuracy(NN nn, Mat t);
size_t weights_to_string(StringBuilder *sb, NN *nn);

#ifdef COMMON_IMPLEMENTATION

size_t ARCH[] = {IMG_SIZE, HIDDEN_LAYER, OUTPUT};

size_t weights_to_string(StringBuilder *sb, NN *nn) {
    sb_append(sb, "[", 1);
    for (size_t i = 0; i < nn->arch_count - 1; ++i) {
        Mat w = nn->ws[i];
        sb_append(sb, "[", 1);
        for (size_t j = 0; j < w.rows; ++j) {
            sb_append(sb, "[", 1);
            for (size_t k = 0; k < w.cols; ++k) {
                if (k == w.cols - 1) {
                    sb_appendf(sb, "%f", MAT_AT(w, j, k));
                } else {
                    sb_appendf(sb, "%f,", MAT_AT(w, j, k));
                }
            }
            if (j == w.rows - 1) {
                sb_append(sb, "]", 1);
            } else {
                sb_append(sb, "],", 2);
            }
        }
        sb_append(sb, "],", 2);
        sb_append(sb, "[", 1);
        Row b = nn->bs[i];
        for (size_t j = 0; j < b.cols; ++j) {
            if (j == b.cols - 1) {
                sb_appendf(sb, "%f", w.elements[j]);
            } else {
                sb_appendf(sb, "%f,", w.elements[j]);
            }
        }
        if (i == nn->arch_count - 2) {
            sb_append(sb, "]", 1);
        } else {
            sb_append(sb, "],", 2);
        }
    }
    sb_append(sb, "]", 1);
    return sb->size;
}

Mat init_train_set(char *img_data, char *label_data, int n_images) {
    Mat t = mat_alloc(NULL, n_images, IMG_SIZE + 10);

    for (int image = 0; image < n_images; ++image) {
        for (int row = 0; row < IMG_HEIGHT; ++row) {
            for (int col = 0; col < IMG_WIDTH; ++col) {
                float pixel =
                    img_data[image * IMG_SIZE + row * IMG_WIDTH + col] / 255.0f;
                MAT_AT(t, image, row * IMG_WIDTH + col) = pixel;
            }
        }
        for (int i = 0; i < OUTPUT; ++i) {
            if (i == label_data[image]) {
                MAT_AT(t, image, IMG_SIZE + label_data[image]) = 1.0f;
            } else {
                MAT_AT(t, image, IMG_SIZE + i) = 0.0f;
            }
        }
    }
    return t;
}

int parse_weights_json(char *bytes, size_t len, float **initial_weights,
                       float **initial_bias, bool accum) {
    Cson c = {0};
    c.b = bytes;
    c.size = len;
    c.cap = len;
    c.cur = 0;

    Token *json = parse_json(&c);
    Token *weights = json->child;
    Token *biases = weights->next;

    if (!accum) {
        for (int i = 0; i < ARCH_COUNT - 1; ++i) {
            initial_weights[i] =
                calloc(1, sizeof(float) * ARCH[i] * ARCH[i + 1]);
            assert(initial_weights[i] != NULL);
            initial_bias[i] = calloc(1, sizeof(float) * ARCH[i + 1]);
            assert(initial_bias[i] != NULL);
        }
    }

    for (size_t i = 0; i < ARCH_COUNT - 1; ++i) {
        Token *row = weights->child;
        for (size_t j = 0; j < ARCH[i]; ++j) {
            Token *col = row->child;
            for (size_t k = 0; k < ARCH[i + 1]; ++k) {
                initial_weights[i][j * ARCH[i + 1] + k] +=
                    strtof(col->text, NULL);
                col = col->next;
            }
            row = row->next;
        }
        weights = weights->next->next;
        Token *bias = biases->child;
        for (size_t j = 0; j < ARCH[i + 1]; ++j) {
            initial_bias[i][j] += strtof(bias->text, NULL);
            bias = bias->next;
        }

        if (biases->next != NULL) {
            biases = biases->next->next;
        }
    }
    free_tokens(json);
    return 0;
}

int insert_weights_to_nn(NN nn, float **initial_weights, float **initial_bias) {
    for (size_t i = 0; i < nn.arch_count - 1; ++i) {
        Mat m = nn.ws[i];
        Row r = nn.bs[i];
        for (size_t j = 0; j < m.rows; ++j) {
            for (size_t k = 0; k < m.cols; ++k) {
                MAT_AT(m, j, k) = initial_weights[i][j * m.cols + k];
            }
        }
        for (size_t j = 0; j < r.cols; ++j) {
            ROW_AT(r, j) = initial_bias[i][j];
        }
    }
    return 0;
}

void init_nn(NN *nn, float **initial_weights, float **initial_bias) {
    if (nn->arch_count == 0) {
        *nn = nn_alloc(NULL, ARCH, ARRAY_LEN(ARCH));
    }
    if (initial_weights == NULL || initial_bias == NULL) {
        nn_rand(*nn, -1, 1);
    } else {
        insert_weights_to_nn(*nn, initial_weights, initial_bias);
    }
}

float accuracy(NN nn, Mat t) {
    NN_ASSERT(NN_INPUT(nn).cols + NN_OUTPUT(nn).cols == t.cols);
    size_t n = t.rows;

    float c = 0;
    for (size_t i = 0; i < n; ++i) {
        Row row = mat_row(t, i);
        Row x = row_slice(row, 0, NN_INPUT(nn).cols);
        Row y = row_slice(row, NN_INPUT(nn).cols, NN_OUTPUT(nn).cols);

        row_copy(NN_INPUT(nn), x);
        nn_forward(nn);
        size_t q = y.cols;
        float max = 0;
        size_t max_idx = 0;
        for (size_t j = 0; j < q; ++j) {
            if (ROW_AT(NN_OUTPUT(nn), j) > max) {
                max = ROW_AT(NN_OUTPUT(nn), j);
                max_idx = j;
            }
        }
        for (size_t j = 0; j < q; ++j) {
            if (ROW_AT(y, j) == 1) {
                if (j == max_idx) {
                    ++c;
                }
                break;
            }
        }
    }

    return c / n;
}

#endif // COMMON_IMPLEMENTATION

#endif // !COMMON_H_
