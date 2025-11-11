#include <iostream>
#include <vector>
#include "Benchmark.h"
#include "BenchmarkCommons.h"

int main()
{
    std::cout << "Conway's Game of Life Benchmark" << std::endl;

    // Implementations that are tested
    std::vector<AlgorithmType> typesToTest = {
            AlgorithmType::Sequential,
            AlgorithmType::OpenMP
    };

    // Board sizes that are tested
    std::vector<BoardSize> sizesToTest = {
            { 512, 512 },
            { 1024, 1024 },
            { 2048, 2048 }
    };

    int generations = 100;

    Benchmark benchmark;
    std::vector<BenchmarkResult> results = benchmark.run(typesToTest, sizesToTest, generations);

    std::cout << "\n---FINAL RESULTS OF BENCHMARKS FOR " << generations <<" GENERATIONS---" << std::endl;
    for (const auto& res : results)
    {
        std::cout << "[" << res.algorithmType << "] for size: "
                  << res.size.width << "x" << res.size.height << " "
                  << "Result is: " << res.totalTimeMs<< " ms/board" << std::endl;
    }

    return 0;
}