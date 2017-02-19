#ifndef LIB_CALVIN__MATRIX__MATRIX_TEST_H
#define LIB_CALVIN__MATRIX__MATRIX_TEST_H 

#include <stdlib.h>
#include "matrix.h"

namespace lib_calvin_matrix {
	
	void matrixTest();
	double doGigaOps();
	void assemblyTest();
	void mklTest(size_t size);
	extern "C" int doAssemblyGigaOp(double *src, double *target, size_t size, size_t iteration);
}

#endif