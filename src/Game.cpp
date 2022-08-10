#include <cmath>
#include <ctime>
#include <cstdlib>
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
  //  Read in config file here 
  //       use the premade PlayerConfig, EnemyConfig, BulletConfig variables
  
  // Read in the window size 
  //std::ifstream fin(path);
  
  // fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S;
  // set up default window parameters

  std::ifstream fin(config);

  std::string name;
  int x, y, fps;
  
  while (fin >> name)
  {
    fin >> x >> y >> fps;
  }

  m_window.create(sf::VideoMode(x, y), "Alien Shooter");
  m_window.setFramerateLimit(60);

  spawnPlayer();
  spawnEnemy();

};

void Game::run()
{
  // TODO: add pause functionality in here 
  //       some systems should function while paused (rendering)
  //       some systems shouldn't (movement / input)
  std::srand(std::time(0));
  
  sf::Clock clock;
  while (m_running)
  {
    // Compute the frame rate 
    float currentTime = clock.restart().asSeconds();
    float fps = 1.0f / (currentTime);
    
    // output for showing frames per second on the console
    // std::cout << "fps: " << fps << std::endl;

    m_entities.update();

    if(!m_paused)
    {
      // sEnemySpawner();
    }
      // if paused only run these systems  
      sMovement();
      if(m_currentFrame > 120) 
      {
        sCollision();
      }
      sUserInput();
      sLifespan();
      sRender();
    // increment the current frame 
    // may need to be moved when pause implemented 
    m_currentFrame++;
  }
}
void Game::spawnPlayer()
{
  // We create every entity by calling EntityManager.addEntity(tag) 
  // This returns a std::shared_ptr<Entity>, so we use 'auto' to save typing 
  std::ifstream fin("../assets/config/player.txt");

  std::string name;
  
  while (fin >> name)
  {
    fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG 
        >> m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V >> m_playerConfig.S; 
  }
  auto entity = m_entities.addEntity("player");

  // Give this entity a Transform so it spawns at (200, 200) with velocity of (1, 1) and angle 0 
  entity->cTransform = std::make_shared<CTransform>(Vec2(200.0f, 200.0f), Vec2(0.0f, 0.0f), 0.0f);

  // The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4 
  entity->cShape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V, 
      sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), 
      sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);

  // Add an input component to the player so that we can use inputs 
  entity->cInput = std::make_shared<CInput>();

  // Add a collision component to player so we can detect collisions with other enemies
  entity->cCollision = std::make_shared<CCollision>(m_playerConfig.SR);

  // Since we want this Entity to be our player, set our Game's player variable to be this Entity 
  // This goes slightly against the EntityManager paradigm, but we use the player so much it's worth it 
  m_player = entity;
}

