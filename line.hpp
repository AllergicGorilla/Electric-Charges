#ifndef LINE_H
#define LINE_H
#include <SFML/Graphics.hpp>
#include <cmath>

class Line : public sf::Drawable, public sf::Transformable
{
  protected:
    sf::VertexArray vArray;

  public:
    Line(sf::Color color) : vArray(sf::Lines, 2)
    {
        setVerticesPosition(sf::Vector2f(0, 0), sf::Vector2f(0, 0));
        setColor(color);
    }
    Line(sf::Vector2f p0 = sf::Vector2f(0, 0),
         sf::Vector2f p1 = sf::Vector2f(0, 0),
         sf::Color color = sf::Color::White)
        : vArray(sf::Lines, 2)
    {
        setVerticesPosition(p0, p1);
        setColor(color);
    }
    void setVertexPosition(sf::Vector2f p, int b);
    sf::Vector2f getVertexPosition(int b) const;
    void setVerticesPosition(sf::Vector2f p0, sf::Vector2f p1);
    void setColor(sf::Color color);
    sf::Vector2f asVector() const;
    const sf::VertexArray& getVertexArray() const;
    sf::Vector2f getCenter() const;
    sf::Vector2f normal() const;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
#endif
