#include "vectorUtilities.hpp"
float VectorUtilities::lengthSqrd(sf::Vector2f v) { return dotProduct(v, v); }
float VectorUtilities::length(sf::Vector2f v) { return sqrt(lengthSqrd(v)); }
float VectorUtilities::distance(sf::Vector2f v, sf::Vector2f w) { return length(v - w); }
sf::Vector2f VectorUtilities::unit(sf::Vector2f v)
{
    return (length(v) == 0) ? v : v / length(v);
}
float VectorUtilities::dotProduct(sf::Vector2f v, sf::Vector2f w)
{
    return v.x * w.x + v.y * w.y;
}
sf::Vector2f VectorUtilities::rotate(sf::Vector2f v, float dw)
{
    // |cos dw -sin dw|
    // |sin dw  cos dw|
    sf::Vector2f w(0, 0);
    w.x = dotProduct(v, sf::Vector2f(cosf(dw), -sinf(dw)));
    w.y = dotProduct(v, sf::Vector2f(sinf(dw), cosf(dw)));
    return w;
}
sf::Vector2f VectorUtilities::normal(sf::Vector2f v)
{
  return unit(sf::Vector2f(-v.y, v.x));
}
