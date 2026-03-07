#include "grid.hpp"
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <span>
#include <string>
#include <vector>

struct Config {
  unsigned int width = 1600;
  unsigned int height = 900;
  double density = 0.5;
};

Config parse_args(int argc, char *argv[]) {
  auto config = Config{};

  if (argc >= 3) {
    config.width = static_cast<unsigned int>(std::stoul(argv[1]));
    config.height = static_cast<unsigned int>(std::stoul(argv[2]));
  }

  if (argc >= 4) {
    config.density = std::stod(argv[3]);
  }

  return config;
}

void render(Grid const &grid, std::span<std::uint8_t> pixels) {
  for (auto y = 0u; y < grid.get_height(); ++y) {
    for (auto x = 0u; x < grid.get_width(); ++x) {
      auto idx = (static_cast<std::size_t>(y) * grid.get_width() + x) * 4;
      auto color = grid.is_alive(x, y) ? std::uint8_t{255} : std::uint8_t{0};
      pixels[idx + 0] = color;
      pixels[idx + 1] = color;
      pixels[idx + 2] = color;
      pixels[idx + 3] = 255;
    }
  }
}

int main(int argc, char *argv[]) {
  auto const config = parse_args(argc, argv);
  auto grid = Grid(config.width, config.height, config.density);

  sf::RenderWindow window(sf::VideoMode({config.width, config.height}),
                          "Game of Life");

  auto texture = sf::Texture(sf::Vector2u{config.width, config.height});
  auto sprite = sf::Sprite(texture);
  auto pixels = std::vector<std::uint8_t>(
      static_cast<std::size_t>(config.width) * config.height * 4);

  auto clock = sf::Clock{};
  auto frame_count = 0u;
  auto fps_timer = sf::Clock{};

  while (window.isOpen()) {
    while (auto event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    grid.tick();
    render(grid, pixels);

    texture.update(pixels.data());
    window.clear();
    window.draw(sprite);
    window.display();

    ++frame_count;
    if (fps_timer.getElapsedTime().asSeconds() >= 1.0f) {
      window.setTitle("Game of Life (" + std::to_string(frame_count) + " FPS)");
      frame_count = 0u;
      fps_timer.restart();
    }
  }
}
