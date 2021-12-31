//
// Created by galqiwi on 31.12.2021.
//

#include "simple_solver.h"
#include "../../grid/grid_facade.h"

#include <iostream>

namespace solvers {

namespace impl {

class TSimpleSolver : public ISolver {
 public:
  TSimpleSolver(grid::ISendableGridPtr grid): grid_(grid) {
  }

  void Solve() override {
    grid_.Init();

    while (auto cell = grid_.PullValidCell()) {
      grid_.Do(*cell);
      std::cout << cell->first << " " << cell->second << std::endl;
    }
  }

 private:
  grid::TSendableGridFacade grid_;
  int n_threads_;
};

}

ISolverPtr CreateSimpleSolver(grid::ISendableGridPtr grid) {
  return std::make_shared<impl::TSimpleSolver>(grid);
}

}