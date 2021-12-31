//
// Created by galqiwi on 28.12.2021.
//

#ifndef RULE30_21_GALQIWI_V1_IMPL_H
#define RULE30_21_GALQIWI_V1_IMPL_H

#include "../IGrid.h"

namespace grid {

struct GalqiwisV1GridConfig {
  int vertical_cell_size = 1024;
  int horizontal_cell_size = 1024;
};

ISendableGridPtr CreateGalqiwisV1Grid(GalqiwisV1GridConfig config = {});

}

#endif  // RULE30_21_GALQIWI_V1_IMPL_H
