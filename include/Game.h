#include <SFML/Graphics.hpp>
#include "EntityManager.h"
#include "Entity.h"

class Game
{

private:
  
  // Private member variables
  sf::RenderWindow m_window;
  EntityManager    m_entities;
  Entity           m_player;
  bool             m_paused;
  bool             m_running;

  // Private member functions
  void init();

  // Systems
  void sMovement();
  void sUserInput();
  void sRender();
  void sEnemySpawner();
  void sCollision();

public:

  // Public member functions
  void update();

};
