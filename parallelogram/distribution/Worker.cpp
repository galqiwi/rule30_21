//
// Created by galqiwi on 18.11.2021.
//

#include "Worker.h"
#include "../crunching/process_parallelogram.h"
#include "SharedArray.hpp"
#include "SharedValue.hpp"
#include "../utils.h"

void Worker::Main(int process_id, size_t processes_n) {
  size_t horizontal_size = 10000; // TODO uniform way to set sizes
  size_t vertical_size = 10000;
  SharedValue<Word> cyanide;
  SharedArray<Word> vertical(vertical_size * 2);
  SharedArray<Word> horizontal(horizontal_size);
  std::vector<Word> buffer_1(horizontal_size + 2);
  std::vector<Word> buffer_2(horizontal_size + 2);

  while (true) {
    auto read_time = TimeIt([&]() {
      cyanide.Update(0);
      if (*cyanide == 0) {
        vertical.Update(0);
        horizontal.Update(0);
      }
    });

    if (*cyanide > 0) {
      return;
    }

    auto processing_time = TimeIt([&]() {
      crunching::ProcessParallelogram(*horizontal, *vertical, buffer_1, buffer_2);
    });

    auto write_time = TimeIt([&]() {
      SharedValue<Word>(process_id).Send(0);
      vertical.Send(0);
      horizontal.Send(0);
    });

//    std::cout << read_time << " " << processing_time << " " << write_time << std::endl;
  }
}
