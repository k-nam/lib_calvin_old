#include "matrix_test.h"
#include <stdlib.h>
#include <cstdlib>
#include <chrono>

#ifdef _WIN64 
#define MKL_ILP64
#endif


#include "mkl/include/mkl_boost_ublas_matrix_prod.hpp" // prod using MKL
//#include "boost/numeric/ublas/matrix.hpp" // prod using BOOST implementation


void lib_calvin_matrix::matrixTest() {	
	typedef double NumericType;
	size_t testSize = 1024;
	lib_calvin_matrix::mklTest(testSize);

	//lib_calvin::matrix<NumericType> t1(10000);
	//lib_calvin::matrix<NumericType> t2(10000);
	//auto t3 = t1 * t2;		
	double rtv = doGigaOps();

	assemblyTest();

	/*
	std::cout << "---------- Beginning matrix test -----------\n\n";
	lib_calvin::matrix<NumericType> m1(testSize);
	m1.check();

	__m128 aa = _mm_set_ps(1, 2, 3, 4);
	__m128 bb = _mm_set_ps(2, 4, 5, 6);
	aa = _mm_mul_ps(aa, bb);
	float *p = (float *)&aa; 
	std::cout << p[0] << " " << p[1] << " " << p[2] << " " << p[3] << "\n";
	std::cout << "\n";
	std::cout << "------------- Matrix test finished ---------------\n\n\n";
	*/
}

