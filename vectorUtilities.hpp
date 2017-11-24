#ifndef VECTORUTILITIES_H
#define VECTORUTILITIES_H
#include <SFML/Graphics.hpp>
#include <cmath>
// Other utilities
namespace VectorUtilities
{
float lengthSqrd(sf::Vector2f v);
float length(sf::Vector2f v);
float distance(sf::Vector2f v, sf::Vector2f w);
sf::Vector2f unit(sf::Vector2f v);
float dotProduct(sf::Vector2f v, sf::Vector2f w);
sf::Vector2f rotate(sf::Vector2f v, float dw);
sf::Vector2f normal(sf::Vector2f v);
}
#endif
