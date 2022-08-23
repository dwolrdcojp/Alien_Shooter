#pragma once 

#include "Common.h"
#include "Entity.h"
#include "EntityManager.h"

struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig  { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };
/*
   Player Config
    -----------
    Shape Radius        SR            int
    Collision Radius    CR            int 
    Fill Color          FR, FG, FB    int, int, int    
    Outline Color       OR, OG, OB    int, int, int 
    Outline Thickness   OT            int 
    Shape Vertices      V             int 
    Speed               S             float 
*/

/*
   Enemy Config
    -----------
    Shape Radius        SR            int
    Collision Radius    CR            int 
    Outline Color       OR, OG, OB    int, int, int 
    Outline Thickness   OT            int 
    Min/Max Vertices    VMIN, VMAX    int, int 
    Small Lifespan      L             int
    Shape Vertices      V             int 
    Spawn Interval      SI            int
    Min / Max Speed     SMIN, SMAX    float
*/

/*
   Bullet Config
    -----------
    Shape Radius        SR            int
    Collision Radius    CR            int 
    Speed               S             float 
    Fill Color          FR, FG, FB    int, int, int    
    Outline Color       OR, OG, OB    int, int, int 
    Outline Thickness   OT            int 
    Shape Vertices      V             int 
    Lifespan            L             int 
*/

class Game
{

private:
  
  // Private member variables
  sf::RenderWindow m_window;             // the window we will draw to
  SoundEngine      m_sound;
  TextureEngine    m_textures;
  EntityManager    m_entities;           // vector of entities to maintain 
  sf::Font         m_font;               // the font we will use to draw 
  sf::Text         m_text;               // the score text to be drawn to the screen 
  PlayerConfig     m_playerConfig;
  EnemyConfig      m_enemyConfig;
  BulletConfig     m_bulletConfig;
  int              m_score = 0;
  int              m_specialTotal = 0;
  int              m_specialUsed = 0;
  int              m_specialAvailable = 0;
  int              m_currentFrame = 0;
  int              m_lastEnemySpawnTime = 0;
  int              m_currentEnemy = 0;
  bool             m_paused = false;     // whether we update game logic
  bool             m_running = true;     // whether the game is running 

  std::shared_ptr<Entity> m_player;
  // Private member functions
  void init(const std::string & config); // initialize the GameState with a config file path
  void setPaused(bool paused);           // Pause the game 

  // Systems
  void sMovement();
  void sUserInput();
  void sLifespan();
  void sRender();
  void sEnemySpawner();
  void sCollision();
  void sSpecialWeapon();

  void spawnPlayer();
  void spawnEnemy();
  void spawnSmallEnemies(std::shared_ptr<Entity> entity);
  void spawnBullet(std::shared_ptr<Entity> entity, const Vec2 & mousePos);
  void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

public:

  Game(const std::string & config);      // constructor, takes in game config
  void run();

};
