//
// Created by galqiwi on 18.11.2021.
//

#include "utils.h"

std::pair<int, size_t> GetProcessInfo() {
  int procid, num_procs;
  MPI_Comm_rank(MPI_COMM_WORLD, &procid);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  return {procid, num_procs};
}