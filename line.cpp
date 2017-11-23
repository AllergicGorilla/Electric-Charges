#include "line.hpp"
void Line::setVertexPosition(sf::Vector2f p, int b)
{
    vArray[b % 2].position = p;
}
void Line::setVerticesPosition(sf::Vector2f p0, sf::Vector2f p1)
{
    vArray[0].position = p0;
    vArray[1].position = p1;
}
void Line::setColor(sf::Color color)
{
    vArray[0].color = vArray[1].color = color;
}
sf::Vector2f Line::asVector() const
{
    return vArray[1].position - vArray[0].position;
}
const sf::VertexArray& Line::getVertexArray() const { return vArray; }
sf::Vector2f Line::getCenter() { return this->asVector() / 2.f; }
void Line::translate(sf::Vector2f dx)
{
    vArray[0].position += dx;
    vArray[1].position += dx;
}
