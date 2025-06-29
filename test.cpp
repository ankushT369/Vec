#include "vec.hpp"
#include <vector>
#include <iostream>
#include <chrono>
#include <iomanip>

void print_separator(int width = 50) {
        std::cout << std::string(width, '-') << '\n';
}

void print_centered(const std::string& msg, int width = 50) {
        int padding = (width - msg.size()) / 2;
        std::cout << std::string(padding, ' ') << msg << '\n';
}

int main() {
        constexpr int N = 299999999;

        print_separator();
        print_centered("Vector Benchmark");
        print_separator();
        std::cout << "Elements to insert: " << N << "\n\n";

        // ----------------- Custom Vec Benchmark -----------------
        Vec<int> myvec;
        auto start_custom = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < N; ++i) {
                myvec.push(i);
        }

        auto end_custom = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> time_custom = end_custom - start_custom;

        // ----------------- std::vector Benchmark -----------------
        std::vector<int> stdvec;
        auto start_std = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < N; ++i) {
                stdvec.push_back(i);
        }

        auto end_std = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> time_std = end_std - start_std;

        // ----------------- Results -----------------
        print_separator();
        std::cout << std::left << std::setw(25) << "Implementation"
                  << std::right << std::setw(20) << "Time (ms)" << '\n';
        print_separator();
        std::cout << std::left << std::setw(25) << "Custom Vec<int>"
                  << std::right << std::setw(20) << time_custom.count() << '\n';
        std::cout << std::left << std::setw(25) << "std::vector<int>"
                  << std::right << std::setw(20) << time_std.count() << '\n';
        print_separator();

        std::cout << "\nSpeedup (custom/std): " 
                  << (time_std.count() / time_custom.count()) << "x\n";

        return 0;
}

