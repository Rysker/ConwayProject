#ifndef OPENMPGAME_H
#define OPENMPGAME_H

#include "IConwayAlgorithm.h"
#include <vector>
#include <string>

class OpenMPGame : public IConwayAlgorithm
{
    public:
        OpenMPGame(int width, int height);
        virtual ~OpenMPGame() = default;

        void step() override;
        bool getCellState(int x, int y) const override;
        void setCellState(int x, int y, bool state) override;
        void clearGrid() override;
        int getWidth() const override { return width_; }
        int getHeight() const override { return height_; }

        bool saveToFile(const std::string& filename) const override;
        bool loadFromFile(const std::string& filename) override;

        virtual const char* getImplementationName() const override { return "Wersja OpenMP"; }

    private:
        void calculateNextGeneration();
        void swapBuffers();
        int getCellIndex(int x, int y) const;
        int countLiveNeighbors(int x, int y) const;

        int width_;
        int height_;
        std::vector<char> currentBuffer_;
        std::vector<char> nextBuffer_;
};

#endif //OPENMPGAME_H