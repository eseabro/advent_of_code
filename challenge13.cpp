#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>

#define MAX_LEN 1280

struct Coordinates {
    long long a_x, a_y;
    long long b_x, b_y;
    long double p_x, p_y;
};


void print_coordinates(const Coordinates& coords) {
    std::cout << "Coordinates:" << std::endl;
    std::cout << "a_x: " << coords.a_x << ", a_y: " << coords.a_y << std::endl;
    std::cout << "b_x: " << coords.b_x << ", b_y: " << coords.b_y << std::endl;
    std::cout << "p_x: " << coords.p_x << ", p_y: " << coords.p_y << std::endl;
}


bool is_round(long double x) {
    const long double epsilon = 0.01; // Smallest difference
    return std::abs(x - std::round(x)) < epsilon;
}


// 29511 - 32096
long long get_tokens(Coordinates coords){
    long long a1 = coords.a_x;
    long long b1 = coords.b_x;
    long long a2 = coords.a_y;
    long long b2 = coords.b_y;
    long double c1 = coords.p_x;
    long double c2 = coords.p_y;
    long double delta = a1 * b2 - a2 * b1;

    if (delta == 0) {
        // No unique solution exists if determinant is zero
        return 0;
    }
    long double delta_x = c1 * b2 - c2 * b1;
    long double delta_y = a1 * c2 - a2 * c1;

    long double a = delta_x / delta;
    long double b = delta_y / delta;


    // long double a = (coords.p_x - coords.b_x*coords.p_y/coords.b_y)/(coords.a_x - coords.b_x*coords.a_y/coords.b_y);
    // long double b = (coords.p_y - coords.a_y*a)/coords.b_y;

    if (!is_round(a) || !is_round(b)) {
        // print_coordinates(coords);
        return 0;
    }

    // if (a>100 || b>100){
    //     return 0;
    // }
    long long tokens = static_cast<long long>(std::round(a)) * 3 + static_cast<long long>(std::round(b));

    return tokens;
}




int main() {
    const std::string fname = "day13_input.txt";

    std::ifstream file(fname);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file");
    }


    std::vector<Coordinates> results;
    std::string line;
    int l_num = 0;
    // Regular expressions for each line format
    std::regex const bA{R"~(Button A: X\+(\d+), Y\+(\d+))~"};
    std::regex const bB{R"(Button B: X\+(\d+), Y\+(\d+))"};
    std::regex const bP{R"(Prize: X=(\d+), Y=(\d+))"};

    Coordinates current;
    long long tokens=0;
    while(std::getline(file, line)){
        std::smatch match;
        line.erase(0, line.find_first_not_of(" \t")); // Remove leading spaces
        line.erase(line.find_last_not_of(" \t") + 1); // Remove trailing spaces
        line.erase(line.find_last_not_of("\r\n") + 1);


        if (std::regex_match(line, match, bA)) {
            current.a_x = std::stoll(match[1].str());
            current.a_y = std::stoll(match[2].str());
        } else if (std::regex_match(line, match, bB)) {
            current.b_x = std::stoll(match[1].str());
            current.b_y = std::stoll(match[2].str());
        } else if (std::regex_match(line, match, bP)) {
            current.p_x = std::stold(match[1].str()) + 10000000000000.0;
            current.p_y = std::stold(match[2].str()) + 10000000000000.0;
            results.push_back(current); // Add the completed set to the results
        }

        l_num++;

        if (l_num >= MAX_LEN) {
            break;
        }
    }
    int n_pos = 0;
    for (Coordinates machine: results){
        long long tt = get_tokens(machine);
        if (tt!= 0){
            n_pos++;
        }
        tokens += tt;

    }

    file.close();

    printf("Num tokens: %lld from %d machines\n", tokens, n_pos);

    return 0;
}


