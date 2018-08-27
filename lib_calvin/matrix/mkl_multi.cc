#include "matrix.h"

// This line is important
// As I link 64bit binary files, I must define MKL_ILP64 before including header
#ifdef _WIN64 
#define MKL_ILP64
#endif
#include "mkl/include/mkl.h"

void lib_calvin_matrix::mklMultiAdd(
	matrix<double> const &A, matrix<double> const &B, matrix<double> &C) {
	// C <- alpha*A*B + beta*C
	double alpha = 1;
	double beta = 1;
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
				A.height(), B.width(), A.width(),
				alpha, A.elements_, A.width(), B.elements_, B.width(),
				beta, C.elements_, C.width());
}


void lib_calvin_matrix::mklImpl(double const *A, double const *B, double *C,
								size_t lheight, size_t lwidth, size_t rwidth, size_t Aw, size_t Bw) {

	double alpha = 1;
	double beta = 1;
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
				lheight, rwidth, lwidth,
				alpha, A, Aw, B, Bw,
				beta, C, Bw);
}


void lib_calvin_matrix::mklMultiAdd2(
	matrix<double> const &A, matrix<double> const &B, matrix<double> &C) {

	double *a = (double *)mkl_malloc(A.height() * A.width() * sizeof(double), 64);
	double *b = (double *)mkl_malloc(B.height() * B.width() * sizeof(double), 64);
	double *c = (double *)mkl_malloc(C.height() * C.width() * sizeof(double), 64);

	// C <- alpha*A*B + beta*C
	double alpha = 1;
	double beta = 1;

	lib_calvin::stopwatch watch;
	watch.start();
	for (size_t i = 0; i < A.height(); i++) {
		for (size_t j = 0; j < A.width(); j++) {
			a[i*A.width() + j] = A.getval(i, j);
			//std::cout << a[i*A.width() + j] << "\n";
		}
	}
	for (size_t i = 0; i < B.height(); i++) {
		for (size_t j = 0; j < B.width(); j++) {
			b[i*B.width() + j] = B.getval(i, j);
		}
	}
	for (size_t i = 0; i < C.height(); i++) {
		for (size_t j = 0; j < C.width(); j++) {
			c[i*C.width() + j] = C.getval(i, j);
		}
	}
	watch.stop();
	std::cout << "MKL internal 3: " << watch.read() << "\n";


	watch.start();
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
				A.height(), B.width(), A.width(),
				alpha, a, A.width(), b, B.width(),
				beta, c, C.width());

	watch.stop();
	std::cout << "MKL real time: " << watch.read() << "\n";

	watch.start();
	for (size_t i = 0; i < C.height(); i++) {
		for (size_t j = 0; j < C.width(); j++) {
			C.setval(i, j) = c[i*C.width() + j];
		}
	}
	watch.stop();
	std::cout << "MKL internal 4: " << watch.read() << "\n";

	mkl_free(a);
	mkl_free(b);
	mkl_free(c);
}