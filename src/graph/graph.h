#ifndef LIB_CALVIN__GRAPH__GRAPH_H
#define LIB_CALVIN__GRAPH__GRAPH_H

/*
* 2008-03-08: completed all shortest pair algorithms.
* 2008-03-28: done another method of dfs
* 2008-03-30: done kruskal and prim
*/

#include <utility>
#include <functional>
#include "matrix.h"
#include "adt.h"
#include "set.h"
#include "map.h"
#include "vector.h"
#include "sort.h"
#include "stopwatch.h"
#include "utility.h"
#include "container.h"

// 2008-03-25
// Revising the overall class structure to encompass network flow and
// ..undirected graph. Trying to use inheritance for the first time in
// ..my programming.
// 2008-10-17
// Revising inheritance structure. Undirected graph should derive from directed
// ..one. Network flow derives from weighted graph.
// 2010-04-05
// Got rid of weighted_graph class and thus inheritance relationship.

namespace lib_calvin_graph { // auxiliary for graph class

size_t const UNREACHABLE_VERTEX = SIZE_MAX;

using lib_calvin::map;
using lib_calvin::vector;
using lib_calvin::set;
using std::pair;
using std::cout;
using std::endl;
using lib_calvin_container::Identity;

template <typename W>
struct Tail {
	Tail(); // default constructor should work for matrix elements
	Tail(size_t predecessor, W weight);
	Tail<W> & operator= (Tail<W> const &rhs);
	// operators tuned to specific purposes: this will enable us to just
	// ...use matrix multiplication for shortest path algorithms.
	// Multiplication of weights means concatenation of path
	Tail<W> operator* (Tail<W> const &rhs) const;
	// Addition of weights take minimum of weights.
	Tail<W> const & operator+ (Tail<W> const &rhs) const;
	Tail<W> & operator+= (Tail<W> const &rhs);
	bool operator== (Tail<W> const &rhs) const;
	bool operator!= (Tail<W> const &rhs) const;
	bool isUnreachable() const;
	size_t predecessor_; // negative value indicates non-reachability.
	W weight_; // may be single or total weight, depending on context.
};

// not necessarily the closest path
template <typename W>
struct GeneralTail : public Tail<W> {
	GeneralTail();
	GeneralTail(size_t predecessor, W weight, size_t nThClosest);
	void prsize_t() const;
	bool operator< (GeneralTail<W> const &) const;
	// denotes that the path to predecessor is n'th closest path.
	size_t nThClosest_;

	using Tail<W>::weight_;
};

// extension of Tail for n'th closest path algorithm
template <typename W>
class Node {
public:
	Node(size_t numPathsToFind);
	Node(Node const &);
	Node(Node &&);
	Node & operator= (Node const &);
	Node & operator= (Node &&);
public:
	void relax(GeneralTail<W> const &);
	void foundPath(); // reached the top of heap
	W const & getWeight() const;
	size_t getNumPathFoundUntilNow() const;
	GeneralTail<W> const & getGeneralTail() const;
	bool hasRemainingPaths() const;
public:
	bool operator< (Node const &rhs) const;
public:
	void prsize_t() const;
private:
	bool isFull() const;
private:
	size_t numPathsToFind_;
	size_t numPathsFoundUntilNow_;
	vector<GeneralTail<W>> arcs_;
};

template <typename W>
struct WeightedEdge {
	WeightedEdge();
	WeightedEdge(size_t src, size_t dest, W weight);
	bool operator> (WeightedEdge<W> const &rhs) const;
	bool operator< (WeightedEdge<W> const &rhs) const;
	size_t src_;
	size_t dest_;
	W weight_;
};

class null_edge { };

template <typename V, typename E>
class GraphTest;

// Generic directed graph.
// Does not permit self-loop or multiple edges (will be extended later)
// V must support '<'and '==' operator (key).
// At least two subclasses will be derived from this: undirected graph and
// ..network flow.
template <typename V, typename E = null_edge, typename K = V, typename ExtractKey = Identity<V>>
class graph_base {
public: // basic data access
	graph_base();
	graph_base(graph_base const &);
	graph_base & operator= (graph_base<V, E, K, ExtractKey> const &);
public:
	size_t size() const;
	size_t number_of_vertex() const;
	size_t number_of_edge() const;
	bool has_vertex(K key) const;
	bool has_edge(K src, K dest) const;
	V const & get_vertex(K const &) const;
	V & get_vertex(K const &);
public:
	/// modifying methods force enter size_to dynamic mode automatically
	// returns true when inserted, false when already existed
	bool insert_vertex(V const &src);
	virtual bool insert_edge(K const &src, K const &dest, E const &edge = E());
	// returns true when deleted, false when there was not the edge
	virtual bool remove_edge(K const &src, K const &dest);
	E const & get_edge(K const &src, K const &dest) const;
	E & get_edge(K const &src, K const &dest);
	vector<std::pair<V, E>> get_vertex_edge_pairs_from(K const &src) const;
	vector<V> get_vertices_to(K const &dest) const;
	//virtual void prsize_t() const;
public:
	virtual void goStatic() const; // make data structures ready for algorithms.
public:
	class path {
	public:
		path(path const &);
		path(V const &source, vector<std::pair<V, E>> const &path);
		// If path length is zero, it means there is no path from
		// ..the source to the target vertex
	public:
		size_t length() const; // number of edges
		V const & get_source() const;
		V const & get_vertex(size_t index) const;
		E const & get_edge(size_t index) const;
	public:
		path & operator= (path const &rhs);
		path & operator= (path && rhs);
	protected:
		V const source_; // source vertex
		vector<std::pair<V, E>> const path_; // does not contain source vertex
	};

public: // Algorithms. If empty return vector means there are no solution
		// Shortest path in terms of the number of edges between vertices
	vector<path> get_closest_path(K const &src, K const &target) const;
public:
	friend class lib_calvin_graph::GraphTest<V, E>;
protected:
	template <typename T>
	  path getPathAfterAlgorithm(vector<Tail<T>> result, size_t src, size_t target) const;
	path getPathFromReversedPath(size_t src, vector<size_t> const &reversedPath) const;
protected:
	lib_calvin_container::IntIndexer<K> mapping_; // 1:1 mapping of vertex to int
	lib_calvin_container::BTree<V, K, std::less<K>, ExtractKey> vertices_;
	//lib_calvin_container::HashTable<V, K, ExtractKey> vertices_;
	map<size_t, map<size_t, E>> outLinks_;
	map<size_t, set<size_t>> inLinks_;
public:
	// Only valid in static mode
	mutable vector<vector<std::pair<size_t, E>>> arrayData_;
	mutable bool isDynamic_; // turns true after every modifying operation
private:
	// returns (vertexId, wasInserted)
	std::pair<size_t, bool> insertVertex(V const &);
	// src and dest must be present
	bool hasEdge(size_t src, size_t dest) const;
private:
	size_t numEdges_;
}; // end graph

template <typename V, typename E = null_edge, typename K = V, typename ExtractKey = Identity<V>,
typename W = E, typename ExtractWeight = Identity<E>>
class weighted_graph_base : public virtual graph_base<V, E, K, ExtractKey> {
	public:
		class weighted_path : public graph_base<V, E, K, ExtractKey>::path {
		public:
			weighted_path(typename graph_base<V, E, K, ExtractKey>::path const &);
			W const total_weight() const;
		};
	public:
		weighted_graph_base();
		weighted_graph_base(weighted_graph_base const &);
		weighted_graph_base & operator= (weighted_graph_base const &);
	public:
		// Shortest path in terms of weight
		vector<weighted_path> get_shortest_path(K const &src, K const &target) const;
		// returns n shortest paths
		vector<weighted_path> get_n_shortest_paths(K const &src, K const &target, size_t num) const;
	public:
		virtual void goStatic() const override;
	public:
		friend class lib_calvin_graph::GraphTest<V, E>;
	protected:
		using graph_base<V, E, K, ExtractKey>::getPathAfterAlgorithm;
		template <typename T>
		vector<weighted_path> getPathsAfterAlgorithm(vector<vector<GeneralTail<T>>> result,
			size_t src, size_t target) const;
	private:
		// additional data structure for algorithms
		// graph algorithms are performed only with size_tegers (not V type)
		// For matrix computation:
		// SSSP solution; not solved if size is 0.
		// vector<vector<Tail<W>>> SSSP_;
		// Apsp solution; not solved if size is 1.
		mutable lib_calvin::matrix<lib_calvin_graph::Tail<W>> matrixData_;
		mutable lib_calvin::matrix<lib_calvin_graph::Tail<W>> apspSolution_;

