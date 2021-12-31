//
// Created by galqiwi on 30.12.2021.
//

#include <set>
#include <iostream>
#include "grid/impl/galqiwi_v1_impl.h"
#include "grid/grid_facade.h"
#include "solvers/multithreaded_solver/multithreaded_solver.h"
#include "solvers/simple_solver/simple_solver.h"

int main() {
  auto grid = grid::CreateGalqiwisV1Grid({
      .horizontal_cell_size = 1024
  });
  solvers::CreateSimpleSolver(grid)->Solve();

}