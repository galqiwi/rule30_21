//
// Created by galqiwi on 14.11.2021.
//

#include "SimpleChunkProcessor.h"

#include <iostream>
#include <utility>

class SimpleChunkProcessor: public IChunkProcessor {
public:
    explicit SimpleChunkProcessor() {
    }

    std::pair<Edge, Edge> Process(Edge left, Edge right) override {
        Edge buffer;
        buffer[0] = left[1];
        buffer[1] = left[0];
        buffer[2] = right[1];

        for (size_t line_id = 0; line_id < EDGE_SIZE; ++line_id) {
            // rule 30
            Edge tmp = buffer | (buffer << 1);
            buffer >>= 1;
            buffer = buffer ^ tmp;
//            buffer >>= 1;
            //

            // corners
            if (line_id < EDGE_SIZE - 1) {
//                buffer <<= 2;
                buffer <<= 1;

                buffer[1] = left[2 + 2 * line_id];
                buffer[3 + 2 * line_id] = right[2 + 2 * line_id];
                buffer[0] = left[3 + 2 * line_id];
                buffer[4 + 2 * line_id] = right[3 + 2 * line_id];
            } else {
//                buffer <<= 1;

                buffer[0] = left[2 + 2 * line_id];
                buffer[2 + 2 * line_id] = right[2 + 2 * line_id];
            }
            //

        }
        left[0] = left[EDGE_SIZE * 2];
        right[0] = right[EDGE_SIZE * 2];

        for (size_t i = 0; i < EDGE_SIZE; ++i) {
            left[1 + 2 * i] = buffer[i + 1];
            right[1 + 2 * i] = buffer[2 * EDGE_SIZE - i - 1];

            Edge tmp = buffer | (buffer << 1);
            buffer >>= 1;
            buffer = buffer ^ tmp;

            left[2 + 2 * i] = buffer[i + 1];
            right[2 + 2 * i] = buffer[2 * EDGE_SIZE - i - 1];
        }

        return {left, right};
    }
};

IChunkProcessorPtr MakeSimpleChunkProcessor() {
    auto foo = std::make_shared<SimpleChunkProcessor>();
    return foo;
}