		using graph_base<V, E, K, ExtractKey>::has_vertex;
		using graph_base<V, E, K, ExtractKey>::getPathFromReversedPath;
		using graph_base<V, E, K, ExtractKey>::mapping_;
		using graph_base<V, E, K, ExtractKey>::arrayData_;
		using graph_base<V, E, K, ExtractKey>::isDynamic_;
		using graph_base<V, E, K, ExtractKey>::outLinks_;

};

template <typename V, typename E = null_edge, typename K = V, typename ExtractKey = Identity<V>>
class undirected_graph_base : public virtual graph_base<V, E, K, ExtractKey> {
public:
	virtual bool insert_edge(K const &src, K const &dest, E const &edge = E()) override;
	virtual bool remove_edge(K const &src, K const &dest) override;
};

template <typename V, typename E = null_edge, typename K = V, typename ExtractKey = Identity<V>,
	typename W = E, typename ExtractWeight = Identity<E>>
	class undirected_weighted_graph_base : public undirected_graph_base<V, E, K, ExtractKey>,
	public weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight> {
	public:
		virtual bool insert_edge(K const &src, K const &dest, E const &edge = E()) override;
		virtual bool remove_edge(K const &src, K const &dest) override;
	public:
		void goStatic() const override;
};
} // end namespace lib_calvin_graph

  /************** Adaptor classes for graph representation **************/

  // 2008-10-25
  // Rationale for this adaptor classes are only for performance and code
  // ..simplicity. For example, I wanted my dfs algorithms to perform both on
  // ..dynamic(map<>) and array(vector<>) input type, because if it works only
  // ..on array type, we have to convert dynamic data size_to array data each time
  // ..dfs is called (which is a waste).
  // And I want to avoid redundant codes for those input types. So I need an
  // ..adaptor.
  // 2010-04-05
  // Abandoned above change. It size_troduces needless indirection. Using specific data
  // types like vector and map does not harm generality of algorithms, as we can
  // substitue implementations for those containers at will. And algorithms should be
  // defined with minimal size_terface in the first place.
  //----------------------------- END --------------------------------------------

namespace lib_calvin {
typedef typename lib_calvin_graph::null_edge null_edge;
using lib_calvin_container::Identity;

template <typename V, typename E = null_edge, typename K = V, typename ExtractKey = Identity<V>>
using graph = lib_calvin_graph::graph_base<V, E, K, ExtractKey>;

// Undirected graphs override insertiong and modifying methods to ensure
// ..symmetry at all times.
template <typename V, typename E = null_edge, typename K = V, typename ExtractKey = Identity<V>>
using undirected_graph = lib_calvin_graph::undirected_graph_base<V, E, K, ExtractKey>;

template <typename V, typename E = null_edge, typename K = V, typename ExtractKey = Identity<V>,
	typename W = E, typename ExtractWeight = Identity<E>>
	using weighted_graph = lib_calvin_graph::weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight>;

template <typename V, typename E = null_edge, typename K = V, typename ExtractKey = Identity<V>,
	typename W = E, typename ExtractWeight = Identity<E>>
	using undirected_weighted_graph =
	lib_calvin_graph::undirected_weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight>;

} // end namespace lib_calvin

namespace lib_calvin_graph { // namespace for helper functions

using lib_calvin::matrix;
// should use only this one in actual algorithms! (faster than above)
// It turned out that g++ is so good that it optimizes perfectly in this case:
// ...no need to size_troduce another size_tegrated function at all! (of course only
// ...in -O3 option)
template <typename W>
bool relax(Tail<W> const &lhs1, Tail<W> const &lhs2, Tail<W> &rhs);


template <typename E, typename W, typename ExtractWeight>
void makeArrayData(map<size_t, map<size_t, E>> const &dynamicData,
	vector<vector<pair<size_t, W>>> &arrayData);

template <typename E>
void makeSymArrayData(vector<map<size_t, E>> const &dynamicData,
	vector<vector<pair<size_t, E>>> &arrayData);

template <typename W>
void makeMatrixData(vector<vector<pair<size_t, W>>> const &arrayData,
	matrix<Tail<W>> &matrixData);

template <typename E>
void ripEdge(vector<vector<pair<size_t, E>>> const &arrayData,
	vector<vector<size_t>> &ripped);

void transpose(vector<vector<size_t>> const &source,
	vector<vector<size_t>> &target);

template <typename E>
void transpose(vector<vector<pair<size_t, E>>> const &source,
	vector<vector<pair<size_t, E>>> &target);

} // end namespace lib_calvin_graph

namespace lib_calvin_graph { // namespace for graph algorithms

// dfs on this graph, using visitOrder as starting order of vertices
// ...i.e) starting with vertex visitOrder[0].
// ...save the result(return order of vertices) through argument.
void dfs(vector<vector<size_t>> const &graph, vector<size_t> const &visitOrder,
	vector<size_t> &returnOrder);

// Using priority queue method
void dfs1(vector<vector<size_t>> const &graph, vector<size_t> const &visitOrder,
	vector<size_t> &returnOrder);

// Using simple stack
void dfs2(vector<vector<size_t>> const &graph,
	vector<size_t> const &visitOrder, vector<size_t> &returnOrder);

// Find reachable vertices from a source vertex
// Predecessor table is stored in result
void bfs(vector<vector<size_t>> const &graph, size_t source, vector<Tail<size_t>> &result);

// Find strongly connected components of a directed graph.
// Each value in result indicates the set to which the vertex belongs
// ..value is in range [0 , #sets - 1]

// Solve Scc using dfs method
void sccByDfs(vector<vector<size_t>> const &graph,
	size_t numSets, vector<size_t> &result);

// Solve Scc using transitive closure; an inefficient way, for testing
void sccByTc(vector<vector<size_t>> const &graph,
	size_t &numSets, vector<size_t> &result);

// arcs in result: stores total weight of paths
template <typename W>
void dijkstra(vector<vector<std::pair<size_t, W>>> const &graph, size_t source,
	vector<Tail<W>> &result);

// find n closest paths
template <typename W>
void dijkstra2(vector<vector<std::pair<size_t, W>>> const &graph, size_t source,
	vector<Tail<W>> &result);

template <typename W>
void bellmanFord(vector<vector<std::pair<size_t, W>>> const &graph, size_t source,
	vector<Tail<W>> &result);

template <typename W>
void matrixApsp(matrix<Tail<W>> const &graph, matrix<Tail<W>> &result);

template <typename W>
void floydWarshall(matrix<Tail<W>> const &graph, matrix<Tail<W>> &result);

template <typename W>
void johnson(vector<vector<pair<size_t, W>>> const &graph,
	matrix<Tail<W>> &result);

// check the validity of shortest path solution
// returns true if no error has been detected, false otherwise
template <typename W>
bool shortestPathCheck(vector<vector<pair<size_t, W>>> const &graph, size_t source,
	vector<Tail<W>> &solution);

// Getting transitive closure
// true: edge, false: no edge
template <typename E>
void transitiveClosure(vector<vector<size_t>>const &graph,
	matrix<bool> &closure);

// MST algorithms assume that graph is connected and un-directed
// Result is represented as the set of included edges
template <typename W>
void kruskal(vector<vector<pair<size_t, W>>>const &graph,
	set<pair<size_t, size_t>> &result);

template <typename W>
void prim(vector<vector<pair<size_t, W>>>const &graph,
	set<pair<size_t, size_t>> &result);

} // end namespace lib_calvin_graph

  /******************* struct Tail<W> definition *********************/

