//
// Created by galqiwi on 18.11.2021.
//

#ifndef RULE30_21_PROCESS_PARALLELOGRAM_H
#define RULE30_21_PROCESS_PARALLELOGRAM_H
#include <vector>
#include <iostream>
#include <bitset>
#include <cassert>


using Word = uint32_t; // do not forget about MPI_WORD_SIZE
constexpr size_t kWordSize = sizeof(Word) * 8;

namespace crunching {

void Print(const std::vector<Word>& line);

void ProcessParallelogram(std::vector<Word>& horizontal, std::vector<Word>& vertical, std::vector<Word>& buffer_1, std::vector<Word>& buffer_2);

}

#endif //RULE30_21_PROCESS_PARALLELOGRAM_H
