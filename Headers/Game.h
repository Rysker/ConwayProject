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
        void changeImplementation(int implIndex) override;

    private:
        void processEvents();
        void update();
        void render();

        sf::RenderWindow window_;
        std::unique_ptr<IConwayAlgorithm> simulation_;
        AppState appState_;
        std::unique_ptr<GUIPanel> guiPanel_;
        std::unique_ptr<GameRenderer> gameRenderer_;
        std::map<std::string, std::unique_ptr<ICommand>> commands_;

        sf::Clock gameClock_;
        sf::Clock deltaClock_;

        int gameWidth_;
        int gameHeight_;
        int gridWidth_;
        int gridHeight_;
};

#endif //APPLICATION_H