namespace lib_calvin_graph { // open for definitions

template <typename W>
Tail<W>::Tail() : predecessor_(UNREACHABLE_VERTEX), weight_(0) { // must initialize to 0
}

template <typename W>
Tail<W>::Tail(size_t predecessor, W weight) : predecessor_(predecessor), weight_(weight) {
}

template <typename W>
Tail<W> &
Tail<W>::operator= (Tail<W> const &rhs) {
	predecessor_ = rhs.predecessor_;
	weight_ = rhs.weight_;
	return *this;
}

// addition of paths
template <typename W>
Tail<W>
Tail<W>::operator* (Tail<W> const &rhs) const {
	if (isUnreachable() || rhs.isUnreachable()) {
		return Tail<W>(); // no path
	}
	return Tail<W>(rhs.predecessor_, weight_ + rhs.weight_);
}

// take minimum
template <typename W>
Tail<W> const &
Tail<W>::operator+ (Tail<W> const &rhs) const {
	if (rhs.isUnreachable()) {
		return *this;
	}
	if (isUnreachable() || weight_ > rhs.weight_) {
		return rhs;
	}
	return *this;
}

// take minimum
template <typename W>
Tail<W> &
Tail<W>::operator+= (Tail<W> const &rhs) {
	if (rhs.isUnreachable()) {
		return *this;
	}
	if (isUnreachable() || weight_ > rhs.weight_) {
		predecessor_ = rhs.predecessor_;
		weight_ = rhs.weight_;
		return *this;
	}
	return *this;
}

// equivalence
template <typename W>
bool Tail<W>::operator== (Tail<W> const &rhs) const {
	if (isUnreachable() && rhs.isUnreachable()) {
		return true;
	}
	if (predecessor_ == rhs.predecessor_ && weight_ == rhs.weight_) {
		return true;
	}
	return false;
}

template <typename W>
bool Tail<W>::operator!= (Tail<W> const &rhs) const {
	if (predecessor_ == rhs.predecessor_ && weight_ == rhs.weight_) {
		return false;
	}
	return true;
}

template <typename W>
bool Tail<W>::isUnreachable() const {
	return predecessor_ == UNREACHABLE_VERTEX;
}

template <typename W>
GeneralTail<W>::GeneralTail() : Tail<W>(), nThClosest_(0) { }

template <typename W>
GeneralTail<W>::GeneralTail(size_t predecessor, W weight, size_t nThClosest) :
	Tail<W>(predecessor, weight), nThClosest_(nThClosest) { }

template <typename W>
void GeneralTail<W>::prsize_t() const {
	//std::cout << "This arc. predecessor = " << predecessor_ << ", weight: " << weight_ <<
	//"nThClosest: " << nThClosest_ << "\n";
}

template <typename W>
bool GeneralTail<W>::operator< (GeneralTail<W> const &rhs) const {
	return weight_ < rhs.weight_;
}

/******************* Node definition *********************/

template <typename W>
Node<W>::Node(size_t numPathsToFind) : numPathsToFind_(numPathsToFind),
numPathsFoundUntilNow_(0) { }

template <typename W>
Node<W>::Node(Node const &rhs) :
	numPathsToFind_(rhs.numPathsToFind_), numPathsFoundUntilNow_(rhs.numPathsFoundUntilNow_),
	arcs_(rhs.arcs_) {
	//std::cout << "Node copy ctor\n";
}

template <typename W>
Node<W>::Node(Node &&rhs) :
	numPathsToFind_(rhs.numPathsToFind_), numPathsFoundUntilNow_(rhs.numPathsFoundUntilNow_),
	arcs_(std::move(rhs.arcs_)) {
	//std::cout << "Node move ctor\n";
}

template <typename W>
Node<W> &
Node<W>::operator= (Node const &rhs) {
	numPathsToFind_ = rhs.numPathsToFind_;
	numPathsFoundUntilNow_ = rhs.numPathsFoundUntilNow_;
	arcs_ = rhs.arcs_;
	//std::cout << "Node copy assignment\n";
	return *this;
}

template <typename W>
Node<W> &
Node<W>::operator= (Node &&rhs) {
	numPathsToFind_ = rhs.numPathsToFind_;
	numPathsFoundUntilNow_ = rhs.numPathsFoundUntilNow_;
	arcs_ = std::move(rhs.arcs_);
	//std::cout << "Node move assignment\n";
	return *this;
}

template <typename W>
void Node<W>::relax(GeneralTail<W> const &arc) { // general arcs should be in ascending order always
	bool isInserted = false;
	//std::cout << "\nbefore node was:\n\t";
	prsize_t();
	for (auto iter = arcs_.begin(); iter != arcs_.end(); iter++) {
		if (arc < *iter) {
			arcs_.insert(iter, arc);
			isInserted = true;
			break;
		}
	}
	if (!isInserted) { // this arc is biggest
		if (isFull()) { // we have already enough arcs
		} else {
			arcs_.push_back(arc);
		}
	}
	if (arcs_.size() > numPathsToFind_) {
		arcs_.resize(numPathsToFind_);
	}
	//std::cout << "node relaxing input arc was:\n\t";
	//arc.prsize_t();
	//std::cout << "result node was:\n\t";
	//prsize_t();
	//std::cout << "\n\n\n";
}

template <typename W>
void Node<W>::prsize_t() const {
	//std::cout << "This Node is as below size(" << arcs_.size() << ")\n";
	//for (auto iter = arcs_.begin(); iter != arcs_.end(); iter++) {
	//std::cout <<"\t";
	//iter->prsize_t();
	//}
}

template <typename W>
void Node<W>::foundPath() {
	numPathsFoundUntilNow_++;
}

template <typename W>
W const &
Node<W>::getWeight() const { return arcs_[numPathsFoundUntilNow_].weight_; }

template <typename W>
size_t Node<W>::getNumPathFoundUntilNow() const { return numPathsFoundUntilNow_; }

template <typename W>
GeneralTail<W> const &
Node<W>::getGeneralTail() const {
	return arcs_[numPathsFoundUntilNow_];
}

template <typename W>
bool Node<W>::hasRemainingPaths() const {
	return arcs_.size() > numPathsFoundUntilNow_;
}

template <typename W>
bool Node<W>::operator< (Node const &rhs) const {
	if (arcs_.size() <= numPathsFoundUntilNow_) { // currently not reachable; infinitely far
		return false;
	}
	return getGeneralTail() < rhs.getGeneralTail();
}

template <typename W>
bool Node<W>::isFull() const { return arcs_.size() == numPathsToFind_; }

/***************** struct WeightedEdge<W> definitions ************/

template <typename W>
WeightedEdge<W>::WeightedEdge() :
	src_(0), dest_(0), weight_() {
}

template <typename W>
WeightedEdge<W>::WeightedEdge(size_t src, size_t dest, W weight) :
	src_(src), dest_(dest), weight_(weight) {
}

template <typename W>
bool WeightedEdge<W>::operator> (WeightedEdge<W> const &rhs) const {
	if (weight_ > rhs.weight_)
		return true;
	return false;
}

template <typename W>
bool WeightedEdge<W>::operator< (WeightedEdge<W> const &rhs) const {
	if (weight_ < rhs.weight_)
		return true;
	return false;
}
} // end namespace lib_calvin_graph for definitions


  //----------------------------- graph<V, E, ExtractWeight> methods ------------------------

