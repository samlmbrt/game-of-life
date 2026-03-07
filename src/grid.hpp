#pragma once

#include <cstdint>
#include <span>
#include <vector>

class Grid {
public:
  Grid(unsigned int width, unsigned int height, double density);

  void tick();
  [[nodiscard]] std::span<std::uint8_t const> cells() const { return current_; }
  [[nodiscard]] unsigned int get_width() const { return width_; }
  [[nodiscard]] unsigned int get_height() const { return height_; }

private:
  unsigned int width_;
  unsigned int height_;
  std::vector<std::uint8_t> current_;
  std::vector<std::uint8_t> next_;
  std::vector<std::uint8_t> active_;

  [[nodiscard]] std::size_t index(unsigned int x, unsigned int y) const {
    return static_cast<std::size_t>(y) * width_ + x;
  }
  [[nodiscard]] unsigned int count_neighbors(unsigned int x,
                                             unsigned int y) const;
  void mark_neighbors_active(unsigned int x, unsigned int y);

  template <typename F>
  void for_each_neighbor(unsigned int x, unsigned int y, F func) const {
    auto const ix = static_cast<int>(x);
    auto const iy = static_cast<int>(y);
    auto const iw = static_cast<int>(width_);
    auto const ih = static_cast<int>(height_);

    for (auto dy = -1; dy <= 1; ++dy) {
      for (auto dx = -1; dx <= 1; ++dx) {
        if (dx == 0 && dy == 0)
          continue;

        auto nx = ix + dx;
        auto ny = iy + dy;

        if (nx < 0 || ny < 0 || nx >= iw || ny >= ih)
          continue;

        func(static_cast<unsigned int>(nx), static_cast<unsigned int>(ny));
      }
    }
  }
};
