#include "utilities.hpp"
// Other utilities
float lengthSqrd(sf::Vector2f v) { return dotProduct(v, v); }
float length(sf::Vector2f v) { return sqrt(lengthSqrd(v)); }
float distance(sf::Vector2f v, sf::Vector2f w) { return length(v - w); }
sf::Vector2f unit(sf::Vector2f v)
{
    return (length(v) == 0) ? v : v / length(v);
}
float dotProduct(sf::Vector2f v, sf::Vector2f w)
{
    return v.x * w.x + v.y * w.y;
}
