#include "train.h"
#include "zephyr/kernel.h"
#define NN_IMPLEMENTATION
#include "nn.h"
#include <string.h>

static const unsigned char train_images[] = {
#include <train-images.bin.inc>
};

static const unsigned char train_labels[] = {
#include <train-labels.bin.inc>
};

#define IMG_HEIGHT 28
#define IMG_WIDTH 28
#define IMG_SIZE (IMG_HEIGHT * IMG_WIDTH)
#define NUM_IMAGES 7500
#define NUM_EPOCHS 10
#define NUM_BATCHES (NUM_IMAGES / NUM_EPOCHS)
#define BATCH_SIZE (NUM_IMAGES / NUM_BATCHES)

Mat *training_data(Region *r, int from, int to) {
    assert(from < to);
    assert(to <= NUM_IMAGES);

    Mat *training_matrices = region_alloc(r, (to - from) * sizeof(Mat));
    assert(training_matrices);

    // printf("INFO: loading image %d to %d\n", from, to);
    for (int image = from; image < to; ++image) {
        Mat mat_train = mat_alloc(r, 1, IMG_SIZE + 10);
        for (int row = 0; row < IMG_HEIGHT; ++row) {
            for (int col = 0; col < IMG_WIDTH; ++col) {
                float pixel = train_images[row * IMG_HEIGHT + col] / 255.0f;
                MAT_AT(mat_train, 0, row * IMG_HEIGHT + col) = pixel;
            }
        }
        MAT_AT(mat_train, 0, IMG_SIZE + train_labels[image]) = 1;
        training_matrices[image - from] = mat_train;
    }
    return training_matrices;
}

void train() {
    printf("INFO: training image size is %zu\n", sizeof(train_images));
    int label = train_labels[0];
    printf("INFO: Size of labels is %zu\n", sizeof(train_labels));
    // printf("INFO: Label is %d\n", train_labels[0]);

    size_t arch[] = {IMG_SIZE, 16, 10};
    float rate = 1.0f;
    Region r = region_alloc_alloc(1024 * 1024);
    NN nn = nn_alloc(&r, arch, ARRAY_LEN(arch));
    nn_rand(nn, -1, 1);
    printf("INFO: NN setup!\n");

    Mat *first_sample = training_data(&r, 0, 1);

    Region sample_region = region_alloc_alloc(25 * 1024 * 1024);
    Mat *samples = training_data(&sample_region, 0, NUM_IMAGES);
    for (int epoch = 0; epoch < NUM_EPOCHS; ++epoch) {
        printf("INFO: Epoch %d\n", epoch);
        for (int batch = 0; batch < NUM_BATCHES; ++batch) {
            Region temp = region_alloc_alloc(1024 * 1024);
            for (int sample = 0; sample < BATCH_SIZE; ++sample) {
                int from = batch * BATCH_SIZE;
                NN g = nn_backprop(&temp, nn,
                                   samples[batch * BATCH_SIZE + sample]);
                nn_learn(nn, g, rate);
            }
            k_free(temp.words);
            // if (batch % 10 == 0 || batch == NUM_BATCHES - 1) {
            //     printf("Batch %d. Cost is: %f\n", batch, cost / BATCH_SIZE);
            // }
        }
        float cost = 0.0f;
        for (int sample = 0; sample < NUM_IMAGES; ++sample) {
            cost += nn_cost(nn, samples[sample]);
        }
        printf("Epoch %d. Cost: %f\n", epoch, cost / NUM_IMAGES);
    }

    // printf("INFO: output for label %d\n", label);
    // Mat t = mat_alloc(&r, 1, 10);
    // mat_fill(t, 0.0f);
    // // MAT_AT(t, 1, 3) = 1.0f;
    // Mat s = mat_alloc(&r, 1, IMG_SIZE);
    // for (int i = 0; i < IMG_SIZE; ++i) {
    //     MAT_AT(s, 1, i) = MAT_AT(first_sample[0], 1, i);
    // }
    //
    // mat_dot(t, s, nn.ws[0]);
    // mat_sum(t, row_as_mat(nn.bs[0]));
    // mat_print(t, "Matrix", 1);
}
