
#include "graph_test.h"

using namespace lib_calvin_graph;

void lib_calvin_graph::graphTest() {
	std::cout << "--------------- Beginning graph test ----------------\n\n";

	GraphTest<intmax_t, intmax_t> test(1000, 10000, 0);
  test.insertionTest();
	test.insertionTest2();
	test.dfsTest();
	test.bfsTest();
	test.algorithmTest();
	test.undirectedAlgorithmTest();
	test.getClosestPathTest();
	std::cout << "-------------- Graph test finished ---------------\n\n\n";
}


