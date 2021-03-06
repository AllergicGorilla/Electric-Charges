#include "simulation.hpp"

Simulation::Simulation()
    : mainWindow(sf::VideoMode(1024, 1024), "Electric"),
      guiView(sf::Vector2f(0, 0), sf::Vector2f(800, 800)), bWidth(1024),
      bHeight(1024), gui(mainWindow), grid(16.0f, 64, 64),
      electricField(64, 64, 16.0f, [&](const sf::Vector2f& pos) {
          return sf::Vector2f(0.0f, 0.0f);
      })

{
    lockView = false;
    viewSpeed = 0.8f;
    zoomAmount = 1.05f;
    dt = 1.0f / 60.0f;
    currentTool = charge;
    // Background setup
    sf::RectangleShape bgRect({bWidth, bHeight});
    bgRect.setFillColor(sf::Color::White);
    if (!bgTexture.create(bWidth, bHeight))
        std::cout << "bgTexture could not be created!" << std::endl;
    bgTexture.clear();
    bgTexture.draw(bgRect);
    bgTexture.display();
    bgSprite = sf::Sprite(bgTexture.getTexture());
    // Text
    chargeCount = "Charges: 0";
    //
    mainView = sf::View(sf::Vector2f(bWidth / 2, bHeight / 2),
                        sf::Vector2f(bWidth, bHeight));
    // Grid
    showGrid = false;
    // Shaders
    if (chargeHighlightShader.loadFromFile("chargeHighlightShader.frag",
                                           sf::Shader::Fragment)) {
    } else {
        std::cout << "chargeHighlightShader could not be loaded!" << std::endl;
    }
    //TODO: Organize the equipotential code
    std::ifstream file("equipotentialShader.frag");
    if (file.good()) {
        equipotentialShaderCode =
            std::string((std::istreambuf_iterator<char>(file)),
                        (std::istreambuf_iterator<char>()));
        file.close();
        equipotentialShader.loadFromMemory("#define NUMBER_OF_CHARGES 1\n" +
                                               equipotentialShaderCode,
                                           sf::Shader::Fragment);
        equipotentialShader.setUniform("u_resolution",
                                       sf::Vector2f(bWidth, bHeight));
    } else {
        std::cout << "equipotentialShader could not be loaded!" << std::endl;
    }
}
void Simulation::loadWidgets()
{
    auto width = tgui::bindWidth(gui);
    auto height = tgui::bindHeight(gui);
    // GUI
    // Text
    chargeCountText = tgui::Label::create();
    chargeCountText->setTextColor(sf::Color::White);
    chargeCountText->setText(chargeCount);
    chargeCountText->setSize(sf::Vector2f(120, 30));
    chargeCountText->setPosition(-width * 0.5f, -height * 0.5f);
    gui.add(chargeCountText);

    radiusText = tgui::Label::create();
    radiusText->setTextColor(sf::Color::White);
    radiusText->setText("Radius=1.0");
    radiusText->setSize(sf::Vector2f(120, 30));
    radiusText->setPosition(-width * 0.5f, -height * 0.45f);
    gui.add(radiusText);

    massText = tgui::Label::create();
    massText->setTextColor(sf::Color::White);
    massText->setText("Mass=1.0");
    massText->setSize(sf::Vector2f(120, 30));
    massText->setPosition(-width * 0.5f, -height * 0.4f);
    gui.add(massText);

    chargeText = tgui::Label::create();
    chargeText->setTextColor(sf::Color::White);
    chargeText->setText("Charge=1.0");
    chargeText->setSize(sf::Vector2f(120, 30));
    chargeText->setPosition(-width * 0.5f, -height * 0.35f);
    gui.add(chargeText);
    // Horizontal layout for Toolbar
    h_Layout = tgui::HorizontalLayout::create();
    h_Layout->setSize(sf::Vector2f(500, 20));
    h_Layout->setPosition(-width * 0.125f, height * 0.45f);
    gui.add(h_Layout);
    // Tool buttons
    forceToolButton = tgui::Button::create("Force (F)");
    forceToolButton->setSize(sf::Vector2f(120, 20));
    forceToolButton->connect("pressed", [&]() { currentTool = force; });
    h_Layout->add(forceToolButton);

    chargeCreatorToolButton = tgui::Button::create("Charge (C)");
    chargeCreatorToolButton->setSize(sf::Vector2f(120, 20));
    chargeCreatorToolButton->connect("pressed",
                                     [&]() { currentTool = charge; });
    h_Layout->add(chargeCreatorToolButton);

    selectionToolButton = tgui::Button::create("Selection (S)");
    selectionToolButton->setSize(sf::Vector2f(120, 20));
    selectionToolButton->connect("pressed", [&]() { currentTool = select; });
    h_Layout->add(selectionToolButton);

    followToolButton = tgui::Button::create("Follow (G)");
    followToolButton->setSize(sf::Vector2f(120, 20));
    followToolButton->connect("pressed", [&]() { currentTool = follow; });
    h_Layout->add(followToolButton);

    placeWallToolButton = tgui::Button::create("Wall (P)");
    placeWallToolButton->setSize(sf::Vector2f(120, 20));
    placeWallToolButton->connect("pressed", [&]() { currentTool = placeWall; });
    h_Layout->add(placeWallToolButton);

    showGridButton = tgui::Button::create("Show grid");
    showGridButton->setSize(sf::Vector2f(120, 20));
    showGridButton->connect("pressed", [&]() { showGrid = !showGrid; });
    h_Layout->add(showGridButton);
}
void Simulation::run()
{
    loadWidgets();
    sf::Time frameTime = mainClock.restart();
    float accumulator = 0.f;
    while (mainWindow.isOpen()) {
        frameTime = mainClock.restart();
        accumulator += frameTime.asSeconds();
        mainMousePos =
            mainWindow.mapPixelToCoords(sf::Mouse::getPosition(mainWindow));
        processEvents();
        processRealTimeInput();
        while (accumulator >= dt) {
            update();
            accumulator -= dt;
        }
        render();
    }
}
void Simulation::processEvents()
{
    sf::Event event;
    while (mainWindow.pollEvent(event)) {
        bool guiHandledEvent = gui.handleEvent(event);
        if (event.type == sf::Event::Closed)
            mainWindow.close();
        if (event.type == sf::Event::MouseButtonPressed && !guiHandledEvent) {
            handleMouseEvent(event.mouseButton.button, true);
        }
        if (event.type == sf::Event::MouseButtonReleased && !guiHandledEvent) {
            handleMouseEvent(event.mouseButton.button, false);
            chargeCount = "Charges: " + std::to_string(chargeVector.size());
            chargeCountText->setText(chargeCount);
        }
        if (event.type == sf::Event::Resized) {
            // Update the mainView to the new size of the mainWindow
            mainView.setSize(event.size.width, event.size.height);
            mainWindow.setView(mainView);
            guiView.setSize(event.size.width, event.size.height);
            gui.setView(guiView);
        }
        if (event.type == sf::Event::KeyPressed) {
            handleKeyboardEvent(event.key.code, true);
            chargeCount = "Charges: " + std::to_string(chargeVector.size());
            chargeCountText->setText(chargeCount);
        }
        if (event.type == sf::Event::MouseWheelScrolled) {
            if (event.mouseWheelScroll.delta > 0)
                zoomMainViewAt(
                    {event.mouseWheelScroll.x, event.mouseWheelScroll.y},
                    (1.f / zoomAmount));
            else if (event.mouseWheelScroll.delta < 0)
                zoomMainViewAt(
                    {event.mouseWheelScroll.x, event.mouseWheelScroll.y},
                    zoomAmount);
        }
    }
}
void Simulation::handleKeyboardEvent(sf::Keyboard::Key key, bool isPressed)
{
    switch (key) {
    case sf::Keyboard::C: currentTool = charge; break;
    case sf::Keyboard::F: currentTool = force; break;
    case sf::Keyboard::G: currentTool = follow; break;
    case sf::Keyboard::S: currentTool = select; break;
    case sf::Keyboard::L: lockView = false; break;
    case sf::Keyboard::X: selectionTool.removeCharges(chargeVector); break;
    case sf::Keyboard::A: selectionTool.selectAll(chargeVector); break;
    case sf::Keyboard::P: currentTool = placeWall; break;
    default: break;
    }
}
void Simulation::handleMouseEvent(sf::Mouse::Button button, bool isPressed)
{
    switch (button) {
    case sf::Mouse::Left: {
        switch (currentTool) {
        case force:
            forceTool.usePrimary(isPressed, chargeVector, mainMousePos);
            break;
        case charge: {
            chargeCreatorTool.usePrimary(isPressed, chargeVector, mainMousePos);
            //TODO: Organize the equipotential code
            int numberOfCharges = chargeVector.size();
            if (numberOfCharges > 0) {
                std::string recompiledEquipotentialShaderCode =
                    "#define NUMBER_OF_CHARGES " +
                    std::to_string(numberOfCharges) + "\n" +
                    equipotentialShaderCode;
                equipotentialShader.loadFromMemory(
                    recompiledEquipotentialShaderCode, sf::Shader::Fragment);
                equipotentialShader.setUniform("u_resolution",
                                               sf::Vector2f(bWidth, bHeight));
                std::vector<sf::Vector3f> pos_q_vector;
                for (auto charge : chargeVector) {
                    sf::Vector2f pos = charge->getPosition();
                    pos = sf::Vector2f(pos.x, bHeight - pos.y);
                    pos_q_vector.push_back(
                        sf::Vector3f(pos.x, pos.y, charge->getCharge()));
                }
                equipotentialShader.setUniformArray(
                    "chargeArray", &pos_q_vector[0], numberOfCharges);
            }
            break;
        }
        case follow:
            if (isPressed) {
                followTool.usePrimary(isPressed, chargeVector, mainMousePos);
                lockView = followTool.focusViewOnCharge(mainView);
            }
            break;
        case select:
            selectionTool.usePrimary(isPressed, chargeVector, mainMousePos);
            break;
        case placeWall:
            placeWallTool.usePrimary(isPressed, wallVector, mainMousePos);
        default: break;
        }
        break;
    }
    default: break;
    }
}

