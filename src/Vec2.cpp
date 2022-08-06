#include "../include/Vec2.h"
#include <math.h>

Vec2::Vec2()
{

}

Vec2::Vec2(float xin, float yin)
  : x(xin), y(yin)
{

}

bool Vec2::operator == (const Vec2 & rhs) const
{
  return (x == rhs.x && rhs.y == y); 
}

bool Vec2::operator != (const Vec2 & rhs) const
{
  return (x != rhs.x && rhs.y != y);
}

Vec2 Vec2::operator + (const Vec2 & rhs) const 
{

  return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2 & rhs) const 
{
  return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator / (const Vec2 & rhs) const 
{
  return Vec2(x / rhs.x, y / rhs.y);
}

Vec2 Vec2::operator * (const Vec2 & rhs) const 
{
  return Vec2(x * rhs.x, y * rhs.y);
}

void Vec2::operator += (const Vec2 & rhs)
{
  x += rhs.x;
  y += rhs.y;
}

void Vec2::operator -= (const Vec2 & rhs)
{
  x -= rhs.x;
  y -= rhs.y;
}

void Vec2::operator *= (const float val)
{
  x *= val;
  y *= val;
}

void Vec2::operator /= (const float val)
{
  x /= val;
  y /= val;
}
void Vec2::normalize() 
{
  double L = length();
  x = x / L;
  y = y / L;
}

double Vec2::length()
{
  double L = sqrtf((x*x) + (y*y));
  return L;
}

std::ostream& operator << (std::ostream& os, const Vec2& vc)
{
  os << "Vec2: (" << vc.x << ", " << vc.y << ") ";
  return os;
}
