#include "common.hpp"
#include <random>

using std::vector;

vector<int> fill_vector(int size, bool serial_data) {
    vector<int> v(size);
    if (serial_data) {
        for (int i = 0; i < size; ++i) {
            v[i] = i + 1;
        }
    } else {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);
        for (int i = 0; i < size; i++) {
            v[i] = dis(gen);
        }
    }
    return v;
}