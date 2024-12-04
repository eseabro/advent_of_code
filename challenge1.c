#include <stdio.h> 
#include <stdlib.h>

int comp(const void* a, const void* b) {
      
    return (*(int*)a - *(int*)b);
}

int freq(int *array, int size, int val)
{
    int freq = 0;
    while(size--) freq += *array++ == val;
    return freq;
}


int similarity(int list1[], int list2[], int N){
    int out_score = 0;
    for (int i=0;i<N; i++){
        int ff = freq(list2, N, list1[i]);
        out_score = out_score + (list1[i]*ff);
    }
    return out_score;
}

int compare(int list1[], int list2[], int N){
    int output = 0;
    for (int i = 0; i < N; i++){
        output = output + abs(list2[i] - list1[i]);
    }
    return output;
}

int main() {
    // read arrays
    FILE *file = fopen("day1_input.txt", "r");
    if (file == NULL) {
        printf("Error: Unable to open file\n");
        return 1;
    }

    int size = 1000;
    int l1[1000], l2[1000];
    int i = 0;

    // Read data into arrays
    while (fscanf(file, "%d %d", &l1[i], &l2[i]) == 2) {
        i++;
    }

    fclose(file);

    printf("f1: %d\n", l1[999]);

    // sort arrays
    qsort(l1, size, sizeof(int), comp);
    qsort(l2, size, sizeof(int), comp);

    // compare arrays return subtracted 
    printf("Output: %d\n", compare(l1, l2, size));
    printf("Similarity: %d\n", similarity(l1, l2, size));
    return 0;
}
