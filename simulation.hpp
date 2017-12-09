#ifndef SIMULATION_H
#define SIMULATION_H

#include "charge.hpp"
#include "tools.hpp"
#include "vectorUtilities.hpp"
#include <TGUI/TGUI.hpp>
#include <cmath>
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
    void loadWidgets();

  private:
    //Tools
    enum tool { force, charge, follow, select, placeWall };
    tool currentTool;
    ForceTool forceTool;
    ChargeCreatorTool chargeCreatorTool;
    SelectionTool selectionTool;
    FollowTool followTool;
    PlaceWallTool placeWallTool;
    //Window & view
    sf::Vector2f mainMousePos;
    sf::RenderWindow mainWindow;
    sf::View mainView;
    bool lockView;
    float viewSpeed;
    //Text
    sf::Font courierPrime;
    sf::String chargeCount;
    //Objects
    std::vector<std::shared_ptr<Charge>> chargeVector;
    std::vector<std::shared_ptr<Wall>> wallVector;
    //Time
    sf::Clock mainClock;
    float dt;
    //GUI
    tgui::Gui gui;
    tgui::Label::Ptr chargeText;
    tgui::HorizontalLayout::Ptr h_Layout;
    tgui::Button::Ptr forceToolButton;
    tgui::Button::Ptr chargeCreatorToolButton;
    tgui::Button::Ptr selectionToolButton;
    tgui::Button::Ptr followToolButton;
    tgui::Button::Ptr placeWallToolButton;


  public:
    Simulation();
    void run();
};
bool detectChargeWallCollision(const Charge& charge, const Wall& wall);
#endif
