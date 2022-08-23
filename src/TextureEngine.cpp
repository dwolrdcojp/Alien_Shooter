#include <assert.h>
#include "../include/TextureEngine.h"

TextureEngine* TextureEngine::m_s_Instance = nullptr;

TextureEngine::TextureEngine()
{
  assert(m_s_Instance == nullptr);
  m_s_Instance = this;
}

void TextureEngine::addTexture(std::string const& filename)
{
  // Get a reference to m_Tetures using m_s_Instance
  // auto is std::map<std::string, sf::Texture>
  auto& texturesMap = m_s_Instance->m_TexturesMap;

  // Create an iterator to hold a key value pair 
  // and search for the required kvp 
  // using the passed in file name
  // auto is equivalent to std::map<std::string, sf::Texture>::iterator
  auto keyValuePair = texturesMap.find(filename);

  // No match found so save the texture in the map 
  if (keyValuePair == texturesMap.end())
  {
    auto& texture = texturesMap[filename];

    texture.loadFromFile(filename);
  }
}

sf::Texture& TextureEngine::getTexture(std::string const& filename)
{
  auto& m = m_s_Instance->m_TexturesMap;

  auto keyValuePair = m.find(filename);

  if (keyValuePair != m.end())
  {
    return keyValuePair->second;
  } else 
  {
    std::cout << "TextureEngine::getTexture() texture not found!" << std::endl;
  }
}






  