void Game::spawnEnemy() 
{
  std::ifstream fin("../assets/config/enemy.txt");

  std::string name;
  EnemyConfig ec;
  std::vector<EnemyConfig> enemies;
  // Read in enemy data from enemy config
  // Store each struct of enemy config data in a vector enemies
  while (fin >> name)
  {
    fin >> ec.SR >> ec.CR >> ec.OR >> ec.OG >> ec.OB >> ec.OT >> 
      ec.VMIN >> ec.VMAX >> ec.L >> ec.SI >> ec.SMIN >> ec.SMAX;
    enemies.push_back(ec);
  }


  // Add enemey entities and construct properties & by the number of enemies in the enemy config data 
  for (auto e : enemies)
  {
    auto entity = m_entities.addEntity("enemy");


    // Give this entity a Transform so it spawns at random x, y within sceen resolution and random velocity between (-5, -5) and (5, 5)  
    entity->cTransform = std::make_shared<CTransform>(Vec2((rand() % 1280),(rand() % 720)), Vec2((rand() % 10)-5.1, (rand() % 10) -5.1), 0.0f);

    // The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4 
    entity->cShape = std::make_shared<CShape>(e.SR, e.VMIN, 
        sf::Color(0, 0, 0), 
        sf::Color(e.OR, e.OG, e.OB), e.OT);

    // Give the enemies a collision component so that we can detetc collision with the player and bullets
    entity->cCollision = std::make_shared<CCollision>(e.SR);    

    // record when the most recent enemy was spawned 
    m_lastEnemySpawnTime = m_currentFrame; 

  }

}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> e) 
{
  // TODO: spawn small enemies at the location of the input enemy e 

  int vertices = e->cShape->circle.getPointCount();
  
  for (int i = 0; i < vertices; i++)
  {
    auto smallEnemy = m_entities.addEntity("small");
    smallEnemy->cTransform = std::make_shared<CTransform>(e->cTransform->pos, Vec2( (rand() % 10)-5.1, (rand() % 10) -5.1), 0.0f);

    smallEnemy->cShape = std::make_shared<CShape>(e->cShape->circle.getRadius(), 
                                                  e->cShape->circle.getPointCount(), 
                                                  e->cShape->circle.getFillColor(), 
                                                  e->cShape->circle.getOutlineColor(), 
                                                  e->cShape->circle.getOutlineThickness());
    int radius = e->cShape->circle.getRadius();
    smallEnemy->cShape->circle.setRadius(radius / 2.0f);

    smallEnemy->cCollision = std::make_shared<CCollision>(smallEnemy->cShape->circle.getRadius());

    smallEnemy->cLifespan = std::make_shared<CLifespan>(120, m_currentFrame);
    // smallEnemy->cScore = std::make_shared<CScore>(e->cScore->score*2);
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
  // spawns a bullet from a given entity to a target location 
  // implement the spawning of a bullet which travels towards a target 
  // -- bullet speed is given as a scalar speed 
  // -- you must set the velocity by using formula in notes
  // bring in all the of the BulletConfig attributes from the bullet config file 

  std::ifstream fin("../assets/config/bullet.txt");

  std::string name;

  while (fin >> name)
  {
    fin >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB >> m_bulletConfig.OR 
        >> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
  }
  
  // Calculate the velocity vector for the bullet based on mouse position relative to the player 
  Vec2 D(target.x - entity->cTransform->pos.x, target.y - entity->cTransform->pos.y);
  D.normalize();
  Vec2 N = D;
  N *= m_bulletConfig.S;
  Vec2 velocity = N;
  
  auto bullet = m_entities.addEntity("bullet");

  // Give this entity a Transform so it spawns at (400, 400) with velocity of (0, 0) and angle 0 
  bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, velocity, 0.0f);

  // The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4 
  bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V, 
        sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),  
        sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);

  // Give the bullet a collision component to detect collisions between bullets and enemies
  bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.SR);

  // Give the bullet a lifespan component 
  bullet->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L, m_currentFrame);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
  // TODO: implement a special weapon
}

void Game::sMovement()
{
  Vec2 playerVelocity;

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
    if(e->tag() == "player")
    {
      e->cTransform->pos += e->cTransform->velocity;
      if(e->cTransform->pos.x < 0)
      {
        e->cTransform->pos.x = 0;
      }
      if(e->cTransform->pos.x > m_window.getSize().x)
      {
        e->cTransform->pos.x = m_window.getSize().x;
      }
      if(e->cTransform->pos.y < 0)
      {
        e->cTransform->pos.y = 0;
      }
      if(e->cTransform->pos.y > m_window.getSize().y)
      {
        e->cTransform->pos.y = m_window.getSize().y;
      }
    }
  }
  for (auto e : m_entities.getEntities()) 
  {
    if(e->tag() != "player")
    {
      e->cTransform->pos += e->cTransform->velocity;
      if(e->cTransform->pos.x < 0)
      {
        e->cTransform->velocity.x *= -1;
      }
      if(e->cTransform->pos.x > m_window.getSize().x)
      {
        e->cTransform->velocity.x *= -1;
      }
      if(e->cTransform->pos.y < 0)
      {
        e->cTransform->velocity.y *= -1;
      }
      if(e->cTransform->pos.y > m_window.getSize().y)
      {
        e->cTransform->velocity.y *= -1;
      }
    }
  }


}