namespace lib_calvin_graph { // open for definitions

template <typename V, typename E, typename K, typename ExtractKey>
graph_base<V, E, K, ExtractKey>::graph_base() : isDynamic_(true), numEdges_(0) { }

template <typename V, typename E, typename K, typename ExtractKey>
graph_base<V, E, K, ExtractKey>::graph_base(graph_base const &rhs) {
	*this = rhs;
}

template <typename V, typename E, typename K, typename ExtractKey>
graph_base<V, E, K, ExtractKey> &
graph_base<V, E, K, ExtractKey>::operator= (graph_base<V, E, K, ExtractKey> const &rhs) {
	if (this == &rhs) {
		return *this;
	}
	isDynamic_ = rhs.isDynamic_;
	numEdges_ = rhs.numEdges_;
	mapping_ = rhs.mapping_;
	vertices_ = rhs.vertices_;
	outLinks_ = rhs.outLinks_;
	inLinks_ = rhs.inLinks_;
	arrayData_ = rhs.arrayData_;
	return *this;
}
template <typename V, typename E, typename K, typename ExtractKey>
size_t graph_base<V, E, K, ExtractKey>::size() const {
	return mapping_.size();
}

template <typename V, typename E, typename K, typename ExtractKey>
size_t graph_base<V, E, K, ExtractKey>::number_of_vertex() const {
	return size();
}

template <typename V, typename E, typename K, typename ExtractKey>
size_t graph_base<V, E, K, ExtractKey>::number_of_edge() const {
	return numEdges_;
}

template <typename V, typename E, typename K, typename ExtractKey>
bool graph_base<V, E, K, ExtractKey>::has_vertex(K vertex) const {
	return mapping_.indexOf(vertex).second == true;
}

template <typename V, typename E, typename K, typename ExtractKey>
bool graph_base<V, E, K, ExtractKey>::has_edge(K src, K dest) const {
	auto result1 = mapping_.indexOf(src);
	auto result2 = mapping_.indexOf(dest);
	return hasEdge(result1.first, result2.first);
}
template <typename V, typename E, typename K, typename ExtractKey>
V const &
graph_base<V, E, K, ExtractKey>::get_vertex(K const &key) const {
	return *vertices_.find(key);
}

template <typename V, typename E, typename K, typename ExtractKey>
V &
graph_base<V, E, K, ExtractKey>::get_vertex(K const &key) {
	return *vertices_.find(key);
}

template <typename V, typename E, typename K, typename ExtractKey>
bool graph_base<V, E, K, ExtractKey>::hasEdge(size_t srcId, size_t destId) const {
	return outLinks_.find(srcId)->second.count(destId) != 0;
}
template <typename V, typename E, typename K, typename ExtractKey>
bool graph_base<V, E, K, ExtractKey>::insert_vertex(V const &src) {
	return insertVertex(src).second;
}

template <typename V, typename E, typename K, typename ExtractKey>
std::pair<size_t, bool>
graph_base<V, E, K, ExtractKey>::insertVertex(V const &src) {
	auto result = mapping_.insert(ExtractKey()(src));
	vertices_.insert(src);
	if (result.second == true) {
		outLinks_.insert(std::make_pair(result.first, map<size_t, E>()));
		inLinks_.insert(std::make_pair(result.first, set<size_t>()));
	}
	return result;
}

template <typename V, typename E, typename K, typename ExtractKey>
bool graph_base<V, E, K, ExtractKey>::insert_edge(K const &src, K const &dest, E const &edge) {
	if (src == dest) { // No self loop !!!
		return false;
	}
	size_t srcId = mapping_.indexOf(src).first;
	size_t destId = mapping_.indexOf(dest).first;
	if (hasEdge(srcId, destId)) {
		return false;
	} else {
		outLinks_[srcId].insert(std::make_pair(destId, edge));;
		inLinks_[destId].insert(srcId);
		isDynamic_ = true; // graph has been modified
		numEdges_++;
		return true;
	}
}

template <typename V, typename E, typename K, typename ExtractKey>
bool graph_base<V, E, K, ExtractKey>::remove_edge(K const &src, K const &dest) {
	size_t srcId = mapping_.indexOf(src).first;
	size_t destId = mapping_.indexOf(dest).first;
	if (srcId < 0 || destId < 0) {
		return false;
	}
	size_t result1 = outLinks_[srcId].erase(destId);
	size_t result2 = inLinks_[destId].erase(srcId);
	if (result1 != result2) {
		std::cout << "graph remove_edge error\n";
		exit(0);
	}
	if (result1 == 0) {
		return false;
	} else {
		isDynamic_ = true;
		numEdges_--;
		return true;
	}
}

template <typename V, typename E, typename K, typename ExtractKey>
E const &
graph_base<V, E, K, ExtractKey>::get_edge(K const &src, K const &dest) const {
	size_t srcId = mapping_.indexOf(src).first;
	size_t destId = mapping_.indexOf(dest).first;
	return outLinks_.find(srcId)->second.find(destId)->second;
}

template <typename V, typename E, typename K, typename ExtractKey>
E &
graph_base<V, E, K, ExtractKey>::get_edge(K const &src, K const &dest) {
	return const_cast<E &>(const_cast<graph_base<V, E, K, ExtractKey> const *>(this)->get_edge(src, dest));
}

template <typename V, typename E, typename K, typename ExtractKey>
vector<std::pair<V, E>>
graph_base<V, E, K, ExtractKey>::get_vertex_edge_pairs_from(K const &src) const {
	size_t srcId = mapping_.indexOf(src).first;
	vector<std::pair<V, E>> result;
	map<size_t, E> const & edges = outLinks_.find(srcId)->second;
	result.reserve(edges.size());
	for (auto iter = edges.begin(); iter != edges.end(); ++iter) {
		result.push_back(std::make_pair(*vertices_.find(mapping_[iter->first]), iter->second));
	}
	return result;
}

template <typename V, typename E, typename K, typename ExtractKey>
vector<V>
graph_base<V, E, K, ExtractKey>::get_vertices_to(K const &dest) const {
	size_t destId = mapping_.indexOf(dest).first;
	vector<V> result;
  auto targetIds = inLinks_.find(destId)->second;
	result.reserve(targetIds.size());
	for (auto iter = targetIds.begin(); iter != targetIds.end(); ++iter) {
		result.push_back(*vertices_.find(mapping_[*iter]));
	}
	return result;
}

/*------------- prsize_t -------------*/

/*
// assuming the V and E can be prsize_ted with cout
template <typename V, typename E, typename K, typename ExtractKey, typename W, typename ExtractWeight>
void graph<V, E, ExtractWeight>::prsize_t() const {
	for (size_t i = 0; i < numV_; ++i) { // for each source vertex
		cout << mapping_[i] << ": ";
		// prsize_t all edges
		map<size_t, E> const &temp = dynamicData_[i];
		typename map<size_t, E>::const_iterator iter;
		for (iter = temp.begin(); iter != temp.end(); ++iter) {
			cout << mapping_[iter->first]; // target vertex
			cout << "(" << iter->second << ")  "; // weight
		}
		cout << endl;

	}
}
*/

template <typename V, typename E, typename K, typename ExtractKey>
void graph_base<V, E, K, ExtractKey>::goStatic() const {
	if (isDynamic_ == true) {
		lib_calvin_graph::makeArrayData<E, E, Identity<E>>(outLinks_, arrayData_);
	}
}

// Shortest path in terms of the number of edges
template <typename V, typename E, typename K, typename ExtractKey>
vector<typename graph_base<V, E, K, ExtractKey>::path>
graph_base<V, E, K, ExtractKey>::get_closest_path(K const &src, K const &target) const {
	goStatic();
	vector<path> paths;
	if (!has_vertex(src) || !has_vertex(target)) { // input is invalid
		std::cout << "get_closest_path input error\n";
		exit(0);
	}
	size_t srcVertex = mapping_.indexOf(src).first;
	size_t targetVertex = mapping_.indexOf(target).first;
	vector<vector<size_t>> graph;
	ripEdge(arrayData_, graph);
	vector<Tail<size_t>> result;
	bfs(graph, srcVertex, result);
	//std::cout << "after bfs\n";
	if (result[targetVertex].isUnreachable()) { // not reachable
	} else {
		paths.push_back(getPathAfterAlgorithm<size_t>(result, srcVertex, targetVertex));
	}
	return paths;
}

template <typename V, typename E, typename K, typename ExtractKey, typename W, typename ExtractWeight>
weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight>::weighted_graph_base():
	graph_base<V, E, K, ExtractKey>(),
	matrixData_(1), apspSolution_(1) { }

template <typename V, typename E, typename K, typename ExtractKey, typename W, typename ExtractWeight>
weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight>::weighted_graph_base(weighted_graph_base const &rhs) :
	graph_base<V, E, K, ExtractKey>(rhs), matrixData_(rhs.matrixData_), apspSolution_(rhs.apspSolution_) { }

template <typename V, typename E, typename K, typename ExtractKey, typename W, typename ExtractWeight>
weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight> &
weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight>::operator= (weighted_graph_base const &rhs) {
	graph_base<V, E, K, ExtractKey>::operator=(rhs);
	matrixData_ = rhs.matrixData_;
	apspSolution_ = rhs.apspSolution_;
}

template <typename V, typename E, typename K, typename ExtractKey, typename W, typename ExtractWeight>
vector<typename weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight>::weighted_path>
weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight>::get_shortest_path(K const &src, K const &target) const {
	goStatic();
	vector<weighted_path> paths;
	bool pathExists = true;
	if (!has_vertex(src) || !has_vertex(target) || src == target) { // input is invalid
		std::cout << "get_shortest_path input error\n";
		exit(0);
	}
	size_t srcVertex = mapping_.indexOf(src).first;
	size_t targetVertex = mapping_.indexOf(target).first;
	vector<Tail<W>> result;
	dijkstra(arrayData_, srcVertex, result);
	//std::cout << "after dijkstra\n";
	if (result[targetVertex].isUnreachable()) { // not reachable
	} else {
		paths.push_back(getPathAfterAlgorithm(result, srcVertex, targetVertex));
	}
	return paths;
}

template <typename V, typename E, typename K, typename ExtractKey, typename W, typename ExtractWeight>
vector<typename weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight>::weighted_path>
weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight>::
get_n_shortest_paths(K const &src, K const &target, size_t num) const {
	goStatic();
	if (!has_vertex(src) || !has_vertex(target) || src == target) { // input is invalid, return empty vector
		std::cout << "get_n_shortest_paths input error\n";
		exit(0);
	}
	size_t srcVertex = mapping_.indexOf(src).first;
	size_t targetVertex = mapping_.indexOf(target).first;
	vector<vector<GeneralTail<W>>> result;
	dijkstra2(arrayData_, srcVertex, result, num);
	//std::cout << "after dijkstra2\n";
	if (result[targetVertex].empty()) { // not reachable
		return vector<weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight>::weighted_path>();
	} else {
		return weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight>::getPathsAfterAlgorithm<W>(
			result, srcVertex, targetVertex);
	}
}

template <typename V, typename E, typename K, typename ExtractKey>
template <typename T>
typename graph_base<V, E, K, ExtractKey>::path
graph_base<V, E, K, ExtractKey>::getPathAfterAlgorithm(
	vector<Tail<T>> result, size_t srcVertex, size_t targetVertex) const {
	vector<size_t> reversedPath;
	for (size_t curVertex = targetVertex; curVertex != srcVertex; ) {
		//std::cout << "following predeccsor array\n";
		reversedPath.push_back(curVertex);
		curVertex = result[curVertex].predecessor_;
	}
	return getPathFromReversedPath(srcVertex, reversedPath);
}

template <typename V, typename E, typename K, typename ExtractKey, typename W, typename ExtractWeight>
template <typename T>
vector<typename weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight>::weighted_path>
weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight>::getPathsAfterAlgorithm(
	vector<vector<GeneralTail<T>>> result, size_t srcVertex, size_t targetVertex) const {
	size_t numPaths = result[targetVertex].size();
	vector<vector<size_t>> reversedPaths(numPaths);
	for (size_t nThCloest = 0; nThCloest < numPaths; nThCloest++) {
		size_t curNThClosest = nThCloest; // path to curVertex is n'th closest path
		for (size_t curVertex = targetVertex; curVertex != srcVertex; ) {
			//std::cout << "following predeccsor array\n";
			reversedPaths[nThCloest].push_back(curVertex);
			GeneralTail<T> arc = result[curVertex][curNThClosest];
			curVertex = arc.predecessor_;
			curNThClosest = arc.nThClosest_;
		}
	}
	vector<weighted_path> realPaths;
	for (size_t i = 0; i < numPaths; i++) {
		realPaths.push_back(getPathFromReversedPath(srcVertex, reversedPaths[i]));
	}
	return realPaths;
}

template <typename V, typename E, typename K, typename ExtractKey>
typename graph_base<V, E, K, ExtractKey>::path
graph_base<V, E, K, ExtractKey>::getPathFromReversedPath(size_t srcVertex, vector<size_t> const &reversedPath) const {
	vector<std::pair<V, E>> realPath;
	size_t previousVertex = srcVertex;
	size_t curVertex;
	for (size_t i = 0; i < reversedPath.size(); ++i) {
		//std::cout << "making final result\n";
		curVertex = reversedPath[reversedPath.size() - 1 - i];
		realPath.push_back(std::pair<V, E>(
			*vertices_.find(mapping_[curVertex]), outLinks_.find(previousVertex)->second.find(curVertex)->second));
		previousVertex = curVertex;
	}
	return graph_base<V, E, K, ExtractKey>::path(mapping_[srcVertex], realPath);
}

template <typename V, typename E, typename K, typename ExtractKey>
graph_base<V, E, K, ExtractKey>::path::path(path const &rhs) :
	source_(rhs.source_), path_(rhs.path_) { }

template <typename V, typename E, typename K, typename ExtractKey>
graph_base<V, E, K, ExtractKey>::path::path(V const &source, vector<std::pair<V, E>> const &path) :
	source_(source), path_(path) { }

template <typename V, typename E, typename K, typename ExtractKey>
size_t graph_base<V, E, K, ExtractKey>::path::length() const {
	// Size is path length. If it is zero, it means there is no path from
	// ..the source to the target vertex
	return path_.size();
}

template <typename V, typename E, typename K, typename ExtractKey>
V const &
graph_base<V, E, K, ExtractKey>::path::get_source() const {
	return source_;
}

template <typename V, typename E, typename K, typename ExtractKey>
V const &
graph_base<V, E, K, ExtractKey>::path::get_vertex(size_t index) const {
	if (index > 0) {
		return path_[index - 1].first;
	} else {
		return source_;
	}
}
template <typename V, typename E, typename K, typename ExtractKey>
E const &
graph_base<V, E, K, ExtractKey>::path::get_edge(size_t index) const {
	return path_[index].second;
}

template <typename V, typename E, typename K, typename ExtractKey>
typename graph_base<V, E, K, ExtractKey>::path &
graph_base<V, E, K, ExtractKey>::path::operator= (path const &rhs) {
	source_ = rhs.source_;
	path_ = rhs.path_;
	return *this;
}

template <typename V, typename E, typename K, typename ExtractKey>
typename graph_base<V, E, K, ExtractKey>::path &
graph_base<V, E, K, ExtractKey>::path::operator= (path &&rhs) {
	source_ = rhs.source_;
	path_ = std::forward<vector<std::pair<V, E>>>(rhs.path_);
	return *this;
}

template <typename V, typename E, typename K, typename ExtractKey, typename W, typename ExtractWeight>
weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight>::weighted_path::weighted_path(
  typename graph_base<V, E, K, ExtractKey>::path const & inPath) :
	graph_base<V, E, K, ExtractKey>(inPath) { }

template <typename V, typename E, typename K, typename ExtractKey, typename W, typename ExtractWeight>
W const
weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight>::weighted_path::total_weight() const {
	W totalWeight = 0;
	for (size_t i = 0; i < graph_base<V, E, K, ExtractKey>::path::length(); i++) {
		totalWeight += ExtractWeight()(graph_base<V, E, K, ExtractKey>::path::get_edge(i));
	}
	return totalWeight;
}

template <typename V, typename E, typename K, typename ExtractKey, typename W, typename ExtractWeight>
void
weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight>::goStatic() const {
	graph_base<V, E, K, ExtractKey>::goStatic();
	if (isDynamic_ == true) {
		lib_calvin_graph::makeArrayData<E, W, ExtractWeight>(outLinks_, arrayData_);
		lib_calvin_graph::makeMatrixData<W>(arrayData_, matrixData_);
	}
}

template <typename V, typename E, typename K, typename ExtractKey>
bool undirected_graph_base<V, E, K, ExtractKey>::insert_edge(K const &src, K const &dest, E const &edge) {
	bool result1 = graph_base<V, E, K, ExtractKey>::insert_edge(src, dest, edge);
	bool result2 = graph_base<V, E, K, ExtractKey>::insert_edge(dest, src, edge);
	if (result1 != result2) {
		std::cout << "undirected_weighted_graph insert error!!\n";
		exit(0);
	}
	return result1;
}

template <typename V, typename E, typename K, typename ExtractKey>
bool undirected_graph_base<V, E, K, ExtractKey>::remove_edge(K const &src, K const &dest) {
	bool result1 = graph_base<V, E, K, ExtractKey>::remove_edge(src, dest);
	bool result2 = graph_base<V, E, K, ExtractKey>::remove_edge(dest, src);
	if (result1 != result2) {
		std::cout << "undirected_weighted_graph remove error!!\n";
		exit(0);
	}
	return result1;
}

template <typename V, typename E, typename K, typename ExtractKey, typename W, typename ExtractWeight>
bool undirected_weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight>::insert_edge(
	K const &src, K const &dest, E const &edge) {
	return undirected_graph_base<V, E, K, ExtractKey>::insert_edge(src, dest, edge);
}

template <typename V, typename E, typename K, typename ExtractKey, typename W, typename ExtractWeight>
bool undirected_weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight>::remove_edge(K const &src, K const &dest) {
	return undirected_graph_base<V, E, K, ExtractKey>::remove_edge(src, dest);
}

template <typename V, typename E, typename K, typename ExtractKey, typename W, typename ExtractWeight>
void undirected_weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight>::goStatic() const {
	weighted_graph_base<V, E, K, ExtractKey, W, ExtractWeight>::goStatic();
}

} // end namespace lib_calvin_graph

  //--------------------------- Global functions ----------------------------

