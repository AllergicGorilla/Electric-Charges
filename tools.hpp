#ifndef TOOLS_H
#define TOOLS_H

#include "charge.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

class Tool
{
  public:
    Tool()
        : primaryButtonReleased(true), initialPos(sf::Vector2f(0, 0)),
          currentPos(sf::Vector2f(0, 0))
    {
    }
    sf::Vector2f initialPos;
    sf::Vector2f currentPos;
    bool primaryButtonReleased;
    void setInitialPos(sf::Vector2f pos);
    void setCurrentPos(sf::Vector2f pos);
};
class ForceTool : public Tool
{
  private:
    std::shared_ptr<Charge> forceCharge;
    Line forceLine;
    void setInitialPos(sf::Vector2f pos) {}

  public:
    ForceTool() : Tool(), forceLine(sf::Color::Green) {}
    void usePrimary(bool isPressed,
                    std::vector<std::shared_ptr<Charge>>& chargeVector,
                    sf::Vector2f mousePos);
    void applyForce();
    void updateInitialPos();
    void draw(sf::RenderWindow& window);
    void setCurrentPos(sf::Vector2f pos);
};
class ChargeCreatorTool : public Tool
{
  private:
    Line velocityLine;

  public:
    ChargeCreatorTool() : velocityLine(sf::Color::White) {}
    void usePrimary(bool isPressed,
                    std::vector<std::shared_ptr<Charge>>& chargeVector,
                    sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window);
    void setCurrentPos(sf::Vector2f pos);
};
class FollowTool
{
  private:
    std::shared_ptr<Charge> followCharge;
    sf::Color previousColor;
    sf::Color newColor;

  public:
    void usePrimary(bool isPressed,
                    const std::vector<std::shared_ptr<Charge>>& chargeVector,
                    sf::Vector2f mousePos);
    bool focusViewOnCharge(sf::View& view);
    const std::shared_ptr<Charge> getFollowCharge() const;
};
#endif
