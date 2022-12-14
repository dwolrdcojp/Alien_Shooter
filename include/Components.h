#pragma once

#include "Common.h"

class CTransform
{
public:
  // pos represents the center of the object, player, tile, bullet, etc... 
  Vec2 pos      = { 0.0, 0.0 };
  Vec2 velocity = { 0.0, 0.0 };
  float angle   =   0;

  CTransform(const Vec2 & p, const Vec2 & v, float a)
    : pos(p), velocity(v), angle(a) {}
};

class CShape 
{
public:
  sf::CircleShape circle;

  CShape(float radius, int points, const sf::Color & fill, const sf::Color & outline, float thickness)
    : circle(radius, points)
  {
    circle.setFillColor(fill);
    circle.setOutlineColor(outline);
    circle.setOutlineThickness(thickness);
    circle.setOrigin(sf::Vector2f(radius, radius));
  }
};

class CCollision
{
public:
  float radius = 0;
  int frameSpawned = 0;
  CCollision(float r, int frame)
    : radius(r), frameSpawned(frame) {}
};

class CScore
{
public:
  int score = 0;
  CScore(int s) 
    : score(s) {}
};

class CLifespan
{
public:
  int remaining      = 0; // amount of lifespan remaining on the entity 
  int total          = 0; // the total initial amount of lifespan
  int frameSpawned   = 0;
  CLifespan(int total, int frame)
    : remaining(total), total(total), frameSpawned(frame) {}
};

class CInput
{
public:
  bool up     = false;
  bool left   = false;
  bool right  = false;
  bool down   = false;
  bool shoot  = false;

  CInput() {}
};
