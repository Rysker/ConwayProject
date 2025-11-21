#include "MPIGame.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

MPIGame::MPIGame(int globalWidth, int globalHeight) : globalWidth_(globalWidth), globalHeight_(globalHeight)
{
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize_);

    int rowsPerProcess = globalHeight_ / worldSize_;
    int remainder = globalHeight_ % worldSize_;

    localHeight_ = rowsPerProcess + (rank_ < remainder ? 1 : 0);
    localOffset_ = rank_ * rowsPerProcess + (rank_ < remainder ? rank_ : remainder);

    currentBuffer_.resize(globalWidth_ * (localHeight_ + 2), 0);
    nextBuffer_.resize(globalWidth_ * (localHeight_ + 2), 0);
}

MPIGame::~MPIGame()
{
}

int MPIGame::getLocalIndex(int x, int y) const
{
    return y * globalWidth_ + x;
}

void MPIGame::step()
{
    int rankAbove = (rank_ - 1 + worldSize_) % worldSize_;
    int rankBelow = (rank_ + 1) % worldSize_;

    char* sendTop = &currentBuffer_[getLocalIndex(0, 1)];
    char* recvBottom = &currentBuffer_[getLocalIndex(0, localHeight_ + 1)];

    char* sendBottom = &currentBuffer_[getLocalIndex(0, localHeight_)];
    char* recvTop = &currentBuffer_[getLocalIndex(0, 0)];

    int rowSize = globalWidth_;

    MPI_Sendrecv(sendTop, rowSize, MPI_CHAR, rankAbove, 0,
        recvBottom, rowSize, MPI_CHAR, rankBelow, 0,
        MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    MPI_Sendrecv(sendBottom, rowSize, MPI_CHAR, rankBelow, 1,
        recvTop, rowSize, MPI_CHAR, rankAbove, 1,
        MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    calculateNextGeneration();
    swapBuffers();

    MPI_Barrier(MPI_COMM_WORLD);
}

void MPIGame::calculateNextGeneration()
{
    for (int y = 1; y <= localHeight_; y++)
    {
        for (int x = 0; x < globalWidth_; x++)
        {
            int index = getLocalIndex(x, y);
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

void MPIGame::swapBuffers()
{
    currentBuffer_.swap(nextBuffer_);
}

int MPIGame::countLiveNeighbors(int x, int y) const
{
    int count = 0;
    for (int j = -1; j <= 1; j++)
    {
        for (int i = -1; i <= 1; i++)
        {
            if (i == 0 && j == 0) continue;
            int nx = (x + i + globalWidth_) % globalWidth_;
            int ny = y + j;

            if (currentBuffer_[getLocalIndex(nx, ny)])
                count++;
        }
    }
    return count;
}

bool MPIGame::loadFromFile(const std::string& filename)
{
    std::vector<char> globalBuffer;
    int success_flag = 0;

    if (rank_ == 0)
    {
        std::ifstream file(filename);
        if (file.is_open())
        {
            success_flag = 1;
            globalBuffer.resize(globalWidth_ * globalHeight_, 0);
            std::string line;
            for (int y = 0; y < globalHeight_; y++)
            {
                if (!std::getline(file, line))
                    break;
                for (int x = 0; x < globalWidth_ && x < line.length(); x++)
                {
                    if (line[x] == 'O' || line[x] == '1')
                        globalBuffer[y * globalWidth_ + x] = 1;
                }
            }
        }
        else
            std::cerr << "MPI [Rank 0] ERROR: Nie mozna otworzyc pliku " << filename << std::endl;
    }

    MPI_Bcast(&success_flag, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (success_flag == 0)
        return false;

    std::vector<int> sendCounts(worldSize_);
    std::vector<int> displacements(worldSize_);
    int currentDisplacement = 0;

    for (int i = 0; i < worldSize_; i++)
    {
        int h = (globalHeight_ / worldSize_) + (i < (globalHeight_ % worldSize_) ? 1 : 0);
        sendCounts[i] = h * globalWidth_;
        displacements[i] = currentDisplacement;
        currentDisplacement += sendCounts[i];
    }

    char* localRecvBuffer = &currentBuffer_[getLocalIndex(0, 1)];

    MPI_Scatterv(
        (rank_ == 0) ? globalBuffer.data() : nullptr,
        sendCounts.data(), displacements.data(), MPI_CHAR,
        localRecvBuffer, sendCounts[rank_], MPI_CHAR,
        0, MPI_COMM_WORLD
    );

    return true;
}

bool MPIGame::saveToFile(const std::string& filename) const
{
    std::vector<char> globalBuffer;
    std::vector<int> recvCounts(worldSize_);
    std::vector<int> displacements(worldSize_);
    int currentDisplacement = 0;

    for (int i = 0; i < worldSize_; i++)
    {
        int h = (globalHeight_ / worldSize_) + (i < (globalHeight_ % worldSize_) ? 1 : 0);
        recvCounts[i] = h * globalWidth_;
        displacements[i] = currentDisplacement;
        currentDisplacement += recvCounts[i];
    }

    if (rank_ == 0)
        globalBuffer.resize(globalWidth_ * globalHeight_);

    const char* localSendBuffer = &currentBuffer_[getLocalIndex(0, 1)];

    MPI_Gatherv(
        localSendBuffer, recvCounts[rank_], MPI_CHAR,
        (rank_ == 0) ? globalBuffer.data() : nullptr,
        recvCounts.data(), displacements.data(), MPI_CHAR,
        0, MPI_COMM_WORLD
    );

    if (rank_ == 0)
    {
        std::ofstream file(filename);
        if (!file.is_open()) return false;
        for (int y = 0; y < globalHeight_; y++)
        {
            for (int x = 0; x < globalWidth_; x++)
                file << (globalBuffer[y * globalWidth_ + x] ? 'O' : '.');
            file << '\n';
        }
        std::cout << "MPI [Rank 0] Zapisano plik " << filename << std::endl;
    }
    return true;
}

const char* MPIGame::getImplementationName() const
{
    return "MPI (Distributed)";
}


bool MPIGame::getCellState(int x, int y) const { return false; }
void MPIGame::setCellState(int x, int y, bool state) {}
void MPIGame::clearGrid()
{
    std::fill(currentBuffer_.begin(), currentBuffer_.end(), 0);
    std::fill(nextBuffer_.begin(), nextBuffer_.end(), 0);
}