#include "../include/Common.h"
#include "../include/Game.h"

Game::Game(const std::string & config)
{
  init(config);
  // Example config file 
  /* Window 1280 720 60 0
     Font fonts/arial.ttf 24 255 255 255 
     Player 32 32 5 5 5 5 0 0 4 8
     Enemy 32 32 3 3 255 255 2 3 8 90 120 
     Bullet 10 10 20 255 255 255 255 255 255 2 20 90 
     */
}

void Game::init(const std::string & config)
{
  // TODO: Read in config file here 
  //       use the premade PlayerConfig, EnemyConfig, BulletConfig variables
  
  // Read in the window size 
  //std::ifstream fin(path);
  
  // fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S;
  // set up default window parameters
  m_window.create(sf::VideoMode(1280, 720), "Alien Shooter");
  m_window.setFramerateLimit(60);

  spawnPlayer();
  spawnEnemy();
  spawnEnemy();

};

void Game::run()
{
  // TODO: add pause functionality in here 
  //       some systems should function while paused (rendering)
  //       some systems shouldn't (movement / input)
  while (m_running)
  {
    m_entities.update();

    if(!m_paused)
    {
      // sEnemySpawner();
      // sCollision();
    }
      // if paused only run these systems  
      sUserInput();
      sMovement();
      sRender();
    // increment the current frame 
    // may need to be moved when pause implemented 
    m_currentFrame++;
  }
}

void Game::spawnPlayer()
{
  // TODO: Finish adding all the properties of the player with the correct values from the config 
  //       Then use those in the constructor of the player components 
  // We create every entity by calling EntityManager.addEntity(tag) 
  // This returns a std::shared_ptr<Entity>, so we use 'auto' to save typing 
  auto entity = m_entities.addEntity("player");

  // Give this entity a Transform so it spawns at (200, 200) with velocity of (1, 1) and angle 0 
  entity->cTransform = std::make_shared<CTransform>(Vec2(200.0f, 200.0f), Vec2(1.0f, 1.0f), 0.0f);

  // The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4 
  entity->cShape = std::make_shared<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);

  // Add an input component to the player so that we can use inputs 
  entity->cInput = std::make_shared<CInput>();

  // Since we want this Entity to be our player, set our Game's player variable to be this Entity 
  // This goes slightly against the EntityManager paradigm, but we use the player so much it's worth it 
  m_player = entity;
}

void Game::spawnEnemy() 
{
  // TODO: make sure the enemy is spawned properly with the m_enemyConfig variables
  //       the enemy must be spawned completely within the bounds of the window 

  auto entity = m_entities.addEntity("enemy");

  // Give this entity a Transform so it spawns at (400, 400) with velocity of (0, 0) and angle 0 
  entity->cTransform = std::make_shared<CTransform>(Vec2(400.0f, 400.0f), Vec2(0.0f, 0.0f), 0.0f);

  // The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4 
  entity->cShape = std::make_shared<CShape>(32.0f, 4, sf::Color(10, 10, 10), sf::Color(0, 255, 0), 5.0f);

  // record when the most recent enemy was spawned 
  m_lastEnemySpawnTime = m_currentFrame; 

}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> e) 
{
  // TODO: spawn small enemies at the location of the input enemy e 

  int vertices = e->cShape->circle.getPointCount();
  
  for (int i = 0; i < vertices; i++)
  {
    auto smallEnemy = m_entities.addEntity("small");
    smallEnemy->cScore = std::make_shared<CScore>(e->cScore->score*2);
  }

  // When we create the smaller enemy, we have to read the values of the original enemy 
  // - spawn a number of small enemies equal to the vertices of the original enemy 
  // - set each small enemy to the same color as the original, half the size 
  // - small enemies are worth double the points of the original enemy 
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2 & target)
{
  // entity is the entity shooting the bullet (typically the player) 
  // target is the mouse position 
  // TODO spawns a bullet from a given entity to a target location 
  // implement the spawning of a bullet which travels towards a target 
  // -- bullet speed is given as a scalar speed 
  // -- you must set the velocity by using formula in notes
  // bring in all the of the BulletConfig attributes from the bullet config file 

}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
  // TODO: implement a special weapon
}

void Game::sMovement()
{
  // TODO: Implement all entity movement in this function 
  // you should read the m_player->cInput component to determine if the player is moving 
  Vec2 playerVelocity;
  m_playerConfig.S = 0.1f;

  if (m_player->cInput->left)
  {
    playerVelocity.x -= m_playerConfig.S;
  }

  if (m_player->cInput->right)
  {
    playerVelocity.x += m_playerConfig.S;
  }

  if (m_player->cInput->up)
  {
    playerVelocity.y -= m_playerConfig.S;
  }

  if (m_player->cInput->down)
  {
    playerVelocity.y += m_playerConfig.S;
  }

  m_player->cTransform->velocity = playerVelocity;

  for (auto e : m_entities.getEntities()) 
  {
    e->cTransform->pos += e->cTransform->velocity;
  }

}

