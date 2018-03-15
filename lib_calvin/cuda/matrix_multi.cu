#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "device_functions.h"
#include <cstdio>
#include <iostream>
#include "../util/stopwatch.h"

#define BLOCK_SIZE 25

// gridDim = (n / sizeofblock, same, same)
// blockDim = n
__global__ void matrixMultiKernel(int *a, int *b, int *target, size_t size) {
	size_t const blockSizeInMemory = BLOCK_SIZE * BLOCK_SIZE;
	__shared__ int aBlock[blockSizeInMemory];
	__shared__ int bBlock[blockSizeInMemory];
	__shared__ int targetBlock[blockSizeInMemory];

	size_t iBlock = blockIdx.x;
	size_t jBlock = blockIdx.y;
	size_t kBlock = blockIdx.z;

	// Load some data into cache
	size_t i = threadIdx.x;
	for (size_t j = 0; j < BLOCK_SIZE; j++) {
		aBlock[BLOCK_SIZE*i + j] = a[size*(iBlock*BLOCK_SIZE + i) + (jBlock*BLOCK_SIZE + j)];
		bBlock[BLOCK_SIZE*i + j] = b[size*(jBlock*BLOCK_SIZE + i) + (kBlock*BLOCK_SIZE + j)];
		targetBlock[BLOCK_SIZE*i + j] = 0;
	}
	__syncthreads();
	
	for (size_t j = 0; j < BLOCK_SIZE; j++) {
		for (size_t k = 0; k < BLOCK_SIZE; k++) {
			atomicAdd(&targetBlock[BLOCK_SIZE*i + k], 
				aBlock[BLOCK_SIZE*i + j] * bBlock[BLOCK_SIZE*j + k]);
		}
	}
	__syncthreads();

	// Write back to global memory
	for (size_t j = 0; j < BLOCK_SIZE; j++) {
		atomicAdd(&target[size*(iBlock*BLOCK_SIZE + i) + (kBlock*BLOCK_SIZE + j)],
			targetBlock[BLOCK_SIZE*i + j]);
	}
}

// gridDim = (n, n, n)
// blockDim = 1
__global__ void matrixMultiKernel2(int *a, int *b, int *target, size_t size) {
	size_t i = blockIdx.x;
	size_t j = blockIdx.y;
	size_t k = blockIdx.z;

	__shared__ int block_a; 
	__shared__ int block_b; 
	__shared__ int block_target;

	block_a = a[size*i + j];
	block_b = b[size*j + k];
	//__syncthreads();
	block_target = block_a * block_b;

	atomicAdd(&target[size*i + k], block_target);
}

// gridDim = n
// blockDim = n
__global__ void matrixMultiKernel3(int *a, int *b, int *target, size_t size) {
	size_t i = blockIdx.x;
	size_t j = threadIdx.x;
	for (size_t k = 0; k < size; k++) {
		atomicAdd(&target[size*i + k], a[size*i + j] * b[size*j + k]);
	}
}

cudaError_t matrixMulti(int *h_a, int *h_b, int *h_target, size_t size) {
	int *d_a;
	int *d_b;
	int *d_target;
	size_t sizeInMemory = size * size * sizeof(int);
	lib_calvin::stopwatch watch;

	cudaMalloc((void**)&d_a, sizeInMemory);
	cudaMalloc((void**)&d_b, sizeInMemory);
	cudaMalloc((void**)&d_target, sizeInMemory);
	
	cudaMemcpy(d_a, h_a, sizeInMemory, cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, h_b, sizeInMemory, cudaMemcpyHostToDevice);
	cudaMemcpy(d_target, h_target, sizeInMemory, cudaMemcpyHostToDevice);

	size_t numBlocks = size / BLOCK_SIZE;
	dim3 grid(numBlocks, numBlocks, numBlocks);
	
	watch.start();
	matrixMultiKernel<<< grid, BLOCK_SIZE >>>(d_a, d_b, d_target, size);
	//matrixMultiKernel2 <<< dim3(size, size, size), 1 >>>(d_a, d_b, d_target, size);
	//matrixMultiKernel3 <<< size, size >>>(d_a, d_b, d_target, size);

	cudaError_t cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "launch failed: %s\n", cudaGetErrorString(cudaStatus));
		goto Error;
	}

	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	cudaStatus = cudaDeviceSynchronize();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching!\n", cudaStatus);
		goto Error;
	}

	watch.stop();
	std::cout << "matrixMulti took: " << watch.read() << " sec. GFLOPS: " <<
		2 * size*size*size / watch.read() / (1000 * 1000 * 1000) << "\n";

	cudaMemcpy(h_target, d_target, sizeInMemory, cudaMemcpyDeviceToHost);

Error:
	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_target);

	return cudaStatus;
}