double lib_calvin_matrix::doGigaOps() {
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
	double returnValue1 = 1;
	double returnValue2 = 2;
	double returnValue3 = 2;
	double returnValue4 = 2;
	double returnValue5 = 2;
	double returnValue6 = 2;
	double returnValue7 = 2;
	double returnValue8 = 2;
	double returnValue9 = 2;
	double returnValue10 = 2;

	__m256d x1 = _mm256_set_pd(0, 0, 0, 0); 
	__m256d x2 = _mm256_set_pd(0, 0, 0, 0);
	__m256d x3 = _mm256_set_pd(0, 0, 0, 0);
	__m256d x4 = _mm256_set_pd(0, 0, 0, 0);

	__m256d y1 = _mm256_set_pd(0, 0, 0, 0);
	__m256d y2 = _mm256_set_pd(0, 0, 0, 0);
	__m256d y3 = _mm256_set_pd(0, 0, 0, 0);
	__m256d y4 = _mm256_set_pd(0, 0, 0, 0);

	__m256d a1 = _mm256_set_pd(0, 0, 0, 0);
	__m256d a2 = _mm256_set_pd(0, 0, 0, 0);
	__m256d a3 = _mm256_set_pd(0, 0, 0, 0);
	__m256d a4 = _mm256_set_pd(0, 0, 0, 0);


	for (int ii = 0; ii < 1; ++ii) {
		for (int i = 0; i < iter; ++i) {
			for (int j = 0; j < arraySize - 100; j += 8) {
				/*
				returnValue1 *= 2;				
				returnValue2 *= 2;
				returnValue3 *= 2;
				returnValue4 *= 2;
				returnValue5 *= 2;
				returnValue6 *= 2;
				returnValue7 *= 2;
				returnValue8 *= 2;
				returnValue9 *= 2;
				returnValue10 *= 2;
				*/
				//c[j] = a[j] * b[j];
				//d[j] += a[j] * b[j];
				//d[j + 1] += (a[j + 1] * b[j + 1]);
				//d[j + 2] += (a[j + 2] * b[j + 2]);
				//d[j + 3] += (a[j + 3] * b[j + 3]);
				
				x1 = _mm256_load_pd(a + j);
				x2 = _mm256_load_pd(a + j + 4);
				//x3 = _mm256_load_pd(a + j + 8);
				//x4 = _mm256_load_pd(a + j + 12);

				y1 = _mm256_load_pd(b + j);
				y2 = _mm256_load_pd(b + j + 4);
				//y3 = _mm256_load_pd(b + j + 8);
				//y4 = _mm256_load_pd(b + j + 12);

				a1 = _mm256_load_pd(d + j);
				a2 = _mm256_load_pd(d + j + 4);
				//a3 = _mm256_load_pd(d + j + 8);
				//a4 = _mm256_load_pd(d + j + 12);

				x1 = _mm256_mul_pd(x1, y1);
				x2 = _mm256_mul_pd(x2, y2);
				//x3 = _mm256_mul_pd(x3, y3);
				//x4 = _mm256_mul_pd(x4, y4);
				
				a1 = _mm256_add_pd(a1, x1);
				a2 = _mm256_add_pd(a2, x2);
				//a3 = _mm256_add_pd(a3, x3);
				//a4 = _mm256_add_pd(a4, x4);			

				_mm256_store_pd(d + j, a1);
				_mm256_store_pd(d + j + 4, a2);
				//_mm256_store_pd(d + j + 8, a3);
				//_mm256_store_pd(d + j + 12, a4);

				//d[j] = j;
				//c[j] += 2;
				//d[j] += a[j] ;
				//d[j + 1] += a[j + 1];
				//returnValue += a[j + 2];
				//returnValue += a[j + 3];
			}
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
		(double)giga  * 2 / watch.read() / (1000*1000*1000) << "\n\n";
	for (int i = 0; i < arraySize; ++i) {
		returnValue1 += c[i];
	}

	return returnValue1 + returnValue2 + returnValue8 + *((double *)&a1) * *((double *)&a2) *
		*((double *)&a3) * *((double *)&a4) * 
		d[150] ;
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
	size_t returnValue = doAssemblyGigaOp(source, target, arraySize, iteration);
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

void lib_calvin_matrix::mklTest(size_t size) {
	std::cout << "---------- Beginning mklTest -----------\n\n";

	typedef double NumericType;
	using boost::numeric::ublas::matrix;
	lib_calvin::stopwatch watch;

	size_t const size2 = 100;

	matrix<NumericType> a(size, size);
	matrix<NumericType> b(size, size);
	matrix<NumericType> c(size2, size);
	matrix<NumericType> x(size, 1);
	matrix<NumericType> y(1, size);
	for (size_t i = 0; i < size; ++i) {
		for (size_t j = 0; j < size; j++) {
			a(i, j) = 1.2;
			b(i, j) = 1.5;
		}
		x(i, 0) = 1.5;
		y(0, i) = 1.2;
	}

	watch.start();
	matrix<NumericType> result = prod(a, b);
	watch.stop();
	std::cout << "mkl size: " << size << ". " << watch.read() << "  GFLOPS: " <<
		(double)size * size * size * 2 / watch.read() / 1000000000 << "\n";
	for (size_t i = 0; i < size; ++i) {
		for (size_t j = 0; j < size; j++) {
			double residual = result(i, j) - 1.2 * 1.5 * size;
			if (residual*residual > 0.01) {
				std::cout << "MKL is lie\n";
				exit(0);
			}
		}
	}
	watch.start();
	result = prod(c, x);
	watch.stop();
	std::cout << "mkl matrix*vector: " << size << ". " << watch.read() << "  GFLOPS: " <<
		(double)size * size2 * 2 / watch.read() / 1000000000 << "\n";
	watch.start();
	result = prod(y, x);
	watch.stop();
	std::cout << "mkl vector*vector: " << size << ". " << watch.read() << "  GFLOPS: " <<
		(double)size * 2 / watch.read() / 1000000000 << "\n";
	std::cout << "\n";

	lib_calvin::matrix<NumericType> c1(size2, size);
	lib_calvin::matrix<NumericType> x1(size, 1);
	lib_calvin::matrix<NumericType> y1(1, size);
	watch.start();
	auto result1 = c1 * x1;
	watch.stop();
	std::cout << "lib_calvin matrix*vector: " << size << ". " << watch.read() << "  GFLOPS: " <<
		(double)size * size2 * 2 / watch.read() / 1000000000 << "\n";
	watch.start();
	result1.reset(y1 * x1);
	watch.stop();
	std::cout << "lib_calvin vector*vector: " << size << ". " << watch.read() << "  GFLOPS: " <<
		(double)size * 2 / watch.read() / 1000000000 << "\n";
	std::cout << "\n";

	std::cout << "------------- mklTest finished ---------------\n\n";
}