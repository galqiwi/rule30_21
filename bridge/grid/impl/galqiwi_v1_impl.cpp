//
// Created by galqiwi on 28.12.2021.
//

#include <map>
#include <assert.h>
#include <set>
#include <unordered_set>
#include <mutex>
#include "galqiwi_v1_impl.h"
#include "crunching/process_parallelogram.h"

namespace grid {

namespace impl {

using Data = std::vector<crunching::Word>;

class TGalqiwisV1Grid : public ISendableGrid {
 public:
  TGalqiwisV1Grid() {
  }

  void Init() override {
    std::lock_guard<std::mutex> guard(mutex_);
    assert(tops_.empty() && lefts_.empty());
    Cell init_cell = {0, 0};
    Prepare(init_cell);
  }
  void Do(Cell cell) override {

    auto [buffer_1, buffer_2, top, left] = [&]() {
      std::lock_guard<std::mutex> guard(mutex_);

      assert(Validate(cell));
      Prepare(cell);

      while (buffers_.size() < 2) {
        buffers_.push_back(std::move(crunching::CreateBuffer(tops_[cell])));
      }

      auto buffer_1_output = std::move(buffers_[buffers_.size() - 2]);
      auto buffer_2_output = std::move(buffers_[buffers_.size() - 1]);
      buffers_.resize(buffers_.size() - 2);


      auto top_output = std::move(tops_[cell]);
      auto left_output = std::move(lefts_[cell]);
      tops_.erase(cell);
      lefts_.erase(cell);

      return std::make_tuple(
          std::move(buffer_1_output),
          std::move(buffer_2_output),
          std::move(top_output),
          std::move(left_output));
    }();

    crunching::ProcessParallelogram(top, left, buffer_1, buffer_2);

    {
      std::lock_guard<std::mutex> guard(mutex_);

      tops_[GetBottomCell(cell)] = std::move(top);
      lefts_[GetRightCell(cell)] = std::move(left);

      buffers_.push_back(std::move(buffer_1));
      buffers_.push_back(std::move(buffer_2));
    }
  }
  bool Crunchable(Cell cell) override {
    std::lock_guard<std::mutex> guard(mutex_);
    assert(Validate(cell));

    return (cell.first < 10 && cell.second < 10);
  }

  virtual void SendTop(Cell cell) override {
  }
  virtual void ReceiveTop(Cell cell) override {
  }

  virtual void SendLeft(Cell cell) override {
  }
  virtual void ReceiveLeft(Cell cell) override {
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
  std::mutex mutex_;
};

}

ISendableGridPtr CreateGalqiwisV1Grid() {
  return std::make_shared<impl::TGalqiwisV1Grid>();
}

}