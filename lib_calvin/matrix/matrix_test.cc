#include "matrix_test.h"
#include <stdlib.h>
#include <cstdlib>
#include <chrono>
#include <immintrin.h>

#ifdef _WIN64 
#define MKL_ILP64
#endif


#include "mkl/include/mkl_boost_ublas_matrix_prod.hpp" // prod using MKL
//#include "boost/numeric/ublas/matrix.hpp" // prod using BOOST implementation


void lib_calvin_matrix::matrixTest() {	
	std::cout << "---------- Beginning matrix test -----------\n\n";
	typedef double NumericType;
	size_t testSize = 200;
	lib_calvin_matrix::mklTest(testSize);
	
	lib_calvin::stopwatch watch;
	watch.start();
	double rtv = doGigaOps();
	watch.stop();
	std::cout << watch.read() << "\n";

	assemblyTest();
	
	matrixFunctionTest();
	matrixPerformanceTest();

	__m128 aa = _mm_set_ps(1, 2, 3, 4);
	__m128 bb = _mm_set_ps(2, 4, 5, 6);
	aa = _mm_mul_ps(aa, bb);
	float *p = (float *)&aa; 
	std::cout << p[0] << " " << p[1] << " " << p[2] << " " << p[3] << "\n";
	std::cout << "\n";
	std::cout << "------------- Matrix test finished ---------------\n\n\n";	
}

void lib_calvin_matrix::matrixFunctionTest() {
	std::cout << "---------- Beginning matrixFunctionTest -----------\n\n";
	// If test size is not 40 * 2^n, naiveMultiAdd2 in matrix.cc will cause runtime error
	//  with _mm256_store_pd().
	size_t testSize = 125;
	lib_calvin::matrix<double> m(testSize);
	m.check();
	std::cout << "------------- matrixFunctionTest finished ---------------\n\n\n";
}

void lib_calvin_matrix::matrixPerformanceTest() {
	std::cout << "---------- Beginning matrixPerformanceTest -----------\n\n";
	size_t testSize = 2560;
	lib_calvin::matrix<double> m(testSize);
	m.check(false);
	std::cout << "------------- matrixPerformanceTest finished ---------------\n\n\n";
}

void lib_calvin_matrix::mklTest(size_t size) {
	std::cout << "---------- Beginning mklTest -----------\n\n";

	typedef double NumericType;
	using boost::numeric::ublas::matrix;
	using boost::numeric::ublas::vector;
	lib_calvin::stopwatch watch;


	lib_calvin_matrix::matrix<NumericType> mmy(size, size);
	mmy.randomize();

	size_t const size2 = 100;

	matrix<NumericType> a(size, size);
	matrix<NumericType> b(size, size);
	matrix<NumericType> c(size, size);

	lib_calvin_matrix::matrix<NumericType> test(size, size);
	vector<NumericType> x(size);
	vector<NumericType> y(size);

	
	for (size_t i = 0; i < size; ++i) {
		for (size_t j = 0; j < size; j++) {
			a(i, j) = 1.2;
			b(i, j) = 1.5;
			c(i, j) = mmy(j, j);
		}
		x(i) = 1.5;
		y(i) = 1.2;
	}

	c = prod(a, b);
	watch.start();
	for (size_t i = 0; i < size; ++i) {
		for (size_t j = 0; j < size; j++) {
			test.setval(i, j) = a(i, j);
			c(i, j) = mmy.getval(i, j);
		}
	}
	matrix<NumericType> result = prod(a, b);
	watch.stop();
	std::cout << "size: " << size << "\n";
	std::cout << "matrix*matrix: " << watch.read() << "  GFLOPS: " <<
		(double)size * size * size / watch.read() / 1000000000 << "\n";
	for (size_t i = 0; i < size; ++i) {
		for (size_t j = 0; j < size; j++) {
			double residual = result(i, j) - 1.2 * 1.5 * size;
			if (residual*residual > 0.01) {
				//std::cout << "MKL is lie\n";
				//exit(0);
			}
		}
	}
	watch.start();
	x = prod(a, x);
	watch.stop();
	std::cout << "matrix*vector: " << size << ". " << watch.read() << "  GFLOPS: " <<
		(double)size * size2 / watch.read() / 1000000000 << "\n";
	watch.start();
	double innerProdResult = inner_prod(x, y);
	watch.stop();
	std::cout << "vector*vector: " << size << ". " << watch.read() << "  GFLOPS: " <<
		(double)size / watch.read() / 1000000000 << "\n";
	std::cout << "\n";

	lib_calvin::matrix<NumericType> c1(size, size);
	lib_calvin::matrix<NumericType> x1(size, 1);
	lib_calvin::matrix<NumericType> y1(1, size);
	watch.start();
	auto result1 = c1 * x1;
	watch.stop();
	std::cout << "lib_calvin matrix*vector: " << size << ". " << watch.read() << "  GFLOPS: " <<
		(double)size * size2 / watch.read() / 1000000000 << "\n";
	watch.start();
	result1.reset(y1 * x1);
	watch.stop();
	std::cout << "lib_calvin vector*vector: " << size << ". " << watch.read() << "  GFLOPS: " <<
		(double)size / watch.read() / 1000000000 << "\n";
	std::cout << "\n";

	std::cout << "------------- mklTest finished ---------------\n\n";
}

