#ifndef LIB_CALVIN__GRAPH__GRAPH_TEST_H
#define LIB_CALVIN__GRAPH__GRAPH_TEST_H
/*
 * 2008-02-04
 */

// Include additional test to see difference of array and dynamic(set)
// graph data in bfs
//#define BFS_GRAPH_DATA_STRUCTURE

#include "graph.h"
#include "stopwatch.h"

namespace lib_calvin_graph {
	void graphTest();

	template <typename V, typename E, typename W = E, typename ExtractWeight = std::identity<E>>
	class GraphTest {
	public:
		void insertionTest(int numV, int numE, int numNegativeE);
		void insertionTest2();
		void algorithmTest(int numV, int numE);
		void undirectedAlgorithmTest(int numV, int numE);
		void dfsTest(int numV, int numE);
		void bfsTest(int numV, int numE);
		void getClosestPathTest();	
		
	private:
		void populateGraph(graph_base<V, E, W, ExtractWeight> &graph, int numV, int numE, int numNegativeE);
		void populateGraph(graph_base<V, E, W, ExtractWeight> &graph, int numV, int numE, int numNegativeE,
											 E const &defaultEdge);
	};

	struct SampleEdge {
		SampleEdge(int weight = 0): weight_(weight) { }
		int getWeight() const {
			return weight_; }
		int weight_;
	};
	struct SampleEdgeWeight {
		int operator()(SampleEdge sampleEdge) {
			return sampleEdge.getWeight();}
	};

	struct my {
		my(): value_(0) { }  
		my(int value): value_(value) { }
		operator int() { return value_; }
		bool operator== (const struct my &rhs) const {
			if (value_ == rhs.value_) return true;
			return false;
		}
		bool operator!= (struct my const &rhs) const {
			return !(operator==(rhs));
		}
		bool operator< (struct my const &rhs) const {
			if (value_ < rhs.value_) return true;
			return false;
		}
		bool operator<= (struct my const &rhs) const {
			if (value_ <= rhs.value_) return true;
			return false;
		}
		bool operator> (struct my const &rhs) const {
			if (value_ > rhs.value_) return true;
			return false;
		}
		struct my & operator= (struct my const &rhs) {
			value_ = rhs.value_;
			return *this;
		}
		struct my & operator+= (struct my const &rhs) {
			value_ += rhs.value_;
			return *this;
		}
		struct my & operator-= (struct my const &rhs) {
			value_ -= rhs.value_;
			return *this;
		}
		struct my const operator+ (struct my const &rhs) const {
			return my(value_ + rhs.value_);
		}
		int value_;
		int dummy1;
		//int dummy2;
		//int dummy3;
		//double dummy4;
	};
} // end namespace lib_calvin_graph

template <typename V, typename E, typename W, typename ExtractWeight>
void lib_calvin_graph::GraphTest<V, E, W, ExtractWeight>::insertionTest(int numV, int numE, int numNegativeE) {
	using namespace lib_calvin;
  graph<V, E> graph1;
  undirected_graph<V, E> graph2; 
  stopwatch watch;
  watch.start();
	populateGraph(graph1, numV, numE, numNegativeE);
	populateGraph(graph2, numV, numE, numNegativeE);
  watch.stop();
  cout << "Insertion into graph took: " << watch.read() << endl;
  /*
  graph1.insert (0, 1, 2);
  graph1.insert (1, 2, 2);
  graph1.insert (2, 0, 1);
  graph1.insert (0, 2, 5);
  */
	std::cout << "\n";
}

template <typename V, typename E, typename W, typename ExtractWeight>
void lib_calvin_graph::GraphTest<V, E, W, ExtractWeight>::insertionTest2() {
	using namespace lib_calvin;
	graph_base<V, E> *graph1, *graph2;
	graph1 = new graph<V, E>();
	graph2 = new undirected_graph<V, E>();
	graph1->insert(2, 4);
	graph1->insert(2, 3);
	graph1->modify(4, 2);
	graph1->remove(3, 2);
	graph2->insert(2, 4);
	graph2->insert(2, 3);
	graph2->modify(4, 2);
	graph2->remove(3, 2);
	std::cout << "# edges in graph e-size: " << graph1->number_of_edge() << "\n";
	std::cout << "# edges in undirected graph e-size: " << graph2->number_of_edge() << "\n";
	std::cout << "\n";
}


