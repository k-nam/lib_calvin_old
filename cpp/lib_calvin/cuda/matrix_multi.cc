#include <stdlib.h>
#include <iostream>
#include "matrix_multi.h"
#include "../util/stopwatch.h"

void matrixMultiTest() {
	size_t const giga = 1000 * 1000 * 1000;
	size_t const size = 1000;
	size_t sizeInMemory = size * size * sizeof(double);
	double *a = (double *)_aligned_malloc(sizeInMemory, 32);
	double *b = (double *)_aligned_malloc(sizeInMemory, 32);
	double *target = (double *)_aligned_malloc(sizeInMemory, 32);

	lib_calvin::stopwatch watch;	
	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < size; j++) {
			a[size*i + j] = 1;
			b[size*i + j] = 1;		
			target[size*i + j] = 0;
		}
	}
	watch.start();
	matrixMulti(a, b, target, size);
	watch.stop();
	std::cout << "matrixMulti took: " << watch.read() << " sec.\n";
}