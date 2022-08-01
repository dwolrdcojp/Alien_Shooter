// Compile command
// g++ --std=c++17 *.cpp -I /opt/homebrew/Cellar/sfml/2.5.1_1 -o prog -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
#include <SFML/Window.hpp>

int main()
{
  sf::Window window(sf::VideoMode(800, 600), "Alien Shooter");

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed) { window.close(); }
    }
  }

  return 0;
}
