#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <set>
#include <cstdio>
#include <algorithm>
#include <tuple>
#include <unordered_map>


#define MAP_DIM 7


std::unordered_map<char, std::pair<int,int>> moves = {
                {'^', {-1, 0}},
                {'v', {1, 0}},
                {'<', {0, -1}},
                {'>', {0, 1}}};


void printMap(std::vector<std::string>& map) {
    for (const auto& row : map) {
        std::cout << row;
        std::cout << '\n';
    }
    return;
}


std::pair<int,int> move_obj(std::vector<std::string>& map, std::pair<int, int> obj_pos, char move, char self) {
   
    int new_i = obj_pos.first + moves[move].first;
    int new_j = obj_pos.second + moves[move].second;

    if (map[new_i][new_j] == '#') {
        return obj_pos; // Can't move into a wall
    }

    if (map[new_i][new_j] == 'O') {
        move_obj(map, std::make_pair(new_i, new_j), move, 'O');
        
        if (map[new_i][new_j] == 'O') {
            return obj_pos; 
        }
    }

    map[new_i][new_j] = self;
    map[obj_pos.first][obj_pos.second] = '.'; 
    return std::make_pair(new_i, new_j);
}

std::pair<int, int> move_obj_new(std::vector<std::string>& map, std::pair<int, int> obj_pos, char move, char self) {
    int new_i = obj_pos.first + moves[move].first;
    int new_j = obj_pos.second + moves[move].second;

    // Boundary check
    if (new_i < 0 || new_i >= map.size() || new_j < 0 || new_j >= map[0].size()) {
        return obj_pos; // Can't move out of bounds
    }

    char destination = map[new_i][new_j];
    std::pair<int, int> d_pos = {new_i, new_j};

    // Handle wall
    if (destination == '#') {
        return obj_pos; // Can't move into a wall
    }

    // Handle empty space
    if (destination == '.') {
        map[new_i][new_j] = self;
        map[obj_pos.first][obj_pos.second] = '.';
        return {new_i, new_j};
    }

    // Handle paired objects: '[' and ']'
    if (destination == '[' || destination == ']') {
        int offset = (destination == '[') ? 1 : -1; // Direction of the paired object
        int paired_j = new_j + offset;

        // Ensure the paired object exists
        char paired_obj = map[new_i][paired_j];
        if (paired_obj != (destination == '[' ? ']' : '[')) {
            return obj_pos; // Invalid paired object
        }
        std::pair<int, int> paired_pos_o = {new_i, paired_j};
        std::pair<int, int> dest_pos_o = {new_i, new_j};
        // Attempt to move the paired object first
        std::pair<int, int> paired_pos = move_obj_new(map, paired_pos_o, move, paired_obj);

        if (paired_pos == paired_pos_o) {
            return obj_pos; // Paired object couldn't move
        }
        // move original object
        std::pair<int, int> dest_pos = move_obj_new(map, dest_pos_o, move, destination);

        if (dest_pos == dest_pos_o) {
            map[paired_pos.first][paired_pos.second] = '.';
            map[paired_pos_o.first][paired_pos_o.second] = paired_obj;
            return obj_pos; // Paired object couldn't move
        }

        // Move this object after paired object has moved
        map[new_i][new_j] = self;
        map[obj_pos.first][obj_pos.second] = '.';
        return {new_i, new_j};
    }

    return {0, 0};
}



int get_GPS(std::vector<std::string>& map){
    int GPS = 0;
    for (int i=0; i<MAP_DIM; i++){
        for (int j=0; j<MAP_DIM; j++){
            if (map[i][j] == 'O'){
                GPS += 100*i + j;
            }
        }
    }
    return GPS;
}

long long get_GPS_new(std::vector<std::string>& map){
    long long GPS = 0;
    for (int i=0; i<map.size(); i++){
        for (int j=0; j<map[i].size(); j++){
            if (map[i][j] == '['){
                GPS += 100*i +j;
            }
        }
    }
    return GPS;
}

std::vector<std::string> get_new_map(std::vector<std::string> map){
    int GPS = 0;
    std::vector<std::string> new_map;
    for (int i=0; i<MAP_DIM; i++){
        std::string row = "";
        for (int j=0; j<MAP_DIM; j++){
            if (map[i][j] == '#'){
                row = row + "##";
            }else if(map[i][j] == 'O'){
                row = row + "[]";
            }else if(map[i][j] == '.'){
                row = row + "..";
            }else if(map[i][j] == '@'){
                row = row + "@.";
            }else{
                std::cerr << "Unknown character!";
            }
        }
        new_map.push_back(row);
    }
    return new_map;
}


int main() {
    const std::string fname = "day15_test.txt";
    std::vector<std::string> map;
    std::string robot_moves;

    std::ifstream file(fname);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file");
    }

    std::string line;
    int l_num = 0;

    while (std::getline(file, line)) {
        line.erase(line.find_last_not_of("\r\n") + 1);

        if (line.length() > MAP_DIM){
            robot_moves = robot_moves + line;
        }else{
            map.push_back(line);
        }
        l_num++;
    }

    file.close();
    printMap(map);

    std::pair<int, int> c_pos;
    for (int i=0; i<MAP_DIM; i++){
        for (int j=0; j<MAP_DIM; j++){
            if (map[i][j] == '@'){
                c_pos = {i,j};
            }
        }
    }
    printf("current position: (%d, %d)\n", c_pos.first, c_pos.second);
    // Part 2
    std::vector<std::string> new_map = get_new_map(map);

    for (int i=0; i<robot_moves.length(); i++){
        c_pos = move_obj(map, c_pos, robot_moves[i], '@');
        // printMap(map);
        // std::cin.get();
    }
    int gps = get_GPS(map);
    printf("GPS: %d\n", gps);


    std::pair<int, int> n_pos;
    bool found = false;
    printMap(new_map);
    for (int i=0; i<new_map.size(); i++){
        for (int j=0; j<new_map[i].size(); j++){
            if (new_map[i][j] == '@'){
                n_pos = {i,j};
                found = true;
                break;
            }
        }
        if (found){
            break;
        }
    }
    printf("current position2: (%d, %d)\n", n_pos.first, n_pos.second);
    if (n_pos.first <= 0 || n_pos.second <= 0) {
        throw std::runtime_error("Robot position is invalid");
    }


    // Wider map 
    for (int i=0; i<robot_moves.length(); i++){
        // printf("going %c at %d\n", robot_moves[i], i);

        n_pos = move_obj_new(new_map, n_pos, robot_moves[i], '@');
        // printMap(new_map);
        // printf("\n");
        // std::cin.get();
    }

    printMap(new_map);
    long long gps2 = 0;
    std::cout << "calculating GPS" <<std::endl;
    gps2 = get_GPS_new(new_map);
    printf("GPS2: %lld\n", gps2);

    return 0;
}
