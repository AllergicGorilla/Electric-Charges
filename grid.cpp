#include "grid.hpp"
void Grid::draw(sf::RenderWindow& window)
{
    for (int x = 0; x < columns; x++) {
        sf::VertexArray line(sf::Lines, 2);
        line[0] = vertexGrid[x][0];
        line[1] = vertexGrid[x].back();
        window.draw(line);
    }
    for (int y = 0; y < rows; y++) {
        sf::VertexArray line(sf::Lines, 2);
        line[0] = vertexGrid[0][y];
        line[1] = vertexGrid.back()[y];
        window.draw(line);
    }
}
