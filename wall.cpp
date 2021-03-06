#include "wall.hpp"
Wall::Wall(sf::Vector2f p0, sf::Vector2f p1,
           sf::Color color)
    : Line(p0, p1, color)
{
}
void Wall::rotate(float dw)
{
    sf::Vector2f center = this->getCenter();
    sf::Vector2f vertex0 = vArray[0].position - center;
    sf::Vector2f vertex1 = vArray[1].position - center;
    vertex0 = VectorUtilities::rotate(vertex0, dw) + center;
    vertex1 = VectorUtilities::rotate(vertex1, -dw) + center;
}
