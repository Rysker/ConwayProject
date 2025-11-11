#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <vector>
#include "BenchmarkCommons.h"
#include "AlgorithmFactory.h"

class Benchmark
{
    public:
        Benchmark();

        std::vector<BenchmarkResult> run(const std::vector<AlgorithmType>& types, const std::vector<BoardSize>& sizes, int generations);

    private:
        AlgorithmFactory factory_;

        BenchmarkResult runSingleTest(AlgorithmType type, BoardSize size, int generations);
};

#endif //BENCHMARK_H
