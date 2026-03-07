# Game of Life

Conway's Game of Life implemented in C++23 with SFML 3. Each pixel represents a cell.

<!-- Replace the link below with your video/gif -->
https://github.com/user-attachments/assets/VIDEO_ID

## How It Works

### Simulation

The simulation follows the standard [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) rules. Each cell is either alive or dead, and its next state depends on its eight neighbors:

- A live cell with 2 or 3 neighbors survives; otherwise it dies.
- A dead cell with exactly 3 neighbors becomes alive.

The grid uses a **double-buffered** approach: two flat `vector<uint8_t>` buffers (`current` and `next`) are swapped each generation, avoiding allocations during the simulation loop.

### Active Cell Tracking

To avoid recomputing stable regions, a third buffer tracks which cells are **active** — meaning they or a neighbor changed in the previous generation. Cells that haven't changed and whose neighbors haven't changed are skipped entirely. On a typical board, this reduces the per-tick work significantly as the simulation stabilizes.

### Rendering

Each pixel maps 1:1 to a cell. A per-cell **brightness** buffer drives a green glow effect:

- **Live cells** ramp up in brightness over successive frames.
- **Newly dead cells** fade out gradually, leaving a dim green trail.
- **Long-dead cells** render as black.

The pixel buffer (RGBA, 4 bytes per cell) is uploaded to an `sf::Texture` each frame and drawn as a single `sf::Sprite`.

## Prerequisites

- CMake 3.28+
- C++23 compiler (Clang 17+, GCC 14+, or Apple Clang 17+)
- SFML 3

### macOS

```bash
brew install cmake sfml
```

### Linux

> **Note:** This project requires SFML 3. Most Linux distros currently ship SFML 2.x in their package managers. If `make configure` fails with a version error, you may need to [build SFML 3 from source](https://github.com/SFML/SFML).

Ubuntu/Debian:

```bash
sudo apt install cmake g++ libsfml-dev
```

Fedora:

```bash
sudo dnf install cmake gcc-c++ SFML-devel
```

Arch Linux:

```bash
sudo pacman -S cmake gcc sfml
```

## Building

Configure both build targets (only needed once or after changing `CMakeLists.txt`):

```bash
make configure
```

Build and run:

```bash
make debug    # debug build with sanitizers
make release  # optimized release build
```

Other targets:

```bash
make format   # format all source files with clang-format
make clean    # remove all build artifacts
```

## Usage

```bash
./build/release/game-of-life [width] [height] [density]
```

| Argument  | Default | Description                                         |
|-----------|---------|-----------------------------------------------------|
| `width`   | 1600    | Window width in pixels                              |
| `height`  | 900     | Window height in pixels                             |
| `density` | 0.5     | Initial probability of a cell being alive (0.0-1.0) |

### Examples

```bash
make release                              # run with defaults (1600x900, 50% density)
./build/release/game-of-life 800 600      # smaller window
./build/release/game-of-life 1600 900 0.3 # sparser initial population
```
