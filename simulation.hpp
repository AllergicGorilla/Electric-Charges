#ifndef SIMULATION_H
#define SIMULATION_H

#include "charge.hpp"
#include "tools.hpp"
#include "utilities.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <memory>

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
    enum tool { force, charge, follow };

    tool currentTool;
    ForceTool forceTool;
    ChargeCreatorTool chargeCreatorTool;
    FollowTool followTool;
    sf::Vector2f mainMousePos;
    sf::RenderWindow mainWindow;
    sf::View mainView;
    sf::Font courierPrime;
    std::vector<std::shared_ptr<Charge>> chargeVector;
    sf::Text chargeCount;
    sf::Clock mainClock;
    sf::Time dt;
    bool lockView;
    float viewSpeed;

  public:
    Simulation();
    void run();
};
#endif
