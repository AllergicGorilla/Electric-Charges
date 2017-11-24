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
SelectionTool::SelectionTool()
    : Tool(), selectedChargeOutlineColor(sf::Color::Magenta)
{
    sf::Color fillColor = sf::Color(20, 165, 255, 100);
    sf::Color outlineColor = fillColor;
    outlineColor.a = 255;
    selectionRectangle.setFillColor(fillColor);
    selectionRectangle.setOutlineThickness(1.f);
    selectionRectangle.setOutlineColor(outlineColor);
}
void SelectionTool::usePrimary(
    bool isPressed, const std::vector<std::shared_ptr<Charge>>& chargeVector,
    sf::Vector2f mousePos)
{
    if (isPressed) {
        primaryButtonReleased = false;
        initialPos = mousePos;
        selectionRectangle.setPosition(initialPos);
        selectionBounds.left = initialPos.x;
        selectionBounds.top = initialPos.y;
    } else {
        primaryButtonReleased = true;
    }
}
void SelectionTool::updateSelection(
    const std::vector<std::shared_ptr<Charge>>& chargeVector)
{
    // Insert or remove charge* according to the selection
    // Updates the color along the way
    using chargePtrIter = std::vector<std::shared_ptr<Charge>>::const_iterator;
    for (chargePtrIter it = chargeVector.begin(); it != chargeVector.end();
         it++) {
        if (selectionBounds.contains((*it)->getPosition())) {
            // Only push_back if the element wasn't found
            if (std::find(selectedCharges.begin(), selectedCharges.end(),
                          *it) == selectedCharges.end())
                selectedCharges.push_back(*it);
            (*it)->setOutlineColor(selectedChargeOutlineColor);
        } else {
            (*it)->setOutlineColor(sf::Color::Transparent);
            selectedCharges.erase(std::remove(selectedCharges.begin(),
                                              selectedCharges.end(), *it),
                                  selectedCharges.end());
        }
    }
}
void SelectionTool::updateSize(sf::Vector2f pos)
{
    currentPos = pos;
    selectionRectangle.setSize(currentPos - initialPos);
    selectionBounds.width = (currentPos - initialPos).x;
    selectionBounds.height = (currentPos - initialPos).y;
}
void SelectionTool::removeCharges(
    std::vector<std::shared_ptr<Charge>>& chargeVector)
{
    using chargePtrIter = std::vector<std::shared_ptr<Charge>>::iterator;
    for (chargePtrIter it = selectedCharges.begin();
         it != selectedCharges.end(); it++) {
        chargeVector.erase(
            std::remove(chargeVector.begin(), chargeVector.end(), *it),
            chargeVector.end());
    }
    this->reset();
}
void SelectionTool::selectAll(
    const std::vector<std::shared_ptr<Charge>>& chargeVector)
{
    selectedCharges = chargeVector;
    using chargePtrIter = std::vector<std::shared_ptr<Charge>>::const_iterator;
    for (chargePtrIter it = selectedCharges.begin();
         it != selectedCharges.end(); it++) {
        (*it)->setOutlineColor(selectedChargeOutlineColor);
    }
}
void SelectionTool::reset()
{
    selectedCharges.erase(selectedCharges.begin(), selectedCharges.end());
}
void SelectionTool::draw(sf::RenderWindow& window) const
{
    if (!primaryButtonReleased) {
        window.draw(selectionRectangle);
    }
}
void PlaceWallTool::usePrimary(bool isPressed, std::vector<std::shared_ptr<Wall>>& wallVector,
                sf::Vector2f mousePos)
{
  if (isPressed) {
      primaryButtonReleased = false;
      initialPos = mousePos;
      wallLine.setVertexPosition(initialPos, 0);
  } else {
      primaryButtonReleased = true;
      Wall newWall(initialPos, currentPos);
      wallVector.push_back(std::make_shared<Wall>(newWall));
  }
}
void PlaceWallTool::setCurrentPos(sf::Vector2f pos)
{
  currentPos = pos;
}
