#include <SFML/Graphics.hpp>
#include <cmath>
class Line
{
  private:
  sf::VertexArray vArray;
  public:
    Line() : vArray(sf::Lines, 2)
    {
      vArray[0].position = vArray[1].position = sf::Vector2f(0,0);
      vArray[0].color = vArray[1].color = sf::Color::White;
    }
    void setVertexPosition(sf::Vector2f p, int b);
    void setVerticesPosition(sf::Vector2f p0, sf::Vector2f p1);
    void setColor(sf::Color color);
    const sf::Vector2f asVector();
    const sf::VertexArray& getVertexArray();
};
//Other utilities
float length(sf::Vector2f v);
float distance(sf::Vector2f v, sf::Vector2f w);
sf::Vector2f unit(sf::Vector2f v);
