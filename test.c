#include "stdlib.h"
#include <time.h>
#define IS_SERVER
#define NN_BACKPROP_TRADITIONAL
#define NN_IMPLEMENTATION
#define CSON_IMPLEMENTATION
#define SB_IMPLEMENTATION
#define COMMON_IMPLEMENTATION
#define TRAIN_IMPLEMENTATION
#include "./zflclient/src/train.h"
#include "errno.h"

Trainer TRAINER = {0};
int NUM_EPOCHS = 100;
int BATCH_SIZE = 100;

int train_labels_size = 60000;
int train_images_size = 60000;

int main(void) {
    srand(time(0));
    TRAINER.n_images = 60000;
    StringBuilder images = {0};
    sb_init(&images, 1024);
    printf("INFO: opening test image!\n");
    sb_open_file(&images, "./data/test-images");
    StringBuilder labels = {0};
    sb_init(&labels, 1024);

    printf("INFO: opening test label!\n");

    sb_open_file(&labels, "./data/test-labels");

    printf("INFO: image: %zu, labels: %zu\n", images.size, labels.size);
    assert(images.size / IMG_SIZE == labels.size);

    printf("INFO: opening train image!\n");
    StringBuilder train_images = {0};
    sb_init(&train_images, 1024);
    sb_open_file(&train_images, "./data/train-images-main");
    StringBuilder train_labels = {0};
    sb_init(&train_labels, 1024);
    printf("INFO: opening train label!\n");
    sb_open_file(&train_labels, "./data/train-labels-main");

    Mat test_set = init_train_set(images.data, labels.data, 10000);
    TRAINER.samples =
        init_train_set(train_images.data, train_labels.data, 60000);

    init_nn(&TRAINER.model, NULL, NULL);
    train(&TRAINER);

    float acc = accuracy(TRAINER.model, test_set);
    printf("INFO: final model accuracy against test set is %f\n", acc);
}
