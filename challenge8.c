#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAP_DIM 50
// #define MAP_DIM 12

void printMap(char map[MAP_DIM][MAP_DIM]) {
    for (int i = 0; i < MAP_DIM; i++) {
        for (int j=0; j<MAP_DIM; j++){
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}



int get_num_antinodes(char map[MAP_DIM][MAP_DIM]){
    int antinodes = 0;
    for (int i = 0; i < MAP_DIM; i++) {
        for (int j=0; j<MAP_DIM; j++){
            char ch = map[i][j];
            if ((ch >= 'A' && ch <= 'Z')||(ch >= 'a' && ch <= 'z')||(ch >= '0' && ch <= '9')){

                for (int m = 0; m < MAP_DIM; m++) {
                    for (int n=0; n<MAP_DIM; n++){
                        if ((map[m][n] == ch)&&((m!=i)||(n!=j))){

                            for (int k=1; k<MAP_DIM; k++){

                                int dx = (n - j)*k;
                                int dy = (m - i)*k;
                                if (dx>0){
                                    if ((j - dx >=0)&&(i - dy>=0)){
                                        if (map[i - dy][j - dx]== '.'){
                                            map[i - dy][j - dx] = '#';
                                            antinodes++;}
                                        // }else if (map[i - dy][j - dx]!= '#'){
                                        //     antinodes++;
                                        // }
                                    }
                                    if ((n + dx <MAP_DIM)&&(m + dy<MAP_DIM)){
                                        if (map[m+ dy][n + dx]== '.'){
                                            map[m+ dy][n + dx] = '#';
                                            antinodes++;}
                                        // }else if (map[m+ dy][n + dx]!= '#'){
                                        //     antinodes++;
                                        // }
                                    }
                                }else{
                                    if ((n - abs(dx) >=0)&&(m + dy < MAP_DIM)){
                                        if (map[m + dy][n - abs(dx)]== '.'){
                                            map[m + dy][n - abs(dx)] = '#';
                                            antinodes++;}
                                        // }else if (map[m + dy][n - abs(dx)]!= '#'){
                                        //     antinodes++;
                                        // }
                                    }
                                    if ((j + abs(dx) <MAP_DIM)&&(i - dy>=0)){
                                        if (map[i - dy][j + abs(dx)]== '.'){
                                            map[i - dy][j + abs(dx)] = '#';
                                            antinodes++;}
                                        // }else if (map[i - dy][j + abs(dx)]!= '#'){
                                        //     antinodes++;
                                        // }
                                    }
                                }
                            }
                        }
                    }
                }
                map[i][j] = '#';
                antinodes++;
            }
        }
    }
    return antinodes;

}


int main() {
    const char *fname = "day8_input.txt";

    char map[MAP_DIM][MAP_DIM];

    FILE *file = fopen(fname, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAP_DIM+3];
    int l_num = 0;

    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';
        strcpy(map[l_num], line);

        l_num++;
        if (l_num >=MAP_DIM){
            break;
        }
    }
    fclose(file);


    int cell_c = get_num_antinodes(map);

    printf("out (%d)\n", cell_c);


    return 0;
}