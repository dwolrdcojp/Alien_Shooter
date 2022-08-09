#include "../include/EntityManager.h"

void EntityManager::removeDeadEntities(EntityVec & vec) 
{
  // Avoid iterator invalidation
  for(auto it = vec.begin(); it != vec.end(); it++)
  {
    if(!it->get()->isActive()) 
    {
      vec.erase(it);
      if(!vec.empty())
      {
        it = vec.begin();
      }
      else 
      {
        break;
      }
    }
  }
}

EntityManager::EntityManager()
{

}

void EntityManager::update()
{
  // Called at the beginning of each frame by the game engine 
  // entities added will now be available to use this frame 
  // TODO
  // Add entities from m_entitiesToAdd to the proper location(s)
  // - add them to the vector of all entities 
  // - add them to the vector inside the map, with the tag as a key 
  for (auto e : m_entitiesToAdd)
  {
    m_entities.push_back(e);
    m_entityMap[e->tag()].push_back(e);
  }
  
  m_entitiesToAdd.clear();
  // remove dead entities from the vector of all entities 
  removeDeadEntities(m_entities);

  // remove dead entities from each vector in the entity map 
  // C++17 way of iterating through [key, value] pairs in a map 
  for (auto & [tag, entityVec] : m_entityMap)
  {
    removeDeadEntities(entityVec);
  }


}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
  // Implement this function so that entities are added to the m_entitiesToAdd vector here
  // and then added to the proper locations in the update() function 

  // Create the entity shared pointer 
  auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

  m_entitiesToAdd.push_back(entity);

  return entity;
}



const EntityVec & EntityManager::getEntities() 
{
  return m_entities;
}

const EntityVec & EntityManager::getEntities(const std::string & tag)
{
  return m_entityMap[tag];
}

