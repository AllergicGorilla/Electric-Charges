#ifndef TOOLS_H
#define TOOLS_H

#include "charge.hpp"
#include "wall.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
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
    virtual void setInitialPos(sf::Vector2f pos);
    virtual void setCurrentPos(sf::Vector2f pos);

  public:
    sf::Vector2f initialPos;
    sf::Vector2f currentPos;
    bool primaryButtonReleased;
};
class ForceTool : public Tool
{
  private:
    std::shared_ptr<Charge> forceCharge;
    Line forceLine;

  private:
    void setInitialPos(sf::Vector2f pos) {}

  public:
    ForceTool() : Tool(), forceLine(sf::Color::Green) {}
    void usePrimary(bool isPressed,
                    const std::vector<std::shared_ptr<Charge>>& chargeVector,
                    sf::Vector2f mousePos);
    void applyForce() const;
    void updateInitialPos();
    void draw(sf::RenderWindow& window) const;
    void setCurrentPos(sf::Vector2f pos);
};
class ChargeCreatorTool : public Tool
{
  private:
    Line velocityLine;


  public:
    ChargeCreatorTool() : Tool(), velocityLine(sf::Color::White)
    {
        chargeMass = 1.f;
        chargeRadius = 1.f;
        electricCharge = 1.f;
    }
    void usePrimary(bool isPressed,
                    std::vector<std::shared_ptr<Charge>>& chargeVector,
                    sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window) const;
    void setCurrentPos(sf::Vector2f pos);
    float chargeMass;
    float chargeRadius;
    float electricCharge;
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

  public:
    const std::shared_ptr<Charge> getFollowCharge() const;
};
class SelectionTool : public Tool
{
  private:
    std::vector<std::shared_ptr<Charge>> selectedCharges;
    sf::Color selectedChargeOutlineColor;
    sf::RectangleShape selectionRectangle;
    sf::FloatRect selectionBounds;

  public:
    SelectionTool();
    void usePrimary(bool isPressed,
                    const std::vector<std::shared_ptr<Charge>>& chargeVector,
                    sf::Vector2f mousePos);
    void updateSize(sf::Vector2f pos);
    void
    updateSelection(const std::vector<std::shared_ptr<Charge>>& chargeVector);
    void removeCharges(std::vector<std::shared_ptr<Charge>>& chargeVector);
    void selectAll(const std::vector<std::shared_ptr<Charge>>& chargeVector);
    void reset();
    void draw(sf::RenderWindow& window) const;
};
class PlaceWallTool : public Tool
{
  private:
    Line wallLine;

  public:
    void usePrimary(bool isPressed,
                    std::vector<std::shared_ptr<Wall>>& wallVector,
                    sf::Vector2f mousePos);
    void setCurrentPos(sf::Vector2f pos);
};
#endif
