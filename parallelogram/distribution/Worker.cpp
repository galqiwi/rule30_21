//
// Created by galqiwi on 18.11.2021.
//

#include "Worker.h"
#include "../crunching/process_parallelogram.h"
#include "SharedArray.hpp"
#include "SharedValue.hpp"

void Worker::Main(int process_id, size_t processes_n) {
  size_t horizontal_size = 10000; // TODO uniform way to set sizes
  size_t vertical_size = 10000;
  SharedValue<Word> cyanide;
  SharedArray<Word> vertical(vertical_size * 2);
  SharedArray<Word> horizontal(horizontal_size);
  std::vector<Word> buffer_1(horizontal_size + 2);
  std::vector<Word> buffer_2(horizontal_size + 2);

  while (true) {
    cyanide.Update(0);
    if (*cyanide > 0) {
      return;
    }
    vertical.Update(0);
    horizontal.Update(0);
    // processing
    crunching::ProcessParallelogram(*horizontal, *vertical, buffer_1, buffer_2);

    SharedValue<Word>(process_id).Send(0);
    vertical.Send(0);
    horizontal.Send(0);
  }
}
