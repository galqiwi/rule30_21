//
// Created by galqiwi on 28.12.2021.
//

#include <map>
#include <assert.h>
#include <set>
#include <unordered_set>
#include "galqiwi_v1_impl.h"
#include "crunching/process_parallelogram.h"

namespace grid {

namespace impl {

using Data = std::vector<crunching::Word>;

class TGalqiwisV1Grid : public ISendableGrid {
 public:
  TGalqiwisV1Grid() {
  }

  void Init() {
    Cell init_cell = {0, 0};
    Prepare(init_cell);
  }
  void Do(Cell cell) {
    assert(Validate(cell));
    Prepare(cell);
    while (buffers_.size() < 2) {
      buffers_.push_back(std::move(crunching::CreateBuffer(tops_[cell])));
    }

    auto buffer_1 = std::move(buffers_[buffers_.size() - 2]);
    auto buffer_2 = std::move(buffers_[buffers_.size() - 1]);
    buffers_.resize(buffers_.size() - 2);

    auto top = std::move(tops_[cell]);
    auto left = std::move(lefts_[cell]);
    tops_.erase(cell);
    lefts_.erase(cell);

    crunching::ProcessParallelogram(top, left, buffer_1, buffer_2);

    tops_[GetBottomCell(cell)] = std::move(top);
    lefts_[GetRightCell(cell)] = std::move(left);
  }
  bool Crunchable(Cell cell) {
    assert(Validate(cell));

    return (cell.first < 10 && cell.second < 10);
  }

  virtual void SendTop(Cell cell) {
    assert(Validate(cell));
  }
  virtual void ReceiveTop(Cell cell) {
  }

  virtual void SendLeft(Cell cell) {
    assert(Validate(cell));
  }
  virtual void ReceiveLeft(Cell cell) {
  }

 private:
  bool Validate(Cell cell) {
    bool top_ok = (tops_.count(cell) > 0 || cell.second == 0);
    bool left_ok = (lefts_.count(cell) > 0 || cell.first == 0);
    return top_ok && left_ok;
  }

  void Prepare(Cell cell) {
    assert(Validate(cell));
    if (tops_.count(cell) < 1) {
      tops_[cell] = std::move(crunching::CreateInitialHorizontalData(100));
    }
    if (lefts_.count(cell) < 1) {
      lefts_[cell] = std::move(crunching::CreateInitialVerticalData(100));
    }
  }

  std::map<Cell, Data> tops_;
  std::map<Cell, Data> lefts_;
  std::vector<Data> buffers_;
};

}

ISendableGridPtr CreateGalqiwisV1Grid() {
  return std::make_shared<impl::TGalqiwisV1Grid>();
}

}