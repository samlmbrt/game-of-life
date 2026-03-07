#pragma once

#include <vector>

class Grid {
public:
  Grid(unsigned int width, unsigned int height, double density);

  void tick();
  [[nodiscard]] bool is_alive(unsigned int x, unsigned int y) const {
    return current_[index(x, y)];
  }
  [[nodiscard]] unsigned int get_width() const { return width_; }
  [[nodiscard]] unsigned int get_height() const { return height_; }

private:
  unsigned int width_;
  unsigned int height_;
  std::vector<bool> current_;
  std::vector<bool> next_;

  [[nodiscard]] std::size_t index(unsigned int x, unsigned int y) const {
    return static_cast<std::size_t>(y) * width_ + x;
  }
  [[nodiscard]] unsigned int count_neighbors(unsigned int x,
                                             unsigned int y) const;
};
