//
// Created by galqiwi on 31.12.2021.
//

#ifndef RULE30_21_ISOLVER_H
#define RULE30_21_ISOLVER_H

#include <memory>

namespace solvers {

class ISolver {
 public:
  virtual void Solve() = 0;
};

using ISolverPtr = std::shared_ptr<ISolver>;

}

#endif  // RULE30_21_ISOLVER_H
