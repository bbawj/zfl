#ifndef TRAIN_H_
#define TRAIN_H_

#define IMG_HEIGHT 28
#define IMG_WIDTH 28
#define IMG_SIZE (IMG_HEIGHT * IMG_WIDTH)
#define NUM_IMAGES 7500
#define NUM_EPOCHS 10
#define NUM_BATCHES (NUM_IMAGES / NUM_EPOCHS)
#define BATCH_SIZE (NUM_IMAGES / NUM_BATCHES)
#define HIDDEN_LAYER 16
#define OUTPUT 10
#define ARCH_COUNT 3
#define ARCH                                                                   \
    (size_t[]) { IMG_SIZE, HIDDEN_LAYER, OUTPUT }

#ifdef __cplusplus
extern "C" {
#endif

void train(float **initial_weights, float **initial_bias);

#ifdef __cplusplus
}
#endif
#endif // !TRAIN_H_
