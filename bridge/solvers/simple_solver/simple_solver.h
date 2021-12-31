//
// Created by galqiwi on 31.12.2021.
//

#ifndef RULE30_21_SIMPLE_SOLVER_H
#define RULE30_21_SIMPLE_SOLVER_H

#include "../ISolver.h"
#include "../../grid/IGrid.h"

namespace solvers {

ISolverPtr CreateSimpleSolver(grid::ISendableGridPtr grid);

}

#endif  // RULE30_21_SIMPLE_SOLVER_H
