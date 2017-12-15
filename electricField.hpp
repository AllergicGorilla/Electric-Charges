#ifndef ELECTRICFIELD_H
#define ELECTRICFIELD_H
#include "line.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
class ElectricField
{
  private:
    std::vector<std::vector<sf::Vector2f>> vectorField;
    float unitLength;
    int width;
    int height;

  public:
    ElectricField(int width, int height, float unitLength,
                  std::function<sf::Vector2f(const sf::Vector2f&)> lambda)
        : vectorField(width, std::vector<sf::Vector2f>(height))
    {
        this->unitLength = unitLength;
        this->width = width;
        this->height = height;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                sf::Vector2f eField = lambda(unitLength * sf::Vector2f(x, y));
                vectorField[x][y] = eField;
            }
        }
    }
    void draw(sf::RenderWindow& window)
    {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                sf::Vector2f base =
                    unitLength * sf::Vector2f(x + 0.5f, y + 0.5f);
                Line line =
                    Line(base, base + 0.01f * vectorField[x][y], sf::Color::Red);
                window.draw(line.getVertexArray());
            }
        }
    }
};
#endif