template <typename V, typename E, typename W, typename ExtractWeight>
void lib_calvin_graph::GraphTest<V, E, W, ExtractWeight>::dfsTest(int numV, int numE) {
	using namespace lib_calvin;
	graph<V, E> graph;
	populateGraph(graph, numV, numE, 0);
	graph.goStatic();
  vector<vector<V>> arrayDataWithoutEdge;
	ripEdge(graph.arrayData_, arrayDataWithoutEdge);
  vector<int> visitOrder(numV);
  for (int i = 0; i < numV / 3; ++i) {
    visitOrder[i] = i;
  }
  vector<int> returnOrder;
  vector<int> returnOrder2;

  // dfs with array data
	stopwatch watch;
  watch.start();
	dfs1(arrayDataWithoutEdge, visitOrder, returnOrder);
  watch.stop();
  cout << "dfs1 of vertices: " << numV << "\tedges: " << numE << 
    "\t" << watch.read() << endl;

  watch.start();
	dfs2(arrayDataWithoutEdge, visitOrder, returnOrder2);
  watch.stop();
  cout << "dfs2 of vertices: " << numV << "\tedges: " << numE << 
    "\t" << watch.read() << endl;

	if (returnOrder != returnOrder2) {
		cout << "dfs algorithms does not match\n";
		exit(0);
	}
}

template <typename V, typename E, typename W, typename ExtractWeight>
void lib_calvin_graph::GraphTest<V, E, W, ExtractWeight>::bfsTest(int numV, int numE) {
	using namespace lib_calvin;
	graph<V, int> graph;
	populateGraph(graph, numV, numE, 0, 1);
  graph.goStatic();
  vector<Arc<int>> result1, result2;
  vector<vector<int>> arrayDataWithoutEdge;
  ripEdge(graph.arrayData_, arrayDataWithoutEdge);
	stopwatch watch;
  watch.start();
  bfs(arrayDataWithoutEdge, 0, result1);
  watch.stop();
  cout << "bfs of vertices: " << numV << "\tedges: " << numE << "\t" <<
    watch.read() << endl;
	
  dijkstra(graph.arrayData_, 0, result2);
  if (result1.size() != result2.size()) {
    cout << "bfs error\n";
    exit(0);
  }
  for (unsigned i = 0; i < result1.size(); ++i) {
    if (result1[i].weight_ != result2[i].weight_) {
      cout << "bfs error\n";
      exit(0);
    }
  }
  cout << "bfs correct\n";
}

template <typename V, typename E, typename W, typename ExtractWeight>
void lib_calvin_graph::GraphTest<V, E, W, ExtractWeight>::algorithmTest(int numV, int numE) {
	using namespace lib_calvin;
	graph<V, E, W, ExtractWeight> graph;
	populateGraph(graph, numV, numE, 0);
	graph.goStatic();
	// Additional tests for weighted graph: shortetst paths problems
	// dijkstra with array data
	vector<Arc<W>> solution;
	stopwatch watch;
  watch.start();
  dijkstra<E>(graph.arrayData_, 0, solution);
  watch.stop();
	if (shortestPathCheck<E>(graph.arrayData_, 0, solution) == false) {
    cout << "dijkstra error.\n";
		exit(0);
	}
  cout << "dijkstra of vertices: " << numV << "\tedges: " << numE << 
    "\t" << watch.read() << endl;

  // Vellman-Ford
  vector<Arc<W>> solution_v;
  for (int i = numV - 1; i < numV; ++i) {
    watch.start();
    vellmanFord (graph.arrayData_, i, solution_v);
    watch.stop();
		if (shortestPathCheck (graph.arrayData_, i, solution_v) == false) {
      cout << "vellmanFord error.\n";
			exit(0);
		}
  }
  cout << "Vellman-Ford of vertices: " << numV << "\tedges: " << numE << 
    "\t" << watch.read() << endl;
	
  vector<Arc<W>> row;
  // matrixApsp
  matrix<Arc<W>> apspResult(1);
  watch.start();
  matrixApsp(graph.matrixData_, apspResult);
  watch.stop();
  cout << "matrixApsp of vertices: " << numV << "\tedges: " << numE << 
    "\t" << watch.read() << endl;

  // Floyd-Earshall
  matrix<Arc<W>> floydResult(1);
  watch.start();
  floydWarshall (graph.matrixData_, floydResult);
  watch.stop();
  cout << "floydWarshall of vertices: " << numV << "\tedges: " << numE << 
    "\t" << watch.read() << endl;

  // johnson's
  matrix<Arc<W>> johnsonResult(1);
  watch.start();
  johnson(graph.arrayData_, johnsonResult);
  watch.stop();
  cout << "johnson of vertices: " << numV << "\tedges: " << numE << 
    "\t" << watch.read() << endl;

  bool isApspCorrect = true;
	bool isFloydCorrect = true;
	bool isJohnsonCorrect = true;
  for (int i = 0; i < numV; ++i) {
    vellmanFord(graph.arrayData_, i, row); 
    for (int j = 0; j < numV; j++) {
      if (apspResult.getval(i, j).weight_ != row[j].weight_ && 
          row[j].predecessor_!= -1) {
        isApspCorrect = false;
      }
      if (floydResult.getval(i, j).weight_ != row[j].weight_ && 
          row[j].predecessor_!= -1) {
        isFloydCorrect = false;
      }
      if (johnsonResult.getval(i, j).weight_ != row[j].weight_ && 
          row[j].predecessor_!= -1) {
        isJohnsonCorrect = false;
      }
    }
  }
	if (isApspCorrect == false) {
    cout << "Apsp error (to vellmanFord)\n";
		exit(0);
	}
	if (isFloydCorrect == false) {
    cout << "FLOYD error (to vellmanFord)\n";
		exit(0);
	}
	if (isJohnsonCorrect == false) {
    cout << "JOHNSON error (to vellmanFord)\n";
		exit(0);
	}
	std::cout << "\n";
}

