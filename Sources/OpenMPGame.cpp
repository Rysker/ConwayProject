#include "OpenMPGame.h"
#include <fstream>
#include <iostream>

OpenMPGame::OpenMPGame(int width, int height): width_(width), height_(height)
{
    currentBuffer_.resize(width * height, 0);
    nextBuffer_.resize(width * height, 0);
}

void OpenMPGame::step()
{
    calculateNextGeneration();
    swapBuffers();
}

void OpenMPGame::calculateNextGeneration()
{
    #pragma omp parallel for collapse(2)
    for (int y = 0; y < height_; y++)
    {
        for (int x = 0; x < width_; x++)
        {
            int index = getCellIndex(x, y);
            int liveNeighbors = countLiveNeighbors(x, y);
            bool isAlive = currentBuffer_[index];

            if (!isAlive && liveNeighbors == 3)
                nextBuffer_[index] = 1;
            else if (isAlive && (liveNeighbors == 2 || liveNeighbors == 3))
                nextBuffer_[index] = 1;
            else
                nextBuffer_[index] = 0;
        }
    }
}

void OpenMPGame::swapBuffers()
{
    currentBuffer_.swap(nextBuffer_);
}

int OpenMPGame::getCellIndex(int x, int y) const
{
    return y * width_ + x;
}

int OpenMPGame::countLiveNeighbors(int x, int y) const
{
    int count = 0;
    for (int j = -1; j <= 1; j++)
    {
        for (int i = -1; i <= 1; i++)
        {
            if (i == 0 && j == 0)
                continue;

            int nx = (x + i + width_) % width_;
            int ny = (y + j + height_) % height_;

            if (currentBuffer_[getCellIndex(nx, ny)])
                count++;
        }
    }
    return count;
}

bool OpenMPGame::getCellState(int x, int y) const
{
    if (x < 0 || x >= width_ || y < 0 || y >= height_)
        return false;

    return currentBuffer_[getCellIndex(x, y)];
}

void OpenMPGame::setCellState(int x, int y, bool state)
{
    if (x < 0 || x >= width_ || y < 0 || y >= height_)
        return;

    currentBuffer_[getCellIndex(x, y)] = state ? 1 : 0;
}

void OpenMPGame::clearGrid()
{
    std::fill(currentBuffer_.begin(), currentBuffer_.end(), 0);
    std::fill(nextBuffer_.begin(), nextBuffer_.end(), 0);
}

bool OpenMPGame::saveToFile(const std::string& filename) const
{
    std::ofstream file(filename);

    if (!file.is_open())
        return false;

    for (int y = 0; y < height_; y++)
    {
        for (int x = 0; x < width_; x++)
            file << (currentBuffer_[getCellIndex(x, y)] ? 'O' : '.');
        file << '\n';
    }
    return true;
}

bool OpenMPGame::loadFromFile(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
        return false;

    clearGrid();
    std::string line;
    for (int y = 0; y < height_; y++)
    {
        if (!std::getline(file, line))
            break;

        for (int x = 0; x < width_; x++)
        {
            if (x >= line.length())
                break;

            if (line[x] == 'O' || line[x] == '1')
                currentBuffer_[getCellIndex(x, y)] = 1;
        }
    }
    return true;
}