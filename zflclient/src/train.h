#ifndef TRAIN_H_
#define TRAIN_H_

#include "../../common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int n_images;
    Mat samples;
    NN model;

    bool samples_ready;
} Trainer;

extern int NUM_EPOCHS;
extern int BATCH_SIZE;

void train(Trainer *t);
#ifdef TRAIN_IMPLEMENTATION
void train(Trainer *t) {
    int NUM_BATCHES = t->n_images / BATCH_SIZE;
    float rate = 0.1f;

    for (int epoch = 1; epoch <= NUM_EPOCHS; ++epoch) {
        // printf("Epoch %d\n", epoch);
        float acc = 0.0;
        float x = 0.f;
        for (int batch = 0; batch < NUM_BATCHES; ++batch) {
            size_t from = batch * BATCH_SIZE;
            Mat sample = mat_slice(t->samples, from, BATCH_SIZE);
            Region temp = region_alloc_alloc(500 * 1024);
            NN g = nn_backprop(&temp, t->model, sample);
            nn_learn(t->model, g, rate);
#ifdef IS_SERVER
            free(temp.words);
#else
            k_free(temp.words);
#endif // IS_SERVER
            if (epoch % 10 == 0) {
                acc += nn_cross_entropy(t->model, sample);
                x += accuracy(t->model, sample);
            }
        }
        if (epoch % 10 == 0)
            printf("Epoch %d. Accuracy: %f, Loss: %f\n", epoch, x / NUM_BATCHES,
                   acc / NUM_BATCHES);
    }
}
#endif // TRAIN_IMPLEMENTATION
#ifdef __cplusplus
}
#endif
#endif // !TRAIN_H_