template <typename V, typename E, typename W, typename ExtractWeight>
void lib_calvin_graph::GraphTest<V, E, W, ExtractWeight>::undirectedAlgorithmTest(int numV, int numE) {
	// Additional test only for symmetric graph
  // MST algorithms
	using namespace lib_calvin;
	undirected_graph<V, E, W, ExtractWeight> graph;
	populateGraph(graph, numV, numE, 0);
	graph.goStatic();
	set<std::pair<int, int>> result1, result2;  
	stopwatch watch;
  watch.start();
	lib_calvin_graph::kruskal(graph.arrayData_, result1);
  watch.stop();
  cout << "kruskal of vertices: " << numV << "\tedges: " << numE << 
    "\t" << watch.read() << endl;

  watch.start();
	lib_calvin_graph::prim(graph.arrayData_, result2);
  watch.stop();
  cout << "prim of vertices: " << numV << "\tedges: " << numE << 
    "\t" << watch.read() << endl;

  if (result1 != result2) {
		set<std::pair<int, int>>::iterator iter;
    cout << "MST algorithms does not match each other: size of kruskal: " << result1.size() << 
			" size of prim " << result2.size();
    /*
    cout << "kruskal result\n";
    for (iter = result1.begin(); iter != result1.end(); ++iter) {
      cout << iter->first << "  " << iter->second << endl;
    }
    cout << "\nPrim result\n";
    for (iter = result2.begin(); iter != result2.end(); ++iter) {
      cout << iter->first << "  " << iter->second << endl;
    }*/
  }
}

template <typename V, typename E, typename W, typename ExtractWeight>
void lib_calvin_graph::GraphTest<V, E, W, ExtractWeight>::getClosestPathTest() {
	using namespace lib_calvin;
	graph<V, E, W, ExtractWeight> testGraph;
	testGraph.insert(1, 2, 2);
	testGraph.insert(1, 4, 5);
	testGraph.insert(2, 4, 2);
	testGraph.insert(2, 3, 2);
	testGraph.insert(4, 5, 2);
	auto path1 = testGraph.get_closest_path(1, 5);
	auto path2 = testGraph.get_shortest_path(1, 5);
	std::cout << "Closest path result\n";
	for (size_t i = 0; i <= path1.size(); ++i) {
		std::cout << path1.get_vertex(i) << "\n";
	}
	std::cout << "Shortest path result\n";
	for (size_t i = 0; i <= path2.size(); ++i) {
		std::cout << path2.get_vertex(i) << "\n";
	}
	std::cout << "\n";
}

template <typename V, typename E, typename W, typename ExtractWeight>
void lib_calvin_graph::GraphTest<V, E, W, ExtractWeight>::populateGraph(
			graph_base<V, E, W, ExtractWeight> &graph, int numV, int numE, int numNegativeE) {
	using namespace lib_calvin;
  for (int i = 0; i < numE; ++i) {
    int edge = rand() % 100000 + 1000;
		if (i < numNegativeE ) {
      edge = -edge / 500 + 5; // negative edge
		}
		int srcV = (rand() * 32768 + rand()) % numV;
		int targetV = (rand() * 32768 + rand()) % numV;
    graph.insert(srcV, targetV, edge);
  }
}

template <typename V, typename E, typename W, typename ExtractWeight>
void lib_calvin_graph::GraphTest<V, E, W, ExtractWeight>::populateGraph(
			graph_base<V, E, W, ExtractWeight> &graph, 
			int numV, int numE, int numNegativeE, E const &defaultEdge) {
  for (int i = 0; i < numE; ++i) {
		int srcV = (rand() * 32768 + rand()) % numV;
		int targetV = (rand() * 32768 + rand()) % numV;
    graph.insert(srcV, targetV, defaultEdge);
  }
}

#endif