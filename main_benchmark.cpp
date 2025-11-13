#include <iostream>
#include <vector>
#include <mpi.h> // NAG£ÓWEK MPI
#include "Benchmark.h"
#include "BenchmarkCommons.h"

int main(int argc, char* argv[])
{
    std::vector<AlgorithmType> typesToTest = {
        AlgorithmType::Sequential,
        AlgorithmType::OpenMP,
        AlgorithmType::MPI,
        AlgorithmType::CUDA
    };

    std::vector<BoardSize> sizesToTest = {
        { 512, 512 },
        { 1024, 1024 },
        { 2048, 2048 }
    };

	int generations = 1000;

    Benchmark runner;
    std::vector<BenchmarkResult> allResults;

    std::cout << "Starting Conway's Game of Life Benchmark..." << std::endl;

    for (const auto& type : typesToTest)
    {
        int rank = 0;
        int worldSize = 1;

        std::vector<AlgorithmType> currentTestType = { type };

        if (type == AlgorithmType::MPI)
        {
            MPI_Init(&argc, &argv);
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

            if (rank == 0)
            {
                std::cout << "\n--- Starting MPI Benchmark ---" << std::endl;
            }
        }
        else
        {
            if (rank == 0)
            {
                std::cout << "\n--- Starting "

                    << (type == AlgorithmType::Sequential ? "Sequential"
                        : (type == AlgorithmType::OpenMP ? "OpenMP" : "CUDA"))
                    << " Benchmark ---" << std::endl;
            }
        }


        std::vector<BenchmarkResult> currentResults;

        if (rank == 0)
        {
            currentResults = runner.run(currentTestType, sizesToTest, generations);

            allResults.insert(allResults.end(), currentResults.begin(), currentResults.end());
        }
        else
        {
            runner.run(currentTestType, sizesToTest, generations);
        }

        if (type == AlgorithmType::MPI)
        {
            MPI_Finalize();
        }
    }

    std::cout << "\n--- FINAL REPORT ---" << std::endl;
    for (const auto& res : allResults)
    {
        if (res.totalTimeMs > 0)
        {
            std::cout << "[" << res.algorithmType << "]\t"
                << res.size.width << "x" << res.size.height << "\t"
                << "Total: " << res.totalTimeMs << " ms" << std::endl;
        }
    }

    return 0;
}