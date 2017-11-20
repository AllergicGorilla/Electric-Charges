#include "charge.hpp"
#include "utilities.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <memory>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 1024), "Electric");
    // Define view
    sf::Vector2f center(0, 0);
    sf::Vector2f viewSize(800, 800);
    sf::View view(center, viewSize);
    window.setView(view);

    // Font setup
    sf::Font courier_prime;
    if (!courier_prime.loadFromFile("Courier Prime.ttf")) {
        // Hold up, mah boi
        std::cout << "Font file was unable to be loaded" << std::endl;
        window.close();
    }
    //
    std::vector<std::shared_ptr<Charge>> charge_vector;
    // Velocity line
    Line velocity_line;
    //
    Line force_line;
    force_line.setColor(sf::Color::Green);
    // Charge count text
    sf::Text chargeCount("Charges: " + std::to_string(0), courier_prime, 30);
    chargeCount.setPosition(window.mapPixelToCoords(sf::Vector2i(0, 0)));

    //
    sf::Clock clock;
    //
    bool leftMouseReleased = true;
    sf::Vector2f nextChargePosition(0, 0);
    //
    enum Tool { charge, force, follow, trash };
    Tool tool = charge;
    std::shared_ptr<Charge> forceCharge = nullptr;
    std::shared_ptr<Charge> followCharge = nullptr;
    //
    float viewSpeed = 0.8f;
    bool lockView = false;
    // Main loop
    while (window.isOpen()) {
        chargeCount.setPosition(window.mapPixelToCoords(sf::Vector2i(0, 0)));
        sf::Vector2f mousePos =
            window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Time dt = clock.restart();
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    std::cout << "the left button was pressed" << std::endl;
                    std::cout << "mouse x: " << event.mouseButton.x
                              << std::endl;
                    std::cout << "mouse y: " << event.mouseButton.y
                              << std::endl;
                    switch (tool) {
                    case force: {
                        for (auto chargePtr : charge_vector) {
                            if (chargePtr->getIsCursorOn()) {
                                forceCharge = chargePtr;
                                break;
                            } else
                                forceCharge = nullptr;
                        }
                        if (forceCharge != nullptr) {
                            force_line.setVertexPosition(
                                forceCharge->getPosition(), 0);
                        }
                    } break;
                    case charge: {
                        nextChargePosition = mousePos;
                        leftMouseReleased = false;
                    } break;
                    case follow: {
                        for (auto chargePtr : charge_vector) {
                            if (chargePtr->getIsCursorOn()) {
                                followCharge = chargePtr;
                                break;
                            }
                        }
                        if (followCharge != nullptr) {
                            lockView = true;
                            view.setCenter((*followCharge).getPosition());
                            window.setView(view);
                        }
                    } break;
                    case trash: {
                        for (auto it = charge_vector.begin();
                             it != charge_vector.end(); it++) {
                            if ((*it)->getIsCursorOn()) {
                                charge_vector.erase(it);
                                chargeCount.setString(
                                    "Charges: " +
                                    std::to_string(charge_vector.size()));
                                break;
                            }
                        }
                    } break;
                    }
                    // Change v_line's base
                    velocity_line.setVertexPosition(mousePos, 0);
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (tool == force)
                        forceCharge = nullptr;
                    if (tool == charge) {
                        // Add charge
                        sf::Vector2f chargeVelocity = velocity_line.asVector();

                        Charge c(10.f, 30, 0.f, 1.f, chargeVelocity);
                        c.setOrigin(10.f, 10.f);
                        c.setPosition(nextChargePosition);
                        std::cout << c.id << std::endl;
                        charge_vector.push_back(std::make_shared<Charge>(c));
                        // Edit Text
                        chargeCount.setString(
                            "Charges: " + std::to_string(charge_vector.size()));
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::C: tool = charge; break;
                case sf::Keyboard::F: tool = force; break;
                case sf::Keyboard::G: tool = follow; break;
                case sf::Keyboard::X: tool = trash; break;
                case sf::Keyboard::L:
                    lockView = false;
                    followCharge = nullptr;
                    break;
                }
            }
            if (event.type == sf::Event::Resized) {
                // Update the view to the new size of the window
                view.setSize(event.size.width, event.size.height);
                window.setView(view);
            }
        }
        // View movement
        if (!lockView) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                view.move(-viewSpeed, 0);
                window.setView(view);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                view.move(viewSpeed, 0);
                window.setView(view);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                view.move(0, -viewSpeed);
                window.setView(view);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                view.move(0, viewSpeed);
                window.setView(view);
            }
        } else {
            if (followCharge != nullptr) {
                view.setCenter(followCharge->getPosition());
                window.setView(view);
            }
        }
        // Update physics
        for (auto s : charge_vector) {
            sf::Vector2f sPos = s->getPosition();
            s->setIsCursorOn(distance(sPos, mousePos) < s->getRadius());
            s->getIsCursorOn() ? s->setFillColor(sf::Color::Red) :
                                 s->setFillColor(sf::Color::White);
            for (auto r : charge_vector) {
                if (detectChargeChargeCollision(*r, *s, dt.asSeconds())) {
                    std::cout << "COLLISION" << std::endl;
                    sf::Vector2f velDiff = r->getVelocity() - s->getVelocity();
                    sf::Vector2f normal =
                        unit(s->getPosition() -
                             r->getPosition()); // Points from r to s
                    float reducedMass = (r->getMass() * s->getMass()) /
                                        (r->getMass() + s->getMass());
                    sf::Vector2f J =
                        2 * reducedMass * dotProduct(velDiff, normal) * normal;
                    r->incrementVelocity(-J / r->getMass());
                    s->incrementVelocity(J / s->getMass());
                }
            }
            s->setPosition(sPos + s->getVelocity() * dt.asSeconds());
        }
        if (forceCharge != nullptr) {
            sf::Vector2f dF = force_line.asVector() * dt.asSeconds();
            forceCharge->incrementVelocity(dF / forceCharge->getMass());
        }

        // Drawing routine
        window.clear();
        // Draw charges
        for (auto s : charge_vector)
            window.draw(*s);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (tool == force && forceCharge != nullptr) {
                force_line.setVerticesPosition(forceCharge->getPosition(),
                                               mousePos);
                if (!leftMouseReleased)
                    window.draw(force_line.getVertexArray());
            }
            if (tool == charge) {
                velocity_line.setVertexPosition(mousePos, 1);
                if (!leftMouseReleased)
                    window.draw(velocity_line.getVertexArray());
            }
        }
        // Draw Text
        window.draw(chargeCount);
        window.display();
    }

    return 0;
}
