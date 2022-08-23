#pragma once
#ifndef TEXTURE_ENGINE_H
#define TEXTURE_ENGINE_H

#include "../include/Common.h"
#include <map>
#include <SFML/Graphics.hpp>

class TextureEngine {
private:
  std::map<std::string, sf::Texture> m_TexturesMap;
  static TextureEngine* m_s_Instance;

public:
  TextureEngine();
  static sf::Texture& getTexture(std::string const& filename);
  static void addTexture(std::string const& filename);
  sf::Sprite m_background;
};
#endif
