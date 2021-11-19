//
// Created by galqiwi on 19.11.2021.
//

#ifndef RULE30_21_GRID_HPP
#define RULE30_21_GRID_HPP

using Cell = std::pair<size_t, size_t>;

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

  Cell GetWork() {
    assert(HasWork());
    auto out = *free_cells_.begin();
    free_cells_.erase(out);
    return out;
  }

  std::pair<T, T> Erase(Cell cell) {
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

  const T& GetHorizontal(Cell cell) {
    auto [x, y] = cell;
    return data_[std::make_tuple(x, y, Horizontal())];
  }

  const T& GetVertical(Cell cell) {
    auto [x, y] = cell;
    return data_[std::make_tuple(x, y, Vertical())];
  }

  void SubmitWork(Cell cell, bool type, T work) {
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
  std::set<Cell> free_cells_;
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

#endif  // RULE30_21_GRID_HPP
