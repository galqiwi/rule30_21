//
// Created by galqiwi on 18.11.2021.
//

#include <chrono>
#include "utils.h"

std::pair<int, size_t> GetProcessInfo() {
  int procid, num_procs;
  MPI_Comm_rank(MPI_COMM_WORLD, &procid);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  return {procid, num_procs};
}

double TimeIt(std::function<void()> function) {
  auto start = std::chrono::steady_clock::now();
  function();
  auto stop = std::chrono::steady_clock::now();
  return std::chrono::duration<double>(stop - start).count();
}