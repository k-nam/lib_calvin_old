#ifndef LIB_CALVIN__GRAPH__GRAPH_TEST_H
#define LIB_CALVIN__GRAPH__GRAPH_TEST_H
/*
* 2008-02-04
*/

#include "graph.h"
#include "stopwatch.h"

namespace lib_calvin_graph {
void graphTest();

template <typename V, typename E>
class GraphTest {
public:
	GraphTest(size_t numV, size_t numE, size_t numNegativeE);
	void insertionTest();
	void insertionTest2();
	void algorithmTest();
	void undirectedAlgorithmTest();
	void dfsTest();
	void bfsTest();
	void getClosestPathTest();

private:
	size_t numV_;
	size_t numE_;
	size_t numNegativeE_;
};

template <typename V, typename E>
void populateGraph(graph_base<V, E> &graph, size_t numV, size_t numE, size_t numNegativeE);

template <typename V, typename E>
void populateGraph(graph_base<V, E> &graph, size_t numV, size_t numE, size_t numNegativeE,
	E const &defaultEdge);

struct SampleEdge {
	SampleEdge(size_t weight = 0) : weight_(weight) { }
	size_t getWeight() const {
		return weight_;
	}
	size_t weight_;
};
struct SampleEdgeWeight {
	size_t operator()(SampleEdge sampleEdge) {
		return sampleEdge.getWeight();
	}
};

struct my {
	my() : value_(0) { }
	my(int value) : value_(value) { }
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
	//size_t dummy2;
	//size_t dummy3;
	//double dummy4;
};
} // end namespace lib_calvin_graph

template <typename V, typename E>
lib_calvin_graph::GraphTest<V, E>::GraphTest(size_t numV, size_t numE, size_t numNegativeE) :
	numV_(numV), numE_(numE), numNegativeE_(numNegativeE_) { }

template <typename V, typename E>
void lib_calvin_graph::GraphTest<V, E>::insertionTest() {
	using namespace lib_calvin;
	graph<V, E> graph1;
	undirected_graph<V, E> graph2;
	weighted_graph<V, E> graph3;
	undirected_weighted_graph<V, E> graph4;
	stopwatch watch;
	watch.start();
	populateGraph(graph1, numV_, numE_, numNegativeE_);
	populateGraph(graph2, numV_, numE_, numNegativeE_);
	populateGraph(graph3, numV_, numE_, numNegativeE_);
	populateGraph(graph4, numV_, numE_, numNegativeE_);
	watch.stop();
	cout << "Insertion size_to graph took: " << watch.read() << endl;
	/*
	graph1.insert (0, 1, 2);
	graph1.insert (1, 2, 2);
	graph1.insert (2, 0, 1);
	graph1.insert (0, 2, 5);
	*/
	std::cout << "\n";
}

template <typename V, typename E>
void lib_calvin_graph::GraphTest<V, E>::insertionTest2() {
	using namespace lib_calvin;
	graph<V, E> graph1;
	undirected_graph<V, E> graph2;
	graph1.insert_vertex(2);
	graph1.insert_vertex(3);
	graph1.insert_vertex(4);
	graph1.insert_edge(2, 4);
	graph1.insert_edge(2, 3);
	graph1.remove_edge(3, 2);

	graph2.insert_vertex(2);
	graph2.insert_vertex(3);
	graph2.insert_vertex(4);
	graph2.insert_edge(2, 4);
	graph2.insert_edge(2, 3);
	graph2.remove_edge(3, 2);
	std::cout << "# edges in graph e-size: " << graph1.number_of_edge() << "\n";
	std::cout << "# edges in undirected graph e-size: " << graph2.number_of_edge() << "\n";
	std::cout << "\n";
}


template <typename V, typename E>
void lib_calvin_graph::GraphTest<V, E>::dfsTest() {
	using namespace lib_calvin;
	graph<V, E> graph;
	populateGraph(graph, numV_, numE_, 0);
	graph.goStatic();
	vector<vector<size_t>> arrayDataWithoutEdge;
	ripEdge(graph.arrayData_, arrayDataWithoutEdge);
	vector<size_t> visitOrder(numV_);
	for (size_t i = 0; i < numV_ / 3; ++i) {
		visitOrder[i] = i;
	}
	vector<size_t> returnOrder;
	vector<size_t> returnOrder2;

	// dfs with array data
	stopwatch watch;
	watch.start();
	dfs1(arrayDataWithoutEdge, visitOrder, returnOrder);
	watch.stop();
	cout << "dfs1 of vertices: " << numV_ << "\tedges: " << numE_ <<
		"\t" << watch.read() << endl;

	watch.start();
	dfs2(arrayDataWithoutEdge, visitOrder, returnOrder2);
	watch.stop();
	cout << "dfs2 of vertices: " << numV_ << "\tedges: " << numE_ <<
		"\t" << watch.read() << "\n";

	if (returnOrder != returnOrder2) {
		cout << "dfs algorithms does not match\n";
		exit(0);
	}
	cout << "\n";
}

