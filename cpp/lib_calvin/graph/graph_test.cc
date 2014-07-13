
#include "graph_test.h"

using namespace lib_calvin_graph;

void lib_calvin_graph::graphTest() {
	std::cout << "\n---------- Beginning graph test -----------\n";
  algorithmTest(500, 80000, 0);
	insertionTest();
	getClosestPathTest();
}

void lib_calvin_graph::algorithmTest(int numV, int numE, int numNegativeE) {
	using namespace lib_calvin;
  graph<int, int> graph1;
  graph<int, struct my> graph2;
  undirected_graph<int, int> graph3;  

  stopwatch watch;
  watch.start();
  for (int i = 0; i < numE; ++i) {
		if (i % 1000000 == 0) {
			std::cout << "now inserted " << i << "edges\n";
		}
    int weight = rand() % 100000 + 1000;
		if (i < numNegativeE ) {
      weight = -weight / 500 + 5; // negative edge
		}
		int srcV = (rand() * 32768 + rand()) % numV;
		int targetV = (rand() * 32768 + rand()) % numV;
    graph1.insert(srcV, targetV, weight);
    graph2.insert(srcV, targetV, weight);
		graph3.insert(srcV, targetV, weight);
  }
  watch.stop();
  cout << "Insertion into graph took: " << watch.read() << endl;
  /*
  graph1.insert (0, 1, 2);
  graph1.insert (1, 2, 2);
  graph1.insert (2, 0, 1);
  graph1.insert (0, 2, 5);
  */
  
	graph1.check();
  graph3.check();
	std::cout << "\n";
}

void lib_calvin_graph::insertionTest() {
	using namespace lib_calvin;
	graph<int, int> *pGraph1, *pGraph2;
	pGraph1 = new graph<int, int>();
	pGraph2 = new undirected_graph<int, int>();
	pGraph1->insert(2, 4, 2);
	pGraph2->insert(2, 4, 2);
	std::cout << "one edge in graph e-size: " << pGraph1->number_of_edge() << "\n";
	std::cout << "one edge in u-graph e-size: " << pGraph2->number_of_edge() << "\n";
	std::cout << "\n";
}

void lib_calvin_graph::getClosestPathTest() {
	using namespace lib_calvin;
	graph<int, struct empty> testGraph;
	testGraph.insert(1, 2);
	testGraph.insert(3, 4);
	testGraph.insert(2, 4);
	testGraph.insert(2, 3);
	graph<int, struct empty>::path path1 = testGraph.getClosestPath(1, 4);
	for (int i = 0; i < path1.size(); ++i) {
		std::cout << path1[i].first << "\n";
	}
	std::cout << "\n";
}