#include "matrix.h"

void lib_calvin_matrix::naiveMultiAdd2(double const * __restrict A,
		double const * __restrict B, double * __restrict C,
		size_t lheight, size_t lwidth, size_t rwidth, size_t Aw, size_t Bw) {

	size_t Cw = Bw;
	size_t const loopUnroll = 32;

	//std::cout << "naiveMultiAdd2 template specialization\n";
	__m256d a = _mm256_set_pd(0, 0, 0, 0);

	__m256d b1 = _mm256_set_pd(0, 0, 0, 0);
	__m256d b2 = _mm256_set_pd(0, 0, 0, 0);
	__m256d b3 = _mm256_set_pd(0, 0, 0, 0);
	__m256d b4 = _mm256_set_pd(0, 0, 0, 0);
	__m256d b5 = _mm256_set_pd(0, 0, 0, 0);
	__m256d b6 = _mm256_set_pd(0, 0, 0, 0);
	__m256d b7 = _mm256_set_pd(0, 0, 0, 0);
	__m256d b8 = _mm256_set_pd(0, 0, 0, 0);

	__m256d c1 = _mm256_set_pd(0, 0, 0, 0);
	__m256d c2 = _mm256_set_pd(0, 0, 0, 0);
	__m256d c3 = _mm256_set_pd(0, 0, 0, 0);
	__m256d c4 = _mm256_set_pd(0, 0, 0, 0);
	__m256d c5 = _mm256_set_pd(0, 0, 0, 0);
	__m256d c6 = _mm256_set_pd(0, 0, 0, 0);
	__m256d c7 = _mm256_set_pd(0, 0, 0, 0);
	__m256d c8 = _mm256_set_pd(0, 0, 0, 0);

	for (size_t i = 0; i < lheight; ++i) {		
		size_t k = 0;
		for (; k + loopUnroll - 1 < rwidth; k += loopUnroll) {
			c1 = _mm256_load_pd(C + Cw * i + k + 0);
			c2 = _mm256_load_pd(C + Cw * i + k + 4);
			c3 = _mm256_load_pd(C + Cw * i + k + 8);
			c4 = _mm256_load_pd(C + Cw * i + k + 12);
			c5 = _mm256_load_pd(C + Cw * i + k + 16);
			c6 = _mm256_load_pd(C + Cw * i + k + 20);
			c7 = _mm256_load_pd(C + Cw * i + k + 24);
			c8 = _mm256_load_pd(C + Cw * i + k + 28);

			for (size_t j = 0; j < lwidth; ++j) {
				/*
				C[Cw * i + (k + 0)] += A[Aw * i + j] * B[Bw * j + (k + 0)];
				C[Cw * i + (k + 1)] += A[Aw * i + j] * B[Bw * j + (k + 1)];
				C[Cw * i + (k + 2)] += A[Aw * i + j] * B[Bw * j + (k + 2)];
				C[Cw * i + (k + 3)] += A[Aw * i + j] * B[Bw * j + (k + 3)];
				*/			
				a = _mm256_broadcast_sd(A + Aw * i + j);

				b1 = _mm256_load_pd(B + Bw * j + k + 0);
				b2 = _mm256_load_pd(B + Bw * j + k + 4);
				b3 = _mm256_load_pd(B + Bw * j + k + 8);
				b4 = _mm256_load_pd(B + Bw * j + k + 12);
				b5 = _mm256_load_pd(B + Bw * j + k + 16);
				b6 = _mm256_load_pd(B + Bw * j + k + 20);
				b7 = _mm256_load_pd(B + Bw * j + k + 24);
				b8 = _mm256_load_pd(B + Bw * j + k + 28);

				c1 = _mm256_fmadd_pd(a, b1, c1);
				c2 = _mm256_fmadd_pd(a, b2, c2);
				c3 = _mm256_fmadd_pd(a, b3, c3);
				c4 = _mm256_fmadd_pd(a, b4, c4);
				c5 = _mm256_fmadd_pd(a, b5, c5);
				c6 = _mm256_fmadd_pd(a, b6, c6);
				c7 = _mm256_fmadd_pd(a, b7, c7);
				c8 = _mm256_fmadd_pd(a, b8, c8);
			}
			/*
			C[Cw * i + k + 0] = ((double *)&c1)[0];
			C[Cw * i + k + 1] = ((double *)&c1)[1];
			C[Cw * i + k + 2] = ((double *)&c1)[2];
			C[Cw * i + k + 3] = ((double *)&c1)[3];

			C[Cw * i + k + 4] = ((double *)&c2)[0];
			C[Cw * i + k + 5] = ((double *)&c2)[1];
			C[Cw * i + k + 6] = ((double *)&c2)[2];
			C[Cw * i + k + 7] = ((double *)&c2)[3];

			C[Cw * i + k + 8] = ((double *)&c3)[0];
			C[Cw * i + k + 9] = ((double *)&c3)[1];
			C[Cw * i + k + 10] = ((double *)&c3)[2];
			C[Cw * i + k + 11] = ((double *)&c3)[3];

			C[Cw * i + k + 12] = ((double *)&c4)[0];
			C[Cw * i + k + 13] = ((double *)&c4)[1];
			C[Cw * i + k + 14] = ((double *)&c4)[2];
			C[Cw * i + k + 15] = ((double *)&c4)[3];

			C[Cw * i + k + 16] = ((double *)&c5)[0];
			C[Cw * i + k + 17] = ((double *)&c5)[1];
			C[Cw * i + k + 18] = ((double *)&c5)[2];
			C[Cw * i + k + 19] = ((double *)&c5)[3];

			C[Cw * i + k + 20] = ((double *)&c6)[0];
			C[Cw * i + k + 21] = ((double *)&c6)[1];
			C[Cw * i + k + 22] = ((double *)&c6)[2];
			C[Cw * i + k + 23] = ((double *)&c6)[3];
			*/
			_mm256_store_pd(C + Cw * i + k, c1);
			_mm256_store_pd(C + Cw * i + k + 4, c2);
			_mm256_store_pd(C + Cw * i + k + 8, c3);
			_mm256_store_pd(C + Cw * i + k + 12, c4);
			_mm256_store_pd(C + Cw * i + k + 16, c5);
			_mm256_store_pd(C + Cw * i + k + 20, c6);
			_mm256_store_pd(C + Cw * i + k + 24, c7);
			_mm256_store_pd(C + Cw * i + k + 28, c8);
		}
		for (; k < rwidth; ++k) {
			for (size_t j = 0; j < lwidth; ++j) {
				C[Cw * i + k] += (A[Aw * i + j] * B[Bw * j + k]);
			}
		}
	}
}