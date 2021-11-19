//
// Created by galqiwi on 18.11.2021.
//

#include <vector>
#include <iostream>
#include <bitset>
#include <cassert>
#include <chrono>

#include "crunching/process_parallelogram.h"
#include <fstream>

int main() {
  size_t n = 1000;
  n /= 2 * 64;

  n = 4;
  size_t horizontal_size = n * 2;
  size_t vertical_size = n;

  std::vector<Word> horizontal(horizontal_size, 0);
  std::vector<Word> vertical(vertical_size * 2, 0);
  std::vector<Word> buffer_1(horizontal_size + 2);
  std::vector<Word> buffer_2(horizontal_size + 2);
  horizontal[horizontal_size - 1] = 1;

//  auto start = std::chrono::steady_clock::now();
//  crunching::ProcessParallelogram(horizontal, vertical, buffer_1, buffer_2);
//  auto stop = std::chrono::steady_clock::now();
//
//  std::cout << std::chrono::duration<double>(stop - start).count() << std::endl;
//
//  std::ofstream fout("out.data");
//
//  auto start_2 = std::chrono::steady_clock::now();
//  for (auto word: horizontal) {
//    fout << word << "\n";
//  }
//  auto stop_2 = std::chrono::steady_clock::now();
//  std::cout << std::chrono::duration<double>(stop_2 - start_2).count() << std::endl;
//  std::cout << horizontal.size() << std::endl;

  auto answer = crunching::Restore(horizontal, vertical, buffer_1, buffer_2, crunching::Offset{horizontal_size - 1, 0});

  for (bool bit: answer) {
    std::cout << bit;
  }
  std::cout << std::endl;
}