#include <stdlib.h>
#include <iostream>
#include "matrix_multi.h"
#include "../util/stopwatch.h"

void matrixMultiTest() {
	size_t const giga = 1000 * 1000 * 1000;
	size_t const size = 1000;
	size_t sizeInMemory = size * size * sizeof(double);
	int *a = (int *)_aligned_malloc(sizeInMemory, 32);
	int *b = (int *)_aligned_malloc(sizeInMemory, 32);
	int *target = (int *)_aligned_malloc(sizeInMemory, 32);
	
	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < size; j++) {
			a[size*i + j] = 1;
			b[size*i + j] = 1;		
			target[size*i + j] = 0;
		}
	}
	lib_calvin::stopwatch watch;
	watch.start();
	matrixMulti(a, b, target, size);
	watch.stop();
	std::cout << "Total time: " << watch.read() << "\n";
	bool isCorrect = true;
	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < size; j++) {
			//std::cout << target[size*i + j] << ",";
			if (target[size*i + j] != size) {
				isCorrect = false;
			}
		}
		//std::cout << "\n";
	}
	if (isCorrect == true) {
		std::cout << "Correct~ \n";
	} else {
		std::cout << "error! \n";
	}
	int x = 1;
	int y = ~x ^ 1;
	std::cout << y;
}