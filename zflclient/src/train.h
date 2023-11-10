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
#ifdef __cplusplus
}
#endif
#endif // !TRAIN_H_
