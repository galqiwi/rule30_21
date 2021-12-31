//
// Created by galqiwi on 30.12.2021.
//

#include <set>
#include <iostream>
#include "grid/impl/galqiwi_v1_impl.h"
#include "grid/grid_facade.h"

int main() {
  auto grid = grid::TSendableGridFacade(grid::CreateGalqiwisV1Grid());

  grid.Init();

  for (int i = 0; i < 1000; i++) {
    auto cell = grid.GetValidCell();
    if (cell) {
      std::cout << cell->first << " " << cell->second << std::endl;
      grid.Do(*cell);
    } else {
      break;
    }
  }
}