#include "CudaGame.h"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <iostream>
#include <fstream>
#include <stdexcept>

#define CUDA_CHECK(call) \
    do { \
        cudaError_t err = call; \
        if (err != cudaSuccess) { \
            fprintf(stderr, "CUDA Error w %s:%d: %s\n", __FILE__, __LINE__, cudaGetErrorString(err)); \
            throw std::runtime_error(cudaGetErrorString(err)); \
        } \
    } while (0)

#define TILE_DIM 16
#define SH_MEM_DIM (TILE_DIM + 2)

__device__ int countLiveNeighborsGpu(const char* s_tile, int tx, int ty) 
{
    int count = 0;
    count += s_tile[(ty - 1) * SH_MEM_DIM + (tx - 1)];
    count += s_tile[(ty - 1) * SH_MEM_DIM + (tx    )];
    count += s_tile[(ty - 1) * SH_MEM_DIM + (tx + 1)];
    count += s_tile[(ty    ) * SH_MEM_DIM + (tx - 1)];
    count += s_tile[(ty    ) * SH_MEM_DIM + (tx + 1)];
    count += s_tile[(ty + 1) * SH_MEM_DIM + (tx - 1)];
    count += s_tile[(ty + 1) * SH_MEM_DIM + (tx    )];
    count += s_tile[(ty + 1) * SH_MEM_DIM + (tx + 1)];
    return count;
}

__global__ void gameOfLifeKernel(char* d_next, const char* d_curr, int width, int height) 
{
    __shared__ char s_tile[SH_MEM_DIM * SH_MEM_DIM];

    int x = blockIdx.x * TILE_DIM + threadIdx.x;
    int y = blockIdx.y * TILE_DIM + threadIdx.y;

    int tx = threadIdx.x;
    int ty = threadIdx.y;
    
    int st_x = tx + 1;
    int st_y = ty + 1;

    if (x < width && y < height) 
    {
        s_tile[st_y * SH_MEM_DIM + st_x] = d_curr[y * width + x];
    } 
    else 
    {
        s_tile[st_y * SH_MEM_DIM + st_x] = 0; // Wype³nij zerami poza plansz¹
    }

    int x_left = (x - 1 + width) % width;
    int x_right = (x + 1) % width;
    int y_up = (y - 1 + height) % height;
    int y_down = (y + 1) % height;

    if (tx == 0 && ty == 0) s_tile[0 * SH_MEM_DIM + 0] = d_curr[y_up * width + x_left]; // lewy-górny
    if (tx == TILE_DIM-1 && ty == 0) s_tile[0 * SH_MEM_DIM + (TILE_DIM+1)] = d_curr[y_up * width + x_right]; // prawy-górny
    if (tx == 0 && ty == TILE_DIM-1) s_tile[(TILE_DIM+1) * SH_MEM_DIM + 0] = d_curr[y_down * width + x_left]; // lewy-dolny
    if (tx == TILE_DIM-1 && ty == TILE_DIM-1) s_tile[(TILE_DIM+1) * SH_MEM_DIM + (TILE_DIM+1)] = d_curr[y_down * width + x_right]; // prawy-dolny
    
    if (tx == 0) s_tile[st_y * SH_MEM_DIM + 0] = d_curr[y * width + x_left]; // lewa
    if (tx == TILE_DIM-1) s_tile[st_y * SH_MEM_DIM + (TILE_DIM+1)] = d_curr[y * width + x_right]; // prawa
    if (ty == 0) s_tile[0 * SH_MEM_DIM + st_x] = d_curr[y_up * width + x]; // górna
    if (ty == TILE_DIM-1) s_tile[(TILE_DIM+1) * SH_MEM_DIM + st_x] = d_curr[y_down * width + x]; // dolna

    __syncthreads();

    if (x < width && y < height) 
    {
        int liveNeighbors = countLiveNeighborsGpu(s_tile, st_x, st_y);
        bool isAlive = s_tile[st_y * SH_MEM_DIM + st_x]; // Stan obecny

        if (!isAlive && liveNeighbors == 3) 
        {
            d_next[y * width + x] = 1;
        } 
        else if (isAlive && (liveNeighbors == 2 || liveNeighbors == 3)) 
        {
            d_next[y * width + x] = 1;
        } 
        else 
        {
            d_next[y * width + x] = 0;
        }
    }
}



