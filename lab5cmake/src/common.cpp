#include "common.hpp"
#include <random>
#include <vector>

using std::vector;

custom_vector get_filled_vector(size_t n) {
    custom_vector v(n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(1, 100);
    for (int i = 0; i < n; ++i) {
        v[i] = dis(gen);
    }
    return v;
}

vector<custom_vector> get_filled_matrix(size_t n) {
    vector<custom_vector> v;
    for (int i = 0; i < n; ++i) {
        v.push_back(std::move(get_filled_vector(n)));
    }
    return v;
}