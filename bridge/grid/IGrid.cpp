//
// Created by galqiwi on 23.11.2021.
//

#include "IGrid.h"

namespace grid {

Cell GetRightCell(Cell cell) {
  return {cell.first + 1, cell.second};
}
Cell GetBottomCell(Cell cell) {
  return {cell.first, cell.second + 1};
}

}