template <typename V, typename E>
void lib_calvin_graph::GraphTest<V, E>::bfsTest() {
	using namespace lib_calvin;
	graph<V, size_t> graph;
	populateGraph<V, size_t>(graph, numV_, numE_, 0, 1);
	graph.goStatic();
	vector<Tail<size_t>> result1;
	vector<Tail<size_t>> result2;
	vector<vector<size_t>> arrayDataWithoutEdge;
	ripEdge(graph.arrayData_, arrayDataWithoutEdge);
	stopwatch watch;
	watch.start();
	bfs(arrayDataWithoutEdge, 0, result1);
	watch.stop();
	cout << "bfs of vertices: " << numV_ << "\tedges: " << numE_ << "\t" <<
		watch.read() << endl;

	dijkstra<size_t>(graph.arrayData_, 0, result2);
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
	cout << "\n";
}

template <typename V, typename E>
void lib_calvin_graph::GraphTest<V, E>::algorithmTest() {
	using namespace lib_calvin;
	weighted_graph<V, E> graph;
	populateGraph(graph, numV_, numE_, 0);
	graph.goStatic();
	// Additional tests for weighted graph: shortetst paths problems
	// dijkstra with array data
	vector<Tail<E>> solution;
	stopwatch watch;
	watch.start();
	dijkstra<E>(graph.arrayData_, 0, solution);
	watch.stop();
	if (shortestPathCheck<E>(graph.arrayData_, 0, solution) == false) {
		cout << "dijkstra error.\n";
		exit(0);
	}
	cout << "dijkstra of vertices: " << numV_ << "\tedges: " << numE_ <<
		"\t" << watch.read() << endl;

	// Vellman-Ford
	vector<Tail<E>> solution_v;
	for (size_t i = numV_ - 1; i < numV_; ++i) {
		watch.start();
		bellmanFord(graph.arrayData_, i, solution_v);
		watch.stop();
		if (shortestPathCheck(graph.arrayData_, i, solution_v) == false) {
			cout << "bellmanFord error.\n";
			exit(0);
		}
	}
	cout << "Vellman-Ford of vertices: " << numV_ << "\tedges: " << numE_ <<
		"\t" << watch.read() << endl;

	vector<Tail<E>> row;
	// matrixApsp
	matrix<Tail<E>> apspResult(1);
	watch.start();
	matrixApsp(graph.matrixData_, apspResult);
	watch.stop();
	cout << "matrixApsp of vertices: " << numV_ << "\tedges: " << numE_ <<
		"\t" << watch.read() << endl;

	// Floyd-Earshall
	matrix<Tail<E>> floydResult(1);
	watch.start();
	floydWarshall(graph.matrixData_, floydResult);
	watch.stop();
	cout << "floydWarshall of vertices: " << numV_ << "\tedges: " << numE_ <<
		"\t" << watch.read() << endl;

	// johnson's
	matrix<Tail<E>> johnsonResult(1);
	watch.start();
	johnson(graph.arrayData_, johnsonResult);
	watch.stop();
	cout << "johnson of vertices: " << numV_ << "\tedges: " << numE_ <<
		"\t" << watch.read() << endl;

	bool isApspCorrect = true;
	bool isFloydCorrect = true;
	bool isJohnsonCorrect = true;
	for (size_t i = 0; i < numV_; ++i) {
		bellmanFord(graph.arrayData_, i, row);
		for (size_t j = 0; j < numV_; j++) {
			if (apspResult.getval(i, j).weight_ != row[j].weight_ &&
				row[j].predecessor_ != UNREACHABLE_VERTEX) {
				isApspCorrect = false;
			}
			if (floydResult.getval(i, j).weight_ != row[j].weight_ &&
				row[j].predecessor_ != UNREACHABLE_VERTEX) {
				isFloydCorrect = false;
			}
			if (johnsonResult.getval(i, j).weight_ != row[j].weight_ &&
				row[j].predecessor_ != UNREACHABLE_VERTEX) {
				isJohnsonCorrect = false;
			}
		}
	}
	if (isApspCorrect == false) {
		cout << "Apsp error (to bellmanFord)\n";
		exit(0);
	}
	if (isFloydCorrect == false) {
		cout << "FLOYD error (to bellmanFord)\n";
		exit(0);
	}
	if (isJohnsonCorrect == false) {
		cout << "JOHNSON error (to bellmanFord)\n";
		exit(0);
	}
	std::cout << "\n";
}

