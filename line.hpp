#ifndef LINE_H
#define LINE_H
#include <SFML/Graphics.hpp>
#include <cmath>

class Line
{
  protected:
    sf::VertexArray vArray;

  public:
    Line(sf::Color color) : vArray(sf::Lines, 2)
    {
        vArray[0].position = vArray[1].position = sf::Vector2f(0, 0);
        vArray[0].color = vArray[1].color = color;
    }
    Line(sf::Vector2f p0 = sf::Vector2f(0, 0),
         sf::Vector2f p1 = sf::Vector2f(0, 0),
         sf::Color color = sf::Color::White)
        : vArray(sf::Lines, 2)
    {
        vArray[0].position = p0;
        vArray[1].position = p1;
        vArray[0].color = vArray[1].color = color;
    }
    void setVertexPosition(sf::Vector2f p, int b);
    sf::Vector2f getVertexPosition(int b) const;
    void setVerticesPosition(sf::Vector2f p0, sf::Vector2f p1);
    void setColor(sf::Color color);
    sf::Vector2f asVector() const;
    const sf::VertexArray& getVertexArray() const;
    sf::Vector2f getCenter() const;
    void translate(sf::Vector2f dx);
    sf::Vector2f normal() const;
};
#endif
