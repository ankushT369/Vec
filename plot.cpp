#include "vec.hpp"
#include <vector>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <cmath>

void benchmark_and_print(int64_t N) {
    Vec<int> myvec;
    auto start_custom = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        myvec.push(i);
    }
    auto end_custom = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_custom = end_custom - start_custom;

    std::vector<int> stdvec;
    auto start_std = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        stdvec.push_back(i);
    }
    auto end_std = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_std = end_std - start_std;

    std::cout << N << " "
              << time_custom.count() << " "
              << time_std.count() << "\n";
}

int main() {
    std::vector<int64_t> sizes = {
        10000, 50000, 100000, 500000, 1000000,
        //5000000, 10000000, 50000000, 100000000, 500000000
    };

    std::cout << "#N CustomVec(ms) StdVector(ms)\n";

    for (int64_t N : sizes) {
        benchmark_and_print(N);
    }

    return 0;
}

