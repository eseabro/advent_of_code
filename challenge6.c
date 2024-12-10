#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define MAP_DIM 10
#define MAP_DIM 130

void printMap(char map[MAP_DIM][MAP_DIM]) {
    for (int i = 0; i < MAP_DIM; i++) {
        for (int j=0; j<MAP_DIM; j++){
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}

int propagate_map(char map[MAP_DIM][MAP_DIM], int *g_pos_x, int *g_pos_y, int *dir){
    if (*dir==0){
        if (map[*g_pos_y-1][*g_pos_x]=='#'){
            *dir=1;
        }else{
            map[*g_pos_y][*g_pos_x] = 'X';
            *g_pos_y = *g_pos_y - 1;
            if (*g_pos_y < 0){
                return 1;
            }
            map[*g_pos_y][*g_pos_x] = '^';
        }
        return 1;
    }else if(*dir==1){
        if (map[*g_pos_y][*g_pos_x+1]=='#'){
            *dir=2;
        }else{
            map[*g_pos_y][*g_pos_x] = 'X';
            *g_pos_x = *g_pos_x + 1;
            if (*g_pos_x > MAP_DIM){
                return 1;
            }
            map[*g_pos_y][*g_pos_x] = '>';
        }
        return 1;
    }else if(*dir==2){
        if (map[*g_pos_y+1][*g_pos_x]=='#'){
            *dir=3;
        }else{
            map[*g_pos_y][*g_pos_x] = 'X';
            *g_pos_y = *g_pos_y + 1;
            if (*g_pos_y > MAP_DIM){
                return 1;
            }
            map[*g_pos_y][*g_pos_x] = 'v';
        }
        return 1;
    }else if(*dir==3){
        if (map[*g_pos_y][*g_pos_x-1]=='#'){
            *dir=0;
        }else{
            map[*g_pos_y][*g_pos_x] = 'X';
            *g_pos_x = *g_pos_x - 1;
            if (*g_pos_x < 0){
                return 1;
            }
            map[*g_pos_y][*g_pos_x] = '<';
        }
        return 1;
    }else{
        return 0;
    }
}


int get_cell_count(char map[MAP_DIM][MAP_DIM], int g_pos_x, int g_pos_y, int dir){
    int visited[MAP_DIM][MAP_DIM][4] = {{{0}}};
    while ((g_pos_x < MAP_DIM-1)&&(g_pos_y < MAP_DIM-1)&&(g_pos_x>0)&&(g_pos_y>0)){
        if (visited[g_pos_y][g_pos_x][dir]) {
            return -1; // Loop detected
        }
        visited[g_pos_y][g_pos_x][dir] = 1;
        if (!propagate_map(map, &g_pos_x, &g_pos_y, &dir)){
            printf("ERROR");
            break;
        }
    }
    int visited_count = 0;
    for (int i = 0; i < MAP_DIM; i++) {
        for (int j=0; j<MAP_DIM; j++){
            if (map[i][j] == 'X'){
                visited_count++;
            };
        }
    }
    return visited_count;
}

int check_obstructions(char map[MAP_DIM][MAP_DIM], int g_pos_x, int g_pos_y, int dir){
    int n_obs = 0;
    for (int i = 0; i < MAP_DIM; i++) {
        for (int j=0; j<MAP_DIM; j++){
            if (map[i][j] == '.'){
                char m_cpy[MAP_DIM][MAP_DIM] = {{0}};
                memcpy(m_cpy, map, MAP_DIM * MAP_DIM * sizeof(char));
                m_cpy[i][j] = '#';
                int cc = get_cell_count(m_cpy, g_pos_x, g_pos_y, dir);
                if (cc==-1){
                    n_obs = n_obs + 1;
                }
            }
        }
    }
    return n_obs;
}

int main() {
    const char *fname = "day6_input.txt";

    char map[MAP_DIM][MAP_DIM];

    FILE *file = fopen(fname, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAP_DIM+3];
    int l_num = 0;
    int g_pos_x, g_pos_y;
    int dir; // up 0, right 1, down 2, left 3
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';
        strcpy(map[l_num], line);
        for (int i=0; i<MAP_DIM; i++){
            if ((line[i]=='v')){
                g_pos_x = i;
                g_pos_y = l_num;
                dir = 2;
                
            }else if((line[i]=='^')){
                g_pos_x = i;
                g_pos_y = l_num;
                dir = 0;
            }else if((line[i]=='>')){
                g_pos_x = i;
                g_pos_y = l_num;
                dir = 1;
            }else if((line[i]=='<')){
                g_pos_x = i;
                g_pos_y = l_num;
                dir = 3;
            }
        }
        l_num++;
        if (l_num >=MAP_DIM){
            break;
        }
    }

    // int out = get_cell_count(map, g_pos_x, g_pos_y, dir);
    int cell_c = check_obstructions(map, g_pos_x, g_pos_y, dir);
    // printMap(map);
    // printf("out (%d)\n", out);
    printf("out (%d)\n", cell_c);


    return 0;
}