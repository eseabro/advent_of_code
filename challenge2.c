#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 1000
#define MAX_COLS 10
#define TERMINATOR -1

int comp(const void* a, const void* b) {
      
    return (*(int*)a - *(int*)b);
}

int comp2(const void* a, const void* b) {
      
    return (*(int*)b - *(int*)a);
}

int check_margins(int e1, int e2){
    if ((abs(e2-e1)<4)&&(abs(e2-e1)>0)){
        return 0;
    }
    return 1;
}



int arraysEqual(int arr1[], int arr2[], int size1) {

    for (int i = 0; i < size1; i++) {
        if (arr1[i] != arr2[i]) {
            return 0;
        }else if((i>0)&&(check_margins(arr1[i-1], arr1[i]))){
            return 0;
        }
    }
    return 1; // All elements match
}

int remove_and_recompare(int list1[], int N){
    for (int j=0; j<N;j++){
        //remove one item from i
        int l1_cpy[MAX_COLS];
        memcpy(l1_cpy, list1, (N)*sizeof(int));
        for (int i = j; i <N-1; i++) {
            l1_cpy[i] = l1_cpy[i + 1];
        }
        int l_asc[MAX_COLS], l_desc[MAX_COLS];

        memcpy(l_asc, l1_cpy, (N-1)*sizeof(int));
        memcpy(l_desc, l1_cpy, (N-1)*sizeof(int));

        qsort(l_asc, N-1, sizeof(int), comp);
        qsort(l_desc, N-1, sizeof(int), comp2);
        if (arraysEqual(l1_cpy, l_asc, N-1)||arraysEqual(l1_cpy, l_desc, N-1)){
            return 1;
        }
    }
    return 0;
}


int compare_and_sum(int list1[MAX_ROWS][MAX_COLS], int N){
    int output = 0;

    for (int i = 0; i < N; i++){
        int l_asc[MAX_COLS], l_desc[MAX_COLS];
                
        int n_cols = 0;
        while (list1[i][n_cols] != TERMINATOR && n_cols < MAX_COLS) {
            n_cols++;
        }

        memcpy(l_asc, list1[i], n_cols*sizeof(int));
        memcpy(l_desc, list1[i], n_cols*sizeof(int));

        qsort(l_asc, n_cols, sizeof(int), comp);
        qsort(l_desc, n_cols, sizeof(int), comp2);
        
        if (arraysEqual(list1[i], l_asc, n_cols)||arraysEqual(list1[i], l_desc, n_cols)){
            output++;
        }else{
            output = output + remove_and_recompare(list1[i], n_cols);
        }

    }
    return output;
}

int main() {
    FILE *file = fopen("day2_input.txt", "r");
    if (file == NULL) {
        printf("Error: Unable to open file\n");
        return 1;
    }
    int data[MAX_ROWS][MAX_COLS];     // Storage for the file's data
    int colCounts[MAX_ROWS] = {0};    // Store the number of columns in each row
    int rows = 5;

    char line[1024]; // Buffer to store each line
    int row = 0;

    while (fgets(line, sizeof(line), file) && row < MAX_ROWS) {
        int col = 0;
        char *token = strtok(line, " \t\n"); // Tokenize the line by spaces, tabs, and newline
        while (token != NULL && col < MAX_COLS) {
            data[row][col] = atoi(token); // Convert the token to an integer
            col++;
            token = strtok(NULL, " \t\n"); // Get the next token
        }
        data[row][col] = TERMINATOR; // Store the number of columns for this row
        row++;
    }

    fclose(file);


    int sum = compare_and_sum(data, row);
    printf("Output is: %d", sum);


    return 0;
}