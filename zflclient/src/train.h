#ifndef TRAIN_H_
#define TRAIN_H_

#include "../../common.h"

#define NUM_EPOCHS 1
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int n_images;
    Mat samples;
    NN model;

    bool samples_ready;
} Trainer;

void train(Trainer *t);
#ifdef __cplusplus
}
#endif
#endif // !TRAIN_H_
