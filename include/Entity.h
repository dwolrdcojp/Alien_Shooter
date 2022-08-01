#include <iostream>
#include <string>

class Entity
{
private:
  // private member variables
  const size_t      m_id    = 0;
  const std::string m_tag   = "Default";
  bool              m_active = true;

public:
  // public member variables
  std::shared_ptr<CTransform> cTransform;
  std::shared_ptr<CShape>     cShape;
  std::shared_ptr<CCollision> cCollision;
  std::shared_ptr<CInput>     cInput;
  std::shared_ptr<CScore>     cScore;
  std::shared_ptr<CLifespan>  cLifespan;
  
  // public member functions
  Entity(const std::string& tag, size_t id);
  void destroy() { m_active = false; }
  bool isActive() { return m_active; }
  const std::string& tag() { return m_tag; }
  size_t id() { return m_id; }
};
