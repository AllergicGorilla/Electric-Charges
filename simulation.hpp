#ifndef SIMULATION_H
#define SIMULATION_H

#include "charge.hpp"
#include "tools.hpp"
#include "vectorUtilities.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <memory>
#include <cmath>

class Simulation
{
  private:
    void processRealTimeInput();
    void processEvents();
    void handleKeyboardEvent(sf::Keyboard::Key key, bool isPressed);
    void handleMouseEvent(sf::Mouse::Button button, bool isPressed);
    void update();
    void render();

  private:
    enum tool { force, charge, follow, select, placeWall };

    tool currentTool;
    ForceTool forceTool;
    ChargeCreatorTool chargeCreatorTool;
    SelectionTool selectionTool;
    FollowTool followTool;
    PlaceWallTool placeWallTool;
    sf::Vector2f mainMousePos;
    sf::RenderWindow mainWindow;
    sf::View mainView;
    sf::Font courierPrime;
    std::vector<std::shared_ptr<Charge>> chargeVector;
    std::vector<std::shared_ptr<Wall>> wallVector;
    sf::Text chargeCount;
    sf::Clock mainClock;
    sf::Time dt;
    bool lockView;
    float viewSpeed;

  public:
    Simulation();
    void run();
};
bool detectChargeWallCollision(const Charge& charge, const Wall& wall);
#endif
