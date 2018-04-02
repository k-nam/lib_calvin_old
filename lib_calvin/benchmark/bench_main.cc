#include "sorting_bench.h"
#include "container_bench.h"
#include "matrix_bench.h"

int main(int argc, char* argv[])
{
	lib_calvin_benchmark::sorting::sortBench();
	lib_calvin_benchmark::container::containerBench();
	lib_calvin_benchmark::matrix::matrixBench();
	return 0;
}
