//
// Created by galqiwi on 30.12.2021.
//

#include "grid_facade.h"
#include <assert.h>

namespace grid {

TSendableGridFacade::TSendableGridFacade(grid::ISendableGridPtr backend)
    : backend_(backend) {
}

void TSendableGridFacade::Init() {
  {
    std::lock_guard<std::mutex> guard(mutex_);
    assert(tops_.empty() && lefts_.empty());
    Cell init_cell = {0, 0};
    tops_.insert(init_cell);
    lefts_.insert(init_cell);
    valid_cells_.insert(init_cell);
  }

  backend_->Init();
}

void TSendableGridFacade::Do(Cell cell) {
  {
    std::lock_guard<std::mutex> guard(mutex_);
    assert(Validate(cell));
    tops_.erase(cell);
    lefts_.erase(cell);
    valid_cells_.erase(cell);
  }
  backend_->Do(cell);
  {
    std::lock_guard<std::mutex> guard(mutex_);
    auto bottom_cell = GetBottomCell(cell);
    auto right_cell = GetRightCell(cell);
    tops_.insert(bottom_cell);
    lefts_.insert(right_cell);

    if (Validate(bottom_cell) && Crunchable(bottom_cell)) {
      valid_cells_.insert(bottom_cell);
    }
    if (Validate(right_cell) && Crunchable(right_cell)) {
      valid_cells_.insert(right_cell);
    }
  }
}

bool TSendableGridFacade::Crunchable(Cell cell) {
  {
    std::lock_guard<std::mutex> guard(mutex_);
    assert(Validate(cell));
  }
  return backend_->Crunchable(cell);
}

void TSendableGridFacade::SendTop(Cell cell) {
  {
    std::lock_guard<std::mutex> guard(mutex_);
    tops_.erase(cell);
    valid_cells_.erase(cell);
  }
  backend_->SendTop(cell);
}

void TSendableGridFacade::ReceiveTop(Cell cell) {
  backend_->ReceiveTop(cell);
  {
    std::lock_guard<std::mutex> guard(mutex_);
    tops_.insert(cell);
    if (Validate(cell) && Crunchable(cell)) {
      valid_cells_.insert(cell);
    }
  }
}

void TSendableGridFacade::SendLeft(Cell cell) {
  {
    std::lock_guard<std::mutex> guard(mutex_);
    lefts_.erase(cell);
    valid_cells_.erase(cell);
  }
  backend_->SendLeft(cell);
}

void TSendableGridFacade::ReceiveLeft(Cell cell) {
  backend_->ReceiveLeft(cell);
  {
    std::lock_guard<std::mutex> guard(mutex_);
    lefts_.insert(cell);
    if (Validate(cell) && Crunchable(cell)) {
      valid_cells_.insert(cell);
    }
  }
}
bool TSendableGridFacade::Validate(Cell cell) {
  bool top_ok = (tops_.count(cell) > 0 || cell.second == 0);
  bool left_ok = (lefts_.count(cell) > 0 || cell.first == 0);
  return top_ok && left_ok;
}
std::optional<Cell> TSendableGridFacade::PullValidCell() {
  std::lock_guard<std::mutex> guard(mutex_);
  if (!valid_cells_.empty()) {
    auto output = std::move(*valid_cells_.begin());
    valid_cells_.erase(valid_cells_.begin());
    return std::move(output);
  } else {
    return std::nullopt;
  }
}

}