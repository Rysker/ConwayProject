#ifndef APPLICATION_H
#define APPLICATION_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <map>
#include <string>
#include "IAppControls.h"
#include "AppState.h"
#include "IConwayAlgorithm.h"
#include "GUIPanel.h"
#include "GameRenderer.h"
#include "ICommand.h"

class Game : public IAppControls
{
    public:
        Game();
        ~Game() override;
        void run();

    protected:
        void play() override;
        void pause() override;
        void clearBoard() override;
        void saveState(const char* filename) override;
        void loadState(const char* filename) override;

    private:
        void processEvents();
        void update();
        void render();

        sf::RenderWindow m_window;
        std::unique_ptr<IConwayAlgorithm> m_simulation;
        AppState m_appState;
        std::unique_ptr<GUIPanel> m_guiPanel;
        std::unique_ptr<GameRenderer> m_gameRenderer;
        std::map<std::string, std::unique_ptr<ICommand>> m_commands;

        sf::Clock m_gameClock;
        sf::Clock m_deltaClock;

        int m_gameWidth;
        int m_gameHeight;
};

#endif //APPLICATION_H