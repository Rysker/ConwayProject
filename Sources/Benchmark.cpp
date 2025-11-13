#include "Benchmark.h"
#include <iostream>
#include <stdexcept>
#include <chrono>

Benchmark::Benchmark() {}

std::vector<BenchmarkResult> Benchmark::run(const std::vector<AlgorithmType>& types, const std::vector<BoardSize>& sizes, int generations)
{
    std::vector<BenchmarkResult> allResults;

    for (const auto& type : types)
    {
        for (const auto& size : sizes)
        {
            BenchmarkResult result = runSingleTest(type, size, generations);
            allResults.push_back(result);
        }
    }
    std::cout << "Benchmarking finished." << std::endl;
    return allResults;
}

BenchmarkResult Benchmark::runSingleTest(AlgorithmType type, BoardSize size, int generations)
{
    std::unique_ptr<IConwayAlgorithm> algorithm = factory_.createAlgorithm(type, size);

    if (!algorithm)
        return { AlgorithmTypeToString(type), size, generations, -1.0};

    std::ostringstream filenameStream;
    filenameStream << "benchmark_" << size.width << "x" << size.height << ".txt";
    std::string filename = filenameStream.str();

    if (!algorithm->loadFromFile(filename))
    {
        std::cerr << "There is no benchmarking board for given size!\n" << "Terminating program!\n";
         throw std::runtime_error("Program terminated because board for benchmarking doesnt exist!");
    }

    auto startTime = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < generations; i++)
        algorithm->step();
    auto endTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> totalDuration = endTime - startTime;
    double totalTimeMs = totalDuration.count();

    return {
        algorithm->getImplementationName(),
            size,
            generations,
            totalTimeMs
    };
}