#include "matrix_test.h"

#ifdef _WIN64 
#define MKL_ILP64
#endif


//#include "mkl/include/mkl_boost_ublas_matrix_prod.hpp" // prod using MKL
#include "boost/numeric/ublas/matrix.hpp" // prod using BOOST implementation

void lib_calvin_matrix::matrixTest() {
	size_t testSize = 1000;
	lib_calvin_matrix::mklTest(testSize);
	std::cout << "---------- Beginning matrix test -----------\n\n";	
	using boost::numeric::ublas::matrix;
	lib_calvin::stopwatch watch;
	typedef double NumericType;
	watch.start();
	double rtv = doGigaOps();
	watch.stop();
	std::cout << "gigaop GFLOPS: " << 2 / watch.read() << " (" << rtv << ")\n";

	lib_calvin::matrix<NumericType> m1(testSize);
	m1.check();

	__m128 aa = _mm_set_ps(1, 2, 3, 4);
	__m128 bb = _mm_set_ps(2, 4, 5, 6);
	aa = _mm_mul_ps(aa, bb);
	float *p = (float *)&aa; 
	std::cout << p[0] << " " << p[1] << " " << p[2] << " " << p[3] << "\n";
	std::cout << "\n";
	std::cout << "------------- Matrix test finished ---------------\n\n\n";
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

void lib_calvin_matrix::mklTest(size_t size) {
	std::cout << "---------- Beginning mklTest -----------\n\n";

	typedef double NumericType;
	using boost::numeric::ublas::matrix;
	lib_calvin::stopwatch watch;

	matrix<NumericType> a(size, size);
	matrix<NumericType> b(size, size);
	matrix<NumericType> x(size, 1);
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; j++) {
			a(i, j) = 1.2;
			b(i, j) = 1.5;
		}
		x(i, 0) = 1.5;
	}

	watch.start();
	matrix<NumericType> c = prod(a, b);
	watch.stop();
	std::cout << "mkl size: " << size << ". " << watch.read() << "  GFLOPS: " <<
		(double)size*size*size * 2 / watch.read() / 1000000000 << "\n";
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; j++) {
			double residual = c(i, j) - 1.2 * 1.5 * size;
			if (residual*residual > 0.01) {
				std::cout << "MKL is lie\n";
				exit(0);
			}
		}
	}
	std::cout << "\n";

	watch.start();
	c = prod(a, x);
	watch.stop();
	std::cout << "mkl matrix*vector: " << size << ". " << watch.read() << "  GFLOPS: " <<
		(double)size*size * 2 / watch.read() / 1000000000 << "\n";


	lib_calvin::matrix<NumericType> e1(size, size);
	lib_calvin::matrix<NumericType> f(size, 1);
	watch.start();
	auto g = e1 * f;
	watch.stop();
	std::cout << "lib_calvin matrix*vector: " << size << ". " << watch.read() << "  GFLOPS: " <<
		(double)size*size * 2 / watch.read() / 1000000000 << "\n";

	std::cout << "\n";
	std::cout << "------------- mklTest finished ---------------\n\n\n";
}