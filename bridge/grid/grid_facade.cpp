//
// Created by galqiwi on 30.12.2021.
//

#include "grid_facade.h"
#include <assert.h>
#include <iostream>

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
    valid_cells_semaphore_.Release();
  }

  backend_->Init();
}

void TSendableGridFacade::Do(Cell cell) {
  {
    std::lock_guard<std::mutex> guard(mutex_);
    assert(Validate(cell));
    tops_.erase(cell);
    lefts_.erase(cell);

    assert(valid_cells_.count(cell) == 0);
  }
  backend_->Do(cell);
  {
    std::lock_guard<std::mutex> guard(mutex_);
    auto bottom_cell = GetBottomCell(cell);
    auto right_cell = GetRightCell(cell);
    tops_.insert(bottom_cell);
    lefts_.insert(right_cell);

    if (Validate(bottom_cell) && backend_->Crunchable(bottom_cell)) {
      valid_cells_.insert(bottom_cell);
      valid_cells_semaphore_.Release();
    }
    if (Validate(right_cell) && backend_->Crunchable(right_cell)) {
      valid_cells_.insert(right_cell);
      valid_cells_semaphore_.Release();
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
    if (valid_cells_.count(cell) > 0) {
      valid_cells_semaphore_.Acquire();
    }
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
      valid_cells_semaphore_.Release();
    }
  }
}

void TSendableGridFacade::SendLeft(Cell cell) {
  {
    std::lock_guard<std::mutex> guard(mutex_);
    lefts_.erase(cell);
    if (valid_cells_.count(cell) > 0) {
      valid_cells_semaphore_.Acquire();
    }
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
      valid_cells_semaphore_.Release();
    }
  }
}
bool TSendableGridFacade::Validate(Cell cell) {
  bool top_ok = (tops_.count(cell) > 0 || cell.second == 0);
  bool left_ok = (lefts_.count(cell) > 0 || cell.first == 0);
  return top_ok && left_ok;
}
std::optional<Cell> TSendableGridFacade::PullValidCell() {
  valid_cells_semaphore_.Acquire();

  std::lock_guard<std::mutex> guard(mutex_);
  assert(!valid_cells_.empty());

  auto output = std::move(*valid_cells_.begin());
  valid_cells_.erase(valid_cells_.begin());
  return std::move(output);

}

}