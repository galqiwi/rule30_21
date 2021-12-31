//
// Created by galqiwi on 30.12.2021.
//

#ifndef RULE30_21_GRID_FACADE_H
#define RULE30_21_GRID_FACADE_H

#include <unordered_set>
#include <set>
#include <mutex>
#include "IGrid.h"

namespace grid {

class TSendableGridFacade : public grid::ISendableGrid {
 public:
  TSendableGridFacade() = delete;
  explicit TSendableGridFacade(grid::ISendableGridPtr backend);

  void Init() override;
  void Do(Cell) override;
  bool Crunchable(Cell) override;

  void SendTop(Cell) override;
  void ReceiveTop(Cell) override;

  virtual void SendLeft(Cell) override;
  virtual void ReceiveLeft(Cell) override;

  std::optional<Cell> PullValidCell();
 private:
  bool Validate(Cell) ;

  grid::ISendableGridPtr backend_;
  std::set<Cell> tops_;
  std::set<Cell> lefts_;
  std::set<Cell> valid_cells_;
  std::mutex mutex_;
};

}

#endif  // RULE30_21_GRID_FACADE_H
