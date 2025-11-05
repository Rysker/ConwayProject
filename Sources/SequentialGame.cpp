#include "SequentialGame.h"
#include <fstream>
#include <iostream>

SequentialGame::SequentialGame(int width, int height): m_width(width), m_height(height)
{
    m_currentBuffer.resize(width * height, 0);
    m_nextBuffer.resize(width * height, 0);
}

void SequentialGame::step()
{
    calculateNextGeneration();
    swapBuffers();
}

void SequentialGame::calculateNextGeneration()
{
    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            int index = getCellIndex(x, y);
            int liveNeighbors = countLiveNeighbors(x, y);
            bool isAlive = m_currentBuffer[index];

            if (!isAlive && liveNeighbors == 3)
                m_nextBuffer[index] = 1;
            else if (isAlive && (liveNeighbors == 2 || liveNeighbors == 3))
                m_nextBuffer[index] = 1;
            else
                m_nextBuffer[index] = 0;
        }
    }
}

void SequentialGame::swapBuffers()
{
    m_currentBuffer.swap(m_nextBuffer);
}

int SequentialGame::getCellIndex(int x, int y) const
{
    return y * m_width + x;
}

int SequentialGame::countLiveNeighbors(int x, int y) const
{
    int count = 0;
    for (int j = -1; j <= 1; j++)
    {
        for (int i = -1; i <= 1; i++)
        {
            if (i == 0 && j == 0) continue;
            int nx = (x + i + m_width) % m_width;
            int ny = (y + j + m_height) % m_height;
            if (m_currentBuffer[getCellIndex(nx, ny)])
                count++;
        }
    }
    return count;
}

bool SequentialGame::getCellState(int x, int y) const
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) return false;
    return m_currentBuffer[getCellIndex(x, y)];
}

void SequentialGame::setCellState(int x, int y, bool state)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) return;
    m_currentBuffer[getCellIndex(x, y)] = state ? 1 : 0;
}

void SequentialGame::clearGrid()
{
    std::fill(m_currentBuffer.begin(), m_currentBuffer.end(), 0);
    std::fill(m_nextBuffer.begin(), m_nextBuffer.end(), 0);
}

bool SequentialGame::saveToFile(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Cant save file!" << filename << std::endl;
        return false;
    }
    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            file << (m_currentBuffer[getCellIndex(x, y)] ? 'O' : '.');
        }
        file << '\n';
    }
    return true;
}

bool SequentialGame::loadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Can't open file" << filename << std::endl;
        return false;
    }
    clearGrid();
    std::string line;
    for (int y = 0; y < m_height; y++)
    {
        if (!std::getline(file, line)) break;
        for (int x = 0; x < m_width; x++)
        {
            if (x >= line.length()) break;
            if (line[x] == 'O')
                m_currentBuffer[getCellIndex(x, y)] = 1;
        }
    }
    return true;
}