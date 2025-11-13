#ifndef CUDAGAME_H
#define CUDAGAME_H

#include "IConwayAlgorithm.h"
#include <vector>
#include <string>

class CudaGame : public IConwayAlgorithm
{
public:
    CudaGame(int width, int height);
    virtual ~CudaGame();

    void step() override;
    bool getCellState(int x, int y) const override;
    void setCellState(int x, int y, bool state) override;
    void clearGrid() override;
    int getWidth() const override { return width_; }
    int getHeight() const override { return height_; }

    bool saveToFile(const std::string& filename) const override;
    bool loadFromFile(const std::string& filename) override;

    const char* getImplementationName() const override;

private:
    void copyGpuToHost() const;
    void copyHostToGpu(const std::vector<char>& hostBuffer);

    int width_;
    int height_;
    size_t dataSize_;

    char* d_currentBuffer_;
    char* d_nextBuffer_;

    mutable std::vector<char> h_renderBuffer_;
    mutable bool isHostBufferDirty_;
};

#endif //CUDAGAME_H