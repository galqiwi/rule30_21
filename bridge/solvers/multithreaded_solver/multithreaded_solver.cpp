//
// Created by galqiwi on 31.12.2021.
//

#include "multithreaded_solver.h"
#include "../../grid/grid_facade.h"

namespace solvers {

namespace impl {

class TMultithreadedSolver : public ISolver {
 public:
  TMultithreadedSolver(grid::ISendableGridPtr grid, int n_threads): grid_(grid), n_threads_(n_threads) {
  }

  void Solve() override {

  }

 private:
  grid::TSendableGridFacade grid_;
  int n_threads_;
};

}

ISolverPtr CreateMultithreadedSolver(grid::ISendableGridPtr grid, int n_threads) {
  return std::make_shared<impl::TMultithreadedSolver>(grid, n_threads);
}

}