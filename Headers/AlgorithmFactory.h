#ifndef ALGORITHMFACTORY_H
#define ALGORITHMFACTORY_H

#include <memory>
#include "IConwayAlgorithm.h"
#include "BenchmarkCommons.h"

class AlgorithmFactory
{
    public:
        std::unique_ptr<IConwayAlgorithm> createAlgorithm(AlgorithmType type, BoardSize size);
};

#endif //ALGORITHMFACTORY_H