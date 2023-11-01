#ifndef TRAIN_H_
#define TRAIN_H_

#include "common.h"
#include "sb.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/fs/ext2.h>
#include <zephyr/fs/fs.h>
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/shell/shell.h>

#define NUM_EPOCHS 10
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int n_images;
    Mat samples;
    NN model;

    bool samples_ready;
    bool model_ready;
} Trainer;

void train(Trainer *trainer);
char *weights_to_string(StringBuilder *sb, Trainer *t);

#ifdef __cplusplus
}
#endif
#endif // !TRAIN_H_
