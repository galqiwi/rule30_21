//
// Created by galqiwi on 18.11.2021.
//

#include "Master.h"

#include <map>
#include <set>

#include "SharedArray.hpp"
#include "SharedValue.hpp"
#include "Grid.hpp"

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

  std::map<int, Cell> assignments;
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
