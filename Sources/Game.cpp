#include "Game.h"
#include "SequentialGame.h"
#include "imgui.h"
#include "imgui-sfml.h"
#include <stdexcept>
#include <optional>
#include "CommandImpl/PlayCommand.h"
#include "CommandImpl/PauseCommand.h"
#include "CommandImpl/ClearCommand.h"
#include "CommandImpl/SaveCommand.h"
#include "CommandImpl/LoadCommand.h"

Game::Game()
        : m_gameWidth(800),
          m_gameHeight(600)
{
    int menuWidth = 250;
    int gridWidth = 200;
    int gridHeight = 150;

    m_window.create(sf::VideoMode({(unsigned int)(m_gameWidth + menuWidth), (unsigned int)m_gameHeight}), "Conway");
    m_window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(m_window))
    {
        throw std::runtime_error("Failed to initialize ImGui-SFML");
    }

    m_simulation = std::make_unique<SequentialGame>(gridWidth, gridHeight);
    m_simulation->loadFromFile("glider.txt");

    m_guiPanel = std::make_unique<GUIPanel>(m_appState, m_simulation->getImplementationName());
    m_gameRenderer = std::make_unique<GameRenderer>(m_window, m_gameWidth, m_gameHeight);

    char* filenamePtr = m_guiPanel->getFilenameBufferPtr();

    m_commands["play"] = std::make_unique<PlayCommand>(*this);
    m_commands["pause"] = std::make_unique<PauseCommand>(*this);
    m_commands["clear"] = std::make_unique<ClearCommand>(*this);
    m_commands["save"] = std::make_unique<SaveCommand>(*this, filenamePtr);
    m_commands["load"] = std::make_unique<LoadCommand>(*this, filenamePtr);
}

Game::~Game()
{
    ImGui::SFML::Shutdown();
}

void Game::run()
{
    while (m_window.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents()
{
    while (const std::optional event = m_window.pollEvent())
    {
        ImGui::SFML::ProcessEvent(m_window, *event);

        if (event->is<sf::Event::Closed>())
            m_window.close();
    }
}

void Game::update()
{
    m_gameRenderer->handleInput(*m_simulation, m_appState);

    if (!m_appState.isPaused && m_gameClock.getElapsedTime().asSeconds() > m_appState.generationTimeSec)
    {
        m_simulation->step();
        m_gameClock.restart();
    }

    ImGui::SFML::Update(m_window, m_deltaClock.restart());
}

void Game::render()
{
    m_window.clear(sf::Color::Black);
    m_gameRenderer->draw(*m_simulation);
    m_guiPanel->draw(m_commands);
    ImGui::SFML::Render(m_window);
    m_window.display();
}

void Game::play()
{
    m_appState.isPaused = false;
    m_gameClock.restart();
}

void Game::pause()
{
    m_appState.isPaused = true;
}

void Game::clearBoard()
{
    m_simulation->clearGrid();
    m_appState.isPaused = true;
}

void Game::saveState(const char* filename)
{
    m_simulation->saveToFile(filename);
}

void Game::loadState(const char* filename)
{
    m_simulation->loadFromFile(filename);
    m_appState.isPaused = true;
}