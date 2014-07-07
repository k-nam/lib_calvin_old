#include "matrix_test.h"
#include <mkl/include/mkl_boost_ublas_matrix_prod.hpp>

void lib_calvin_matrix::matrixTest() {
	std::cout << "\n---------- Beginning matrix test -----------\n";
	using boost::numeric::ublas::matrix;
	lib_calvin::stopwatch watch;
	int const size = 500;
	typedef double TYPE;
	watch.start();
	double rtv = doGigaOps();
	watch.stop();
	std::cout << "gigaop GFLOPS: " << 2 / watch.read() << " (" << rtv << ")\n";
	
	matrix<TYPE> a(size, size);	
	matrix<TYPE> b(size, size);
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; j++) {
			a(i, j) = 1.2;
			b(i, j) = 1.5;
		}
	}

	watch.start();
	matrix<TYPE> c = prod(a, b);
	watch.stop();
	std::cout << "mkl time" << watch.read() << "  GFLOPS: " << (double)size*size*size*2 / watch.read() / 1000000000 << "\n";
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; j++) {
			double residual = c(i, j) - 1.2 * 1.5 * size;
			if (residual*residual > 0.01) {
				std::cout << "MKL is lie\n";
				exit(0);
			}
		}
	}

	lib_calvin::matrix<double> m1(size);
	m1.check();

	__m128 aa = _mm_set_ps(1, 2, 3, 4);
	__m128 bb = _mm_set_ps(2, 4, 5, 6);
	aa = _mm_mul_ps(aa, bb);
	float *p = (float *)&aa; 
	std::cout << p[0] << " " << p[1] << " " << p[2] << " " << p[3] << "\n";
	std::cout << "\n";
}

double lib_calvin_matrix::doGigaOps() {
	int const giga = 1000000000;
	int const arraySize = 1000;
	int const iter = giga / arraySize;
	double *a = new double[arraySize];
	double *b = new double[arraySize];
	double *c = new double[arraySize];

	for (int i = 0; i < arraySize; ++i) {
		a[i] = sqrt(i);
		b[i] = sqrt(a[i] + 1);
		c[i] = 0;
	}
	for (int i = 0; i < iter; ++i) {
		for (int j = 0; j < arraySize; j++) {
			c[j] += a[j] * b[j];
		}
	}
	double returnValue = 1;
	for (int i = 0; i < arraySize; ++i) {
		returnValue += c[i];
	}
	delete[] a;
	delete[] b;
	delete[] c;
	return returnValue;
}