double lib_calvin_matrix::doGigaOps() {
	std::cout << "---------- Beginning Gigaops -----------\n\n";
	lib_calvin::stopwatch watch;
	
	int const giga = 1000*1000*1000;
	int const arraySize = 1000;
	int const matrixSize = 1000;

	int const iter = giga / arraySize;
	double *a = (double *)_aligned_malloc(arraySize * sizeof(double), 32);
	double *b = (double *)_aligned_malloc(arraySize * sizeof(double), 32);
	double *c = (double *)_aligned_malloc(arraySize * sizeof(double), 32);
	double *d = (double *)_aligned_malloc(arraySize * sizeof(double), 32);
	double * __restrict x = new double[matrixSize*matrixSize];
	double * __restrict y = new double[matrixSize*matrixSize];
	double * __restrict z = new double[matrixSize*matrixSize];

	for (int i = 0; i < arraySize; ++i) {
		a[i] = 2 * i + 5;
		b[i] = 3 * i + 2;
		c[i] = 0;
		d[i] = 3;
	}
	watch.start();
	__m256d x1 = _mm256_set_pd(0, 0, 0, 0); 

	__m256d y1 = _mm256_set_pd(0, 0, 0, 0);
	__m256d y2 = _mm256_set_pd(0, 0, 0, 0);
	__m256d y3 = _mm256_set_pd(0, 0, 0, 0);
	__m256d y4 = _mm256_set_pd(0, 0, 0, 0);
	__m256d y5 = _mm256_set_pd(0, 0, 0, 0);
	__m256d y6 = _mm256_set_pd(0, 0, 0, 0);
	__m256d y7 = _mm256_set_pd(0, 0, 0, 0);
	__m256d y8 = _mm256_set_pd(0, 0, 0, 0);
	__m256d y9 = _mm256_set_pd(0, 0, 0, 0);
	__m256d y10 = _mm256_set_pd(0, 0, 0, 0);

	__m256d a1 = _mm256_set_pd(0, 0, 0, 0);
	__m256d a2 = _mm256_set_pd(0, 0, 0, 0);
	__m256d a3 = _mm256_set_pd(0, 0, 0, 0);
	__m256d a4 = _mm256_set_pd(0, 0, 0, 0);
	__m256d a5 = _mm256_set_pd(0, 0, 0, 0);
	__m256d a6 = _mm256_set_pd(0, 0, 0, 0);
	__m256d a7 = _mm256_set_pd(0, 0, 0, 0);
	__m256d a8 = _mm256_set_pd(0, 0, 0, 0);
	__m256d a9 = _mm256_set_pd(0, 0, 0, 0);
	__m256d a10 = _mm256_set_pd(0, 0, 0, 0);
	
	for (int i = 0; i < iter; ++i) {
		for (int j = 0; j < arraySize; j += 40) {

			x1 = _mm256_load_pd(a + j);

			y1 = _mm256_load_pd(b + j);
			y2 = _mm256_load_pd(b + j + 4);
			y3 = _mm256_load_pd(b + j + 8);
			y4 = _mm256_load_pd(b + j + 12);
			y5 = _mm256_load_pd(b + j + 16);
			y6 = _mm256_load_pd(b + j + 20);
			y7 = _mm256_load_pd(b + j + 24);
			y8 = _mm256_load_pd(b + j + 28);
			y9 = _mm256_load_pd(b + j + 32);
			y10 = _mm256_load_pd(b + j + 36);

			//a1 = _mm256_load_pd(d + j);
			//a1 = _mm256_mul_pd(x1, y1);

			a1 = _mm256_fmadd_pd(x1, y1, a1);
			a2 = _mm256_fmadd_pd(x1, y2, a2);
			a3 = _mm256_fmadd_pd(x1, y3, a3);
			a4 = _mm256_fmadd_pd(x1, y4, a4);
			a5 = _mm256_fmadd_pd(x1, y5, a5);
			a6 = _mm256_fmadd_pd(x1, y6, a6);
			a7 = _mm256_fmadd_pd(x1, y7, a7);
			a8 = _mm256_fmadd_pd(x1, y8, a8);
			a9 = _mm256_fmadd_pd(x1, y9, a9);
			a10 = _mm256_fmadd_pd(x1, y10, a10);

			//_mm256_store_pd(d + j, a1);
		}
	}	
	for (int i = 0; i < matrixSize; ++i) {
		for (int j = 0; j < matrixSize; ++j) {
			for (int k = 0; k < matrixSize; ++k) {
				//z[i * matrixSize + k] += x[i * matrixSize + j] * y[j * matrixSize + k];
				//returnValue *= 2;
			}
		}
	}	
	watch.stop();
	std::cout << "Gigaop: " << watch.read() << "  GFLOPS: " <<
		(double)giga / watch.read() / (1000*1000*1000) << "\n\n";
	std::cout << "---------- Gigaops finished -----------\n\n";

	return *((double *)&a1) * *((double *)&a2) * *((double *)&a3) * *((double *)&a4);
}

void lib_calvin_matrix::assemblyTest() {
	std::cout << "---------- Beginning assemblyTest -----------\n\n";
	size_t arraySize = 1024;
	size_t iteration = 1000 * 1000;
	size_t multiplier = 2;
	double *source = (double *)_aligned_malloc(arraySize * sizeof(double), 32);
	double *target = (double *)_aligned_malloc(arraySize * sizeof(double), 32);
	for (size_t i = 0; i < arraySize; i++) {
		source[i] = 2.0;
		target[i] = 3.0;
	}

	lib_calvin::stopwatch watch;
	watch.start();
	size_t returnValue = doAssembly(source, target, arraySize, iteration);
	watch.stop();
	std::cout << "assemblyTest took: " << watch.read() << " GFLOPS: " << 
		arraySize * iteration * 2 / watch.read() / 1000000000 << "\n";

	double result = 0;
	for (size_t i = 0; i < arraySize; i++) {
		//std::cout << inputArray[i] << "\n";
		result += target[i];
	}
	std::cout << "Result was: " << result << ", return value: " << returnValue << "\n\n";
	std::cout << "------------- assemblyTest finished ---------------\n\n";
}
