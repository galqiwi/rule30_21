//
// Created by galqiwi on 14.11.2021.
//

#include "Edge.h"

Edge MakeCornerEdge() {
    Edge out;
    for (size_t i = 0; i < EDGE_SIZE * 2 + 1; ++i) {
        out[i] = (i % 2 == 0);
    }
    return out;
}