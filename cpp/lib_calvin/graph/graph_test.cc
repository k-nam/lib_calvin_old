
#include "graph_test.h"

using namespace lib_calvin_graph;

void lib_calvin_graph::graphTest() {
	std::cout << "\n---------- Beginning graph test -----------\n";
	GraphTest<int, int> test(1000, 10000, 0);
  test.insertionTest();
	test.insertionTest2();
	test.dfsTest();
	test.bfsTest();
	test.algorithmTest();
	test.undirectedAlgorithmTest();
	test.getClosestPathTest();
}


