#include "../include/Common.h"
#include "../include/Game.h"

int main()
{
  // Game g("config.txt");
  // g.run();

  Vec2 point1(1.0, 2.0);
  Vec2 point2(2.0, 4.0);
  Vec2 point3;
  std::cout << "Vec2 point1(1.0, 2.0);\nVec2 point2(2.0, 4.0);" << std::endl;
  
  std::cout << "point1 == point2: " << (point1 == point2) << std::endl;
  std::cout << "point1 != point2: " << (point1 != point2) << std::endl;

  point3 = point1 + point2;
  std::cout << "point1 + point2: " << point3 << std::endl; 
  point3 = point1 - point2;
  std::cout << "point1 - point2: " << point3 << std::endl; 
  point3 = point1 / point2;
  std::cout << "point1 / point2: " << point3 << std::endl; 
  point3 = point1 * point2;
  std::cout << "point1 * point2: " << point3 << std::endl; 
  point1 += point2; 
  std::cout << "point1 += point2: " << point1 << std::endl;
  point1 -= point2; 
  std::cout << "point1 -= point2: " << point1 << std::endl;
  point1 *= 2.0;
  std::cout << "point1 *= 2.0: " << point1 << std::endl;
  point1 /= 2.0;
  std::cout << "point1 /= 2.0: " << point1 << std::endl;
  
  std::cout << "point1.dist(point2): " << point1.dist(point2) << std::endl;

  point2.normalize();
  std::cout << "point2.normalize(): " << point2 << std::endl;
}


