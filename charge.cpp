#include "charge.hpp"
uint32_t Charge::global_id = 0;

const float Charge::getCharge(){ return charge; }
const float Charge::getMass(){ return mass; }

const sf::Vector2f Charge::getVelocity(){ return velocity; }
void Charge::setVelocity(sf::Vector2f v) { velocity = v; }
void Charge::incrementVelocity(sf::Vector2f dv) { velocity += dv;}

void Charge::setIsCursorOn(bool ico){ isCursorOn = ico; }
const bool Charge::getIsCursorOn(){ return isCursorOn; }

void Charge::setForce(sf::Vector2f f) { force = f; }
const sf::Vector2f Charge::getForce() { return force; }
