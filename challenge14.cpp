#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>
#include <queue>


// #define MAP_DIM 100
#define MAP_DIM_X 101
#define MAP_DIM_Y 103
int QX = MAP_DIM_X/2;
int QY = MAP_DIM_Y/2;

int dims[4][2][2] = {{{0, 0}, {QX, QY}}, 
                    {{QX+1, 0}, {MAP_DIM_X, QY}}, 
                    {{0, QY+1}, {QX, MAP_DIM_Y}}, 
                    {{QX+1, QY+1}, {MAP_DIM_X, MAP_DIM_Y}}};


struct Robot {
    int px, py;
    int p_x, p_y;
    long long vx, vy;
};


std::vector<std::vector<int>> add_to_map(std::vector<std::vector<int>>& map, Robot robo){
    map[robo.py][robo.px] += 1;
    return map;
}

std::vector<std::vector<int>> time_iterate(std::vector<std::vector<int>>& map, Robot& robo, int time){
    int new_py = ( robo.py + time*robo.vy) % MAP_DIM_Y;
    int new_px = ( robo.px + time*robo.vx) % MAP_DIM_X;
    while (new_px < 0) new_px += MAP_DIM_X;
    while (new_py < 0) new_py += MAP_DIM_Y;
    robo.p_x = new_px;
    robo.p_y = new_py;
    map[new_py][new_px] += 1;
    return map;
}

std::vector<int> count_by_quadrant(std::vector<std::vector<int>>& map){
    int safety = 1;
    std::vector<int> out;
    for (int d=0; d<4; d++){
        int q_sum = 0;
        for (int i = dims[d][0][1]; i < dims[d][1][1]; ++i) {
            for (int j = dims[d][0][0]; j < dims[d][1][0]; ++j) {
                q_sum += map[i][j];
            }
        }
        // printf("Q%d: %d\n", d, q_sum);
        safety *= q_sum;
        out.push_back(q_sum);
    }
    return out;

}


void printMap(std::vector<std::vector<int>>& map) {

    for (const auto& row : map) {
        for (const auto& tile : row) {
            if (tile == 0){
                std::cout<<'.';
            }else{
                std::cout << tile;
            }
        }
        std::cout << '\n';
    }

    return;
}

bool is_adjacent(const Robot& r1, const Robot& r2) {
    return std::abs(r1.p_x - r2.p_x) <= 1 && std::abs(r1.p_y - r2.p_y) <= 1;
}

bool are_robots_side_by_side(const std::vector<Robot>& robots) {
    int n = robots.size()*0.5;
    bool out = false;
    int visited=0;

    for (Robot robo1: robots){
        for (Robot robo2: robots){
            // test
            if ((robo1.p_x==robo2.p_x)&&(robo1.p_y==robo2.p_y)){
                continue;
            }
            if (abs(robo1.p_x-robo2.p_x)<=1&&abs(robo1.p_y-robo2.p_y)<=1){
                visited++;
            }
        }
    }
    if (visited/4 >= n){
        out = true;
    }


    return out;
}


int main() {
    const std::string fname = "day14_input.txt";

    std::ifstream file(fname);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file");
    }


    std::string line;
    std::vector<Robot> results;
    int l_num = 0;
    // Regular expressions for each line format
    std::regex const rr{R"~(p=(\d+),(\d+) v=(-?\d+),(-?\d+))~"};


    Robot current;
    long long tokens=0;
    while(std::getline(file, line)){
        std::smatch match;
        line.erase(0, line.find_first_not_of(" \t")); // Remove leading spaces
        line.erase(line.find_last_not_of(" \t") + 1); // Remove trailing spaces
        line.erase(line.find_last_not_of("\r\n") + 1);


        if (std::regex_match(line, match, rr)) {
            current.px = std::stoll(match[1].str());
            current.py = std::stoll(match[2].str());
            current.p_x = 0;
            current.p_y = 0;
            current.vx = std::stoll(match[3].str());
            current.vy = std::stoll(match[4].str());
            results.push_back(current);
        }

        l_num++;

    }
    file.close();

    std::vector<std::vector<int>> map(MAP_DIM_Y, std::vector<int>(MAP_DIM_X, 0));

    int time = 10000;
    for (int t=0; t<time; t++){
        std::vector<std::vector<int>> map(MAP_DIM_Y, std::vector<int>(MAP_DIM_X, 0));
        for (Robot& robo: results){
            // add_to_map(map, robo);
            map = time_iterate(map, robo, t);
        }
        bool pp = are_robots_side_by_side(results);

        if (pp){
            printMap(map);
            printf("\n\n");
            printf("time: %d", t);
            std::cin.get();

        }
    }



    // printf("Safety: %d\n", out);

    return 0;
}


