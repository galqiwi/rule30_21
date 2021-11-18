//
// Created by galqiwi on 14.11.2021.
//

#ifndef RULE30_21_ICHUNKPROCESSOR_H
#define RULE30_21_ICHUNKPROCESSOR_H

#include <memory>
#include "Edge.h"

class IChunkProcessor {
public:
    virtual std::pair<Edge, Edge> Process(Edge left, Edge right) = 0;
};

using IChunkProcessorPtr = std::shared_ptr<IChunkProcessor>;

#endif //RULE30_21_ICHUNKPROCESSOR_H
