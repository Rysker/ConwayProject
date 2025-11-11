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
        virtual int getWidth() const override { return width_; }
        virtual int getHeight() const override { return height_; }

        virtual bool saveToFile(const std::string& filename) const override;
        virtual bool loadFromFile(const std::string& filename) override;

        virtual const char* getImplementationName() const override { return "Wersja sekwencyjna"; }

    private:
        int width_;
        int height_;
        std::vector<char> currentBuffer_;
        std::vector<char> nextBuffer_;

        int getCellIndex(int x, int y) const;
        int countLiveNeighbors(int x, int y) const;
        void calculateNextGeneration();
        void swapBuffers();
};

#endif // SEQUENTIAL_GAME_H
