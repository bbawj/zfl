#define NN_IMPLEMENTATION
#define CSON_IMPLEMENTATION
#define SB_IMPLEMENTATION
#define COMMON_IMPLEMENTATION
#include "train.h"

void train(Trainer *t) {
    // printf("Sample cols: %zu, Model input: %zu, Model output %zu\n",
    //        t->samples.cols, NN_INPUT(t->model).cols,
    //        NN_OUTPUT(t->model).cols);
    int NUM_BATCHES = t->n_images / NUM_EPOCHS;
    int BATCH_SIZE = t->n_images / NUM_BATCHES;
    float rate = 1.0;

    for (int epoch = 0; epoch < NUM_EPOCHS; ++epoch) {
        printf("Epoch %d\n", epoch);
        float acc = 0.0;
        for (int batch = 0; batch < NUM_BATCHES; ++batch) {
            size_t from = batch * BATCH_SIZE;
            Mat sample = mat_slice(t->samples, from, BATCH_SIZE);
            Region temp = region_alloc_alloc(500 * 1024);
            NN g = nn_backprop(&temp, t->model, sample);
            nn_learn(t->model, g, rate);
            k_free(temp.words);
            float acc_temp = accuracy(t->model, sample);
            acc += acc_temp;
        }
        printf("Epoch %d. Accuracy: %f\n", epoch, acc / NUM_BATCHES);
    }
}
