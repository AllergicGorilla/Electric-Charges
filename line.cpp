#include "line.hpp"
#include "vectorUtilities.hpp"
void Line::setVertexPosition(sf::Vector2f p, int b)
{
    vArray[b % 2].position = p;
}
sf::Vector2f Line::getVertexPosition(int b) const { return vArray[b % 2].position; }
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
sf::Vector2f Line::getCenter() const { return (vArray[1].position + vArray[0].position) / 2.f; }
sf::Vector2f Line::normal() const
{
  return VectorUtilities::normal(VectorUtilities::unit(this->asVector()));
}
void Line::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  target.draw(vArray, states);
}
