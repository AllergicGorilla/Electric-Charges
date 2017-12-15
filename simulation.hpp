#ifndef SIMULATION_H
#define SIMULATION_H

#include "charge.hpp"
#include "grid.hpp"
#include "tools.hpp"
#include "vectorUtilities.hpp"
#include "electricField.hpp"
#include <TGUI/TGUI.hpp>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>

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
    void zoomMainViewAt(sf::Vector2i pixel, float zoom);

  private:
    // Tools
    enum tool { force, charge, follow, select, placeWall };
    tool currentTool;
    ForceTool forceTool;
    ChargeCreatorTool chargeCreatorTool;
    SelectionTool selectionTool;
    FollowTool followTool;
    PlaceWallTool placeWallTool;
    // Window & view
    sf::Vector2f mainMousePos;
    sf::RenderWindow mainWindow;
    sf::View mainView;
    sf::View guiView;
    bool lockView;
    float viewSpeed;
    float zoomAmount;
    // Text
    sf::Font courierPrime;
    sf::String chargeCount;
    // Objects
    std::vector<std::shared_ptr<Charge>> chargeVector;
    std::vector<std::shared_ptr<Wall>> wallVector;
    // Boundary
    float bWidth;
    float bHeight;
    // Time
    sf::Clock mainClock;
    float dt;
    // GUI
    tgui::Gui gui;
    tgui::Label::Ptr chargeText;
    tgui::Label::Ptr radiusText;
    tgui::Label::Ptr massText;
    tgui::HorizontalLayout::Ptr h_Layout;
    tgui::Button::Ptr forceToolButton;
    tgui::Button::Ptr chargeCreatorToolButton;
    tgui::Button::Ptr selectionToolButton;
    tgui::Button::Ptr followToolButton;
    tgui::Button::Ptr placeWallToolButton;
    tgui::Button::Ptr showGridButton;
    //
    Grid grid;
    bool showGrid;
    //e field
    ElectricField electricField;

  public:
    Simulation();
    void run();
};
bool detectChargeWallCollision(const Charge& charge, const Wall& wall);

#endif
