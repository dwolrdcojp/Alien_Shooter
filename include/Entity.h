#include <iostream>
#include <string>
#include "Components.h"

class Entity
{
private:

  // EntityManager can construct entities, but no other class can 
  friend class EntityManager;

  // private member variables
  bool        m_active = true;
  size_t      m_id     = 0;
  std::string m_tag    = "default";

  // Private constructor so we can't accidently create any entities from outside the class 
  Entity(const size_t id, const std::string & tag);

public:
  // public member variables
  std::shared_ptr<CTransform> cTransform;
  std::shared_ptr<CShape>     cShape;
  std::shared_ptr<CCollision> cCollision;
  std::shared_ptr<CInput>     cInput;
  std::shared_ptr<CScore>     cScore;
  std::shared_ptr<CLifespan>  cLifespan;
  
  // public member functions
  bool isActive() const;
  const std::string& tag() const;
  const size_t id() const;
  void destroy();
};
