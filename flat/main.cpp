//
// Created by galqiwi on 14.11.2021.
//

#include <deque>
#include <cstdint>
#include <bitset>
#include <iostream>
#include <chrono>
#include <vector>

using Word = uint64_t;
//using Word = uint64_t;
//using Word = unsigned __int128;
constexpr size_t WORD_SIZE = sizeof(Word) * 8;

void print(const std::deque<Word>& queue) {
    for (Word value: queue) {
        std::cout << static_cast<std::bitset<64>>(value);
    }
    std::cout << std::endl;
}

void test(size_t N, std::vector<Word>& data) {
    data[3 * N] = 1;

    for (size_t row_update_id = 0; row_update_id < N; ++row_update_id) {

        #pragma GCC ivdep
        for (size_t i = 3 * N - row_update_id; i <= 3 * N + row_update_id; ++i) {
            auto left_m = data[i - 1];
            auto middle_m = data[i];
            auto right_m = data[i + 1];

            for (size_t cpu_cycle = 0; cpu_cycle < WORD_SIZE; ++cpu_cycle) {
                auto left_l = (left_m << 1) | (middle_m >> (WORD_SIZE - 1));
                auto left_r = (left_m >> 1);

                auto middle_l = (middle_m << 1) | (right_m >> (WORD_SIZE - 1));
                auto middle_r = (middle_m >> 1) | (left_m << (WORD_SIZE - 1));

                auto right_l = (right_m << 1);
                auto right_r = (right_m >> 1) | (middle_m << (WORD_SIZE - 1));

                middle_m = (middle_m | middle_l) ^ middle_r;
                right_m = (right_m | right_l) ^ right_r;
                left_m = (left_m | left_l) ^ left_r;
            }

            data[i] = middle_m;
        }
    }
}

int main() {
    size_t N = 200000 / WORD_SIZE;
    std::vector<Word> data(6 * N, 0);
    auto start = std::chrono::steady_clock::now();
    test(N, data);
    auto stop = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double>(stop - start).count() << std::endl;
}