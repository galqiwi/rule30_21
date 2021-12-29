//
// Created by galqiwi on 30.12.2021.
//

#include "grid/impl/galqiwi_v1_impl.h"

int main() {
  auto grid = grid::CreateGalqiwisV1Grid();
  grid->Init();
  grid->Do({0, 0});
  grid->Do({1, 0});
}