void Game::sLifespan()
{
  // TODO: implement all lifespan functionality
  // for all entities
  //    if entity has no lifespan component, skip it 
  //    if entity has > 0 current lifespan, subtract 1 
  //    if it has lifespan and is alive 
  auto color = m_player->cShape->circle.getFillColor();
  int newAlpha = 100;
  sf::Color newColor(color.r, color.g, color.b, newAlpha);
  //         scale its alpha channel properly 
  //    if it has lifespan and its time is up 
  //         destroy the entity 
}

void Game::sCollision()
{
  for (auto p : m_entities.getEntities("player"))
  {
    for (auto e : m_entities.getEntities("enemy"))
    {
      // if there's a collision between pe, and e 
      // float dist = p->cTransform->pos.dist(e->cTransform->pos);
      // p->cCollision->radius;
    }
  }
}

void Game::sRender()
{
  // TODO: change the code below to draw all of the Entities
  // sample drawing of the player Entity we have created 
  m_window.clear();

  // Set the position of the shape based on the entity's transform->pos 
  m_player->cShape->circle.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);

  // Set the rotation of the shape based on the entity's transform angle 
  m_player->cTransform->angle += 1.0f;
  m_player->cShape->circle.setRotation(m_player->cTransform->angle);

  // Draw the entity's sf::CircleShape
  m_window.draw(m_player->cShape->circle);
  // Draw enemies 
  auto enemy = m_entities.getEntities().back(); 
  enemy->cShape->circle.setPosition(enemy->cTransform->pos.x, enemy->cTransform->pos.y);
  m_window.draw(enemy->cShape->circle);

  m_window.display();
}

void Game::sUserInput()
{
  // TODO: handle user input here 
  //       note that should only be setting the player's input component variables heere 
  //       should not implement the player's movement logic here 
  //       the movement system will read the variables you set in this function 

  sf::Event event;
  while (m_window.pollEvent(event))
  {
    // this event triggers when the window is closed 
    if (event.type == sf::Event::Closed)
    {
      m_running = false;
    }

    // this event is triggered when a key is pressed 
    if (event.type == sf::Event::KeyPressed)
    {
      switch (event.key.code)
      {
        case sf::Keyboard::W:
          std::cout << "W Key Pressed\n";
          // TODO: set player's input component "up" variable to true 
          m_player->cInput->up = true;
          break;

        case sf::Keyboard::S:
          std::cout << "S Key Pressed\n";
          // TODO: set player's input component "up" variable to true 
          m_player->cInput->down = true;
          break;

        case sf::Keyboard::A:
          std::cout << "A Key Pressed\n";
          // TODO: set player's input component "up" variable to true 
          m_player->cInput->left = true;
          break;

        case sf::Keyboard::D:
          std::cout << "D Key Pressed\n";
          // TODO: set player's input component "up" variable to true 
          m_player->cInput->right = true;
          break;
      }
    }

    // This event it triggered when a key is released 
    if (event.type == sf::Event::KeyReleased)
    {
      switch (event.key.code)
      {
        case sf::Keyboard::W:
          std::cout << "W Key Released\n";
          m_player->cInput->up = false;
          break;

        case sf::Keyboard::S:
          std::cout << "S Key Released\n";
          m_player->cInput->down = false;
          break;

        case sf::Keyboard::A:
          std::cout << "A Key Released\n";
          m_player->cInput->left = false;
          break;

        case sf::Keyboard::D:
          std::cout << "D Key Released\n";
          m_player->cInput->right = false;
          break;
      }
    }

    if (event.type == sf::Event::MouseButtonPressed)
    {
      if (event.mouseButton.button == sf::Mouse::Left)
      {
        std::cout << "Left Mouse Button Clicked at (" << event.mouseButton.x << ", " 
                                                      << event.mouseButton.y << ")\n";
        for (auto e : m_entities.getEntities())
        {
          std::cout << "id: " << e->id() << " tag: " << e->tag() << std::endl;
        }
        // call spawnBullet here 
      }

      if (event.mouseButton.button == sf::Mouse::Right)
      {
        std::cout << "Right Mouse Button Clicked at (" << event.mouseButton.x << ", "
                                                       << event.mouseButton.y << ")\n";
        auto entities = m_entities.getEntities();
        entities.back()->destroy();
        // call spawnSpecialWeapon here
      }
    }
  }
}
