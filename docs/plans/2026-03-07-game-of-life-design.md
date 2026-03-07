# Game of Life — Design

## Overview

Conway's Game of Life where each pixel is a cell, rendered via SFML using a texture/sprite approach with VertexArray planned for later.

## Architecture

Two-file approach: `grid.hpp/cpp` for simulation, `main.cpp` for rendering and input.

## Grid (grid.hpp / grid.cpp)

- `Grid` class with two flat `std::vector<bool>` buffers (current + next), swapped each tick
- Constructor: `Grid(unsigned int width, unsigned int height, double density)`
- Random initialization using `std::mt19937` + `std::random_device`, density controls probability of life (0.0–1.0)
- `tick()`: iterate all cells sequentially, count 8 neighbors, apply Conway's rules, swap buffers
- `is_alive(x, y)`: read access for rendering
- Out-of-bounds neighbors are dead (no wrapping)

## Rendering (main.cpp)

- `sf::Texture` + `sf::Sprite` the size of the window
- Each frame: build RGBA pixel buffer from grid state, upload to texture, draw sprite
- Colors: white (alive), black (dead)
- No frame rate limiting

## CLI Arguments

```
./build/debug/game-of-life [width] [height] [density]
```

- `width`: default 1600
- `height`: default 900
- `density`: default 0.5
