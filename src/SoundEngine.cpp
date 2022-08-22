#include "../include/Common.h"
#include <assert.h>
#include "../include/SoundEngine.h"

SoundEngine* SoundEngine::m_s_Instance = nullptr;

SoundEngine::SoundEngine()
{
  assert(m_s_Instance == nullptr);
  m_s_Instance = this;

  // Load the sound into the buffers
  m_ShootBuffer.loadFromFile("../assets/sounds/Shoot.wav");
  m_ShootSound.setBuffer(m_ShootBuffer); 

  m_PlayerExplodeBuffer.loadFromFile("../assets/sounds/PlayerExplode.wav");
  m_PlayerExplodeSound.setBuffer(m_PlayerExplodeBuffer);

  m_EnemyExplodeBuffer.loadFromFile("../assets/sounds/EnemyExplode.wav");
  m_EnemyExplodeSound.setBuffer(m_EnemyExplodeBuffer);

  m_ClickBuffer.loadFromFile("../assets/sounds/Click.wav");
  m_ClickSound.setBuffer(m_ClickBuffer);

  m_PowerupBuffer.loadFromFile("../assets/sounds/Powerup.wav");
  m_PowerupSound.setBuffer(m_PowerupBuffer);

  m_Music.openFromFile("../assets/sounds/Music.wav");
  m_Music.setLoop(true);
}

void SoundEngine::playShoot()
{
  m_s_Instance->m_ShootSound.play();
}

void SoundEngine::playPlayerExplode()
{
  m_s_Instance->m_PlayerExplodeSound.play();
}

void SoundEngine::playEnemyExplode()
{
  m_s_Instance->m_EnemyExplodeSound.play();
}

void SoundEngine::playClick()
{
  m_s_Instance->m_ClickSound.play();
}

void SoundEngine::playPowerup()
{
  m_s_Instance->m_PowerupSound.play();
}

void SoundEngine::playMusic()
{
  m_s_Instance->m_Music.play();
}

void SoundEngine::stopMusic()
{
  m_s_Instance->m_Music.stop();
}

void SoundEngine::pauseMusic()
{
  m_s_Instance->m_Music.pause();
}
