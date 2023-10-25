// static const unsigned char train_images[] = {
// #include <train-images.bin.inc>
// };
//
// static const unsigned char train_labels[] = {
// #include <train-labels.bin.inc>
// };

#ifndef TRAIN_H_
#define TRAIN_H_

#include <stdlib.h>

#define IMG_HEIGHT 28
#define IMG_WIDTH 28
#define IMG_SIZE (IMG_HEIGHT * IMG_WIDTH)
#define NUM_EPOCHS 10
#define HIDDEN_LAYER 16
#define OUTPUT 10
#define ARCH_COUNT 3
#define ARCH                                                                   \
    (size_t[]) { IMG_SIZE, HIDDEN_LAYER, OUTPUT }

#ifdef __cplusplus
extern "C" {
#endif

extern const unsigned char *train_images;
extern const unsigned char *train_labels;
extern int train_images_size;
extern int train_labels_size;

void train(float **initial_weights, float **initial_bias);

#ifdef __cplusplus
}
#endif
#endif // !TRAIN_H_
