#include "AlgorithmFactory.h"
#include "SequentialGame.h"
#include "OpenMPGame.h"
#include "MPIGame.h"
#include "CUDAGame.h"

std::unique_ptr<IConwayAlgorithm> AlgorithmFactory::createAlgorithm(AlgorithmType type, BoardSize size)
{
    switch (type)
    {
        case AlgorithmType::Sequential:
            return std::make_unique<SequentialGame>(size.width, size.height);

        case AlgorithmType::OpenMP:
            return std::make_unique<OpenMPGame>(size.width, size.height);

        case AlgorithmType::MPI:
            return std::make_unique<MPIGame>(size.width, size.height);
        
        case AlgorithmType::CUDA:
            return std::make_unique<CudaGame>(size.width, size.height);

        default:
            return nullptr;
    }
}