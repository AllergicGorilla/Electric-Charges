#ifndef WALL_H
#define WALL_H
#include "line.hpp"
#include "vectorUtilities.hpp"
class Wall: public Line
{
  private:
    float mass;
    bool isMovable;
  public:
    Wall(float mass, bool isMovable, sf::Vector2f p0, sf::Vector2f p1, sf::Color color);
    float getMass() const;
    bool getIsMovable() const;
    void rotate(float dw);

};
#endif
