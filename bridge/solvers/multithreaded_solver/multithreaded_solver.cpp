//
// Created by galqiwi on 31.12.2021.
//

#include <iostream>
#include "multithreaded_solver.h"
#include "../../grid/grid_facade.h"
#include <thread>
#include <vector>

namespace solvers {

namespace impl {

class TMultithreadedSolver : public ISolver {
 public:
  TMultithreadedSolver(grid::ISendableGridPtr grid, int n_threads): grid_(grid), n_threads_(n_threads) {
  }

  void Solve() override {
    std::vector<std::thread> workers;

    grid_.Init();

    for (int i = 0; i < n_threads_; ++i) {
      workers.emplace_back([&]() {
        WorkerRoutine();
      });
    }

    for (auto& worker: workers) {
      worker.join();
    }
  }

 private:
  void WorkerRoutine() {
    while (auto cell = grid_.PullValidCell()) {
      std::cout << "gonna do " << cell->first << " " << cell->second << std::endl;
      grid_.Do(*cell);
      std::cout << cell->first << " " << cell->second << std::endl;
    }
  }

  grid::TSendableGridFacade grid_;
  int n_threads_;
};

}

ISolverPtr CreateMultithreadedSolver(grid::ISendableGridPtr grid, int n_threads) {
  return std::make_shared<impl::TMultithreadedSolver>(grid, n_threads);
}

}