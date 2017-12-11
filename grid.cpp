#include "grid.hpp"

void Grid::draw(sf::RenderWindow& window)
{
    window.draw(vertexGrid);
    if (showHighlight)
        window.draw(highlightSqr);
}
sf::Vector2i Grid::mapWorldToGridCoordinates(sf::Vector2f worldCoords)
{
    // worldCoords must be within the grid's bounds!
    return sf::Vector2i(static_cast<int>(std::floor(worldCoords.x / cellSize)),
                        static_cast<int>(std::floor(worldCoords.y / cellSize)));
}
bool Grid::isWithinGrid(sf::Vector2f v)
{
    return (v.x >= 0.0f && v.x <= (columns * cellSize) && v.y >= 0.0f &&
            v.y <= (rows * cellSize));
}
void Grid::updateHighlightPosition(sf::Vector2f pos)
{
    if (isWithinGrid(pos)) {
        highlightSqr.setPosition(
            cellSize *
            static_cast<sf::Vector2f>(mapWorldToGridCoordinates(pos)));
        showHighlight = true;
    } else
        showHighlight = false;
}
