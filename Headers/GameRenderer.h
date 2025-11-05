#ifndef GAMERENDERER_H
#define GAMERENDERER_H

#include <SFML/Graphics.hpp>
#include "IConwayAlgorithm.h"
#include "AppState.h"

class GameRenderer
{
    public:
        GameRenderer(sf::RenderWindow& window, int gameWidth, int gameHeight);

        void draw(const IConwayAlgorithm& sim) const;
        void handleInput(IConwayAlgorithm& sim, const AppState& state) const;

    private:
        sf::RenderWindow& m_window;
        int m_gameWidth;
        int m_gameHeight;
};

#endif //GAMERENDERER_H