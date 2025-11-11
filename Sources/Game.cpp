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
#include "OpenMPGame.h"

Game::Game(): gameWidth_(800), gameHeight_(600), gridWidth_(200), gridHeight_(150)
{
    int menuWidth = 250;

    window_.create(sf::VideoMode({(unsigned int)(gameWidth_ + menuWidth), (unsigned int)gameHeight_}), "Conway");
    window_.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window_))
        throw std::runtime_error("Failed to initialize ImGui-SFML");


    guiPanel_ = std::make_unique<GUIPanel>(appState_, *this);
    gameRenderer_ = std::make_unique<GameRenderer>(window_, gameWidth_, gameHeight_);
    this->changeImplementation(0);

    char* filenamePtr = guiPanel_->getFilenameBufferPtr();

    commands_["play"] = std::make_unique<PlayCommand>(*this);
    commands_["pause"] = std::make_unique<PauseCommand>(*this);
    commands_["clear"] = std::make_unique<ClearCommand>(*this);
    commands_["save"] = std::make_unique<SaveCommand>(*this, filenamePtr);
    commands_["load"] = std::make_unique<LoadCommand>(*this, filenamePtr);

    //simulation_->loadFromFile("glider.txt");
}

Game::~Game()
{
    ImGui::SFML::Shutdown();
}

void Game::run()
{
    while (window_.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents()
{
    while (const std::optional event = window_.pollEvent())
    {
        ImGui::SFML::ProcessEvent(window_, *event);

        if (event->is<sf::Event::Closed>())
            window_.close();
    }
}

void Game::update()
{
    gameRenderer_->handleInput(*simulation_, appState_);

    if (!appState_.isPaused && gameClock_.getElapsedTime().asSeconds() > appState_.generationTimeSec)
    {
        simulation_->step();
        gameClock_.restart();
    }

    ImGui::SFML::Update(window_, deltaClock_.restart());
}

void Game::render()
{
    window_.clear(sf::Color::Black);
    gameRenderer_->draw(*simulation_);
    guiPanel_->draw(commands_);
    ImGui::SFML::Render(window_);
    window_.display();
}

void Game::play()
{
    appState_.isPaused = false;
    gameClock_.restart();
}

void Game::pause()
{
    appState_.isPaused = true;
}

void Game::clearBoard()
{
    simulation_->clearGrid();
    appState_.isPaused = true;
}

void Game::saveState(const char* filename)
{
    simulation_->saveToFile(filename);
}

void Game::loadState(const char* filename)
{
    simulation_->loadFromFile(filename);
    appState_.isPaused = true;
}

void Game::changeImplementation(int implIndex)
{
    appState_.isPaused = true;
    switch (implIndex)
    {
        case 0:
            simulation_ = std::make_unique<SequentialGame>(gridWidth_, gridHeight_);
            break;
        case 1:
            simulation_ = std::make_unique<OpenMPGame>(gridWidth_, gridHeight_);
            break;
        default:
            simulation_ = std::make_unique<SequentialGame>(gridWidth_, gridHeight_);
            break;
    }
}