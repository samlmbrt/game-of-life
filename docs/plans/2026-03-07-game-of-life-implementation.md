# Game of Life Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Implement Conway's Game of Life where each pixel is a cell, with random initialization and naive sequential simulation.

**Architecture:** Grid class (grid.hpp/cpp) owns two flat buffers and handles simulation. Main.cpp handles CLI args, rendering via sf::Texture, and the game loop.

**Tech Stack:** C++23, SFML 3, CMake

---

### Task 1: Create Grid class with construction and random initialization

**Files:**
- Create: `src/grid.hpp`
- Create: `src/grid.cpp`
- Modify: `CMakeLists.txt:9` (add grid.cpp to executable)

**Step 1: Create grid.hpp**

```cpp
#pragma once

#include <vector>

class Grid {
public:
  Grid(unsigned int width, unsigned int height, double density);

  void tick();
  [[nodiscard]] bool is_alive(unsigned int x, unsigned int y) const;
  [[nodiscard]] unsigned int get_width() const;
  [[nodiscard]] unsigned int get_height() const;

private:
  unsigned int width_;
  unsigned int height_;
  std::vector<bool> current_;
  std::vector<bool> next_;

  [[nodiscard]] int count_neighbors(unsigned int x, unsigned int y) const;
};
```

**Step 2: Create grid.cpp with constructor and is_alive**

```cpp
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
    cell = dist(gen) < density;
  }
}

bool Grid::is_alive(unsigned int x, unsigned int y) const {
  return current_[static_cast<std::size_t>(y) * width_ + x];
}

unsigned int Grid::get_width() const { return width_; }

unsigned int Grid::get_height() const { return height_; }
```

Leave `tick()` and `count_neighbors()` as stubs for now:

```cpp
int Grid::count_neighbors(unsigned int /*x*/, unsigned int /*y*/) const {
  return 0;
}

void Grid::tick() {}
```

**Step 3: Add grid.cpp to CMakeLists.txt**

Change line 9 from:
```cmake
add_executable(game-of-life src/main.cpp)
```
to:
```cmake
add_executable(game-of-life src/main.cpp src/grid.cpp)
```

**Step 4: Build to verify it compiles**

Run: `cmake --build --preset debug`
Expected: builds successfully

**Step 5: Commit**

```
git add src/grid.hpp src/grid.cpp CMakeLists.txt
git commit -m "Add Grid class with random initialization"
```

---

### Task 2: Implement neighbor counting

**Files:**
- Modify: `src/grid.cpp`

**Step 1: Implement count_neighbors**

Replace the stub with:

```cpp
int Grid::count_neighbors(unsigned int x, unsigned int y) const {
  auto count = 0;
  for (auto dy = -1; dy <= 1; ++dy) {
    for (auto dx = -1; dx <= 1; ++dx) {
      if (dx == 0 && dy == 0)
        continue;

      auto nx = static_cast<int>(x) + dx;
      auto ny = static_cast<int>(y) + dy;

      if (nx < 0 || ny < 0 || nx >= static_cast<int>(width_) ||
          ny >= static_cast<int>(height_))
        continue;

      if (current_[static_cast<std::size_t>(ny) * width_ +
                   static_cast<std::size_t>(nx)])
        ++count;
    }
  }
  return count;
}
```

**Step 2: Build to verify it compiles**

Run: `cmake --build --preset debug`
Expected: builds successfully

**Step 3: Commit**

```
git add src/grid.cpp
git commit -m "Implement neighbor counting for Grid"
```

---

### Task 3: Implement tick (simulation step)

**Files:**
- Modify: `src/grid.cpp`

**Step 1: Implement tick**

Replace the stub with:

```cpp
void Grid::tick() {
  for (auto y = 0u; y < height_; ++y) {
    for (auto x = 0u; x < width_; ++x) {
      auto neighbors = count_neighbors(x, y);
      auto idx = static_cast<std::size_t>(y) * width_ + x;
      auto alive = current_[idx];

      // Conway's rules:
      // alive + 2 or 3 neighbors -> stays alive
      // dead + exactly 3 neighbors -> becomes alive
      // everything else -> dead
      next_[idx] = alive ? (neighbors == 2 || neighbors == 3) : (neighbors == 3);
    }
  }
  std::swap(current_, next_);
}
```

**Step 2: Build to verify it compiles**

Run: `cmake --build --preset debug`
Expected: builds successfully

**Step 3: Commit**

```
git add src/grid.cpp
git commit -m "Implement tick with Conway's rules"
```

---

### Task 4: Wire up rendering and game loop in main.cpp

**Files:**
- Modify: `src/main.cpp`

**Step 1: Update main.cpp**

Replace the entire file with:

```cpp
#include "grid.hpp"
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  auto width = 1600u;
  auto height = 900u;
  auto density = 0.5;

  if (argc >= 3) {
    width = static_cast<unsigned int>(std::stoul(argv[1]));
    height = static_cast<unsigned int>(std::stoul(argv[2]));
  }
  if (argc >= 4) {
    density = std::stod(argv[3]);
  }

  auto grid = Grid(width, height, density);

  sf::RenderWindow window(sf::VideoMode({width, height}), "Game of Life");
  auto texture = sf::Texture(sf::Vector2u{width, height});
  auto sprite = sf::Sprite(texture);
  auto pixels = std::vector<std::uint8_t>(
      static_cast<std::size_t>(width) * height * 4);

  while (window.isOpen()) {
    while (auto event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    grid.tick();

    for (auto y = 0u; y < height; ++y) {
      for (auto x = 0u; x < width; ++x) {
        auto idx = (static_cast<std::size_t>(y) * width + x) * 4;
        auto color = grid.is_alive(x, y) ? std::uint8_t{255} : std::uint8_t{0};
        pixels[idx + 0] = color; // R
        pixels[idx + 1] = color; // G
        pixels[idx + 2] = color; // B
        pixels[idx + 3] = 255;   // A
      }
    }

    texture.update(pixels.data());
    window.clear();
    window.draw(sprite);
    window.display();
  }
}
```

**Step 2: Build to verify it compiles**

Run: `cmake --build --preset debug`
Expected: builds successfully

**Step 3: Run it**

Run: `make debug`
Expected: window shows Game of Life simulation running. White pixels alive, black dead.

**Step 4: Commit**

```
git add src/main.cpp
git commit -m "Wire up rendering and game loop"
```
