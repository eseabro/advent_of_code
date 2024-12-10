#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// #define IN_LEN 10
#define IN_LEN 852
#define MAX_LINE 70
#define MAX_INTS 25

int check_operations(double sum, int data[MAX_INTS], int data_len, int depth, double c_sum){
    if (depth == data_len) {
        return (c_sum == sum) ? 1 : 0;
    }

    double c_sum1 = c_sum + data[depth];
    double c_sum2 = c_sum * data[depth];

    int digits = (int)log10(data[depth]) + 1;
    double concat = c_sum * pow(10, digits) + data[depth];

    int result1 = check_operations(sum, data, data_len, depth+1, c_sum1);
    int result2 = check_operations(sum, data, data_len, depth+1, c_sum2);
    int result3 = check_operations(sum, data, data_len, depth+1, concat);

    return (result1||result2||result3);

}



int main() {
    const char *fname = "day7_input.txt";

    FILE *file = fopen(fname, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE];
    int l_num = 0;
    double correct = 0;

    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';
        char *token = strtok(line, ": \t"); // tokenize by :, space, tab, newline
        double sum = atof(token);
        token = strtok(NULL, " \t\n");
        int data[MAX_INTS];
        int c_int = 0;

        while (token != NULL && c_int < MAX_INTS) {

            data[c_int] = atoi(token);
            token = strtok(NULL, " \t\n");
            c_int++;
        }

        if (check_operations(sum, data, c_int, 0, 0)){
            correct = correct + sum;
        }

        l_num++;
    }
    fclose(file);

    printf("out (%f)\n", correct);


    return 0;
}