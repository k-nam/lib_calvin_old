#include "matrix.h"
#include "mkl/include/mkl_boost_ublas_matrix_prod.hpp"

#ifdef _WIN64 
#define MKL_ILP64
#endif

void lib_calvin_matrix::mklMultiAdd(
	matrix<double> const &A, matrix<double> const &B, matrix<double> &C) {
	using boost::numeric::ublas::matrix;
	using boost::numeric::ublas::vector;
	lib_calvin::stopwatch watch;

	boost::numeric::ublas::matrix<double> a(A.height(), A.width());
	boost::numeric::ublas::matrix<double> b(B.height(), B.width());
	boost::numeric::ublas::matrix<double> result(C.height(), C.width());

	/*
	// Copying lib_calvin matrix takes only 1/3 time as MKL
	watch.start();
	lib_calvin_matrix::matrix<double> tempA(A.height(), A.width());
	lib_calvin_matrix::matrix<double> tempB(B.height(), B.width());
	for (size_t i = 0; i < A.height(); ++i) {
	for (size_t j = 0; j < A.width(); j++) {
	tempA.setval(i, j) = A.getval(i, j);
	}
	}
	for (size_t i = 0; i < B.height(); ++i) {
	for (size_t j = 0; j < B.width(); j++) {
	tempB.setval(i, j) = B.getval(i, j);
	}
	}
	watch.stop();
	std::cout << "MKL internal 2: " << watch.read() << "\n";
	*/

	watch.start();
	for (size_t i = 0; i < A.height(); ++i) {
		for (size_t j = 0; j < A.width(); j++) {
			a(i, j) = A.getval(i, j);
		}
	}
	for (size_t i = 0; i < B.height(); ++i) {
		for (size_t j = 0; j < B.width(); j++) {
			b(i, j) = B.getval(i, j);
		}
	}
	watch.stop();
	//std::cout << "MKL internal 3: " << watch.read() << "\n";


	watch.start();
	result = prod(a, b);
	watch.stop();
	//std::cout << "MKL real time: " << watch.read() << "\n";

	watch.start();
	for (size_t i = 0; i < C.height(); ++i) {
		for (size_t j = 0; j < C.width(); j++) {
			C.setval(i, j) = C.getval(i, j) + result(i, j);
		}
	}
	watch.stop();
	//std::cout << "MKL internal 4: " << watch.read() << "\n";
}