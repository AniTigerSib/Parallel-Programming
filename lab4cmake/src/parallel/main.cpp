#include "../common.hpp"
#include <boost/mpi.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/serialization/vector.hpp>
#include <chrono>
#include <functional>
#include <iostream>

namespace mpi = boost::mpi;

int main(int argc, char* argv[]) {
    mpi::environment env(argc, argv);
    mpi::communicator world;

    std::vector<int> vSerlLocal;
    std::vector<int> vRandLocal;
    std::vector<int> vSerl;
    std::vector<int> vRand;
    std::vector<std::vector<int>> vSerlSent;
    std::vector<std::vector<int>> vRandSent;
    long long sum = 0;
    long long lSum = 0;

    const int kSize = world.size();
    const int packSize = kVectorSize / kSize;
    const int lasts = kVectorSize - kSize * packSize;
    auto start = std::chrono::high_resolution_clock::now();
    if (world.rank() == 0) {
        vSerl = std::move(fill_vector(kVectorSize, true));
        vRand = std::move(fill_vector(kVectorSize, false));
        for (int i = 0; i < kSize; ++i) {
            auto itStart = vSerl.begin() + i * packSize;
            auto itEnd = vSerl.begin() + (i + 1) * packSize;
            vSerlSent.push_back(std::move(std::vector<int>(itStart, itEnd)));
        }
        for (int i = 0; i < kSize; ++i) {
            auto itStart = vRand.begin() + i * packSize;
            auto itEnd = vRand.begin() + (i + 1) * packSize;
            vRandSent.push_back(std::move(std::vector<int>(itStart, itEnd)));
        }
        if (lasts != 0) {
            vSerlSent[0].insert(vSerlSent[0].end(), vSerl.begin() + kSize * packSize, vSerl.end());
            vRandSent[0].insert(vRandSent[0].end(), vRand.begin() + kSize * packSize, vRand.end());
        }
    }
    mpi::scatter(world, vSerlSent, vSerlLocal, 0);
    mpi::scatter(world, vRandSent, vRandLocal, 0);

    for (int i = 0; i < vSerlLocal.size(); ++i) {
        lSum += vSerlLocal[i] * vRandLocal[i];
    }

    mpi::reduce(world, lSum, sum, std::plus<long long>(), 0);

    auto end = std::chrono::high_resolution_clock::now();
    if (world.rank() == 0) {
        auto res_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << std::format("Result: {}\nGot in {} milliseconds", sum, res_time) << std::endl;
    }

    return 0;
}