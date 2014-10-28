
#include "graph_test.h"

using namespace lib_calvin_graph;

void lib_calvin_graph::graphTest() {
	std::cout << "\n---------- Beginning graph test -----------\n";
	GraphTest<int, int> test;
  test.insertionTest(1000, 10000, 0);
	test.insertionTest2();
	test.dfsTest(1000, 10000);
	test.bfsTest(1000, 10000);
	test.algorithmTest(1000, 10000);
	test.undirectedAlgorithmTest(1000, 10000);
	test.getClosestPathTest();
}


