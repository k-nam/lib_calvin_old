#include "cuda_runtime.h"
#include "device_launch_parameters.h"

__global__ void matrixMultiKernel(double *a, double *b, double *target, size_t size) {
	int i = blockIdx.x;
	int k = threadIdx.x;
	int sum = 0;
	for (size_t j = 0; j < size; j++) {
		sum += a[size*i + j] * b[size*k + j];
	}
	target[size*i + k] += sum;
}

cudaError_t matrixMulti(double *h_a, double *h_b, double *h_target, size_t size) {
	double *d_a;
	double *d_b;
	double *d_target;
	size_t sizeInMemory = size * size * sizeof(double);

	cudaMalloc((void**)&d_a, sizeInMemory);
	cudaMalloc((void**)&d_b, sizeInMemory);
	cudaMalloc((void**)&d_target, sizeInMemory);

	cudaMemcpy(d_a, h_a, sizeInMemory, cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, h_b, sizeInMemory, cudaMemcpyHostToDevice);
	cudaMemcpy(d_target, h_target, sizeInMemory, cudaMemcpyHostToDevice);

	dim3 grid(size, size);
	matrixMultiKernel<<<size, size>>>(d_a, d_b, d_target, size);

	cudaDeviceSynchronize();

	cudaMemcpy(h_target, d_target, sizeInMemory, cudaMemcpyDeviceToHost);

	return cudaSuccess;
}