#include "../common.hpp"
#include <chrono>
#include <iostream>
#include <vector>

using std::vector;

int main(int argc, char* argv[]) {
    auto start = std::chrono::high_resolution_clock::now();
    custom_vector vectorBase = std::move(get_filled_vector(kVectorSize));
    custom_vector result(kVectorSize, 0);
    custom_vector *vct1 = &vectorBase;
    custom_vector *vct2 = &result;
    vector<custom_vector> matrixBase = std::move(get_filled_matrix(kVectorSize));

    for (int iter = 0; iter < kIterations; ++iter) {
        for (int i = 0; i < kVectorSize; ++i) {
            (*vct2)[i] = 0;
            for (int j = 0; j < kVectorSize; ++j) {
                (*vct2)[i] += (*vct1)[j] * matrixBase[i][j];
            }
        }
        if (iter % 2 == 0) {
            vct1 = &result;
            vct2 = &vectorBase;
        } else {
            vct2 = &result;
            vct1 = &vectorBase;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto res_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << std::format("Result got in {} milliseconds", res_time) << std::endl;

    return 0;
}