#ifndef MPIGAME_H
#define MPIGAME_H

#include "IConwayAlgorithm.h"
#include <vector>
#include <string>
#include <mpi.h>

class MPIGame : public IConwayAlgorithm
{
public:
    MPIGame(int globalWidth, int globalHeight);
    virtual ~MPIGame();

    void step() override;
    bool getCellState(int x, int y) const override;
    void setCellState(int x, int y, bool state) override;
    void clearGrid() override;
    int getWidth() const override { return globalWidth_; }
    int getHeight() const override { return globalHeight_; }

    bool saveToFile(const std::string& filename) const override;
    bool loadFromFile(const std::string& filename) override;

    const char* getImplementationName() const override;

private:
    void calculateNextGeneration();
    void swapBuffers();
    int getLocalIndex(int x, int y) const;
    int countLiveNeighbors(int x, int y) const;

    int rank_;
    int worldSize_;
    int globalWidth_;
    int globalHeight_;
    int localHeight_;
    int localOffset_;
    std::vector<char> currentBuffer_;
    std::vector<char> nextBuffer_;
};

#endif //MPIGAME_H