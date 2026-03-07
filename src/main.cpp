#include <SFML/Graphics.hpp>
#include <string>

int main(int argc, char *argv[]) {
  auto width = 1600u;
  auto height = 900u;

  if (argc >= 3) {
    width = static_cast<unsigned int>(std::stoul(argv[1]));
    height = static_cast<unsigned int>(std::stoul(argv[2]));
  }

  sf::RenderWindow window(sf::VideoMode({width, height}), "Game of Life");

  while (window.isOpen()) {
    while (auto event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    window.clear();
    window.display();
  }
}
