#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <set>
#include <cstdio>
#include <algorithm>
#include <tuple>

#define MAP_DIM 140
int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

class Region {
public:
    std::pair<int, int> position;
    std::vector<std::pair<int, int>> neighbors;
    char value;
    int area;
    int perimeter;
    int n_sides; // Number of unique sides
    const std::vector<std::string>& map;
    std::vector<std::tuple<char, int, std::vector<std::pair<int, int>>>> sides;


    // Constructor
    Region(const std::pair<int, int>& pos, char val, const std::vector<std::string>& m)
        : position(pos), value(val), map(m), area(0), perimeter(0), n_sides(0) {
        find_neighbors(pos);
        n_sides = sides.size();
        re_verify_sides();
        print_sides();

    }


    void find_neighbors(const std::pair<int, int>& pos) {
        // Boundary or already visited check
        if (pos.first < 0 || pos.first >= MAP_DIM || pos.second < 0 || pos.second >= MAP_DIM ||
            std::find(neighbors.begin(), neighbors.end(), pos) != neighbors.end()) {
            return;
        }

        // If the current position is part of the region
        if (map[pos.first][pos.second] == value) {
            neighbors.push_back(pos);
            area++;

            // Explore each of the 4 neighboring directions
            for (int d = 0; d < 4; ++d) {
                int ni = pos.first + directions[d][0];
                int nj = pos.second + directions[d][1];

                // Determine if this is a boundary (transition or out of bounds)
                if (ni < 0 || ni >= MAP_DIM || nj < 0 || nj >= MAP_DIM) {
                    // Add the boundary side
                    if (d < 2) { // Horizontal side (-)
                        add_side(sides, {pos.first, pos.second}, {ni, nj}, '-');
                    } else { // Vertical side (|)
                        add_side(sides, {pos.first, pos.second}, {ni, nj}, '|');
                    }
                    perimeter++; // Increment perimeter for exposed edges
                } else if (map[ni][nj] != value) {
                    if (d < 2) { // Horizontal side (-)
                        add_side(sides, {pos.first, pos.second}, {ni, nj}, '-');
                    } else { // Vertical side (|)
                        add_side(sides, {pos.first, pos.second}, {ni, nj}, '|');
                    }
                    perimeter++; // Increment perimeter for exposed edges
                    find_neighbors({ni, nj}); // Recursively find neighbors
                }else{
                    find_neighbors({ni, nj});
                }
            }
        }
    }

    void print_sides() {
        std::cout << "Sides: " << sides.size() << std::endl;
        for (const auto& side : sides) {
            char first = std::get<0>(side);
            int second = std::get<1>(side);
            std::vector<std::pair<int, int>> pt = std::get<2>(side);
            std::cout << "Type: " << first << "Row: " << second << " Including: ";
            for (const auto& p : pt){ 
                std::cout << "(" << p.first << ", " << p.second << "), ";
            }
            std::cout << std::endl;
        }
    }
    void re_verify_sides() {
        for (auto it1 = sides.begin(); it1 != sides.end(); ++it1) {
            for (auto it2 = sides.begin(); it2 != sides.end(); ) {
                // Skip comparing the same side
                if (it1 == it2) {
                    ++it2;
                    continue;
                }

                // Extract data from the sides
                char type1 = std::get<0>(*it1);
                int pos1 = std::get<1>(*it1);
                std::vector<std::pair<int, int>>& pts1 = std::get<2>(*it1);

                char type2 = std::get<0>(*it2);
                int pos2 = std::get<1>(*it2);
                std::vector<std::pair<int, int>>& pts2 = std::get<2>(*it2);

                // Check if the sides are of the same type and position
                if (type1 == type2 && pos1 == pos2) {
                    bool merged = false;

                    // Check for overlapping points between pts1 and pts2
                    for (const auto& pt1 : pts1) {
                        for (int d = 0; d < 4; ++d) { // Check all neighbors
                            int ni = pt1.first + directions[d][0];
                            int nj = pt1.second + directions[d][1];
                            std::pair<int, int> neighbor = {ni, nj};

                            if (std::find(pts2.begin(), pts2.end(), neighbor) != pts2.end()) {

                                // Merge pts2 into pts1
                                for (const auto& pt2 : pts2) {
                                    if (std::find(pts1.begin(), pts1.end(), pt2) == pts1.end()) {
                                        pts1.push_back(pt2);
                                    }
                                }
                                // Remove the redundant side (it2) after merging
                                it2 = sides.erase(it2);
                                merged = true;
                                n_sides--;
                                break;
                            }
                        }
                        if (merged) break;
                    }

                    // If we didn't merge, move to the next side
                    if (!merged) {
                        ++it2;
                    }
                } else {
                    ++it2; // Move to the next side if types/positions don't match
                }
            }
        }
    }



