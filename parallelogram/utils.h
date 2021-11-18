//
// Created by galqiwi on 18.11.2021.
//

#ifndef RULE30_21_UTILS_H
#define RULE30_21_UTILS_H

#include <mpi.h>
#include <functional>

std::pair<int, size_t> GetProcessInfo();

double TimeIt(std::function<void()>);

#endif  // RULE30_21_UTILS_H
