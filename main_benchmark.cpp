#include <iostream>
#include <vector>
#include <mpi.h>
#include "Benchmark.h"
#include "BenchmarkCommons.h"

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    int rank, worldSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

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

    if (worldSize == 1)
    {
        std::cout << "Sequential, OpenMP and CUDA benchmarks" << std::endl;
        std::vector<AlgorithmType> types = {
                AlgorithmType::Sequential,
                AlgorithmType::OpenMP,
                AlgorithmType::CUDA
        };
        allResults = runner.run(types, sizesToTest, generations);
    }

    else
    {
        if (rank == 0)
            std::cout << "MPI Benchmark" << std::endl;

        std::vector<AlgorithmType> mpiType = { AlgorithmType::MPI };

        std::vector<BenchmarkResult> mpiResults = runner.run(mpiType, sizesToTest, generations);

        if (rank == 0)
            allResults = mpiResults;
    }

    if (rank == 0)
    {
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
    }

    MPI_Finalize();

    return 0;
}