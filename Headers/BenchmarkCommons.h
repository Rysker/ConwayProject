#ifndef BENCHMARKCOMMONS_H
#define BENCHMARKCOMMONS_H

#include <string>

enum class AlgorithmType
{
    Sequential,
    OpenMP
};

struct BoardSize
{
    int width;
    int height;
};

struct BenchmarkResult
{
    std::string algorithmType;
    BoardSize size;
    int generations;
    double totalTimeMs;
};

inline const char* AlgorithmTypeToString(AlgorithmType type)
{
    switch (type)
    {
        case AlgorithmType::Sequential: return "Sequential";
        case AlgorithmType::OpenMP:     return "OpenMP";
        default:                        return "Unknown";
    }
}

#endif //BENCHMARKCOMMONS_H