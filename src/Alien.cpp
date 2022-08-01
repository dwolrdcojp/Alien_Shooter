// Compile command
// g++ --std=c++17 *.cpp -I /opt/homebrew/Cellar/sfml/2.5.1_1 -o prog -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


int main()
{
  sf::RenderWindow window(sf::VideoMode(800, 600), "Alien Shooter");

  sf::CircleShape circle(100);
  circle.setFillColor(sf::Color(255, 0, 0));
  circle.setOutlineThickness(10.0f);
  circle.setOutlineColor(sf::Color(255, 255, 0));
  circle.setOrigin(100.f, 100.f);
  circle.setPosition(sf::Vector2f(window.getSize().x/2.0, window.getSize().y/2.0));

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed) { window.close(); }
    }

    // Clear the window with black
    window.clear(sf::Color::Black);

    // Draw stuff here 
    window.draw(circle);
    
    // End the current frame
    window.display();

  }

  return 0;
}
