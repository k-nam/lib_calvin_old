#include "sorting_bench.h"
#include "container_bench.h"
#include "matrix_bench.h"
#include "string_bench.h"
#include "graph_bench.h"
#include "etc_bench.h"

int main(int argc, char* argv[])
{
	//lib_calvin_benchmark::sorting::sortBench();
	lib_calvin_benchmark::container::containerBench();
	//lib_calvin_benchmark::matrix::matrixBench();
	//lib_calvin_benchmark::string::stringBench();
	//lib_calvin_benchmark::graph::graphBench();
	// lib_calvin_benchmark::etc::etc_bench();
	return 0;
}
