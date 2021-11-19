//
// Created by galqiwi on 18.11.2021.
//

#include "process_parallelogram.h"

namespace crunching {

void Print(const std::vector<Word>& line) {
  for (auto word: line) {
    std::cout << static_cast<std::bitset<kWordSize>>(word);
  }
  std::cout << std::endl;
}

template <size_t n_lines = kWordSize, size_t shift = 1>
void LineUpdate(const std::vector<Word>& old_line, std::vector<Word>& new_line, size_t start, size_t stop) {
    for (size_t i = start; i < stop; ++i) {
        auto left_m = old_line[i - 1];
        auto middle_m = old_line[i];
        auto right_m = old_line[i + 1];

        for (size_t cpu_cycle = 0; cpu_cycle < n_lines; ++cpu_cycle) {
            auto left_l = (left_m << 1) | (middle_m >> (kWordSize - 1));
            auto left_r = (left_m >> 1);

            auto middle_l = (middle_m << 1) | (right_m >> (kWordSize - 1));
            auto middle_r = (middle_m >> 1) | (left_m << (kWordSize - 1));

            auto right_l = (right_m << 1);
            auto right_r = (right_m >> 1) | (middle_m << (kWordSize - 1));

            middle_m = (middle_m | middle_l) ^ middle_r;
            right_m = (right_m | right_l) ^ right_r;
            left_m = (left_m | left_l) ^ left_r;
        }

        new_line[i - shift] = middle_m;
    }
}

void ProcessParallelogram(std::vector<Word>& horizontal,
                          std::vector<Word>& vertical,
                          std::vector<Word>& buffer_1,
                          std::vector<Word>& buffer_2) {
    size_t horizontal_size = horizontal.size();
    size_t vertical_size = vertical.size() / 2;
    assert(buffer_1.size() == horizontal_size + 2);
    assert(buffer_1.size() == buffer_2.size());
    assert(vertical.size() % 2 == 0);

    for (size_t i = 0; i < horizontal_size; ++i) {
        buffer_1[i] = horizontal[i];
    }

    for (size_t line_id = 0; line_id < vertical_size; ++line_id) {
        buffer_1[horizontal_size] = vertical[2 * line_id];
        buffer_1[horizontal_size + 1] = vertical[2 * line_id + 1];
        vertical[2 * line_id] = buffer_1[0];
        vertical[2 * line_id + 1] = buffer_1[1];

        LineUpdate(buffer_1, buffer_2, 1, horizontal_size + 1);
        std::swap(buffer_1, buffer_2);
    }

    for (size_t i = 0; i < horizontal_size; ++i) {
        horizontal[i] = buffer_1[i];
    }
}
std::vector<bool> Restore(std::vector<Word>& horizontal,
                          std::vector<Word>& vertical,
                          std::vector<Word>& buffer_1,
                          std::vector<Word>& buffer_2, Offset offset) {
  size_t horizontal_size = horizontal.size();
  size_t vertical_size = vertical.size() / 2;
  assert(buffer_1.size() == horizontal_size + 2);
  assert(buffer_1.size() == buffer_2.size());
  assert(vertical.size() % 2 == 0);

  WordCoordinates word_coordinates = offset.GetCoordinates(horizontal_size,
                                                           vertical_size);

  std::vector<bool> out;

  for (size_t i = 0; i < horizontal_size; ++i) {
    buffer_1[i] = horizontal[i];
  }

  for (size_t line_id = 0; line_id < vertical_size; ++line_id) {
    buffer_1[horizontal_size] = vertical[2 * line_id];
    buffer_1[horizontal_size + 1] = vertical[2 * line_id + 1];
    vertical[2 * line_id] = buffer_1[0];
    vertical[2 * line_id + 1] = buffer_1[1];

    assert(word_coordinates.y >= line_id);

    if (word_coordinates.y > line_id) {
      LineUpdate(buffer_1, buffer_2, 1, horizontal_size + 1);
      std::swap(buffer_1, buffer_2);
    } else {
      for (size_t cpu_cycle = 0; cpu_cycle < kWordSize; ++cpu_cycle) {
        Word current_word = buffer_1[horizontal_size + 1 - word_coordinates.x];
        bool new_bit = (((current_word >> offset.bit_offset) & 1) > 0);
        out.push_back(new_bit);
        LineUpdate<1, 0>(buffer_1, buffer_2, 1, horizontal_size + 1);
        std::swap(buffer_1, buffer_2);
      }
      LineUpdate<0, 1>(buffer_1, buffer_2, 1, horizontal_size + 1);
      std::swap(buffer_1, buffer_2);
      word_coordinates = word_coordinates.Next();
    }
  }

  for (size_t i = 0; i < horizontal_size; ++i) {
    horizontal[i] = buffer_1[i];
  }

  return std::move(out);
}

WordCoordinates Offset::GetCoordinates(size_t horizontal_size,
                                                 size_t /*vertical_size*/) {
  if (word_offset <= horizontal_size + 1) {
    return {horizontal_size + 1 - word_offset, 0};
  } else {
    return {0, word_offset - horizontal_size - 1};
  }
}

bool WordCoordinates::Calculatable(size_t horizontal_size,
                                   size_t vertical_size) {
  return (x < horizontal_size && y < vertical_size);
}
WordCoordinates WordCoordinates::Next() {
  return {x + 1, y + 1};
}

}