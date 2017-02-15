#include "matrix.h"

template <>
void lib_calvin_matrix::naiveMultiAdd2(double const * __restrict A,
	double const * __restrict B, double * __restrict C,
	size_t lheight, size_t lwidth, size_t rwidth, size_t Aw, size_t Bw) {
	size_t Cw = Bw;
	size_t const loopUnroll = 8;
	//std::cout << "Fuck\n";
	__m256d a = _mm256_set_pd(0, 0, 0, 0);
	__m256d b1 = _mm256_set_pd(0, 0, 0, 0);
	__m256d b2 = _mm256_set_pd(0, 0, 0, 0);
	__m256d c1 = _mm256_set_pd(0, 0, 0, 0);
	__m256d c2 = _mm256_set_pd(0, 0, 0, 0);

	for (size_t i = 0; i < lheight; ++i) {		
		size_t k = 0;
		for (; k + loopUnroll - 1 < rwidth; k += loopUnroll) {
			c1 = _mm256_load_pd(C + Cw * i + k + 0);
			c2 = _mm256_load_pd(C + Cw * i + k + 4);

			for (size_t j = 0; j < lwidth; ++j) {
				/*
				C[Cw * i + (k + 0)] += A[Aw * i + j] * B[Bw * j + (k + 0)];
				C[Cw * i + (k + 1)] += A[Aw * i + j] * B[Bw * j + (k + 1)];
				C[Cw * i + (k + 2)] += A[Aw * i + j] * B[Bw * j + (k + 2)];
				C[Cw * i + (k + 3)] += A[Aw * i + j] * B[Bw * j + (k + 3)];
				*/
				
				double temp = A[Aw * i + j];
				a = _mm256_set_pd(temp, temp, temp, temp);

				b1 = _mm256_load_pd(B + Bw * j + k + 0);
				b2 = _mm256_load_pd(B + Bw * j + k + 4);

				b1 = _mm256_mul_pd(b1, a);
				b2 = _mm256_mul_pd(b2, a);				

				c1 = _mm256_add_pd(c1, b1);
				c2 = _mm256_add_pd(c2, b2);								
			}
			C[Cw * i + k + 0] = ((double *)&c1)[0];
			C[Cw * i + k + 1] = ((double *)&c1)[1];
			C[Cw * i + k + 2] = ((double *)&c1)[2];
			C[Cw * i + k + 3] = ((double *)&c1)[3];
			C[Cw * i + k + 4] = ((double *)&c2)[0];
			C[Cw * i + k + 5] = ((double *)&c2)[1];
			C[Cw * i + k + 6] = ((double *)&c2)[2];
			C[Cw * i + k + 7] = ((double *)&c2)[3];
			//_mm256_store_pd(C + Cw * i + k, c1);
			//_mm256_store_pd(C + Cw * i + k + 4, c2);
		}
		for (; k < rwidth; ++k) {
			for (size_t j = 0; j < lwidth; ++j) {
				C[Cw * i + k] += (A[Aw * i + j] * B[Bw * j + k]);
			}
		}
	}
}