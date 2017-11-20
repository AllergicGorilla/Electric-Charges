#ifndef CHARGE_H
#define CHARGE_H
#include <SFML/Graphics.hpp>
class Charge : public sf::CircleShape
{
  private:
    static uint32_t global_id;
    float charge;
    float mass;
    bool isCursorOn = false;
    sf::Vector2f velocity;
    sf::Vector2f force;

  public:
    // using sf::CircleShape::CircleShape;
    Charge(float radius = 0, std::size_t pointCount = 30, float charge = 0.f,
           float mass = 1.f, sf::Vector2f velocity = sf::Vector2f(0.f, 0.f))
        : CircleShape(radius, pointCount), charge(charge), velocity(velocity),
          mass(mass), id(global_id++), force(sf::Vector2f(0.f, 0.f))
    {
    }
    const uint32_t id;

    const float getCharge();

    const float getMass();

    const sf::Vector2f getVelocity();
    void setVelocity(sf::Vector2f v);
    void incrementVelocity(sf::Vector2f dv);

    void setIsCursorOn(bool ico);
    const bool getIsCursorOn();

    void setForce(sf::Vector2f f);
    const sf::Vector2f getForce();
};
bool detectChargeChargeCollision(Charge c1, Charge c2, float dt);
#endif
