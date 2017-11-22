#include "tools.hpp"
void Tool::setInitialPos(sf::Vector2f pos) { initialPos = pos; }
void Tool::setCurrentPos(sf::Vector2f pos) { currentPos = pos; }
void ForceTool::setCurrentPos(sf::Vector2f pos)
{
    currentPos = pos;
    forceLine.setVertexPosition(currentPos, 1);
}
void ForceTool::usePrimary(
    bool isPressed, const std::vector<std::shared_ptr<Charge>>& chargeVector,
    sf::Vector2f mousePos)
{
    if (isPressed) {
        primaryButtonReleased = false;
        for (auto chargePtr : chargeVector) {
            if (chargePtr->getIsCursorOn()) {
                forceCharge = chargePtr;
                break;
            } else
                forceCharge = nullptr;
        }
        if (forceCharge != nullptr) {
            initialPos = forceCharge->getPosition();
            forceLine.setVertexPosition(initialPos, 0);
        } else {
            primaryButtonReleased = true;
            forceCharge = nullptr;
        }
    } else {
        primaryButtonReleased = true;
        forceCharge = nullptr;
    }
}
void ForceTool::applyForce() const
{
    if (forceCharge != nullptr) {
        forceCharge->incrementForce(forceLine.asVector());
    }
}
void ForceTool::updateInitialPos()
{
    if (forceCharge != nullptr) {
        initialPos = forceCharge->getPosition();
        forceLine.setVertexPosition(initialPos, 0);
    }
}
void ForceTool::draw(sf::RenderWindow& window) const
{
    if (!primaryButtonReleased) {
        window.draw(forceLine.getVertexArray());
    }
}

// ChargeCreatorTool
void ChargeCreatorTool::usePrimary(
    bool isPressed, std::vector<std::shared_ptr<Charge>>& chargeVector,
    sf::Vector2f mousePos)
{
    if (isPressed) {
        primaryButtonReleased = false;
        initialPos = mousePos;
        velocityLine.setVertexPosition(initialPos, 0);
    } else {
        primaryButtonReleased = true;
        sf::Vector2f chargeVelocity = currentPos - initialPos;
        Charge newCharge(10.f, 30, 0.f, 1.f, chargeVelocity);
        newCharge.setOrigin(10.f, 10.f);
        newCharge.setPosition(initialPos);
        std::cout << newCharge.id << std::endl;
        chargeVector.push_back(std::make_shared<Charge>(newCharge));
    }
}
void ChargeCreatorTool::draw(sf::RenderWindow& window) const
{
    if (!primaryButtonReleased) {
        window.draw(velocityLine.getVertexArray());
    }
}
void ChargeCreatorTool::setCurrentPos(sf::Vector2f pos)
{
    currentPos = pos;
    velocityLine.setVertexPosition(currentPos, 1);
}

// FollowTool
void FollowTool::usePrimary(
    bool isPressed, const std::vector<std::shared_ptr<Charge>>& chargeVector,
    sf::Vector2f mousePos)
{
    if (isPressed) {
        for (auto chargePtr : chargeVector) {
            if (chargePtr->getIsCursorOn()) {
                followCharge = chargePtr;
                break;
            }
        }
    }
}
bool FollowTool::focusViewOnCharge(sf::View& view)
{
    if (followCharge != nullptr) {
        view.setCenter(followCharge->getPosition());
        return true;
    } else
        return false;
}
const std::shared_ptr<Charge> FollowTool::getFollowCharge() const
{
    return followCharge;
}
