#ifndef GRID_H
#define GRID_H
#include <SFML/Graphics.hpp>
#include <math.h>
class MeshSquare : public sf::Drawable, public sf::Transformable
{
  private:
    sf::VertexArray lineVertices;
    sf::VertexArray squareVertices;

  public:
    MeshSquare(sf::Vector2f pos, float length,
               sf::Color outlineColor = sf::Color::Red,
               sf::Color fillColor = sf::Color::Transparent)
        : lineVertices(sf::LineStrip, 5), squareVertices(sf::Quads, 4)
    {
        squareVertices[0].position = lineVertices[0].position =
            pos + sf::Vector2f(0, 0);
        squareVertices[1].position = lineVertices[1].position =
            pos + sf::Vector2f(0, length);
        squareVertices[2].position = lineVertices[2].position =
            pos + sf::Vector2f(length, length);
        squareVertices[3].position = lineVertices[3].position =
            pos + sf::Vector2f(length, 0);
        lineVertices[4].position = pos + sf::Vector2f(0, 0);
        setOutlineColor(outlineColor);
        setFillColor(fillColor);
    }
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(squareVertices, states);
        target.draw(lineVertices, states);
    }
    void setOutlineColor(sf::Color outlineColor)
    {
        lineVertices[0].color = lineVertices[1].color = lineVertices[2].color =
            lineVertices[3].color = lineVertices[3].color =
                lineVertices[4].color = outlineColor;
    }
    void setFillColor(sf::Color fillColor)
    {
        squareVertices[0].color = squareVertices[1].color =
            squareVertices[2].color = squareVertices[3].color = fillColor;
    }
};
class Grid
{
  private:
    sf::VertexArray vertexGrid;
    MeshSquare highlightSqr;
    bool showHighlight;
    float cellSize;
    int columns;
    int rows;

  private:
    bool isWithinGrid(const sf::Vector2f& v);

  public:
    Grid(float cellSize, int columns, int rows)
        : cellSize(cellSize), columns(columns), rows(rows),
          vertexGrid(sf::Lines), highlightSqr({0, 0}, cellSize)
    {
        // Set up vertexGrid
        for (int x = 0; x <= columns; x++) {
            sf::Vertex top(cellSize * sf::Vector2f(x, 0), sf::Color::White);
            sf::Vertex bottom(cellSize * sf::Vector2f(x, rows),
                              sf::Color::White);
            vertexGrid.append(top);
            vertexGrid.append(bottom);
        }
        for (int y = 0; y <= rows; y++) {
            sf::Vertex left(cellSize * sf::Vector2f(0, y), sf::Color::White);
            sf::Vertex right(cellSize * sf::Vector2f(columns, y),
                             sf::Color::White);
            vertexGrid.append(left);
            vertexGrid.append(right);
        }
        //
        showHighlight = true;
    }
    void updateHighlightPosition(const sf::Vector2f& pos);
    void draw(sf::RenderWindow& window);
    sf::Vector2i mapWorldToGridCoordinates(const sf::Vector2f& worldCoords);
};
#endif
