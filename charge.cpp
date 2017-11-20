#include "charge.hpp"
#include "utilities.hpp"
uint32_t Charge::global_id = 0;

const float Charge::getCharge() { return charge; }
const float Charge::getMass() { return mass; }

const sf::Vector2f Charge::getVelocity() { return velocity; }
void Charge::setVelocity(sf::Vector2f v) { velocity = v; }
void Charge::incrementVelocity(sf::Vector2f dv) { velocity += dv; }

void Charge::setIsCursorOn(bool ico) { isCursorOn = ico; }
const bool Charge::getIsCursorOn() { return isCursorOn; }

void Charge::setForce(sf::Vector2f f) { force = f; }
const sf::Vector2f Charge::getForce() { return force; }

bool detectChargeChargeCollision(Charge c1, Charge c2, float dt) {
  float distSqrd = lengthSqrd(c1.getPosition() - c2.getPosition());
  float radiusSqrd = (c1.getRadius() + c2.getRadius())*(c1.getRadius() + c2.getRadius());
  bool intersecting = (distSqrd <= radiusSqrd);

  sf::Vector2f newC1pos = c1.getPosition() + c1.getVelocity()*dt;
  sf::Vector2f newC2pos = c2.getPosition() + c2.getVelocity()*dt;
  float newDistSqrd = lengthSqrd(newC1pos - newC2pos);
  bool approaching = (newDistSqrd < distSqrd);
  return (intersecting && approaching);
}
