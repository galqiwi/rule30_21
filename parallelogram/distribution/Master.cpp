//
// Created by galqiwi on 18.11.2021.
//

#include "Master.h"

#include <map>
#include <set>

#include "SharedArray.hpp"
#include "SharedValue.hpp"

template<typename T>
class Grid {
 public:

  Grid(T default_horizontal, T default_vertical) {
    defaults_[Horizontal()] = std::move(default_horizontal);
    defaults_[Vertical()] = std::move(default_vertical);
  }

  bool HasWork() {
    return !free_cells_.empty();
  }

  std::pair<size_t, size_t> GetWork() {
    assert(HasWork());
    auto out = *free_cells_.begin();
    free_cells_.erase(out);
    return out;
  }

  std::pair<T, T> Erase(std::pair<size_t, size_t> cell) {
    auto [x, y] = cell;

    assert(data_.count(std::make_tuple(x, y, false)) == 1);
    assert(data_.count(std::make_tuple(x, y, true)) == 1);

    auto horizontal_data = std::move(
        data_[std::make_tuple(x, y, Horizontal())]
    );
    auto vertical_data = std::move(
        data_[std::make_tuple(x, y, Vertical())]
    );

    data_.erase(std::make_tuple(x, y, Horizontal()));
    data_.erase(std::make_tuple(x, y, Vertical()));

    return {std::move(vertical_data), std::move(horizontal_data)};
  }

  const T& GetHorizontal(std::pair<size_t, size_t> cell) {
    auto [x, y] = cell;
    return data_[std::make_tuple(x, y, Horizontal())];
  }

  const T& GetVertical(std::pair<size_t, size_t> cell) {
    auto [x, y] = cell;
    return data_[std::make_tuple(x, y, Vertical())];
  }

  void SubmitWork(std::pair<size_t, size_t> cell, bool type, T work) {
    auto [x, y] = cell;
    assert(data_.count(std::make_tuple(x, y, type)) == 0);
    data_[std::make_tuple(x, y, type)] = std::move(work);
    if (x == 0 && y != 0) {
      assert(type == Horizontal());
      data_[std::make_tuple(x, y, Vertical())] = defaults_[Vertical()];
    }

    if (x != 0 && y == 0) {
      assert(type == Vertical());
      data_[std::make_tuple(x, y, Horizontal())] = defaults_[Horizontal()];
    }

    if (data_.count(std::make_tuple(x, y, !type))) {
      free_cells_.emplace(x, y);
    }
  }

  static bool Horizontal() {
    return true;
  }

  static bool Vertical() {
    return !Horizontal();
  }
 private:
  std::map<bool, T> defaults_;
  std::set<std::pair<size_t, size_t>> free_cells_;
  std::map<std::tuple<size_t, size_t, bool>, T> data_;
};

Grid<SharedArray<Word>> MakeGrid(size_t horizontal_size, size_t vertical_size) {
  Grid<SharedArray<Word>> out(
      SharedArray<Word>(horizontal_size),
      SharedArray<Word>(vertical_size * 2)
  );

  auto initial_array = SharedArray<Word>(horizontal_size);
  (*initial_array)[0] = 1;
  out.SubmitWork(
      std::make_pair(0, 0),
      out.Horizontal(),
      std::move(initial_array)
  );
  out.SubmitWork(
      std::make_pair(0, 0),
      out.Vertical(),
      SharedArray<Word>(vertical_size * 2)
  );
  return std::move(out);
}

void SendPlacebo(int receiver) {
  SharedValue<Word>(0).Send(receiver);
}

void SendCyanide(int receiver) {
  SharedValue<Word>(1).Send(receiver);
}

void Master::Main(size_t processes_n) {
  size_t horizontal_size = 10000;
  size_t vertical_size = 10000;

  auto grid = MakeGrid(horizontal_size, vertical_size);

  std::map<int, std::pair<size_t, size_t>> assignments;
  std::set<int> free_pids;
  for (size_t i = 1; i < processes_n; ++i) {
    free_pids.emplace(i);
  }

  SharedValue<Word> queue(0);
  for (size_t iteration_id = 0; iteration_id < 100; ++iteration_id) {
    while (!free_pids.empty() && grid.HasWork()) {
      auto pid = *free_pids.begin();
      free_pids.erase(pid);
      auto cell = grid.GetWork();
      SendPlacebo(pid);
      grid.GetVertical(cell).Send(pid);
      grid.GetHorizontal(cell).Send(pid);
      assignments[pid] = cell;
    }

    auto pid = queue.Update(SharedValue<Word>::FromAnywhere());
    auto cell = assignments[pid];
    auto [x, y] = cell;
    auto [vertical, horizontal] = grid.Erase(cell);
    std::cout << pid << " finished (" << x << ", " << y << ")" << std::endl;
    vertical.Update(pid);
    horizontal.Update(pid);
    free_pids.emplace(pid);

    grid.SubmitWork(std::make_pair(x + 1, y), grid.Vertical(), std::move(vertical));
    grid.SubmitWork(std::make_pair(x, y + 1), grid.Horizontal(), std::move(horizontal));
  }

  for (size_t pid = 1; pid < processes_n; ++pid) {
    SendCyanide(pid);
  }
}
