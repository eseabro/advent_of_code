#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 140
#define MAX_COLS 140

int countString(const char *haystack, const char *needle) {
    int count = 0;
    const char *tmp = haystack;
    // printf("Searching for pattern '%s' in string '%s'\n", needle, haystack);
    while ((tmp = strstr(tmp, needle)) != NULL) {
        count++;
        tmp ++;  // Move past the full needle
    }
    return count;
}

void printMatrix(char matrix[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j=0; j<cols; j++){
            printf("%c", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose(char matrix[MAX_ROWS][MAX_COLS], int rows, int cols, char transposed[MAX_COLS+1][MAX_ROWS+1]) {
    // Perform the transposition: convert rows into columns
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            transposed[j][i] = matrix[i][j];  // Flip rows and columns
        }
    }

    // Null-terminate each row in the transposed matrix for string printing
    for (int i = 0; i < cols; i++) {
        transposed[i][rows] = '\0';  // Ensure proper null-termination for each row
    }
}

void extract_submatrix(char matrix[MAX_ROWS][MAX_COLS], char submatrix[3][3], int i, int j) {
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            submatrix[x][y] = matrix[i + x][j + y];
        }
    }
}


int check_xmas(char matrix[MAX_ROWS][MAX_COLS]){
    int outcount = 0;
    for (int i = 0; i < MAX_ROWS - 2; i++) {
        for (int j = 0; j < MAX_COLS - 2; j++) {
            char submatrix[3][3];
            extract_submatrix(matrix, submatrix, i, j);


            if ((submatrix[0][0]=='M')&&
                (submatrix[2][0]=='M')&&
                (submatrix[1][1]=='A')&&
                (submatrix[2][2]=='S')&&
                (submatrix[0][2]=='S')){
                outcount += 1;
            }
            else if ((submatrix[0][0] == 'S') && 
                     (submatrix[2][0] == 'M') && 
                     (submatrix[1][1] == 'A') && 
                     (submatrix[2][2] == 'M') && 
                     (submatrix[0][2] == 'S')) {
                outcount += 1;
            }
            else if ((submatrix[0][0] == 'S') && 
                     (submatrix[2][0] == 'S') && 
                     (submatrix[1][1] == 'A') && 
                     (submatrix[2][2] == 'M') && 
                     (submatrix[0][2] == 'M')) {
                outcount += 1;
            }
            else if ((submatrix[0][0] == 'M') && 
                     (submatrix[2][0] == 'S') && 
                     (submatrix[1][1] == 'A') && 
                     (submatrix[2][2] == 'S') && 
                     (submatrix[0][2] == 'M')) {
                outcount += 1;
            }
        }
    }
    return outcount;
}






void strrev(char* str)
// From GeeksforGeeks
{
    // if the string is empty
    if (!str) {
        return;
    }
    // pointer to start and end at the string
    int i = 0;
    int j = strlen(str) - 1;

    // reversing string
    while (i < j) {
        char c = str[i];
        str[i] = str[j];
        str[j] = c;
        i++;
        j--;
    }
}

void strrev_exclude_last(char *str) {
    if (!str || strlen(str) < 2) {
        return;  // Nothing to do if string is empty or has only one character
    }

    int i = 0;
    int j = strlen(str)-1;  // Exclude the last character (0-based index)

    while (i <= j) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}


int countDiagonalPatterns(char matrix[MAX_ROWS][MAX_COLS], int rows, int cols, const char *pattern) {
    char diagonal[MAX_ROWS + MAX_COLS];
    int count = 0;

    // Main diagonals (top-left to bottom-right)
    for (int start = 0; start < rows; start++) {
        int len = 0;
        for (int i = start, j = 0; i < rows && j < cols; i++, j++) {
            diagonal[len++] = matrix[i][j];
        }
        diagonal[len] = '\0';
        count += countString(diagonal, pattern);
        strrev(diagonal);
        count += countString(diagonal, pattern);
    }
    for (int start = 1; start < cols; start++) {
        int len = 0;
        for (int i = 0, j = start; i < rows && j < cols; i++, j++) {
            diagonal[len++] = matrix[i][j];
        }
        diagonal[len] = '\0';
        count += countString(diagonal, pattern);
        strrev(diagonal);
        count += countString(diagonal, pattern);
    }


    // Anti-diagonals (top-right to bottom-left)
    for (int start = 0; start < cols; start++) {
        int len = 0;
        for (int i = 0, j = start; i < rows && j >= 0; i++, j--) {
            diagonal[len++] = matrix[i][j];

        }
        diagonal[len] = '\0';
        count += countString(diagonal, pattern);
        strrev(diagonal);
        count += countString(diagonal, pattern);
    }

    for (int start = 1; start < rows; start++) {
        int len = 0;
        for (int i = start, j = cols - 1; i < rows && j >= 0; i++, j--)  {
            diagonal[len++] = matrix[i][j];
        }
        diagonal[len] = '\0';
        count += countString(diagonal, pattern);
        strrev(diagonal);
        count += countString(diagonal, pattern);
    }

    return count;
}

int main() {
    FILE *file = fopen("day4_input.txt", "r");

    const char *pattern = "XMAS";
    int output = 0;

    if (file == NULL) {
        return 1;
    }

    char matrix[MAX_ROWS][MAX_COLS] = {{0}};
    char transposed[MAX_COLS+1][MAX_ROWS+1] = {{0}};
    memset(transposed, 0, sizeof(transposed)); // Initialize to zero

    char line[MAX_COLS+1];
    int line_number = 1;

    while (fgets(line, sizeof(line), file)&&(line_number<MAX_ROWS+1)) {
        if (line[0] == '\n') {
            continue; // Skip empty lines
        }
        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) == 0) continue;
        char l1_cpy[MAX_COLS];
        strcpy(matrix[line_number-1], line);
        matrix[line_number-1][MAX_COLS] = '\0';

        memcpy(l1_cpy, line, sizeof(line));
        output += countString(matrix[line_number-1], pattern);
        strrev(l1_cpy);

        output += countString(l1_cpy, pattern);

        line_number++;

    }
    transpose(matrix, MAX_ROWS, MAX_COLS, transposed);


    for (int i = 0; i < line_number; i++) {
        char l1_cpy[MAX_ROWS];
        output += countString(transposed[i], pattern);

        memcpy(l1_cpy, transposed[i], MAX_ROWS);
        strrev_exclude_last(l1_cpy);

        output += countString(l1_cpy, pattern);
    }

    output += countDiagonalPatterns(matrix, MAX_ROWS, MAX_COLS, pattern);
    printf("Out: %d\n", output);

    int out_mas = check_xmas(matrix);
    printf("MAS: %d\n",out_mas);

    fclose(file);
    return 0;
} 
