//
// Created by galqiwi on 14.11.2021.
//

#ifndef RULE30_21_EDGE_H
#define RULE30_21_EDGE_H

#include <bitset>

#ifndef EDGE_SIZE
#define EDGE_SIZE 4
#endif //EDGE_SIZE

using Edge = std::bitset<EDGE_SIZE * 2 + 1>;

Edge MakeCornerEdge();

#endif //RULE30_21_EDGE_H
