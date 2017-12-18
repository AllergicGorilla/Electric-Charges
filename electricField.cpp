#include "electricField.hpp"
void ElectricField::draw(sf::RenderWindow& window)
{
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            sf::Vector2f base = unitLength * sf::Vector2f(x + 0.5f, y + 0.5f);
            Line line =
                Line(base, base + vectorField[x][y], sf::Color::Red);
            window.draw(line);
        }
    }
}
sf::Vector2i
ElectricField::mapWorldToFieldCoordinates(const sf::Vector2f& worldCoords)
{
    // worldCoords must be within the field's bounds!
    return sf::Vector2i(
        static_cast<int>(std::floor(worldCoords.x / unitLength)),
        static_cast<int>(std::floor(worldCoords.y / unitLength)));
}
void ElectricField::applyForceOnCharge(Charge& charge)
{
    if (isWithinField(charge.getPosition())) {
        sf::Vector2i fieldCoords =
            mapWorldToFieldCoordinates(charge.getPosition());
        sf::Vector2f eField = vectorField[fieldCoords.x][fieldCoords.y];
        sf::Vector2f force = charge.getCharge() * eField;
        charge.incrementForce(force);
    }
}
bool ElectricField::isWithinField(const sf::Vector2f& v)
{
    return (v.x >= 0.0f && v.x <= (width * unitLength) && v.y >= 0.0f &&
            v.y <= (height * unitLength));
}
sf::Vector2f ElectricField::calculateField(const Charge& charge,
                                           const sf::Vector2f& point)
{
    using namespace VectorUtilities;
    sf::Vector2f r = point - charge.getPosition();
    return (coulombConstant * charge.getCharge() / lengthSqrd(r)) * unit(r);
}
void ElectricField::reset(){
  for (int x = 0; x < width; x++) {
      for (int y = 0; y < height; y++) {
          sf::Vector2f eField = lambda(unitLength * sf::Vector2f(x, y));
          vectorField[x][y] = eField;
      }
  }
}
void ElectricField::incrementField(const Charge& charge)
{
    sf::Vector2i chargeXY = mapWorldToFieldCoordinates(charge.getPosition());
    if (isWithinField(charge.getPosition())) {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                sf::Vector2f point =
                    unitLength * sf::Vector2f(x + 0.5f, y + 0.5f);
                if (mapWorldToFieldCoordinates(point) != chargeXY)
                  vectorField[x][y] += calculateField(charge, point);
            }
        }
    }
}
