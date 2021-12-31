//
// Created by galqiwi on 31.12.2021.
//

#ifndef RULE30_21_MULTITHREADED_SOLVER_H
#define RULE30_21_MULTITHREADED_SOLVER_H

#include "../ISolver.h"
#include "../../grid/IGrid.h"

namespace solvers {

ISolverPtr CreateMultithreadedSolver(grid::ISendableGridPtr grid, int n_threads);

}

#endif  // RULE30_21_MULTITHREADED_SOLVER_H