template <typename V, typename E>
void lib_calvin_graph::GraphTest<V, E>::undirectedAlgorithmTest() {
	// Additional test only for symmetric graph
	// MST algorithms
	using namespace lib_calvin;
	undirected_graph<V, E> graph;
	populateGraph(graph, numV_, numE_, 0);
	graph.goStatic();
	set<std::pair<size_t, size_t>> result1, result2;
	stopwatch watch;
	watch.start();
	lib_calvin_graph::kruskal(graph.arrayData_, result1);
	watch.stop();
	cout << "kruskal of vertices: " << numV_ << "\tedges: " << numE_ <<
		"\t" << watch.read() << endl;

	watch.start();
	lib_calvin_graph::prim(graph.arrayData_, result2);
	watch.stop();
	cout << "prim of vertices: " << numV_ << "\tedges: " << numE_ <<
		"\t" << watch.read() << endl;

	if (result1 != result2) {
		set<std::pair<size_t, size_t>>::iterator iter;
		cout << "MST algorithms does not match each other: size of kruskal: " << result1.size() <<
			" size of prim " << result2.size() << "\n";
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
	cout << "\n";
}

template <typename V, typename E>
void lib_calvin_graph::GraphTest<V, E>::getClosestPathTest() {
	using namespace lib_calvin;
	weighted_graph<V, E> testGraph;
	testGraph.insert_vertex(1);
	testGraph.insert_vertex(2);
	testGraph.insert_vertex(3);
	testGraph.insert_vertex(4);
	testGraph.insert_vertex(5);
	testGraph.insert_edge(1, 2, 2);
	testGraph.insert_edge(1, 4, 5);
	testGraph.insert_edge(2, 4, 2);
	testGraph.insert_edge(2, 3, 2);
	testGraph.insert_edge(3, 5, 1);
	testGraph.insert_edge(4, 5, 2);
	auto paths1 = testGraph.get_closest_path(1, 5);
	auto paths2 = testGraph.get_shortest_path(1, 5);
	auto paths3 = testGraph.get_n_shortest_paths(1, 5, 4);
	std::cout << "Closest path result\n";
	if (paths1.empty()) {
		std::cout << "Closest path does not exist\n";
	} else {
		auto path = paths1[0];
		std::cout << "Closest path total length is: " << path.length() << "\n";
		for (size_t i = 0; i <= path.length(); ++i) {
			std::cout << path.get_vertex(i) << "\t";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
	std::cout << "Shortest path result\n";
	for (size_t i = 0; i < paths2.size(); ++i) {
		auto path = paths2[i];
		std::cout << i << "'th shortest path. total weight is: " << path.total_weight() << "\n";
		for (size_t i = 0; i <= path.length(); ++i) {
			std::cout << path.get_vertex(i) << "\t";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
	std::cout << "N shortest path result\n";
	for (size_t i = 0; i < paths3.size(); ++i) {
		auto path = paths3[i];
		std::cout << i << " 'th shortest path. total weight is: " << path.total_weight() << "\n";
		for (size_t i = 0; i <= path.length(); ++i) {
			std::cout << path.get_vertex(i) << "\t";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

template <typename V, typename E>
void lib_calvin_graph::populateGraph(
	graph_base<V, E> &graph, size_t numV, size_t numE, size_t numNegativeE) {
	using namespace lib_calvin;
	for (size_t i = 0; i < numE; ++i) {
		int edge = rand() % 100000 + 1000;
		if (i < numNegativeE) {
			edge = -edge / 500 + 5; // negative edge
		}
		int srcV = (rand() * 32768 + rand()) % numV;
		int targetV = (rand() * 32768 + rand()) % numV;
		graph.insert_vertex(srcV);
		graph.insert_vertex(targetV);
		graph.insert_edge(srcV, targetV, edge);
	}
}

template <typename V, typename E>
void lib_calvin_graph::populateGraph(
	graph_base<V, E> &graph,
	size_t numV, size_t numE, size_t numNegativeE, E const &defaultEdge) {
	for (size_t i = 0; i < numE; ++i) {
		int srcV = (rand() * 32768 + rand()) % numV;
		int targetV = (rand() * 32768 + rand()) % numV;
		graph.insert_vertex(srcV);
		graph.insert_vertex(targetV);
		graph.insert_edge(srcV, targetV, defaultEdge);
	}
}

#endif