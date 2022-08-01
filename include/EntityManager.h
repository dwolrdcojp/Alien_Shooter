#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <string>
#include "Entity.h"
// Create a factory pattern to generate entities 
typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map   <std::string, EntityVec>  EntityMap;

class EntityManager
{

private:

  // private member variables 
  EntityVec m_entities;
  EntityVec m_toAdd;
  EntityMap m_entityMap;
  size_t    m_totalEntities = 0;
  // private member functions
  void init();

public:
  
  // public member functions
  EntityManager();
  void update();
  std::shared_ptr<Entity> addEntity(const std::string& tag);
  EntityVec& getEntities();
  EntityVec& getEntities(const std::string& tag);
};
