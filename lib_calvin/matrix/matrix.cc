#include "matrix.h"
#include <immintrin.h>
#include "mkl/include/mkl_boost_ublas_matrix_prod.hpp"

#ifdef _WIN64 
#define MKL_ILP64
#endif

void lib_calvin_matrix::naiveMultiAdd2Impl(double const * __restrict A,
									   double const * __restrict B, double * __restrict C,
									   size_t lheight, size_t lwidth, size_t rwidth, size_t Aw, size_t Bw) {

	if (reinterpret_cast<ptrdiff_t>(C) % (sizeof(double) * 4) != 0 ||
		reinterpret_cast<ptrdiff_t>(B) % (sizeof(double) * 4) != 0 || Bw % 4 != 0) {
		simpleMultiAddImpl(A, B, C, lheight, lwidth, rwidth, Aw, Bw);
		return;
	}

	size_t Cw = Bw;	

	__m256d a = _mm256_set_pd(0, 0, 0, 0);

	__m256d b1 = _mm256_set_pd(0, 0, 0, 0);
	__m256d b2 = _mm256_set_pd(0, 0, 0, 0);
	__m256d b3 = _mm256_set_pd(0, 0, 0, 0);
	__m256d b4 = _mm256_set_pd(0, 0, 0, 0);
	__m256d b5 = _mm256_set_pd(0, 0, 0, 0);
	__m256d b6 = _mm256_set_pd(0, 0, 0, 0);
	__m256d b7 = _mm256_set_pd(0, 0, 0, 0);
	__m256d b8 = _mm256_set_pd(0, 0, 0, 0);
	__m256d b9 = _mm256_set_pd(0, 0, 0, 0);
	__m256d b10 = _mm256_set_pd(0, 0, 0, 0);

	__m256d c1 = _mm256_set_pd(0, 0, 0, 0);
	__m256d c2 = _mm256_set_pd(0, 0, 0, 0);
	__m256d c3 = _mm256_set_pd(0, 0, 0, 0);
	__m256d c4 = _mm256_set_pd(0, 0, 0, 0);
	__m256d c5 = _mm256_set_pd(0, 0, 0, 0);
	__m256d c6 = _mm256_set_pd(0, 0, 0, 0);
	__m256d c7 = _mm256_set_pd(0, 0, 0, 0);
	__m256d c8 = _mm256_set_pd(0, 0, 0, 0);
	__m256d c9 = _mm256_set_pd(0, 0, 0, 0);
	__m256d c10 = _mm256_set_pd(0, 0, 0, 0);

	size_t k = 0;
	size_t const loopUnroll = 40;

	for (; k + loopUnroll - 1 < rwidth; k += loopUnroll) {
		for (size_t i = 0; i < lheight; ++i) {
			c1 = _mm256_load_pd(C + Cw * i + k + 0);
			c2 = _mm256_load_pd(C + Cw * i + k + 4);
			c3 = _mm256_load_pd(C + Cw * i + k + 8);
			c4 = _mm256_load_pd(C + Cw * i + k + 12);
			c5 = _mm256_load_pd(C + Cw * i + k + 16);
			c6 = _mm256_load_pd(C + Cw * i + k + 20);
			c7 = _mm256_load_pd(C + Cw * i + k + 24);
			c8 = _mm256_load_pd(C + Cw * i + k + 28);
			c9 = _mm256_load_pd(C + Cw * i + k + 32);
			c10 = _mm256_load_pd(C + Cw * i + k + 36);

			for (size_t j = 0; j < lwidth; ++j) {	
				a = _mm256_broadcast_sd(A + Aw * i + j);

				b1 = _mm256_load_pd(B + Bw * j + k + 0);
				b2 = _mm256_load_pd(B + Bw * j + k + 4);
				b3 = _mm256_load_pd(B + Bw * j + k + 8);
				b4 = _mm256_load_pd(B + Bw * j + k + 12);
				b5 = _mm256_load_pd(B + Bw * j + k + 16);
				b6 = _mm256_load_pd(B + Bw * j + k + 20);
				b7 = _mm256_load_pd(B + Bw * j + k + 24);
				b8 = _mm256_load_pd(B + Bw * j + k + 28);
				b9 = _mm256_load_pd(B + Bw * j + k + 32);
				b10 = _mm256_load_pd(B + Bw * j + k + 36);

				c1 = _mm256_fmadd_pd(a, b1, c1);
				c2 = _mm256_fmadd_pd(a, b2, c2);
				c3 = _mm256_fmadd_pd(a, b3, c3);
				c4 = _mm256_fmadd_pd(a, b4, c4);
				c5 = _mm256_fmadd_pd(a, b5, c5);
				c6 = _mm256_fmadd_pd(a, b6, c6);
				c7 = _mm256_fmadd_pd(a, b7, c7);
				c8 = _mm256_fmadd_pd(a, b8, c8);
				c9 = _mm256_fmadd_pd(a, b9, c9);
				c10 = _mm256_fmadd_pd(a, b10, c10);
			}
			
			_mm256_store_pd(C + Cw * i + k, c1);
			_mm256_store_pd(C + Cw * i + k + 4, c2);
			_mm256_store_pd(C + Cw * i + k + 8, c3);
			_mm256_store_pd(C + Cw * i + k + 12, c4);
			_mm256_store_pd(C + Cw * i + k + 16, c5);
			_mm256_store_pd(C + Cw * i + k + 20, c6);
			_mm256_store_pd(C + Cw * i + k + 24, c7);
			_mm256_store_pd(C + Cw * i + k + 28, c8);
			_mm256_store_pd(C + Cw * i + k + 32, c9);
			_mm256_store_pd(C + Cw * i + k + 36, c10);
		}
	}
	
	size_t loopUnroll2 = 8;

	for (; k + loopUnroll2 - 1 < rwidth; k += loopUnroll2) {
		for (size_t i = 0; i < lheight; ++i) {
			c1 = _mm256_load_pd(C + Cw * i + k + 0);
			c2 = _mm256_load_pd(C + Cw * i + k + 4);

			for (size_t j = 0; j < lwidth; ++j) {
				a = _mm256_broadcast_sd(A + Aw * i + j);

				b1 = _mm256_load_pd(B + Bw * j + k + 0);
				b2 = _mm256_load_pd(B + Bw * j + k + 4);

				c1 = _mm256_fmadd_pd(a, b1, c1);
				c2 = _mm256_fmadd_pd(a, b2, c2);
			}

			_mm256_store_pd(C + Cw * i + k, c1);
			_mm256_store_pd(C + Cw * i + k + 4, c2);
		}
	}

	for (; k < rwidth; k++) {
		for (size_t i = 0; i < lheight; ++i) {
			for (size_t j = 0; j < lwidth; ++j) {
				C[Cw * i + k] += (A[Aw * i + j] * B[Bw * j + k]);
			}
		}
	}

}

void lib_calvin_matrix::mklMultiAdd(
	matrix<double> const &A, matrix<double> const &B, matrix<double> &C) {
	using boost::numeric::ublas::matrix;
	using boost::numeric::ublas::vector;
	lib_calvin::stopwatch watch;

	boost::numeric::ublas::matrix<double> a(A.height(), A.width());
	boost::numeric::ublas::matrix<double> b(B.height(), B.width());
	boost::numeric::ublas::matrix<double> result(C.height(), C.width());

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

	watch.start();
	result = prod(a, b);
	watch.stop();
	
	for (size_t i = 0; i < C.height(); ++i) {
		for (size_t j = 0; j < C.width(); j++) {
			C.set_val(i, j, C.getval(i, j) + result(i, j));
		}
	}
}