CudaGame::CudaGame(int width, int height)
    : width_(width), height_(height), isHostBufferDirty_(true) 
{
    dataSize_ = width_ * height_ * sizeof(char);
    
    h_renderBuffer_.resize(width_ * height_);
    
    CUDA_CHECK(cudaMalloc(&d_currentBuffer_, dataSize_));
    CUDA_CHECK(cudaMalloc(&d_nextBuffer_, dataSize_));
    
    clearGrid();
}

CudaGame::~CudaGame() 
{
    cudaFree(d_currentBuffer_);
    cudaFree(d_nextBuffer_);
}

void CudaGame::step() 
{
    dim3 threadsPerBlock(TILE_DIM, TILE_DIM);
    dim3 numBlocks(
        (width_ + threadsPerBlock.x - 1) / threadsPerBlock.x,
        (height_ + threadsPerBlock.y - 1) / threadsPerBlock.y
    );
    
    gameOfLifeKernel<<<numBlocks, threadsPerBlock>>>(d_nextBuffer_, d_currentBuffer_, width_, height_);
    
    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaDeviceSynchronize());

    std::swap(d_currentBuffer_, d_nextBuffer_);
    
    isHostBufferDirty_ = true;
}

void CudaGame::copyGpuToHost() const 
{
    CUDA_CHECK(cudaMemcpy(
        h_renderBuffer_.data(), 
        d_currentBuffer_, 
        dataSize_, 
        cudaMemcpyDeviceToHost
    ));
    isHostBufferDirty_ = false;
}

void CudaGame::copyHostToGpu(const std::vector<char>& hostBuffer)
{
    CUDA_CHECK(cudaMemcpy(
        d_currentBuffer_,
        hostBuffer.data(),
        dataSize_,
        cudaMemcpyHostToDevice
    ));
    isHostBufferDirty_ = true;
}

bool CudaGame::getCellState(int x, int y) const 
{
    if (isHostBufferDirty_) 
    {
        copyGpuToHost();
    }
    
    if (x < 0 || x >= width_ || y < 0 || y >= height_) return false;
    return h_renderBuffer_[y * width_ + x];
}

void CudaGame::setCellState(int x, int y, bool state) 
{
    if (x < 0 || x >= width_ || y < 0 || y >= height_) return;
    
    h_renderBuffer_[y * width_ + x] = state ? 1 : 0;
    
    CUDA_CHECK(cudaMemcpy(
        d_currentBuffer_ + (y * width_ + x), 
        &h_renderBuffer_[y * width_ + x], 
        sizeof(char), 
        cudaMemcpyHostToDevice
    ));
}

void CudaGame::clearGrid() 
{
    CUDA_CHECK(cudaMemset(d_currentBuffer_, 0, dataSize_));
    CUDA_CHECK(cudaMemset(d_nextBuffer_, 0, dataSize_));
    isHostBufferDirty_ = true;
}

bool CudaGame::saveToFile(const std::string& filename) const 
{
    if (isHostBufferDirty_) 
    {
        copyGpuToHost();
    }
    
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    for (int y = 0; y < height_; y++) 
    {
        for (int x = 0; x < width_; x++) 
        {
            file << (h_renderBuffer_[y * width_ + x] ? 'O' : '.');
        }
        file << '\n';
    }
    return true;
}

bool CudaGame::loadFromFile(const std::string& filename) 
{
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    
    clearGrid(); // Czyœci te¿ bufor CPU (h_renderBuffer_)
    std::fill(h_renderBuffer_.begin(), h_renderBuffer_.end(), 0);

    std::string line;
    for (int y = 0; y < height_; y++) 
    {
        if (!std::getline(file, line)) break;
        for (int x = 0; x < width_; x++) 
        {
            if (x >= line.length()) break;
            if (line[x] == 'O' || line[x] == '1') 
            {
                h_renderBuffer_[y * width_ + x] = 1;
            }
        }
    }
    
    copyHostToGpu(h_renderBuffer_);
    return true;
}

const char* CudaGame::getImplementationName() const 
{
    return "CUDA";
}