//
// Created by galqiwi on 18.11.2021.
//

#include <mpi.h>
#include <vector>
#include <iostream>
#include <bitset>
#include <cassert>
#include <chrono>

#include "crunching/process_parallelogram.h"
#include "distribution/Master.h"
#include "distribution/Worker.h"
#include "utils.h"

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);

  auto [process_id, processes_n] = GetProcessInfo();

  if (process_id == 0) {
    Master().Main(processes_n);
  } else {
    Worker().Main(process_id, processes_n);
  }

  MPI_Finalize();
}