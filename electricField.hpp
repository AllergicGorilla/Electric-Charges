#ifndef ELECTRICFIELD_H
#define ELECTRICFIELD_H
#include "line.hpp"
#include "charge.hpp"
#include "vectorUtilities.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
class ElectricField
{
  private:
    std::vector<std::vector<sf::Vector2f>> vectorField;
    float unitLength;
    int width;
    int height;
    std::function<sf::Vector2f(const sf::Vector2f&)> lambda;
    sf::Vector2i mapWorldToFieldCoordinates(const sf::Vector2f& worldCoords);
    bool isWithinField(const sf::Vector2f& v);
    sf::Vector2f calculateField(const Charge& charge, const sf::Vector2f& point);
    float coulombConstant;



  public:
    ElectricField(int width, int height, float unitLength,
                  std::function<sf::Vector2f(const sf::Vector2f&)> lambda)
        : vectorField(width, std::vector<sf::Vector2f>(height))
    {
        this->unitLength = unitLength;
        this->width = width;
        this->height = height;
        this->lambda = lambda;
        reset();
        coulombConstant = 10000.0f;
    }
    void draw(sf::RenderWindow& window);
    void applyForceOnCharge(Charge& charge);
    void incrementField(const Charge& charge);
    void reset();
};
#endif
