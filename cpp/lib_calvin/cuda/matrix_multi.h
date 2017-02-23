#include "cuda_runtime.h"

cudaError_t matrixMulti(double *a, double *b, double *target, size_t size);

void matrixMultiTest();