#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zflclient);
#define NN_IMPLEMENTATION
#define CSON_IMPLEMENTATION
#define SB_IMPLEMENTATION
#define COMMON_IMPLEMENTATION
#include "train.h"

void train(Trainer *t) {
    const int NUM_BATCHES = t->n_images / NUM_EPOCHS;
    const int BATCH_SIZE = t->n_images / NUM_BATCHES;
    float rate = 1.0f;

    for (int epoch = 0; epoch < NUM_EPOCHS; ++epoch) {
        printf("Epoch %d\n", epoch);
        float acc = 0.0f;
        for (int batch = 0; batch < NUM_BATCHES; ++batch) {
            size_t from = batch * BATCH_SIZE;
            Mat sample = mat_slice(t->samples, from, BATCH_SIZE);
            Region temp = region_alloc_alloc(1024 * 1024);
            NN g = nn_backprop(&temp, t->model, sample);
            nn_learn(t->model, g, rate);
            k_free(temp.words);
            float acc_temp = accuracy(t->model, sample);
            acc += acc_temp;
        }
        printf("Epoch %d. Accuracy: %f\n", epoch, acc / NUM_BATCHES);
    }
}

char *weights_to_string(StringBuilder *sb, Trainer *t) {
    sb_append(sb, "[", 1);
    for (int i = 0; i < t->model.arch_count - 1; ++i) {
        Mat w = t->model.ws[i];
        sb_append(sb, "[", 1);
        for (int j = 0; j < w.cols * w.rows; ++j) {
            if (j == w.cols * w.rows - 1) {
                sb_appendf(sb, "%f", w.elements[j]);
            } else {
                sb_appendf(sb, "%f,", w.elements[j]);
            }
        }
        sb_append(sb, "],", 2);
        sb_append(sb, "[", 1);
        Row b = t->model.bs[i];
        for (int j = 0; j < b.cols; ++j) {
            if (j == b.cols - 1) {
                sb_appendf(sb, "%f", w.elements[j]);
            } else {
                sb_appendf(sb, "%f,", w.elements[j]);
            }
        }
        if (i == t->model.arch_count - 2) {
            sb_append(sb, "]", 1);
        } else {
            sb_append(sb, "],", 2);
        }
    }
    sb_append(sb, "]", 1);
    return sb_string(sb);
}
