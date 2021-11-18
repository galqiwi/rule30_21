#include <iostream>
#include <vector>
#include <set>
#include <chrono>
#include "SimpleChunkProcessor.h"

int main() {
    auto simple_chunk_processor = MakeSimpleChunkProcessor();

    Edge edge = MakeCornerEdge();
    auto start = std::chrono::steady_clock::now();
    auto [left, right] = simple_chunk_processor->Process(edge, edge);
    auto stop = std::chrono::steady_clock::now();

    std::cout << std::chrono::duration<double>(stop - start).count() << std::endl;
//    std::cout << left << " " << right << std::endl;

    return 0;
}
