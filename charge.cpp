#include "charge.hpp"

float Charge::getCharge() const { return charge; }
float Charge::getMass() const { return mass; }

sf::Vector2f Charge::getVelocity() const { return velocity; }
void Charge::setVelocity(sf::Vector2f v) { velocity = v; }
void Charge::incrementVelocity(sf::Vector2f dv) { velocity += dv; }
Line Charge::velocityLine() const
{
    Line line(this->getPosition(), this->getPosition() + this->getVelocity());
    line.setColor(sf::Color::Red);
    return line;
}

void Charge::setIsCursorOn(bool ico) { isCursorOn = ico; }
bool Charge::getIsCursorOn() const { return isCursorOn; }

void Charge::setForce(sf::Vector2f f) { force = f; }
void Charge::incrementForce(sf::Vector2f df) { force += df; }
sf::Vector2f Charge::getForce() const { return force; }

bool detectChargeChargeCollision(const Charge c1, const Charge c2,
                                 const float dt)
{
    using namespace VectorUtilities;
    float distSqrd = lengthSqrd(c1.getPosition() - c2.getPosition());
    float radiusSqrd =
        (c1.getRadius() + c2.getRadius()) * (c1.getRadius() + c2.getRadius());
    bool intersecting = (distSqrd <= radiusSqrd);

    sf::Vector2f newC1pos = c1.getPosition() + c1.getVelocity() * dt;
    sf::Vector2f newC2pos = c2.getPosition() + c2.getVelocity() * dt;
    float newDistSqrd = lengthSqrd(newC1pos - newC2pos);
    bool approaching = (newDistSqrd < distSqrd);
    return (intersecting && approaching);
}
