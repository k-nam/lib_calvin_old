#ifndef LIB_CALVIN__MATRIX__MATRIX_TEST_H
#define LIB_CALVIN__MATRIX__MATRIX_TEST_H 

#include "matrix.h"

namespace lib_calvin_matrix {	
	void matrixTest();
	void mklTest(size_t size);
	double doGigaOps();		
	void assemblyTest();
	extern "C" int doAssembly(double *src, double *target, size_t size, size_t iteration);
	void cudaTest();
}

#endif