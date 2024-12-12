#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <map>
#include <cstdio>
#include <algorithm>

#define MAP_DIM 53

int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

class Trailhead;

class TrailNode {
public:
    std::pair<int, int> position; // (x, y) coordinates
    std::vector<TrailNode*> neighbors; // Neighboring nodes
    int value;  // Trail value (e.g., path number or state)
    const std::vector<std::string>& map;  // Reference to the map
    Trailhead* trailhead;

    // Constructor
    TrailNode(const std::pair<int, int>& pos, int val, const std::vector<std::string>& m, Trailhead* th)
        : position(pos), value(val), map(m), trailhead(th) {}

    // Add a neighboring node
    void addNeighbor(TrailNode* neighbor) {
        neighbors.push_back(neighbor);
    }

    // Display details of the trail node
    void display() const {
        std::cout << "TrailNode at (" << position.first << ", " << position.second << ")\n";
        std::cout << "Neighbors: " << neighbors.size() << "\n";
    }
};

class Trailhead {
public:
    std::pair<int, int> coord;  // (x, y) coordinates
    std::vector<TrailNode*> children;  // Child trail nodes (connected nodes)
    int trail_count;  // Number of trails originating from this trailhead
    std::vector<std::pair<int, int>> visited;

    // Constructor
    Trailhead(const std::pair<int, int>& start)
        : coord(start), trail_count(0) {}

    // Add a child node
    void addChild(TrailNode* child) {
        children.push_back(child);
    }

    // Display details of the trailhead
    void display() const {
        std::cout << "Trailhead at (" << coord.first << ", " << coord.second << ")\n";
        std::cout << "Initial trail count: " << trail_count << std::endl;
        std::cout << "Children: " << children.size() << std::endl << std::endl;
    }
};

void printMap(const std::vector<std::string>& map) {
    for (const auto& row : map) {
        std::cout << "--" << row << "--\n";
    }
}

std::vector<Trailhead*> getTrailheads(const std::vector<std::string>& map) {
    std::vector<Trailhead*> trailheads;
    int row_ind = 0;
    for (const auto& row : map) {
        int col_ind = 0;
        // Iterate over each character in the string (row)
        for (const auto& element : row) {
            if (element == '0') {
                auto* th = new Trailhead({row_ind, col_ind});
                for (int d = 0; d < 4; ++d) {
                        int ni = row_ind + directions[d][0];
                        int nj = col_ind + directions[d][1];
                        if ((ni >= 0 && ni < map.size() && nj >= 0 && nj < map[0].size()) &&
                            (map[ni][nj] == '1')) {
                            TrailNode* tn1 = new TrailNode({ni, nj}, 1, map, th);
                            th->addChild(tn1);
                        }
                    }
                trailheads.push_back(th);
            }
            col_ind++;
        }
        row_ind++;
    }
    return trailheads;
}

void explore_path(TrailNode& node) {
    int r = node.position.first;
    int c = node.position.second;
    printf("Node pos: %d, %d. Node val: %d\n", r, c, node.value);
    // node.trailhead->visited.push_back(node.position);
    if (node.value == 9){
        node.trailhead->trail_count++;
        printf("Point\n");
    }
    for (int d = 0; d < 4; ++d) {
        int ni = r + directions[d][0];
        int nj = c + directions[d][1];
        std::pair<int,int> curr = {ni, nj};
        std::vector<std::pair<int,int>> vv = node.trailhead->visited;
        if ((ni >= 0 && ni < node.map.size() && nj >= 0 && nj < node.map[0].size()) &&
            (node.map[ni][nj] - '0' == node.value + 1)) {
            TrailNode* tn1 = new TrailNode({ni, nj}, node.value + 1, node.map, node.trailhead);
            explore_path(*tn1);  // Recursively explore the path
        }
    }
}


void explore_trails(std::vector<Trailhead*>& trails) {
    for (auto* trail : trails) {
        printf("Trailhead position: %d, %d. ~~~~~~~~~~~~~~~~~~~~~~\n", trail->coord.first, trail->coord.second);;

        for (auto* node : trail->children) {
            explore_path(*node);  // Use reference for better performance and avoid unnecessary copies
        }
    }
}


int main() {
    const std::string fname = "day10_input.txt";
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
            std::cerr << "Row index: " << l_num << "\n";
            std::cerr << line << "\n";
            return EXIT_FAILURE;
        }

        map.push_back(line);
        l_num++;
        if (l_num >= MAP_DIM) {
            break;
        }
    }

    file.close();

    // printMap(map);

    auto trailheads = getTrailheads(map);  // Get all trailheads
    explore_trails(trailheads);  // Explore all the trails

    int total_trails = 0;
    for (const auto* th : trailheads) {
        std::cout << "Going to display trailhead:" << std::endl;
        th->display();
        total_trails += th->trail_count;
    }
    std::cout << "Output:" << total_trails << std::endl;




    return 0;
}
