#include "../common.hpp"
#include <chrono>
#include <format>
#include <iostream>
#include <vector>

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> v_serl = std::move(fill_vector(kVectorSize, true));
    std::vector<int> v_rand = std::move(fill_vector(kVectorSize, false));
    
    long long sum = 0ll;
    for (int i = 0; i < kVectorSize; ++i) {
        sum += v_serl[i] * v_rand[i];
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto res_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << std::format("Result: {}\nGot in {} milliseconds", sum, res_time) << std::endl;
}