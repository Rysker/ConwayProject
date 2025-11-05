#ifndef SEQUENTIAL_GAME_H
#define SEQUENTIAL_GAME_H

#include "IConwayAlgorithm.h"
#include <vector>

class SequentialGame : public IConwayAlgorithm
{
    public:
        SequentialGame(int width, int height);
        ~SequentialGame() override = default;

        virtual void step() override;
        virtual bool getCellState(int x, int y) const override;
        virtual void setCellState(int x, int y, bool state) override;
        virtual void clearGrid() override;
        virtual int getWidth() const override { return m_width; }
        virtual int getHeight() const override { return m_height; }

        virtual bool saveToFile(const std::string& filename) const override;
        virtual bool loadFromFile(const std::string& filename) override;

        virtual const char* getImplementationName() const override { return "Wersja sekwencyjna"; }

    private:
        int m_width;
        int m_height;
        std::vector<char> m_currentBuffer;
        std::vector<char> m_nextBuffer;

        int getCellIndex(int x, int y) const;
        int countLiveNeighbors(int x, int y) const;
        void calculateNextGeneration();
        void swapBuffers();
};

#endif // SEQUENTIAL_GAME_H
