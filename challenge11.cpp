#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdio>
#include <stdexcept>
#include <omp.h>
#include <cmath>
#include <unordered_map>


std::string blink(const std::string& input){
    std::string output;
    int len = input.length();

    // Case 1: Single character '0' -> return '1'
    if (len == 1 && input[0] == '0'){
        output = "1";
    }
    // Case 2: Even length string -> Split the string in half with a space between
    else if (len % 2 == 0){
        int cutoff = len / 2;
        long long n2 = std::stoi(input.substr(cutoff, len));
        output = input.substr(0, cutoff) + " " + std::to_string(n2);
    }
    // Case 3: Odd length string -> Convert string to double and multiply by 2024
    else {
        long long num = std::stoi(input);  // Try to convert the string to a double
        output = std::to_string(num * 2024);  // Multiply by 2024 and convert back to string

    }

    return output;
}

std::string parse_iter(std::string file_input){
    std::string out_stones = "";
    std::vector<std::string> stones;

    std::istringstream strstr(file_input);
    std::string stone;

    while (strstr >> stone) {
        stones.push_back(stone);
    }

    for (const auto& s : stones) {
        out_stones += blink(s);
        out_stones += " "; 
    }

    return out_stones;
}

int get_final_count(std::string final_stones){
    std::vector<std::string> stones;

    std::istringstream strstr(final_stones);
    std::string stone;

    // Split the string into words based on space
    while (strstr >> stone) {
        stones.push_back(stone);
    }

    // Return the count of stones (or words)
    return stones.size();
}

int numDigits(long long n) {
    int digits = 0;
    while (n > 0) {
        n /= 10;
        digits++;
    }
    return digits > 0 ? digits : 1; // Handles n=0 correctly
}

#include <unordered_map>
#include <vector>
#include <cmath>
#include <omp.h>

// Memoization map
std::unordered_map<long long, std::vector<long long>> memo;

// Optimized blink_int
std::vector<long long> blink_int(long long input) {
    // Check the memo map first
    #pragma omp critical
    if (memo.find(input) != memo.end()) {
        return memo[input];
    }

    // Perform the computation
    std::vector<long long> output;
    if (input == 0) {
        output.push_back(1);
    } else {
        int len = (int)std::log10(input) + 1;
        if (len % 2 == 0) {
            long long divider = std::pow(10, len / 2);
            output.push_back(input / divider);
            output.push_back(input % divider);
        } else {
            output.push_back(input * 2024);
        }
    }

    // Store the result in the memo map
    #pragma omp critical
    memo[input] = output;

    return output;
}


// Optimized recursive function
long long parse_iter_recursive(long long input, int c_iter, int max) {
    if (c_iter == max) return 1;


    long long n_stones = 0;
    std::vector<long long> blink_result = blink_int(input);

    // #pragma omp parallel for reduction(+:n_stones)
    for (size_t i = 0; i < blink_result.size(); ++i) {
        n_stones += parse_iter_recursive(blink_result[i], c_iter + 1, max);
    }

    return n_stones;
}


int main() {
    const std::string fname = "day11_input.txt";

    std::ifstream file(fname);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file");
    }

    std::string stones_input;
    int l_num = 0;

    // Read the first line from the file
    std::getline(file, stones_input);

    file.close();
    std::vector<std::string> stones;

    std::istringstream strstr(stones_input);
    std::string stone;

    while (strstr >> stone) {
        stones.push_back(stone);
    }

    // Apply `parse_iter` 25 times
    // for (int i = 0; i < 30; i++){
    //     stones_input = parse_iter(stones_input);
    //     int out = get_final_count(stones_input);
    //     std::cout << "Iteration:" << out << std::endl;
    // }
    long long output = 0;

    // Parallelize the loop
    #pragma omp parallel for reduction(+:output)
    for (int i = 0; i < stones.size(); ++i) {
        long long o = parse_iter_recursive(std::stoi(stones[i]), 0, 75);
        output += o;

        // To avoid race conditions with stdout, you may use a critical section or print after the loop
        printf("Done\n");
    }

    printf("N stones: %lld\n", output);


    // int out = get_final_count(stones_input);

    // Print the final count of stones
    // printf("N stones: %lld\n", output);

    return 0;
}


