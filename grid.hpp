#ifndef GRID_H
#define GRID_H
#include <SFML/Graphics.hpp>
class Grid
{
  private:
    std::vector<std::vector<sf::Vertex>> vertexGrid;
    float cellSize;
    int columns;
    int rows;

  public:
    Grid(float cellSize, int columns, int rows) : cellSize(cellSize), columns(columns), rows(rows), vertexGrid(columns, std::vector<sf::Vertex>(rows))
    {
        for (int x = 0; x < columns; x++) {
            for (int y = 0; y < rows; y++) {
                sf::Vertex point;
                point.position = sf::Vector2f(x * cellSize, y * cellSize);
                point.color = sf::Color::White;
                vertexGrid[x][y] = point;
            }
        }
    }
    void draw(sf::RenderWindow& window);
};
#endif
