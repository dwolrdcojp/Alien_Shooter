#pragma once
#ifndef SOUND_ENGINE_H
#define SOUND_ENGINE_H

#include <SFML/Audio.hpp>

class SoundEngine {
  private:
    sf::SoundBuffer m_ShootBuffer;
    sf::SoundBuffer m_PlayerExplodeBuffer;
    sf::SoundBuffer m_EnemyExplodeBuffer;
    sf::SoundBuffer m_ClickBuffer;
    sf::SoundBuffer m_PowerupBuffer;

    sf::Sound m_ShootSound;
    sf::Sound m_PlayerExplodeSound;
    sf::Sound m_EnemyExplodeSound;
    sf::Sound m_ClickSound;
    sf::Sound m_PowerupSound;
  public:
    SoundEngine();

    static void playShoot();
    static void playPlayerExplode();
    static void playEnemyExplode();
    static void playClick();
    static void playPowerup();

    static SoundEngine* m_s_Instance;
};
#endif
