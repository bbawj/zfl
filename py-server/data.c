#include <assert.h>
#include <stdio.h>

int reverseInt(int i) {
    unsigned char c1, c2, c3, c4;

    c1 = i & 255;
    c2 = (i >> 8) & 255;
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;

    return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
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

    fread((char *)&magic_number, sizeof(magic_number), 1, file);
    magic_number = reverseInt(magic_number);
    fread((char *)&number_of_images, sizeof(number_of_images), 1, file);
    number_of_images = reverseInt(number_of_images);
    fread((char *)&n_rows, sizeof(n_rows), 1, file);
    n_rows = reverseInt(n_rows);
    fread((char *)&n_cols, sizeof(n_cols), 1, file);
    n_cols = reverseInt(n_cols);

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

    FILE *main_data = fopen("./train-images-main", "w");
    FILE *main_label = fopen("./train-labels-main", "w");
    assert(main_data);
    assert(main_label);

    int main_count = number_of_images / 2;

    // fwrite((char *)&magic_number, sizeof(magic_number), 1, main_data);
    // fwrite((char *)&main_count, sizeof(number_of_images), 1, main_data);
    // fwrite((char *)&n_rows, sizeof(n_rows), 1, main_data);
    // fwrite((char *)&n_cols, sizeof(n_cols), 1, main_data);

    for (int i = 0; i < main_count; ++i) {
        unsigned char temp = 0;
        for (int r = 0; r < n_rows; ++r) {
            for (int c = 0; c < n_cols; ++c) {
                fread((char *)&temp, sizeof(temp), 1, file);
                fwrite((char *)&temp, sizeof(temp), 1, main_data);
            }
        }
        fread((char *)&temp, sizeof(temp), 1, label_file);
        fwrite((char *)&temp, sizeof(temp), 1, main_label);
    }
    fclose(main_data);
    fclose(main_label);

    int n_count = (number_of_images - main_count) / n_splits;
    for (int j = 0; j < n_splits; ++j) {
        char file_path[100];
        snprintf(file_path, sizeof(file_path), "./train-images-%d", j + 1);
        FILE *n_file = fopen(file_path, "w");
        assert(n_file);
        snprintf(file_path, sizeof(file_path), "./train-labels-%d", j + 1);
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

int main(void) { read_mnist(4); }
