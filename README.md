# Game of Life

Conway's Game of Life implemented in C++23 with SFML 3. Each pixel represents a cell.

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

| Argument  | Default | Description                          |
|-----------|---------|--------------------------------------|
| `width`   | 1600    | Window width in pixels               |
| `height`  | 900     | Window height in pixels              |
| `density` | 0.5     | Initial probability of a cell being alive (0.0-1.0) |

### Examples

```bash
make release                              # run with defaults (1600x900, 50% density)
./build/release/game-of-life 800 600      # smaller window
./build/release/game-of-life 1600 900 0.3 # sparser initial population
```
