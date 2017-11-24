#ifndef WALL_H
#define WALL_H
#include "line.hpp"
#include "vectorUtilities.hpp"
class Wall: public Line
{
  public:
    Wall(sf::Vector2f p0, sf::Vector2f p1, sf::Color color=sf::Color::Blue);
    void rotate(float dw);
};
#endif
