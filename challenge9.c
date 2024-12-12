#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN 19999
#define NFILES (LINE_LEN / 2)

double get_checksum(int arr[], int new_len) {
    double cs = 0;
    for (int i = 0; i < new_len; i++) {
        if (arr[i] != -1) {
            cs += i * arr[i];
        }
    }
    return cs;
}

void print_compressed_matrix(int arr[], int len) {
    int i = 0;

    for (int i=0; i < len; i++) {
        if (arr[i] == -1) {
            // Handle blank spaces
            printf(".");
        } else {
            printf("%d", arr[i]);
        }
    }
    printf("\n");
}


int move_r_l(int arr[], int len) {
    for (int j = len - 1; j >= 0; j--) {
        if (arr[j] != -1) { // Found a block
            int block_start = j;
            int block_char = arr[j];
            int block_len = 1;

            // Determine the size of the block
            while (j > 0 && arr[j - 1] == block_char) {
                block_len++;
                j--;
            }

            // Search for a blank space at or to the left of the block's start
            for (int i = 0; i <= block_start; i++) {
                if (arr[i] == -1) { // Found a blank space
                    int blank_len = 0; // Length of this blank space from arr_o
                    for (int n=0; n<len-i; n++){
                        if (arr[i+n]==-1){
                            blank_len++;
                        }else{
                            break;
                        }
                    }
                    if (block_len <= blank_len) { // Block fits
                        // print_compressed_matrix(arr, len);
                        // Move the block to the blank space
                        for (int k = 0; k < block_len; k++) {
                            arr[i + k] = block_char;
                            arr[block_start - k] = -1;
                        }
                        break; // Stop processing this block
                    }
                }
            }
        }
    }

    return len;
}



double get_blocks(int arr[LINE_LEN]) {
    int sum = 0;
    for (int i = 0; i < LINE_LEN; i++) {
        sum += arr[i];
    }

    int *new_matrix = (int *)malloc(sum * sizeof(int));
    if (!new_matrix) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    int bl[LINE_LEN / 2] = {0};
    int ind = 0;
    int c_i = 0;

    for (int i = 0; i < LINE_LEN; i++) {
        for (int j = 0; j < arr[i]; j++) {
            if (i % 2 == 0) {
                new_matrix[ind] = c_i;
            } else {
                new_matrix[ind] = -1;
            }
            ind++;
        }

        if (i % 2 == 0) {
            c_i++;
        } else {
            bl[i / 2] = arr[i];
        }
    }

    // printf("Initial Matrix: ");
    // print_compressed_matrix(new_matrix, sum);

    int new_len = move_r_l(new_matrix, sum);


    // printf("Compressed Matrix: ");
    // print_compressed_matrix(new_matrix, new_len);

    double checksum = get_checksum(new_matrix, new_len);
    free(new_matrix);
    return checksum;
}

int main() {
    const char *fname = "day9_input.txt";

    FILE *file = fopen(fname, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[LINE_LEN + 3];
    int input[LINE_LEN];
    int l_num = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';
        size_t line_length = strlen(line);

        if (line_length != LINE_LEN) {
            fprintf(stderr, "Unexpected line length: %zu (expected %d)\n", line_length, LINE_LEN);
            fclose(file);
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < LINE_LEN; i++) {
            input[i] = line[i] - '0';
        }

        l_num++;
        if (l_num >= 2) {
            break;
        }
    }
    fclose(file);

    double cell_c = get_blocks(input);
    printf("Checksum: %f\n", cell_c);

    return 0;
}