void Game::sLifespan()
{
  // TODO: implement all lifespan functionality
  // for all entities
  //    if entity has no lifespan component, skip it 
  //    if entity has > 0 current lifespan, subtract 1 
  //    if it has lifespan and is alive 

  auto entities = m_entities.getEntities();
  for (auto e : entities) 
  {
    if(!e->cLifespan) { continue; }
      // output for showing entities lifespan on the console 
      // std::cout << "lifespan: " << e->cLifespan->remaining << std::endl;
      
      e->cLifespan->remaining -= 1;
      auto current_color = e->cShape->circle.getOutlineColor();
      float current_percent = static_cast<float>(e->cLifespan->remaining) /  static_cast<float>(e->cLifespan->total);
      current_color.a = 255 * current_percent;
      auto new_color = sf::Color(current_color.r, current_color.g, current_color.b, current_color.a);
      e->cShape->circle.setOutlineColor(new_color);
      if(e->cLifespan->remaining < 0)
      {
        e->destroy();
      }
  }

  //auto color = m_player->cShape->circle.getFillColor();
  // int newAlpha = 100;
  // sf::Color newColor(color.r, color.g, color.b, newAlpha);
  //         scale its alpha channel properly 
  //    if it has lifespan and its time is up 
  //         destroy the entity 
}

void Game::sCollision()
{
  // Detect collisions between the player and enemies 
    for (auto e : m_entities.getEntities("enemy"))
    {
      // if there's a collision between p and e 
      if ( abs(e->cTransform->pos.x - m_player->cTransform->pos.x) < e->cCollision->radius + m_player->cCollision->radius && 
            abs(e->cTransform->pos.y - m_player->cTransform->pos.y) < e->cCollision->radius + m_player->cCollision->radius)
      {
        m_player->destroy();
      }
    }
    for (auto e : m_entities.getEntities("small"))
    {
      // if there's a collision between p and e 
      if ( abs(e->cTransform->pos.x - m_player->cTransform->pos.x) < e->cCollision->radius + m_player->cCollision->radius && 
            abs(e->cTransform->pos.y - m_player->cTransform->pos.y) < e->cCollision->radius + m_player->cCollision->radius)
      {
        m_player->destroy();
      }
    }
  // Detetc collisions between enemies and bullets
  for (auto b : m_entities.getEntities("bullet"))
  {
    for (auto e : m_entities.getEntities("enemy"))
    {
      // if there's a collision between b and e 
      if ( abs(e->cTransform->pos.x - b->cTransform->pos.x) < e->cCollision->radius + b->cCollision->radius && 
            abs(e->cTransform->pos.y - b->cTransform->pos.y) < e->cCollision->radius + b->cCollision->radius)
      {
        spawnSmallEnemies(e);
        e->destroy();
        b->destroy();
      }
    }
  }

  for (auto b : m_entities.getEntities("bullet"))
  {
    for (auto e : m_entities.getEntities("small"))
    {
      // if there's a collision between b and e 
      if ( abs(e->cTransform->pos.x - b->cTransform->pos.x) < e->cCollision->radius + b->cCollision->radius && 
            abs(e->cTransform->pos.y - b->cTransform->pos.y) < e->cCollision->radius + b->cCollision->radius)
      {
        e->destroy();
        b->destroy();
      }
    }
  }
}

void Game::sRender()
{
  // set the players rotation angle to change on each update
  m_player->cTransform->angle += 5.0f;
  // Draw enemies 
  m_window.clear();
  for (auto e : m_entities.getEntities())
  {
    if(e->tag() != "player")
    {
      e->cTransform->angle += 3.0f;
    }
    e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
    e->cShape->circle.setRotation(e->cTransform->angle);
    m_window.draw(e->cShape->circle);
  }
  m_window.display();
}

void Game::sUserInput()
{
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
          m_player->cInput->up = true;
          break;

        case sf::Keyboard::S:
          std::cout << "S Key Pressed\n";
          m_player->cInput->down = true;
          break;

        case sf::Keyboard::A:
          std::cout << "A Key Pressed\n";
          m_player->cInput->left = true;
          break;

        case sf::Keyboard::D:
          std::cout << "D Key Pressed\n";
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

        // call spawnBullet here 
        Vec2 mouse_pos(event.mouseButton.x, event.mouseButton.y);
        spawnBullet(m_player, mouse_pos);
      }

      if (event.mouseButton.button == sf::Mouse::Right)
      {
        std::cout << "Right Mouse Button Clicked at (" << event.mouseButton.x << ", "
                                                       << event.mouseButton.y << ")\n";
      }
    }
  }
}
