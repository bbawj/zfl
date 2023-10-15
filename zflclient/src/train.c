#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zflclient);
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

Mat training_data(Region *r, int from, int to) {
    assert(from < to);
    assert(to <= NUM_IMAGES);

    Mat mat_train = mat_alloc(r, to - from, IMG_SIZE + 10);

    for (int image = from; image < to; ++image) {
        for (int row = 0; row < IMG_HEIGHT; ++row) {
            for (int col = 0; col < IMG_WIDTH; ++col) {
                float pixel =
                    train_images[image * IMG_SIZE + row * IMG_WIDTH + col] /
                    255.0f;
                MAT_AT(mat_train, image, row * IMG_WIDTH + col) = pixel;
            }
        }
        for (int i = 0; i < OUTPUT; ++i) {
            if (i == train_labels[image]) {
                MAT_AT(mat_train, image, IMG_SIZE + train_labels[image]) = 1.0f;
            } else {
                MAT_AT(mat_train, image, IMG_SIZE + i) = 0.0f;
            }
        }
    }
    return mat_train;
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
        int max_idx = 0;
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

void train(float **initial_weights, float **initial_bias) {
    printf("Training image size is %zu\n", sizeof(train_images));
    printf("Size of labels is %zu\n", sizeof(train_labels));

    float rate = 1.0f;
    Region r = region_alloc_alloc(1024 * 1024);
    NN nn = nn_alloc(&r, ARCH, ARRAY_LEN(ARCH));

    if (initial_weights == NULL || initial_bias == NULL) {
        nn_rand(nn, -1, 1);
    } else {
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
    }

    printf("NN setup!\n");

    Region sample_region = region_alloc_alloc(25 * 1024 * 1024);
    Mat samples = training_data(&sample_region, 0, NUM_IMAGES);
    for (int epoch = 0; epoch < NUM_EPOCHS; ++epoch) {
        printf("Epoch %d\n", epoch);
        float acc = 0.0f;
        for (int batch = 0; batch < NUM_BATCHES; ++batch) {
            size_t from = batch * BATCH_SIZE;
            Mat sample = mat_slice(samples, from, BATCH_SIZE);
            Region temp = region_alloc_alloc(1024 * 1024);
            NN g = nn_backprop(&temp, nn, sample);
            nn_learn(nn, g, rate);
            k_free(temp.words);
            float acc_temp = accuracy(nn, sample);
            acc += acc_temp;
        }
        printf("Epoch %d. Accuracy: %f\n", epoch, acc / NUM_BATCHES);
    }
    k_free(sample_region.words);
    k_free(r.words);
}
