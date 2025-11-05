#include "GameRenderer.h"
#include "imgui.h"

GameRenderer::GameRenderer(sf::RenderWindow& window, int gameWidth, int gameHeight)
        : m_window(window), m_gameWidth(gameWidth), m_gameHeight(gameHeight) {}

void GameRenderer::draw(const IConwayAlgorithm& sim) const
{
    float cellWidth = static_cast<float>(m_gameWidth) / sim.getWidth();
    float cellHeight = static_cast<float>(m_gameHeight) / sim.getHeight();
    sf::VertexArray grid(sf::PrimitiveType::Triangles, sim.getWidth() * sim.getHeight() * 6);
    sf::Color aliveColor = sf::Color::Yellow;
    sf::Color deadColor(30, 30, 30);
    for (int y = 0; y < sim.getHeight(); y++)
    {
        for (int x = 0; x < sim.getWidth(); x++)
        {
            const sf::Color& color = sim.getCellState(x, y) ? aliveColor : deadColor;
            int v_idx = (y * sim.getWidth() + x) * 6;
            sf::Vector2f topLeft(x * cellWidth, y * cellHeight);
            sf::Vector2f topRight((x + 1) * cellWidth, y * cellHeight);
            sf::Vector2f bottomLeft(x * cellWidth, (y + 1) * cellHeight);
            sf::Vector2f bottomRight((x + 1) * cellWidth, (y + 1) * cellHeight);
            grid[v_idx + 0].position = topLeft;
            grid[v_idx + 1].position = topRight;
            grid[v_idx + 2].position = bottomLeft;
            grid[v_idx + 3].position = topRight;
            grid[v_idx + 4].position = bottomRight;
            grid[v_idx + 5].position = bottomLeft;
            for(int i=0; i<6; ++i) grid[v_idx + i].color = color;
        }
    }
    m_window.draw(grid);
}

void GameRenderer::handleInput(IConwayAlgorithm& sim, const AppState& state) const
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    if (state.isPaused && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
        if (mousePos.x >= 0 && mousePos.x < m_gameWidth)
        {
            int cellX = mousePos.x / (m_gameWidth / sim.getWidth());
            int cellY = mousePos.y / (m_gameHeight / sim.getHeight());
            bool cellState = (state.currentTool == TOOL::BRUSH);
            int r = state.brushSize / 2;
            for (int y = -r; y <= r; y++)
            {
                for (int x = -r; x <= r; x++)
                {
                    if (state.brushSize == 1 || (x*x + y*y) <= (r*r))
                        sim.setCellState(cellX + x, cellY + y, cellState);
                }
            }
        }
    }
}