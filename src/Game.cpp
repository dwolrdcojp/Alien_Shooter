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
  // TODO: Read in config file here 
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
  while (m_running)
  {
    m_entities.update();

    if(!m_paused)
    {
      // sEnemySpawner();
    }
      // if paused only run these systems  
      sMovement();
      sCollision();
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
  // TODO: Finish adding all the properties of the player with the correct values from the config 
  //       Then use those in the constructor of the player components 
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
  // TODO: make sure the enemy is spawned properly with the m_enemyConfig variables
  //       the enemy must be spawned completely within the bounds of the window 

  // EnemyConfig  { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; };
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

  std::srand(std::time(0));

  // Add enemey entities and construct properties & by the number of enemies in the enemy config data 
  for (auto e : enemies)
  {
    auto entity = m_entities.addEntity("enemy");

    float rand_num_x = (std::rand() % 1280);
    float rand_num_y = (std::rand() % 720);

    // Give this entity a Transform so it spawns at (400, 400) with velocity of (0, 0) and angle 0 
    entity->cTransform = std::make_shared<CTransform>(Vec2(rand_num_x, rand_num_y), Vec2(0.1f, 0.1f), 0.0f);

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
  // 
   // Bullet 10 10 20 255 255 255 255 255 255 20 90
  std::ifstream fin("../assets/config/bullet.txt");

  std::string name;

  while (fin >> name)
  {
    fin >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB >> m_bulletConfig.OR 
        >> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
  }
  
  // Calculate the velocity vector for the bullet based on mouse position relative to the player 
  Vec2 D(target.x - entity->cTransform->pos.x, target.y - entity->cTransform->pos.y);
  std::cout << "D: " << D << std::endl;
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
  // TODO: Implement all entity movement in this function 
  // you should read the m_player->cInput component to determine if the player is moving 
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
    if(e->tag() != "enemy")
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
  for (auto e : m_entities.getEntities("enemy")) 
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

//  std::cout << "m_player velocity: " << m_player->cTransform->velocity << std::endl;

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
    if(e->cLifespan != nullptr) 
    {
      // int framesSinceSpawn = m_currentFrame - e->cLifespan->frameSpawned; 
      e->cLifespan->remaining -= 1;
      if(e->cLifespan->remaining < 0)
      {
        e->destroy();
      }
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
        e->destroy();
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
        e->destroy();
        b->destroy();
      }
    }
  }
}

void Game::sRender()
{
  // TODO: change the code below to draw all of the Entities
  // sample drawing of the player Entity we have created 
  // set the players rotation angle to change on each update
  m_player->cTransform->angle += 5.0f;
  // Draw enemies 
  m_window.clear();
  for (auto e : m_entities.getEntities())
  {
    e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
    e->cShape->circle.setRotation(e->cTransform->angle);
    m_window.draw(e->cShape->circle);
  }
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
