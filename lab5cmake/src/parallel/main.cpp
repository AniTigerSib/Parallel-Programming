#include "../common.hpp"
// #include <boost/mpi.hpp>
// #include <boost/mpi/collectives.hpp>
#include <boost/mpi/collectives/broadcast.hpp>
#include <boost/mpi/collectives/scatter.hpp>
#include <boost/mpi/collectives/gather.hpp>
#include <boost/serialization/vector.hpp>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <vector>

namespace mpi = boost::mpi;
using std::vector;
const static long double kEps = 1e-7;

struct vector_concat {
    template<typename T>
    std::vector<T> operator()(const std::vector<T>& a,
                              const std::vector<T>& b) const {
        std::vector<T> result = a;
        result.insert(result.end(), b.begin(), b.end());
        return result;
    }
};

int main(int argc, char* argv[]) {
    mpi::environment env(argc, argv);
    mpi::communicator world;

    bool isRevert = false;
    vector<custom_vector> resTempVector;
    custom_vector localResVector;
    custom_vector vectorBase;
    vector<custom_vector> matrixLocal;
    vector<custom_vector> matrixBase;
    vector<vector<custom_vector>> matrixBaseSent;

    const int kSize = world.size();
    const int kPackSize = kVectorSize / kSize;
    const int kLasts = kVectorSize - kSize * kPackSize;
    auto start = std::chrono::high_resolution_clock::now();
    if (world.rank() == 0) {
        vectorBase = std::move(get_filled_vector(kVectorSize));
        matrixBase = std::move(get_filled_matrix(kVectorSize));
        for (int i = 0; i < kSize; ++i) {
            auto itStart = matrixBase.begin() + i * kPackSize;
            auto itEnd = itStart + kPackSize;
            matrixBaseSent.push_back(std::move(vector<custom_vector>(itStart, itEnd)));
        }
        if (kLasts != 0) {
            matrixBaseSent.back().insert(matrixBaseSent.back().end(), matrixBase.begin() + kSize * kPackSize, matrixBase.end());
        }
    }
    mpi::scatter(world, matrixBaseSent, matrixLocal, 0);

    localResVector.resize(matrixLocal.size());
    for (int iter = 0; iter < kIterations; ++iter) {
        mpi::broadcast(world, vectorBase, 0);
        for (size_t i = 0; i < matrixLocal.size(); ++i) {
            for (size_t j = 0; j < vectorBase.size(); ++j) {
                localResVector[i] = vectorBase[j] * matrixLocal[i][j];
            }
        }
        mpi::gather(world, localResVector, resTempVector, 0);
        if (world.rank() == 0) {
            size_t i = 0;
            for (auto vect : resTempVector) {
                std::copy(vect.begin(), vect.end(), vectorBase.begin() + i);
                i += vect.size();
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    if (world.rank() == 0) {
        auto res_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << std::format("Result got in {} milliseconds", res_time) << std::endl;
    }

    return 0;
}