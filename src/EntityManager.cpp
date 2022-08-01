#include "../include/EntityManager.h"

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
  // Create a new Entity object
  auto e = std::make_shared<Entity>(tag, m_totalEntities++);
  m_toAdd.push_back(e);
  return e;

  // note: above code doesn't handle some map-related edge cases 
}

void EntityManager::update()
{
  // Called at the beginning of each frame by the game engine 
  // entities added will now be available to use this frame 

  for (auto e : m_toAdd)
  {
    m_entities.push_back(e);
    m_entityMap[e->tag()].push_back(e);
  }
  for (auto e : m_entities)
  {
    // Solve iterator invalidation ... 
    // if e is dead, remove it from m_entities
    // if e is dead, remove it from m_entityMap[e->tag()]
  }
  m_toAdd.clear();
}

