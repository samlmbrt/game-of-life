#include "grid.hpp"
#include <random>

Grid::Grid(unsigned int width, unsigned int height, double density)
    : width_(width), height_(height),
      current_(static_cast<std::size_t>(width) * height),
      next_(static_cast<std::size_t>(width) * height) {
  auto rd = std::random_device{};
  auto gen = std::mt19937{rd()};
  auto dist = std::uniform_real_distribution<double>{0.0, 1.0};

  for (auto &cell : current_) {
    cell = dist(gen) < density ? std::uint8_t{1} : std::uint8_t{0};
  }
}

unsigned int Grid::count_neighbors(unsigned int x, unsigned int y) const {
  auto const ix = static_cast<int>(x);
  auto const iy = static_cast<int>(y);
  auto const iw = static_cast<int>(width_);
  auto const ih = static_cast<int>(height_);

  auto count = 0u;
  for (auto dy = -1; dy <= 1; ++dy) {
    for (auto dx = -1; dx <= 1; ++dx) {
      if (dx == 0 && dy == 0)
        continue;

      auto nx = ix + dx;
      auto ny = iy + dy;

      if (nx < 0 || ny < 0 || nx >= iw || ny >= ih)
        continue;

      if (current_[index(static_cast<unsigned int>(nx),
                         static_cast<unsigned int>(ny))])
        ++count;
    }
  }
  return count;
}

void Grid::tick() {
  for (auto y = 0u; y < height_; ++y) {
    for (auto x = 0u; x < width_; ++x) {
      auto neighbors = count_neighbors(x, y);
      auto idx = index(x, y);
      auto alive = current_[idx];

      next_[idx] =
          alive ? (neighbors == 2 || neighbors == 3) : (neighbors == 3);
    }
  }
  std::swap(current_, next_);
}
