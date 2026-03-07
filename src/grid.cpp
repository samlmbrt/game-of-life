#include "grid.hpp"
#include <random>

Grid::Grid(unsigned int width, unsigned int height, double density)
    : width_(width), height_(height),
      current_(static_cast<std::size_t>(width) * height),
      next_(static_cast<std::size_t>(width) * height),
      active_(static_cast<std::size_t>(width) * height, 1),
      changed_(static_cast<std::size_t>(width) * height) {
  auto rd = std::random_device{};
  auto gen = std::mt19937{rd()};
  auto dist = std::uniform_real_distribution<double>{0.0, 1.0};

  for (auto &cell : current_) {
    cell = dist(gen) < density ? std::uint8_t{1} : std::uint8_t{0};
  }
}

unsigned int Grid::count_neighbors(unsigned int x, unsigned int y) const {
  auto count = 0u;
  for_each_neighbor(x, y, [&](unsigned int nx, unsigned int ny) {
    if (current_[index(nx, ny)] != 0)
      ++count;
  });
  return count;
}

void Grid::mark_neighbors_active(unsigned int x, unsigned int y) {
  for_each_neighbor(x, y, [&](unsigned int nx, unsigned int ny) {
    active_[index(nx, ny)] = 1;
  });
}

void Grid::tick() {
  std::fill(changed_.begin(), changed_.end(), std::uint8_t{0});

  for (auto y = 0u; y < height_; ++y) {
    for (auto x = 0u; x < width_; ++x) {
      auto idx = index(x, y);

      if (active_[idx] == 0) {
        next_[idx] = current_[idx];
        continue;
      }

      auto neighbors = count_neighbors(x, y);
      auto alive = current_[idx];
      auto next_state =
          alive ? (neighbors == 2 || neighbors == 3) : (neighbors == 3);

      next_[idx] = next_state ? std::uint8_t{1} : std::uint8_t{0};
      changed_[idx] =
          (next_[idx] != current_[idx]) ? std::uint8_t{1} : std::uint8_t{0};
    }
  }

  std::swap(current_, next_);

  std::fill(active_.begin(), active_.end(), std::uint8_t{0});
  for (auto y = 0u; y < height_; ++y) {
    for (auto x = 0u; x < width_; ++x) {
      if (changed_[index(x, y)] != 0) {
        mark_neighbors_active(x, y);
      }
    }
  }
}