void Simulation::processRealTimeInput()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (currentTool == force) {
            forceTool.updateInitialPos();
            forceTool.setCurrentPos(mainMousePos);
        }
        if (currentTool == charge) {
            chargeCreatorTool.setCurrentPos(mainMousePos);
        }
        if (currentTool == select) {
            selectionTool.updateSize(mainMousePos);
            selectionTool.updateSelection(chargeVector);
        }
        if (currentTool == placeWall) {
            placeWallTool.setCurrentPos(mainMousePos);
        }
    }
    if (!lockView) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            mainView.move(-viewSpeed, 0);
            mainWindow.setView(mainView);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            mainView.move(viewSpeed, 0);
            mainWindow.setView(mainView);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            mainView.move(0, -viewSpeed);
            mainWindow.setView(mainView);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            mainView.move(0, viewSpeed);
            mainWindow.setView(mainView);
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {
        chargeCreatorTool.chargeMass += 0.1f;
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1)
           << chargeCreatorTool.chargeMass;
        massText->setText("Mass=" + ss.str());
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
        chargeCreatorTool.chargeMass -= 0.1f;
        if (chargeCreatorTool.chargeMass < 1.f)
            chargeCreatorTool.chargeMass = 1.f;
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1)
           << chargeCreatorTool.chargeMass;
        massText->setText("Mass=" + ss.str());
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
        chargeCreatorTool.chargeRadius += 0.1f;
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1)
           << chargeCreatorTool.chargeRadius;
        radiusText->setText("Radius=" + ss.str());
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
        chargeCreatorTool.chargeRadius -= 0.1f;
        if (chargeCreatorTool.chargeRadius < 1.f)
            chargeCreatorTool.chargeRadius = 1.f;
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1)
           << chargeCreatorTool.chargeRadius;
        radiusText->setText("Radius=" + ss.str());
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
        chargeCreatorTool.electricCharge += 0.1f;
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1)
           << chargeCreatorTool.electricCharge;
        chargeText->setText("Charge=" + ss.str());
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
        chargeCreatorTool.electricCharge -= 0.1f;
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1)
           << chargeCreatorTool.electricCharge;
        chargeText->setText("Charge=" + ss.str());
    }
    /*else {
        if (followCharge != nullptr) {
            mainView.setCenter(followCharge->getPosition());
            mainWindow.setView(mainView);
        }
    }*/
}
void Simulation::update()
{
    using namespace VectorUtilities;
    // Update physics
    // Reset electric field & forces
    electricField.reset();
    for (auto chargePtr : chargeVector) {
        chargePtr->setForce(sf::Vector2f(0, 0));
        electricField.incrementField(*chargePtr);
    }
    // Apply Forces
    forceTool.applyForce();
    using chargePtrIter = std::vector<std::shared_ptr<Charge>>::const_iterator;
    using wallPtrIter = std::vector<std::shared_ptr<Wall>>::const_iterator;
    for (chargePtrIter s = chargeVector.begin(); s != chargeVector.end(); s++) {
        Charge& sCharge = *(*s);
        // Electric field
        electricField.applyForceOnCharge(sCharge);
        //
        sf::Vector2f sPos = sCharge.getPosition();
        sCharge.setIsCursorOn(distance(sPos, mainMousePos) <
                              sCharge.getRadius());

        // Circle-Circle Collision
        for (chargePtrIter r = s + 1; r != chargeVector.end(); r++) {
            Charge& rCharge = *(*r);
            if (detectChargeChargeCollision(rCharge, sCharge, dt)) {
                sf::Vector2f velDiff =
                    rCharge.getVelocity() - sCharge.getVelocity();
                sf::Vector2f normal =
                    unit(sCharge.getPosition() -
                         rCharge.getPosition()); // Points from (*r) to (*s)
                float reducedMass = (rCharge.getMass() * sCharge.getMass()) /
                                    (rCharge.getMass() + sCharge.getMass());
                sf::Vector2f J =
                    2 * reducedMass * dotProduct(velDiff, normal) * normal;
                (*r)->incrementForce(-J / dt);
                sCharge.incrementForce(J / dt);
            }
        }
        // Circle-Wall collision
        for (wallPtrIter wIter = wallVector.begin(); wIter != wallVector.end();
             wIter++) {
            Wall& wall = *(*wIter);
            if (detectChargeWallCollision(sCharge, wall)) {
                sf::Vector2f velocityParallel =
                    dotProduct(sCharge.getVelocity(), unit(wall.asVector())) *
                    unit(wall.asVector());
                sf::Vector2f velocityPerpendicular =
                    dotProduct(sCharge.getVelocity(), wall.normal()) *
                    wall.normal();
                sf::Vector2f newVelocity =
                    velocityParallel - velocityPerpendicular;
                sCharge.setVelocity(newVelocity);
            }
        }
    }
    // Boundaries
    // Velocity
    // Position
    for (auto charge : chargeVector) {
        charge->incrementVelocity(dt * charge->getForce() / charge->getMass());
        charge->setPosition(charge->getPosition() + charge->getVelocity() * dt);
        // Boundary
        float cx = charge->getPosition().x;
        float cy = charge->getPosition().y;
        float cvx = charge->getVelocity().x;
        float cvy = charge->getVelocity().y;
        if ((cx < 0 && cvx < 0) || (cx > bWidth && cvx > 0))
            charge->setVelocity({-cvx, cvy});
        if ((cy < 0 && cvy < 0) || (cy > bHeight && cvy > 0))
            charge->setVelocity({cvx, -cvy});
    }
    // Grid
    if (showGrid)
        grid.updateHighlightPosition(mainMousePos);
    // WHERE  ELSE TO PUT THIS?
    if (lockView) {
        followTool.focusViewOnCharge(mainView);
        mainWindow.setView(mainView);
    }
}
void Simulation::render()
{
    mainWindow.clear();
    // Draw equipotential lines
    //TODO: Organize the equipotential code
    if (chargeVector.size() != 0) {
        std::vector<sf::Vector3f> pos_q_vector;
        for (auto charge : chargeVector) {
            sf::Vector2f pos = charge->getPosition();
            pos = sf::Vector2f(pos.x, bHeight - pos.y);
            pos_q_vector.push_back(
                sf::Vector3f(pos.x, pos.y, charge->getCharge()));
        }
        equipotentialShader.setUniformArray(
            "chargeArray", &pos_q_vector[0], chargeVector.size());
        mainWindow.draw(bgSprite, &equipotentialShader);
    }
    // Draw Grid
    if (showGrid) {
        grid.draw(mainWindow);
        // Draw Electric field
        electricField.draw(mainWindow);
    }
    // Draw charges
    for (auto s : chargeVector) {
        if (s->getIsCursorOn()) {
            // Draw charge
            mainWindow.draw(*s, &chargeHighlightShader);
            // Draw velocity line
            mainWindow.draw(s->velocityLine(),
                            sf::BlendMode(sf::BlendMode::OneMinusDstColor,
                                          sf::BlendMode::Zero,
                                          sf::BlendMode::Add));
        } else
            mainWindow.draw(*s);
    }
    for (auto wall : wallVector) {
        mainWindow.draw(*wall);
    }
    forceTool.draw(mainWindow);
    chargeCreatorTool.draw(mainWindow);
    selectionTool.draw(mainWindow);
    // Draw Gui
    gui.draw();

    mainWindow.display();
}
void Simulation::zoomMainViewAt(sf::Vector2i pixel, float zoom)
{
    const sf::Vector2f beforeCoord{mainWindow.mapPixelToCoords(pixel)};
    mainView.zoom(zoom);
    mainWindow.setView(mainView);
    const sf::Vector2f afterCoord{mainWindow.mapPixelToCoords(pixel)};
    const sf::Vector2f offsetCoords{beforeCoord - afterCoord};
    mainView.move(offsetCoords);
    mainWindow.setView(mainView);
    viewSpeed *= zoom;
}

bool detectChargeWallCollision(const Charge& charge, const Wall& wall)
{
    // Test if distance <= radius
    // and if the charge is coming at the wall
    using namespace VectorUtilities;
    sf::Vector2f ray = wall.getVertexPosition(0) - charge.getPosition();
    float distance = dotProduct(ray, wall.normal());
    float velocityFlux = dotProduct(charge.getVelocity(), wall.normal());
    bool isComingAtWall = (velocityFlux * distance > 0.f);
    float a = dotProduct(wall.asVector(), wall.asVector());
    float b = 2 * dotProduct(wall.asVector(), ray);
    float c = dotProduct(ray, ray) - charge.getRadius() * charge.getRadius();

    bool intersects = false;
    float discriminant = b * b - 4 * a * c;
    if (discriminant > 0) {
        discriminant = std::sqrt(discriminant);
        float t1 = (-b - discriminant) / (2 * a);
        float t2 = (-b + discriminant) / (2 * a);
        if (t1 >= 0 && t2 <= 1)
            intersects = true;
    }
    return intersects && isComingAtWall;
}