    void add_side(std::vector<std::tuple<char, int, std::vector<std::pair<int, int>>>>& sides,
                const std::pair<int, int>& c_pos, const std::pair<int, int>& n_pos, char type) {
        // Calculate the "end" position of the side based on the type (horizontal or vertical)
        int e_pos;
        if (type == '-') {
            e_pos = std::max(c_pos.first, n_pos.first);
        } else {
            e_pos = std::max(c_pos.second, n_pos.second);
        }


        // Check if the side already exists
        bool side_found = false;
        for (auto& side : sides) {
            char s_type = std::get<0>(side);
            int s_pos = std::get<1>(side);
            std::vector<std::pair<int, int>>& s_vals = std::get<2>(side);  // Access by reference
            if (s_type == type && s_pos == e_pos) {
                // Add the current position if not already present
                if (std::find(s_vals.begin(), s_vals.end(), c_pos) == s_vals.end()) { // if not already in s_vals
                    // check nbrs
                    if (type == '-'){
                        for (int d = 2; d < 4; ++d) {
                            int ni = c_pos.first + directions[d][0];
                            int nj = c_pos.second + directions[d][1];
                            std::pair<int,int> nb = {ni, nj};

                            if (std::find(s_vals.begin(), s_vals.end(), nb) != s_vals.end()){
                                // neighbor is in list
                                s_vals.push_back(c_pos);


                                std::get<2>(side) = s_vals;
                                side_found = true;
                                break;
                            }
                        }
                    }else{
                        for (int d = 0; d < 2; ++d) {
                            int ni = c_pos.first + directions[d][0];
                            int nj = c_pos.second + directions[d][1];
                            std::pair<int,int> nb = {ni, nj};
                            if (std::find(s_vals.begin(), s_vals.end(), nb) != s_vals.end()){
                                // neighbor is in list
                                s_vals.push_back(c_pos);
                                side_found = true;
                                break;
                            }
                        }
                    }
                    
                }
            }
        }

        // If the side doesn't exist, create a new side
        if (!side_found) {
            std::tuple<char, int, std::vector<std::pair<int, int>>> new_side = {type, e_pos, {c_pos}};
            sides.push_back(new_side);  // Add the new side to the list
        }
    }




    int get_price() {
        return area * n_sides; // Pricing logic based on area and number of sides
    }


};




void mark_visited(std::vector<std::string>& visited, const Region& r) {
    for (const auto& nb : r.neighbors) {
        visited[nb.first][nb.second] = '.';
    }
}

int get_fences(std::vector<std::string>& map, std::vector<std::string>& visited, int start_i, int start_j) {
    char curr = map[start_i][start_j];
    Region r({start_i, start_j}, curr, map);
    mark_visited(visited, r);
    int price = r.get_price();
    printf("%c price: %d (sides: %d)\n", curr, price, r.n_sides);
    return price;
}

int iterate_map(std::vector<std::string>& map) {
    std::vector<std::string> visited(MAP_DIM, std::string(MAP_DIM, ' '));
    int total_cost = 0;

    for (int i = 0; i < MAP_DIM; i++) {
        for (int j = 0; j < MAP_DIM; j++) {
            if (visited[i][j] != '.') {
                total_cost += get_fences(map, visited, i, j);
            }
        }
    }
    return total_cost;
}

int main() {
    const std::string fname = "day12_input.txt";
    std::vector<std::string> map;

    std::ifstream file(fname);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file");
    }

    std::string line;
    int l_num = 0;

    while (std::getline(file, line)) {
        if (line.length() != MAP_DIM) {
            std::cerr << "Unexpected line length: " << line.length() << " (expected " << MAP_DIM << ")\n";
            return EXIT_FAILURE;
        }

        map.push_back(line);
        l_num++;
        if (l_num >= MAP_DIM) {
            break;
        }
    }

    file.close();

    int output = iterate_map(map);
    std::cout << "Output: " << output << std::endl;

    return 0;
}
