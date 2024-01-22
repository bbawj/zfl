#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int reverseInt(int i) {
    unsigned char c1, c2, c3, c4;

    c1 = i & 255;
    c2 = (i >> 8) & 255;
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;

    return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}

void magic(FILE *file, int *magic_number, int *number_of_images, int *n_rows,
           int *n_cols) {
    fread((char *)magic_number, sizeof(*magic_number), 1, file);
    *magic_number = reverseInt(*magic_number);
    fread((char *)number_of_images, sizeof(*number_of_images), 1, file);
    *number_of_images = reverseInt(*number_of_images);
    fread((char *)n_rows, sizeof(*n_rows), 1, file);
    *n_rows = reverseInt(*n_rows);
    fread((char *)n_cols, sizeof(*n_cols), 1, file);
    *n_cols = reverseInt(*n_cols);
    printf("magic: %d, number_of_images: %d, n_rows: %d, n_cols: %d\n",
           *magic_number, *number_of_images, *n_rows, *n_cols);
}

void shuffle(FILE *f_images, FILE *f_labels, size_t n_images,
             const size_t image_size) {
    printf("yeet\n");
    const size_t image_file_size = n_images * image_size;
    char *image_buffer = malloc(image_file_size);
    fread(image_buffer, image_file_size, 1, f_images);
    char *label_buffer = malloc(n_images);
    fread(label_buffer, n_images, 1, f_labels);

    char temp_label;
    char temp[image_size];
    for (size_t i = 0; i < n_images - 1; ++i) {
        int target = rand() % n_images;
        memmove(&temp_label, &label_buffer[i], 1);
        memmove(&label_buffer[i], temp, 1);

        memmove(temp, &image_buffer[i * image_size], image_size);
        memmove(&image_buffer[i * image_size],
                &image_buffer[target * image_size], image_size);
    }

    FILE *shuffled_images = fopen("./train-images-shuffled", "w");
    FILE *shuffled_labels = fopen("./train-labels-shuffled", "w");
    fwrite(image_buffer, n_images * image_size, 1, shuffled_images);
    fwrite(label_buffer, n_images, 1, shuffled_labels);
    fclose(shuffled_images);
    fclose(shuffled_labels);
    free(image_buffer);
    free(label_buffer);
}

void read_mnist_test() {
    FILE *file = fopen("./t10k-images.idx3-ubyte", "r");
    FILE *label_file = fopen("./t10k-labels-idx1-ubyte", "r");
    assert(file);
    assert(label_file);

    int magic_number = 0;
    int number_of_images = 0;
    int n_rows = 0;
    int n_cols = 0;
    magic(file, &magic_number, &number_of_images, &n_rows, &n_cols);

    printf("magic: %d, number_of_images: %d, n_rows: %d, n_cols: %d\n",
           magic_number, number_of_images, n_rows, n_cols);

    int label_magic_number = 0;
    int number_of_labels = 0;
    fread((char *)&label_magic_number, sizeof(label_magic_number), 1,
          label_file);
    label_magic_number = reverseInt(label_magic_number);
    fread((char *)&number_of_labels, sizeof(number_of_labels), 1, label_file);
    number_of_labels = reverseInt(number_of_labels);

    printf("magic: %d, number_of_labels: %d", label_magic_number,
           number_of_labels);

    shuffle(file, label_file, number_of_images, n_rows * n_cols);
    fclose(file);
    fclose(label_file);

    file = fopen("./train-images-shuffled", "r");
    label_file = fopen("./train-labels-shuffled", "r");
    assert(file);
    assert(label_file);

    FILE *test_data = fopen("../data/test-images", "w");
    FILE *test_label = fopen("../data/test-labels", "w");
    assert(test_data);
    assert(test_label);

    for (int i = 0; i < number_of_images; ++i) {
        unsigned char temp = 0;
        for (int r = 0; r < n_rows; ++r) {
            for (int c = 0; c < n_cols; ++c) {
                fread((char *)&temp, sizeof(temp), 1, file);
                fwrite((char *)&temp, sizeof(temp), 1, test_data);
            }
        }
        fread((char *)&temp, sizeof(temp), 1, label_file);
        fwrite((char *)&temp, sizeof(temp), 1, test_label);
    }
    fclose(test_data);
    fclose(test_label);
}

void read_mnist(int n_splits) {
    FILE *file = fopen("./train-images.idx3-ubyte", "r");
    FILE *label_file = fopen("./train-labels.idx1-ubyte", "r");
    assert(file);
    assert(label_file);

    int magic_number = 0;
    int number_of_images = 0;
    int n_rows = 0;
    int n_cols = 0;

    magic(file, &magic_number, &number_of_images, &n_rows, &n_cols);

    int label_magic_number = 0;
    int number_of_labels = 0;
    fread((char *)&label_magic_number, sizeof(label_magic_number), 1,
          label_file);
    label_magic_number = reverseInt(label_magic_number);
    fread((char *)&number_of_labels, sizeof(number_of_labels), 1, label_file);
    number_of_labels = reverseInt(number_of_labels);

    printf("magic: %d, number_of_labels: %d", label_magic_number,
           number_of_labels);

    shuffle(file, label_file, number_of_images, n_rows * n_cols);
    fclose(file);
    fclose(label_file);

    file = fopen("./train-images-shuffled", "r");
    label_file = fopen("./train-labels-shuffled", "r");
    assert(file);
    assert(label_file);

    int n_count = number_of_images / n_splits;
    for (int j = 0; j < n_splits; ++j) {
        char file_path[100];
        snprintf(file_path, sizeof(file_path), "./out/train-images-%d", j + 1);
        FILE *n_file = fopen(file_path, "w");
        assert(n_file);
        snprintf(file_path, sizeof(file_path), "./out/train-labels-%d", j + 1);
        FILE *n_label_file = fopen(file_path, "w");
        assert(n_label_file);

        for (int i = 0; i < n_count; ++i) {
            unsigned char temp = 0;
            for (int r = 0; r < n_rows; ++r) {
                for (int c = 0; c < n_cols; ++c) {
                    fread((char *)&temp, sizeof(temp), 1, file);
                    fwrite((char *)&temp, sizeof(temp), 1, n_file);
                }
            }
            fread((char *)&temp, sizeof(temp), 1, label_file);
            fwrite((char *)&temp, sizeof(temp), 1, n_label_file);
        }
        fclose(n_file);
        fclose(n_label_file);
    }
    fclose(file);
    fclose(label_file);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("USAGE:\n ./data [shuffle|test|train] <NUM_SPLITS>\n");
        return 1;
    }
    char *command = argv[1];

    if (strncmp(command, "test", strlen("test")) == 0) {
        read_mnist_test();
    } else if (strncmp(command, "train", strlen("train")) == 0) {
        int n_splits = atoi(argv[2]);
        read_mnist(n_splits);
    } else {
        printf("USAGE:\n ./data [test|train] <NUM_SPLITS>\n");
        return 1;
    }
}