namespace lib_calvin_graph { // open for definitions

							 // return whether the relaxation took place or not
template <typename E>
bool relax(Tail<E> const &lhs1, Tail<E> const &lhs2, Tail<E> &rhs) {
	if (lhs1.isUnreachable() || lhs2.isUnreachable()) {
		return false;
	}
	if (rhs.isUnreachable() || rhs.weight_ > lhs1.weight_ + lhs2.weight_) {
		rhs.predecessor_ = lhs2.predecessor_;
		rhs.weight_ = lhs1.weight_ + lhs2.weight_;
		return true;
	}
	return false;
}

template <typename E, typename W, typename ExtractWeight>
void makeArrayData(map<size_t, map<size_t, E>> const &dynamicData,
	vector<vector<pair<size_t, W>>> &arrayData) {
	size_t numV = dynamicData.size();
	arrayData.clear();
	arrayData.resize(numV);
	size_t i = 0;
	for (auto iter = dynamicData.begin(); iter != dynamicData.end(); iter++, i++) {
		map<size_t, E> const &original = iter->second;
		vector<pair<size_t, W>> &copy = arrayData[i];
		copy.reserve(original.size());
		for (auto iter = original.begin(); iter != original.end(); ++iter) {
			copy.push_back(std::make_pair(iter->first, ExtractWeight()(iter->second)));
		}
	}
}

template <typename E>
void makeSymArrayData(vector<map<size_t, E>> const &dynamicData,
	vector<vector<pair<size_t, E>>> &arrayData) {
	// dynamicData has edges only for src < dest now.
	size_t numV = dynamicData.size();
	arrayData.clear();
	arrayData.resize(numV);
	for (size_t i = 0; i < numV; ++i) { // for each source vertex
		map<size_t, E> const &original = dynamicData[i];
		typename map<size_t, E>::const_iterator iter;
		for (iter = original.begin(); iter != original.end(); ++iter) {
			size_t src = i;
			size_t dest = iter->first;
			E const &edge = iter->second;
			arrayData[src].push_back(pair<size_t, E>(dest, edge));
			arrayData[dest].push_back(pair<size_t, E>(src, edge));
		}
	}
}

template <typename W>
void makeMatrixData(vector<vector<pair<size_t, W>>> const &arrayData,
	matrix<Tail<W>> &matrixData) {
	size_t numV = static_cast<size_t>(arrayData.size());
	// remake matrix (initialization done here: predecessor_ = UNREACHABLE_VERTEX)
	matrixData.reset(numV);
	for (size_t i = 0; i < numV; ++i) { // for each source vertex
		vector<pair<size_t, W>> const &original = arrayData[i];
		typename vector<pair<size_t, W>>::const_iterator iter;
		for (iter = original.begin(); iter != original.end(); ++iter) {
			// setval return a reference to matrix element.
			matrixData.setval(i, iter->first) = Tail<W>(i, iter->second);
		}
		// set self-edge (path of length 0)
		W zero = W();
		matrixData.setval(i, i) = Tail<W>(i, zero);
	}
}

template <typename E>
void ripEdge(vector<vector<pair<size_t, E>>> const &arrayData,
	vector<vector<size_t>> &unweighted) {
	size_t numV = static_cast<size_t>(arrayData.size());
	unweighted.clear();
	unweighted.resize(numV);
	for (size_t i = 0; i < numV; ++i) { // for each source vertex
		vector<pair<size_t, E>> const &original = arrayData[i];
		vector<size_t> &copy = unweighted[i];
		typename vector<pair<size_t, E>>::const_iterator iter;
		for (iter = original.begin(); iter != original.end(); ++iter) {
			copy.push_back(iter->first);
		}
	}
}

template <typename E>
void transpose(vector<vector<pair<size_t, E>>> const &source,
	vector<vector<pair<size_t, E>>> &target) {
	size_t numV = source.size();
	target.clear();
	target.resize(numV);
	for (size_t i = 0; i < numV; ++i) { // for each source vertex
		vector<pair<size_t, E>> const &original = source[i];
		typename vector<pair<size_t, E>>::const_iterator iter;
		for (iter = original.begin(); iter != original.end(); ++iter) {
			target[iter->first].push_back(pair<size_t, E>(i, iter->second));
		}
	}
}

//------------------------------ Algorithms ------------------------------

// For graph algorithms (priority queue)
using lib_calvin_container::IntPq;

// arcs in result: stores total weight of paths
template <typename W>
void dijkstra(vector<vector<pair<size_t, W>>> const &graph, size_t source,
	vector<Tail<W>> &result) {
	size_t numV = static_cast<size_t>(graph.size());
	IntPq<W> pq(numV);
	vector<bool> isFinished(numV); // mark finished vertices
	result.clear();
	result.resize(numV, Tail<W>()); // predecessor_ = UNREACHABLE_VERTEX for default
	result[source].predecessor_ = source; // my convention
										  // push source vertex with priority 0.
	W zero = W(); // W() must represent 0 value (true for built-in type)
	pq.insert(source, zero);
	while (pq.size() != 0) {
		// record current top element
		pair<size_t, W> const &topElem = pq.pop();
		size_t top = topElem.first;
		isFinished[top] = true; // mark for finished !!
		W weight = topElem.second;
		result[top].weight_ = weight;
		// relaxation
		for (size_t i = 0; i < graph[top].size(); ++i) {
			pair<size_t, W> const &iter = graph[top][i];
			if (isFinished[iter.first]) // do nothing if already finished
				continue;
			if (pq.insert(iter.first, iter.second + weight) == true) {
				result[iter.first].predecessor_ = top;
			}
		}
	}
}

template <typename W>
void dijkstra2(vector<vector<pair<size_t, W>>> const &graph, size_t source,
	vector<vector<GeneralTail<W>>> &result, size_t num) {
	size_t const numPathsToFind = 20;
	size_t numV = static_cast<size_t>(graph.size());
	typedef Node<W> Node;
	Node emptyNode(num);
	IntPq<Node> pq(numV, emptyNode);
	result.clear();
	result.resize(numV);
	vector<size_t> numFinishedPath(numV, 0);
	Node zero = emptyNode;
	zero.relax(GeneralTail<W>(source, 0, 0)); // 0'th closest path for itself
	pq.insert(source, zero);
	while (pq.size() != 0) {
		pair<size_t, Node> const &topElem = pq.pop();
		size_t curVertex = topElem.first;
		Node curNode = topElem.second;
		W const & curWeight = curNode.getGeneralTail().weight_;
		result[curVertex].push_back(curNode.getGeneralTail());
		//std::cout << "Node popped curVertex; " << curVertex << ", nTh: " <<
		//curNode.getNumPathFoundUntilNow() <<
		//" weight = " << curNode.getWeight() << "\n";
		if (result[curVertex].size() > numPathsToFind) {
			std::cout << "dijkstra2 error1\n";
			exit(0);
		}
		for (size_t i = 0; i < graph[curVertex].size(); ++i) {
			pair<size_t, W> const &iter = graph[curVertex][i];
			size_t targetVertex = iter.first;
			W const & targetWeight = iter.second;
			if (numFinishedPath[targetVertex] == numPathsToFind) { // this vertex is finished
				continue;
			}
			Node targetNode = emptyNode;
			if (pq.hasKey(targetVertex)) {
				targetNode = pq.getPriority(targetVertex);
			}
			GeneralTail<W> arc(curVertex, curWeight + targetWeight, static_cast<size_t>(curNode.getNumPathFoundUntilNow()));
			targetNode.relax(arc);
		}
		numFinishedPath[curVertex]++;
		curNode.foundPath();
		if (curNode.hasRemainingPaths()) { // send right back to the heap
			pq.insert(curVertex, curNode);
		}
	}
}

// O(VE)
template <typename W>
void bellmanFord(vector<vector<pair<size_t, W>>> const &graph,
	size_t source, vector<Tail<W>> &result) {
	size_t numV = static_cast<size_t>(graph.size());
	result.clear();
	result.resize(numV, Tail<W>());
	result[source].predecessor_ = source; // my convention
	W zero = W();
	result[source].weight_ = zero;
	// longest path can take numV -1 vertices
	bool finished = false;
	for (size_t i = 0; i < numV - 1; ++i) {
		finished = true;
		// relaxation for every edges
		for (size_t src = 0; src < numV; src++) {
			for (size_t j = 0; j < graph[src].size(); j++) {
				pair<size_t, W> const &iter = graph[src][j];
				Tail<W> edge(src, iter.second);
				if (relax(result[src], edge, result[iter.first])) {
					finished = false;
				}
			}
		}
		if (finished) { // no relaxation; finished
			break;
		}
	}
	// negative weighted cycle detection
	vector<Tail<W>> temp = result;
	for (size_t src = 0; src < numV; src++) {
		for (size_t i = 0; i < static_cast<size_t>(graph[src].size()); ++i) {
			pair<size_t, W> const &iter = graph[src][i];
			Tail<W> edge(src, iter.second);
			relax(result[src], edge, result[iter.first]);
		}
	}
	if (result != temp) {
		if (finished == false) {
			// (numV - 1) iteration has been done, but not yet finished
			cout << "Vellman-Ford: negative weighted cycle detected!!!\n";
		} else { // should not enter here at all
			cout << "Vellman-Ford: logic error!!!\n";
			exit(0);
		}
	}
}

// Inferior to floydWarshall; practically useless
// 2008-10-31: Researching this algorithm again for my B.S thesis.
template <typename W>
void matrixApsp(matrix<Tail<W>> const &graph,
	matrix<Tail<W>> &result) {
	size_t numV = graph.height();
	result.reset(graph); // optimal solution of path length 1 (base case)
						 // get ceiling of lg(numV)
	size_t lg_V = lib_calvin_util::log<size_t>(numV - 1);
	cout << "matrixApsp iteration: " << lg_V << endl;
	// matrix multiplication lg_V times
	for (size_t i = 0; i < lg_V + 1; ++i) {
		matrix<Tail<W>> temp = result;
		result = result + result * result;
		//lib_calvin_matrix::blockedMultiAdd(result, result, result);
		//lib_calvin_matrix::recursiveMultiAdd(result, result, result);
		//lib_calvin_matrix::recursiveSelfMultiAdd(result);
		if (result == temp) { // no relaxation occured; finished
			cout << "matrixApsp finished after " << i + 1 << " iterations\n";
			break;
		}
	}

	// negative weighted cycle detection
	for (size_t i = 0; i < numV; ++i) {
		Tail<W> temp = result.getval(i, i);
		if (temp.weight_ < W()) {
			cout << "matrixApsp negative weighted cycle detected!!\n";
			return;
		}
	}
}

// Slower than johnson upto E = 0.5 * V^0.5; fast in very dense graph
// There is a room for performance boost: making * and + operations of
// ...Tail simpler by omitting consideration for infinity (no edge). To do so,
// ...there should a limit on possible edge weight of graph. And then, we can
// ...use a very big positive weight to represent no-edge. But it turned out
// ...that it provides only about 10% boost, so I rejected it because it hampers
// ...design and clarity.
template <typename W>
void floydWarshall(matrix<Tail<W>> const &graph,
	matrix<Tail<W>> &result) {
	size_t numV = graph.height();
	result.reset(graph); // copy
	for (size_t m = 0; m < numV; m++) { // m is the vertex that is being added
		for (size_t i = 0; i < numV; ++i) {
			for (size_t j = 0; j < numV; j++) {
				result.setval(i, j) += result.getval(i, m) * result.getval(m, j);
			}
		}
	}
}

// Make all weights positive by setting values to vertices
// Offset value will affect edge weight in this way:
// value of source vertex added, value of destination vertest subtracted.
// So that the transformed edges are all positive weighted.
template <typename W>
void johnson(vector<vector<pair<size_t, W>>> const &graph,
	matrix<Tail<W>> &result) {
	size_t numV = static_cast<size_t>(graph.size());
	W zero = W();
	result.reset(numV);
	// Use bellmanFord to calculate offsets
	vector<W> offsets(numV); // offset for each vertex
	vector<vector<pair<size_t, W>>> tempGraph = graph;
	tempGraph.push_back(vector<pair<size_t, W>>(numV)); // artificial vertex
	for (size_t i = 0; i < numV; ++i) {
		tempGraph[numV][i] = pair<size_t, W>(i, zero);
	}
	vector<Tail<W>> tempTails;
	bellmanFord(tempGraph, numV, tempTails);
	for (size_t i = 0; i < numV; ++i) {
		offsets[i] = tempTails[i].weight_;
	}
	// Use offsets to adjust graph to a positive weighted one
	vector<vector<pair<size_t, W>>> &positiveGraph = tempGraph;
	positiveGraph.pop_back(); // remove artificial vertex
	for (size_t i = 0; i < numV; ++i) {
		typename vector<pair<size_t, W>>::iterator iter;
		for (iter = positiveGraph[i].begin();
			iter != positiveGraph[i].end(); ++iter) {
			// + source offset, - dest offset to edges
			iter->second += (offsets[i] - offsets[iter->first]);
		}
	}
	// Use dijkstra on transformed graph for every source vertex
	vector<Tail<W>> SSSP_result; // single source shortest path
	for (size_t i = 0; i < numV; ++i) {
		dijkstra(positiveGraph, i, SSSP_result);
		// copy SSSP result size_to matrix row
		for (size_t j = 0; j < numV; j++) {
			result.setval(i, j) = SSSP_result[j];
			// - source offset, + dest offset to edges
			result.setval(i, j).weight_ -= (offsets[i] - offsets[j]);
		}
	}
}

template <typename W>
bool shortestPathCheck(vector<vector<pair<size_t, W>>> const &graph,
	size_t source, vector<Tail<W>> &solution) {
	size_t numV = static_cast<size_t>(graph.size());
	if (solution[source].isUnreachable() || solution[source].weight_ != W()) {
		cout << "input error\n";
		return false;
	}
	typename vector<pair<size_t, W>>::const_iterator iter;
	for (size_t i = 0; i < numV; ++i) { // i: source vertex
		for (iter = graph[i].begin(); iter != graph[i].end(); ++iter) {
			size_t src = i;
			size_t dest = iter->first;
			W weight = iter->second;
			if (solution[src].predecessor_ != UNREACHABLE_VERTEX &&
				(solution[src].weight_ + weight < solution[dest].weight_
					|| solution[dest].predecessor_ == UNREACHABLE_VERTEX)) {
				cout << "source weight: " << solution[src].weight_ << " ";
				cout << "edge weight: " << weight << " ";
				cout << "dest weight: " << solution[dest].weight_ << "\n";
				return false;
			}
		}
	}
	return true;
}

// MST algorithms assume that graph is connected and un-directed
// Result is represented as the set of included edges
template <typename W>
void kruskal(vector<vector<pair<size_t, W>>> const &graph,
	set<pair<size_t, size_t>> &result) {
	size_t numV = static_cast<size_t>(graph.size());
	size_t numE, curE; // curE: current num of edges in temporary MST
	W total = W(); // for debugging
	vector<WeightedEdge<W>> edges;
	lib_calvin_container::DisjointSet sets(numV);
	result.clear();
	// Insert all edges size_to an array
	for (size_t src = 0; src < numV; src++) {
		for (size_t i = 0; i < static_cast<size_t>(graph[src].size()); ++i) {
			pair<size_t, W> const &iter = graph[src][i];
			// Take only half of the entire vertices.
			if (src < iter.first) {
				WeightedEdge<W> temp(src, iter.first, iter.second);
				edges.push_back(temp);
			}
		}
	}
	numE = static_cast<size_t>(edges.size());
	curE = 0;
	// Sorting
	lib_calvin::sort(edges.begin(), edges.end());
	// Growing MST
	for (size_t i = 0; i < numE; ++i) {
		// Insert edge if it connects two sets
		if (!sets.isSameSet(edges[i].src_, edges[i].dest_)) {
			sets.unite(edges[i].src_, edges[i].dest_);
			result.insert(pair<size_t, size_t>(edges[i].src_, edges[i].dest_));
			total += edges[i].weight_;
			++curE;
			if (curE == numV - 1) // MST is completed
				break;
		}
	}
	cout << "kruskal: total weight is " << total << endl;
	for (size_t i = 0; i < numV - 1; ++i) {
		if (sets.isSameSet(i, i + 1) == false) {
			cout << "kruskal fatal error!! (not connected)\n";
			exit(0);
		}
	}
}

template <typename W>
void prim(vector<vector<pair<size_t, W>>>const &graph, set<pair<size_t, size_t>> &result) {
	size_t numV = static_cast<size_t>(graph.size());
	vector<size_t> predecessor(numV, UNREACHABLE_VERTEX); // to save predecessor vertices
	std::cout << "prim size = " << numV << "\n";
	IntPq<W> pq(numV);
	vector<bool> connected(numV); // mark connected vertices
	result.clear();
	W zero = W();
	W total = W(); // for debugging
				   // Special case for the first one
				   // push vertex 0 with priority 0. (any vertex is fine to start with)
	pq.insert(0, zero);
	predecessor[0] = 0; // my convention
	while (pq.size() != 0) {
		// record current top element
		pair<size_t, W> const &topElem = pq.pop();
		size_t top = topElem.first;
		connected[top] = true;
		// insert in order
		if (predecessor[top] < top)
			result.insert(pair<size_t, size_t>(predecessor[top], top));
		else if (predecessor[top] > top)
			result.insert(pair<size_t, size_t>(top, predecessor[top]));
		else { // do nothing for the starting vertex
		}
		total += topElem.second;
		// relaxation
		size_t src = top; // rename
		for (size_t i = 0; i < static_cast<size_t>(graph[src].size()); ++i) {
			pair<size_t, W> const &iter = graph[src][i];
			if (connected[iter.first]) // do nothing for already connected one
				continue;
			if (pq.insert(iter.first, iter.second) == true) {
				predecessor[iter.first] = src;
			}
		}
	}
	cout << "prim: total weight is " << total << endl;
}
} // end namespace lib_calvin_graph for definitions

#endif

