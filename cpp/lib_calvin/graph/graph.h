#ifndef LIB_CALVIN__GRAPH__GRAPH_H 
#define LIB_CALVIN__GRAPH__GRAPH_H 

/*
 * 2008-03-08: completed all shortest pair algorithms.
 * 2008-03-28: done another method of dfs
 * 2008-03-30: done kruskal and prim
 */

#include <utility>
#include <vector>
#include <set>
#include <map>
#include "matrix.h"
#include "adt.h"
#include "set.h"
#include "map.h"
#include "vector.h"
#include "sort.h"
#include "stopwatch.h" 
#include "utility.h" 

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

using lib_calvin::map;
using lib_calvin::vector;
using lib_calvin::set;
using std::pair;
using std::cout;
using std::endl;

template <typename W>
struct Arc { 
  Arc(); // default constructor should work for matrix elements
  Arc(int predecessor, W weight);
  Arc<W> & operator= (Arc<W> const &rhs);
  // operators tuned to specific purposes: this will enable us to just
  // ...use matrix multiplication for shortest path algorithms.
  Arc<W> operator* (Arc<W> const &rhs) const; // addition of weights.
  // take minimum of weights.
  Arc<W> const & operator+ (Arc<W> const &rhs) const;
  Arc<W> & operator+= (Arc<W> const &rhs); 
  bool operator== (Arc<W> const &rhs) const;
  bool operator!= (Arc<W> const &rhs) const;

  int predecessor_; // negative value indicates non-reachability.
  W weight_; // may be single or total weight, depending on context.  
};

// not necessarily the closest path
template <typename W>
struct GeneralArc: public Arc<W> {
	GeneralArc();
	GeneralArc(int predecessor, W weight, int nThClosest);
	void print() const;
	bool operator< (GeneralArc<W> const &) const;
	// denotes that the path to predecessor is n'th closest path.
	int nThClosest_;
};

// extension of Arc for n'th closest path algorithm
template <typename W>
class Node {
public:
	Node(size_t numPathsToFind);
	Node(Node const &);
	Node(Node &&);
	Node & operator= (Node const &);
	Node & operator= (Node &&);
public:
	void relax(GeneralArc<W> const &);
	void foundPath(); // reached the top of heap
	W const & getWeight() const;
	size_t getNumPathFoundUntilNow() const;
	GeneralArc<W> const & getGeneralArc() const;
	bool hasRemainingPaths() const;
public:
	bool operator< (Node const &rhs) const;
public:
	void print() const;
private:
	bool isFull() const;
private:
	size_t numPathsToFind_;
	size_t numPathsFoundUntilNow_;
	vector<GeneralArc<W>> arcs_;
};

template <typename W>
struct WeightedEdge {
  WeightedEdge();
  WeightedEdge(int src, int dest, W weight);
  bool operator> (WeightedEdge<W> const &rhs) const;
  bool operator< (WeightedEdge<W> const &rhs) const;
  int src_;
  int dest_;
  W weight_;
};

class null_edge { };

template <typename V, typename E, typename W, typename ExtractWeight>
class GraphTest;

// Generic directed graph.
// Does not permit self-loop or multiple edges (will be extended later)
// V must support '<'and '==' operator (key).
// At least two subclasses will be derived from this: undirected graph and
// ..network flow.
template <typename V, typename E = null_edge, typename W = E, typename ExtractWeight = std::identity<E>>
class graph_base { 
public: // basic data access
  graph_base();
  graph_base & operator= (graph_base<V, E, W, ExtractWeight> const &);
  int size() const { return numV_; }
  int number_of_vertex() const { return numV_; }
  int number_of_edge() const { return numE_; }
  bool has(V vertex) const;
  bool has(V src, V dest) const;
  E const & get_edge_of(V src, V dest) const;
  /// modifying methods force enter into dynamic mode automatically
  // returns true when inserted, false when already existed
  virtual bool insert(V src, V dest, E edge = E()) = 0;
  // insert or modify automatically
  virtual void modify(V src, V dest, E edge = E()) = 0;
  // returns true when deleted, false when there was not the edge
  virtual bool remove(V src, V dest) = 0;
  //virtual void print() const;
	void goStatic() const; // make data structures ready for algorithms.

	// Return values of algorithms
  struct edge {
  protected:
    V src_;
    V dest_;
    E edge_;
  };

  class path {
  public:
		path(V const &source, vector<std::pair<V, E>> const &path);
    // Size is path length. If it is zero, it means there is no path from
    // ..the source to the target vertex
	public:
		size_t length() const;
		W const total_weight() const;
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
	class weighted_path: public path {
  public:
    E total_weight();
  };

	// solution for a spanning tree (directed graph)
  // ..used for all kinds of shortest path and MST problem (for subclasses)
  class predecessor_table {
  };

  class weighted_predecessor_table: public predecessor_table {
  public:
	weighted_path path_to(V &dest); 
  };

  // solution for Scc or other partition problem
  class components {
  public:
    // size is the num of components, not num of vertices
    int size() const { return components_.size(); }
    graph_base<V, E, W, ExtractWeight> const & get_graph() const { return srcGraph_; }
    int where_is(V const &vertex) const;
    int get_size_of(int set) const { return setSizes_[set]; }
  protected:
    graph_base<V, E, W, ExtractWeight> const &srcGraph_;
    // V maps to a set number it belongs to (0 ~ #sets - 1)
		map<V, int> const components_;
    vector<int> const setSizes_;
  };

public: // Algorithms. If empty return vector means there are no solution
	// Shortest path in terms of the number of edges between vertices
	vector<path> get_closest_path(V const &src, V const &target) const;	
	// Shortest path in terms of weight
	vector<path> get_shortest_path(V const &src, V const &target) const;
	// returns n shortest paths
	vector<path> get_n_shortest_paths(V const &src, V const &target, size_t num) const;
private:
	template <typename T>
		path getPathAfterAlgorithm(vector<Arc<T>> result, int src, int target) const;
	template <typename T>
		vector<path> getPathsAfterAlgorithm(vector<vector<GeneralArc<T>>> result, 
																				int src, int target) const;
		path getPathFromReversedPath(int src, vector<int> const &reversedPath) const;
protected: // member variables
  int numV_; 
  int numE_;
  // true when modifying mode, false when static mode
  mutable bool isDynamic_;
	lib_calvin_adt::IntIndexer<V> mapping_; // 1:1 mapping of verticex and integers
	vector<map<int, E>> dynamicData_; // for dynamic mode, vertex indexed
  // Only valid in static mode
	mutable vector<vector<std::pair<int, W>>> arrayData_; 
	
	// additional data structure for algorithms
  // graph algorithms are performed only with integers (not V type)
	mutable lib_calvin::matrix<lib_calvin_graph::Arc<W>> matrixData_; // For matrix computation.

  // SSSP solution; not solved if size is 0.
  //vector<vector<Arc<W>>> SSSP_; 
	// Apsp solution; not solved if size is 1.
	mutable lib_calvin::matrix<lib_calvin_graph::Arc<W>> apspSolution_; 
}; // end graph
} // end namespace lib_calvin_graph

/************** Adaptor classes for graph representation **************/

// 2008-10-25
// Rationale for this adaptor classes are only for performance and code
// ..simplicity. For example, I wanted my dfs algorithms to perform both on
// ..dynamic(map<>) and array(vector<>) input type, because if it works only
// ..on array type, we have to convert dynamic data into array data each time
// ..dfs is called (which is a waste). 
// And I want to avoid redundant codes for those input types. So I need an 
// ..adaptor.
// 2010-04-05
// Abandoned above change. It introduces needless indirection. Using specific data
// types like vector and map does not harm generality of algorithms, as we can 
// substitue implementations for those containers at will. And algorithms should be
// defined with minimal interface in the first place. 
//----------------------------- END --------------------------------------------

namespace lib_calvin {

template <typename V, typename E, typename W = E, typename ExtractWeight = std::identity<E>>
class graph: public lib_calvin_graph::graph_base<V, E, W, ExtractWeight> {
public:
  bool insert(V src, V dest, E edge) override;
  void modify(V src, V dest, E edge) override;
  bool remove(V src, V dest) override;
	friend class lib_calvin_graph::GraphTest<V, E, W, ExtractWeight>;
};

// Undirected graphs override insertiong and modifying methods to ensure
// ..symmetry at all times.
template <typename V, typename E, typename W = E, typename ExtractWeight = std::identity<E>>
class undirected_graph: public lib_calvin_graph::graph_base<V, E, W, ExtractWeight> {
public:
  // override some methods to ensure symmetry
  bool insert(V src, V dest, E edge) override;
  void modify(V src, V dest, E edge) override;
  bool remove(V src, V dest) override;
	friend class lib_calvin_graph::GraphTest<V, E, W, ExtractWeight>;
};
   
} // end namespace lib_calvin

namespace lib_calvin_graph { // namespace for helper functions

using lib_calvin::matrix;
// should use only this one in actual algorithms! (faster than above)
// It turned out that g++ is so good that it optimizes perfectly in this case:
// ...no need to introduce another integrated function at all! (of course only
// ...in -O3 option)
template <typename W>
bool relax (Arc<W> const &lhs1, Arc<W> const &lhs2, Arc<W> &rhs);

template <typename E, typename W, typename ExtractWeight>
void makeArrayData (vector<map<int, E>> const &dynamicData, 
    vector<vector<pair<int, W>>> &arrayData);

template <typename E>
void makeSymArrayData (vector<map<int, E>> const &dynamicData, 
    vector<vector<pair<int, E>>> &arrayData);

template <typename W>
void makeMatrixData (vector<vector<pair<int, W>>> const &arrayData, 
    matrix<Arc<W>> &matrixData);

template <typename E>
void ripEdge (vector<vector<pair<int, E>>> const &arrayData,
    vector<vector<int>> &ripped);

void transpose (vector<vector<int>> const &source,
    vector<vector<int>> &target);

template <typename E>
void transpose (vector<vector<pair<int, E>>> const &source,
    vector<vector<pair<int, E>>> &target);

} // end namespace lib_calvin_graph

namespace lib_calvin_graph { // namespace for graph algorithms

// dfs on this graph, using visitOrder as starting order of vertices
// ...i.e) starting with vertex visitOrder[0].
// ...save the result(return order of vertices) through argument.
void dfs(vector<vector<int>> const &graph, vector<int> const &visitOrder, 
    vector<int> &returnOrder);

// Using priority queue method
void dfs1(vector<vector<int>> const &graph, vector<int> const &visitOrder, 
    vector<int> &returnOrder);

// Using simple stack
void dfs2(vector<vector<int>> const &graph, 
    vector<int> const &visitOrder, vector<int> &returnOrder);

// Find reachable vertices from a source vertex
// Predecessor table is stored in result
void bfs(vector<vector<int>> const &graph, int source, vector<Arc<int>> &result);

// Find strongly connected components of a directed graph.
// Each value in result indicates the set to which the vertex belongs 
// ..value is in range [0 , #sets - 1]

// Solve Scc using dfs method
void sccByDfs(vector<vector<int>> const &graph, 
    int numSets, vector<int> &result);

// Solve Scc using transitive closure; an inefficient way, for testing
void sccByTc(vector<vector<int>> const &graph, 
    int &numSets, vector<int> &result);

// arcs in result: stores total weight of paths
template <typename W>
void dijkstra(vector<vector<std::pair<int, W>>> const &graph, int source, 
    vector<Arc<W>> &result);

// find n closest paths
template <typename W>
void dijkstra2(vector<vector<std::pair<int, W>>> const &graph, int source, 
    vector<Arc<W>> &result);

template <typename W>
void vellmanFord(vector<vector<std::pair<int, W>>> const &graph, int source,
    vector<Arc<W>> &result);

template <typename W>
void matrixApsp(matrix<Arc<W>> const &graph, matrix<Arc<W>> &result);

template <typename W>
void floydWarshall(matrix<Arc<W>> const &graph, matrix<Arc<W>> &result);

template <typename W>
void johnson(vector<vector<pair<int, W>>> const &graph, 
    matrix<Arc<W>> &result);

// check the validity of shortest path solution
// returns true if no error has been detected, false otherwise
template <typename W>
bool shortestPathCheck(vector<vector<pair<int, W>>> const &graph, int source,
    vector<Arc<W>> &solution);

// Getting transitive closure
// true: edge, false: no edge
template <typename E>
void transitiveClosure(vector<vector<int>>const &graph, 
    matrix<bool> &closure);

// MST algorithms assume that graph is connected and un-directed
// Result is represented as the set of included edges
template <typename W>
void kruskal(vector<vector<pair<int, W>>>const &graph, 
    set<pair<int, int>> &result);

template <typename W>
void prim(vector<vector<pair<int, W>>>const &graph,
    set<pair<int, int>> &result);
      
} // end namespace lib_calvin_graph

/******************* struct Arc<W> definition *********************/

namespace lib_calvin_graph { // open for definitions

template <typename W>
Arc<W>::Arc(): predecessor_(-1), weight_(0) { // must initialize to 0
}

template <typename W>
Arc<W>::Arc(int predecessor, W weight): predecessor_(predecessor), weight_(weight) { 
}

template <typename W>
Arc<W> & 
Arc<W>::operator= (Arc<W> const &rhs) {
  predecessor_ 	= rhs.predecessor_;
  weight_    	= rhs.weight_;
  return *this;
}

// addition of paths
template <typename W>
Arc<W>
Arc<W>::operator* (Arc<W> const &rhs) const {
  if (predecessor_ < 0 || rhs.predecessor_ < 0) {
    return Arc<W> (); // no path
  }
  return ( Arc<W> (rhs.predecessor_, weight_ + rhs.weight_));
}

// take minimum
template <typename W>
Arc<W> const & 
Arc<W>::operator+ (Arc<W> const &rhs) const {
  if (rhs.predecessor_ < 0) 
    return *this;
  if (predecessor_ < 0 || weight_ > rhs.weight_) {
    return rhs;
  }
  return *this; 
}

// take minimum
template <typename W>
Arc<W> & 
Arc<W>::operator+= (Arc<W> const &rhs) {
  if (rhs.predecessor_ < 0) 
    return *this;
  if (predecessor_ < 0 || weight_ > rhs.weight_) {
    predecessor_ = rhs.predecessor_;
    weight_    = rhs.weight_;
    return *this;
  }
  return *this; 
}

// equivalence
template <typename W>
bool Arc<W>::operator== (Arc<W> const &rhs) const {
  if (predecessor_ < 0 && rhs.predecessor_ < 0)
    return true;
  if (predecessor_ == rhs.predecessor_ && weight_ == rhs.weight_)
    return true;
  return false;
}

template <typename W>
bool Arc<W>::operator!= (Arc<W> const &rhs) const {
  if (predecessor_ == rhs.predecessor_ && weight_ == rhs.weight_)
    return false;
  return true;
}

template <typename W>
GeneralArc<W>::GeneralArc(): Arc<W>(), nThClosest_(0) { }

template <typename W>
GeneralArc<W>::GeneralArc(int predecessor, W weight, int nThClosest): 
	Arc<W>(predecessor, weight), nThClosest_(nThClosest) { }

template <typename W>
void GeneralArc<W>::print() const {
	//std::cout << "This arc. predecessor = " << predecessor_ << ", weight: " << weight_ << 
		//"nThClosest: " << nThClosest_ << "\n";
}

template <typename W>
bool GeneralArc<W>::operator< (GeneralArc<W> const &rhs) const {
  return weight_ < rhs.weight_;
}

/******************* Node definition *********************/

template <typename W>
Node<W>::Node(size_t numPathsToFind): numPathsToFind_(numPathsToFind),
		numPathsFoundUntilNow_(0) { }

template <typename W>
Node<W>::Node(Node const &rhs): 
	numPathsToFind_(rhs.numPathsToFind_), numPathsFoundUntilNow_(rhs.numPathsFoundUntilNow_), 
	arcs_(rhs.arcs_) { 
	//std::cout << "Node copy ctor\n";
}

template <typename W>
Node<W>::Node(Node &&rhs): 
	numPathsToFind_(rhs.numPathsToFind_), numPathsFoundUntilNow_(rhs.numPathsFoundUntilNow_), 
	arcs_(std::move(rhs.arcs_)) { 
	//std::cout << "Node move ctor\n";
}

template <typename W>
typename Node<W> &
Node<W>::operator= (Node const &rhs) {
	numPathsToFind_ = rhs.numPathsToFind_;
	numPathsFoundUntilNow_ = rhs.numPathsFoundUntilNow_;
	arcs_ = rhs.arcs_;
	//std::cout << "Node copy assignment\n";
	return *this;
}

template <typename W>
typename Node<W> &
Node<W>::operator= (Node &&rhs) {
	numPathsToFind_ = rhs.numPathsToFind_;
	numPathsFoundUntilNow_ = rhs.numPathsFoundUntilNow_;
	arcs_ = std::move(rhs.arcs_);
	//std::cout << "Node move assignment\n";
	return *this;
}

template <typename W>
void Node<W>::relax(GeneralArc<W> const &arc) { // general arcs should be in ascending order always
	bool isInserted = false;
	//std::cout << "\nbefore node was:\n\t";
	print();
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
	//arc.print();
	//std::cout << "result node was:\n\t";
	//print();
	//std::cout << "\n\n\n";
}

template <typename W>
void Node<W>::print() const {
	//std::cout << "This Node is as below size(" << arcs_.size() << ")\n";
	//for (auto iter = arcs_.begin(); iter != arcs_.end(); iter++) {
		//std::cout <<"\t";
		//iter->print();
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
GeneralArc<W> const & 
Node<W>::getGeneralArc() const {
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
	return getGeneralArc() < rhs.getGeneralArc();
}

template <typename W>
bool Node<W>::isFull() const { return arcs_.size() == numPathsToFind_; }

/***************** struct WeightedEdge<W> definitions ************/

template <typename W>
WeightedEdge<W>::WeightedEdge():
  src_(0), dest_(0), weight_() {
}

template <typename W>
WeightedEdge<W>::WeightedEdge(int src, int dest, W weight):
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

template <typename V, typename E, typename W, typename ExtractWeight>
graph_base<V, E, W, ExtractWeight>::graph_base(): numV_(0), numE_(0), isDynamic_(true), 
												matrixData_(1), apspSolution_(1) {
}

template <typename V, typename E, typename W, typename ExtractWeight>
graph_base<V, E, W, ExtractWeight> &
graph_base<V, E, W, ExtractWeight>::operator= (graph_base<V, E, W, ExtractWeight> const &rhs) {
	if (this == &rhs) {
		return *this;
	}
  numV_       = rhs.numV_;
  numE_       = rhs.numE_,
  isDynamic_    = rhs.isDynamic_;
  mapping_    = rhs.mapping_;
  dynamicData_  = rhs.dynamicData_;
  arrayData_    = rhs.arrayData_;
	matrixData_ = rhs.matrixData_;
  apspSolution_     = rhs.apspSolution_;
  return *this;
}

template <typename V, typename E, typename W, typename ExtractWeight>
bool graph_base<V, E, W, ExtractWeight>::has(V vertex) const {
  if (mapping_.indexOf(vertex) < 0)
    return false;
  return true;
}

template <typename V, typename E, typename W, typename ExtractWeight>
bool graph_base<V, E, W, ExtractWeight>::has(V src, V dest) const {
  int source    = mapping_.indexOf(src);
  int destination = mapping_.indexOf(dest);
  // false if either of vertex is not here
  if (source < 0 || destination < 0)
    return false;
  map<int, E> &connected = dynamicData_[source];
  if (connected.find(destination) == connected.end())
    return false;
  return true;
}

template <typename V, typename E, typename W, typename ExtractWeight>
bool graph_base<V, E, W, ExtractWeight>::insert(V src, V dest, E edge) {
  if (src == dest) { // No self loop !!!
    return false;
  }
  int source = mapping_.insert(src); // auto insertion
  if (source == numV_) { // new vertex
    numV_++;
    dynamicData_.resize(numV_);
  }
  int destination = mapping_.insert(dest); // auto insertion
  if (destination == numV_) { // new vertex
    numV_++;
    dynamicData_.resize(numV_);
  }
  bool isInserted = (dynamicData_[source].insert (
		typename map<int, E>::value_type(destination, edge))).second;
  if (isInserted) {
    isDynamic_ = true; // graph has been modified
    ++numE_;
  }
  return (isInserted);
}

template <typename V, typename E, typename W, typename ExtractWeight>
void graph_base<V, E, W, ExtractWeight>::modify(V src, V dest, E edge) {
  if (src == dest) { // No self loop !!!
    return;
  }
  int source = mapping_.insert(src); // auto insertion
  if (source == numV_) { // new vertex
    numV_++;
    dynamicData_.resize(numV_);
  }
  int destination = mapping_.insert(dest); // auto insertion
  if (destination == numV_) { // new vertex
    numV_++;
    dynamicData_.resize(numV_);
  }
  bool isInserted = (dynamicData_[source].insert (
		typename map<int, E>::value_type(destination, edge))).second;
  if (!isInserted) { // already inserted -> modification
    dynamicData_[source][destination] = edge;
  }
  if (isInserted)
    ++numE_;
  isDynamic_ = true; 
}

template <typename V, typename E, typename W, typename ExtractWeight>
bool graph_base<V, E, W, ExtractWeight>::remove(V src, V dest) {
  int source      = mapping_.indexOf(src);
  int destination = mapping_.indexOf(dest);
  // false if either of vertex is not here
  if (source < 0 || destination < 0) {
    return false;
	}
  size_t erased = dynamicData_[source].erase(destination); // num of erased elements
  if (erased == 0) {
    return false;
	}
  --numE_;
  isDynamic_ = true;
  return true;
}

/*------------- print -------------*/  /*
// assuming the V and E can be printed with cout
template <typename V, typename E, typename W, typename ExtractWeight>
void graph<V, E, ExtractWeight>::print() const {
  for (int i = 0; i < numV_; ++i) { // for each source vertex
    cout <<  mapping_[i] << ": ";
    // print all edges
    map<int, E> const &temp = dynamicData_[i];
    typename map<int, E>::const_iterator iter;
    for (iter = temp.begin(); iter != temp.end(); ++iter) {
      cout << mapping_[iter->first]; // target vertex
      cout << "(" << iter->second << ")  "; // weight
    }
    cout << endl;
  }
}*/

template <typename V, typename E, typename W, typename ExtractWeight>
void graph_base<V, E, W, ExtractWeight>::goStatic() const {
  if (isDynamic_ == false) { // already in static mode
    return;
  }
  // fill static data structures
	lib_calvin_graph::makeArrayData<E, W, ExtractWeight>(dynamicData_, arrayData_);
	lib_calvin_graph::makeMatrixData<W>(arrayData_, matrixData_);
}

// Shortest path in terms of the number of edges
template <typename V, typename E, typename W, typename ExtractWeight>
vector<typename graph_base<V, E, W, ExtractWeight>::path>
graph_base<V, E, W, ExtractWeight>::get_closest_path(V const &src, V const &target) const {
	goStatic();
	vector<path> paths;
	if (!has(src) || !has(target)) { // input is invalid
		std::cout << "get_closest_path input error\n";
		exit(0);
	}
	int srcVertex = mapping_.indexOf(src);
	int targetVertex = mapping_.indexOf(target);
	vector<vector<int>> graph;
	ripEdge(arrayData_, graph);
	vector<Arc<int>> result;
	bfs(graph, srcVertex, result);	
	//std::cout << "after bfs\n";
	if (result[targetVertex].predecessor_ < 0) { // not reachable
	} else {
		paths.push_back(getPathAfterAlgorithm<int>(result, srcVertex, targetVertex));
	}
	return paths;
}

template <typename V, typename E, typename W, typename ExtractWeight>
vector<typename graph_base<V, E, W, ExtractWeight>::path>
graph_base<V, E, W, ExtractWeight>::get_shortest_path(V const &src, V const &target) const {
	goStatic();
	vector<path> paths;
	bool pathExists = true;
	if (!has(src) || !has(target) || src == target) { // input is invalid
		std::cout << "get_shortest_path input error\n";
		exit(0);
	}
	int srcVertex = mapping_.indexOf(src);
	int targetVertex = mapping_.indexOf(target);
	vector<Arc<W>> result;
	dijkstra(arrayData_, srcVertex, result);	
	//std::cout << "after dijkstra\n";
	if (result[targetVertex].predecessor_ < 0) { // not reachable
	} else {
		paths.push_back(getPathAfterAlgorithm<W>(result, srcVertex, targetVertex));
	}
	return paths;
}

template <typename V, typename E, typename W, typename ExtractWeight>
vector<typename graph_base<V, E, W, ExtractWeight>::path>
graph_base<V, E, W, ExtractWeight>::get_n_shortest_paths(V const &src, V const &target, size_t num) const {
	goStatic();
	if (!has(src) || !has(target) || src == target) { // input is invalid, return empty vector
		std::cout << "get_n_shortest_paths input error\n";
		exit(0);
	}
	int srcVertex = mapping_.indexOf(src);
	int targetVertex = mapping_.indexOf(target);
	vector<vector<GeneralArc<W>>> result;
	dijkstra2(arrayData_, srcVertex, result, num);	
	//std::cout << "after dijkstra2\n";
	if (result[targetVertex].empty()) { // not reachable
		return vector<graph_base<V, E, W, ExtractWeight>::path>();
	} else {
		return graph_base<V, E, W, ExtractWeight>::getPathsAfterAlgorithm<W>(result, srcVertex, targetVertex);
	}
}

template <typename V, typename E, typename W, typename ExtractWeight>
template <typename T>
typename graph_base<V, E, W, ExtractWeight>::path
graph_base<V, E, W, ExtractWeight>::getPathAfterAlgorithm(
			vector<Arc<T>> result, int srcVertex, int targetVertex) const {
	vector<int> reversedPath;
	for (int curVertex = targetVertex; curVertex != srcVertex; ) {
		//std::cout << "following predeccsor array\n";
		reversedPath.push_back(curVertex);
		curVertex = result[curVertex].predecessor_;
	}
	return getPathFromReversedPath(srcVertex, reversedPath);
}

template <typename V, typename E, typename W, typename ExtractWeight>
template <typename T>
vector<typename graph_base<V, E, W, ExtractWeight>::path>
graph_base<V, E, W, ExtractWeight>::getPathsAfterAlgorithm(
			vector<vector<GeneralArc<T>>> result, int srcVertex, int targetVertex) const {
	size_t numPaths = result[targetVertex].size();
	vector<vector<int>> reversedPaths(numPaths);
	for (int nThCloest = 0; nThCloest < numPaths; nThCloest++){
		int curNThClosest = nThCloest; // path to curVertex is n'th closest path
		for (int curVertex = targetVertex; curVertex != srcVertex; ) {
			//std::cout << "following predeccsor array\n";
			reversedPaths[nThCloest].push_back(curVertex);
			GeneralArc<T> arc = result[curVertex][curNThClosest];
			curVertex = arc.predecessor_;
			curNThClosest = arc.nThClosest_;
		}
	}
	vector<path> realPaths;
	for (size_t i = 0; i < numPaths; i++) {
		realPaths.push_back(getPathFromReversedPath(srcVertex, reversedPaths[i]));
	}
	return realPaths;
}

template <typename V, typename E, typename W, typename ExtractWeight>
typename graph_base<V, E, W, ExtractWeight>::path
graph_base<V, E, W, ExtractWeight>::getPathFromReversedPath(int srcVertex, vector<int> const &reversedPath) const {
	vector<std::pair<V, E>> realPath;
	int previousVertex = srcVertex;
	int curVertex;
	for (size_t i = 0; i < reversedPath.size(); ++i) {
		//std::cout << "making final result\n";
		curVertex = reversedPath[reversedPath.size() - 1 - i];
		realPath.push_back(std::pair<V, E>(
			mapping_[curVertex], dynamicData_[previousVertex].find(curVertex)->second));
		previousVertex = curVertex;
	}
	return graph_base<V, E, W, ExtractWeight>::path(mapping_[srcVertex], realPath);
}

template <typename V, typename E, typename W, typename ExtractWeight>
graph_base<V, E, W, ExtractWeight>::path::path(V const &source, vector<std::pair<V, E>> const &path): 
			source_(source), path_(path) { }

template <typename V, typename E, typename W, typename ExtractWeight>
size_t graph_base<V, E, W, ExtractWeight>::path::length() const {
	// Size is path length. If it is zero, it means there is no path from
  // ..the source to the target vertex
	return path_.size();
}

template <typename V, typename E, typename W, typename ExtractWeight>
W const
graph_base<V, E, W, ExtractWeight>::path::total_weight() const {
	W totalWeight = 0;
	for (int i = 0; i < path_.size(); i++) {
		totalWeight += ExtractWeight()(path_[i].second);
	}
	return totalWeight;
}

template <typename V, typename E, typename W, typename ExtractWeight>
V const & 
graph_base<V, E, W, ExtractWeight>::path::get_source() const {
	return source_;
}

template <typename V, typename E, typename W, typename ExtractWeight>
V const &
graph_base<V, E, W, ExtractWeight>::path::get_vertex(size_t index) const {
	if (index > 0) {
		return path_[index - 1].first;
	} else {
		return source_;
	}
}
template <typename V, typename E, typename W, typename ExtractWeight>
E const &
graph_base<V, E, W, ExtractWeight>::path::get_edge(size_t index) const {
	return return path_[index].second; 
}

template <typename V, typename E, typename W, typename ExtractWeight>
typename graph_base<V, E, W, ExtractWeight>::path &
graph_base<V, E, W, ExtractWeight>::path::operator= (path const &rhs) {
	source_ = rhs.source_;
	path_ = rhs.path_;
	return *this;
}

template <typename V, typename E, typename W, typename ExtractWeight>
typename graph_base<V, E, W, ExtractWeight>::path &
graph_base<V, E, W, ExtractWeight>::path::operator= (path &&rhs) {
	source_ = rhs.source_;
	path_ = std::forward<vector<std::pair<V, E>>(rhs.path_);
	return *this;
}

} // end namespace lib_calvin_graph

//------------------------- additional methods -------------------------
namespace lib_calvin {

template <typename V, typename E, typename W, typename ExtractWeight>
bool graph<V, E, W, ExtractWeight>::insert(V src, V dest, E edge) {
	return graph_base<V, E, W, ExtractWeight>::insert(src, dest, edge);
}

template <typename V, typename E, typename W, typename ExtractWeight>
void graph<V, E, W, ExtractWeight>::modify(V src, V dest, E edge) {
	graph_base<V, E, W, ExtractWeight>::modify(src, dest, edge);
}

template <typename V, typename E, typename W, typename ExtractWeight>
bool graph<V, E, W, ExtractWeight>::remove(V src, V dest) {
	return graph_base<V, E, W, ExtractWeight>::remove(src, dest);
}

template <typename V, typename E, typename W, typename ExtractWeight>
bool undirected_graph<V, E, W, ExtractWeight>::insert(V src, V dest, E edge) {
  bool result1 = graph_base<V, E, W, ExtractWeight>::insert(src, dest, edge);
  bool result2 = graph_base<V, E, W, ExtractWeight>::insert(dest, src, edge);
  if (result1 != result2) {
    std::cout << "undirected_weighted_graph insert error!!\n";
		exit(0);
	}
  return result1;
}

template <typename V, typename E, typename W, typename ExtractWeight>
void undirected_graph<V, E, W, ExtractWeight>::modify(V src, V dest, E edge) {
  graph_base<V, E, W, ExtractWeight>::modify(src, dest, edge);
  graph_base<V, E, W, ExtractWeight>::modify(dest, src, edge);
}

template <typename V, typename E, typename W, typename ExtractWeight>
bool undirected_graph<V, E, W, ExtractWeight>::remove(V src, V dest) {
  bool result1 = graph_base<V, E, W, ExtractWeight>::remove(src, dest);
  bool result2 = graph_base<V, E, W, ExtractWeight>::remove(dest, src);
  if (result1 != result2) {
    std::cout << "undirected_weighted_graph remove error!!\n";
		exit(0);
	}
  return result1;
}
} // end namespace lib_calvin

//--------------------------- Global functions ----------------------------

namespace lib_calvin_graph { // open for definitions

// return whether the relaxation took place or not
template <typename E>
bool relax (Arc<E> const &lhs1, Arc<E> const &lhs2, Arc<E> &rhs) {
  if (lhs1.predecessor_ < 0 || lhs2.predecessor_ < 0)
    return false;
  if (rhs.predecessor_ < 0 || rhs.weight_ > lhs1.weight_ + lhs2.weight_) {
    rhs.predecessor_ = lhs2.predecessor_;
    rhs.weight_ = lhs1.weight_ + lhs2.weight_;
    return true;
  }
  return false;
}

template <typename E, typename W, typename ExtractWeight>
void makeArrayData (vector<map<int, E>> const &dynamicData, 
    vector<vector<pair<int, W>>> &arrayData) {
  size_t numV = dynamicData.size();
  arrayData.clear();
  arrayData.resize(numV);
  for (size_t i = 0; i < numV; ++i) { // for each source vertex
    map<int, E> const &original = dynamicData[i];
    vector<pair<int, W>> &copy = arrayData[i];
		copy.reserve(original.size());
    for (auto iter = original.begin(); iter != original.end(); ++iter) {
      copy.push_back(std::make_pair(iter->first, ExtractWeight()(iter->second)));
    }
  }
}

template <typename E>
void makeSymArrayData (vector<map<int, E>> const &dynamicData, 
    vector<vector<pair<int, E>>> &arrayData) {
  // dynamicData has edges only for src < dest now.  
  int numV = dynamicData.size();
  arrayData.clear();
  arrayData.resize(numV);
  for (int i = 0; i < numV; ++i) { // for each source vertex
    map<int, E> const &original = dynamicData[i];
    typename map<int, E>::const_iterator iter;
    for (iter = original.begin(); iter != original.end(); ++iter) {
      int src = i;
      int dest = iter->first;
      E const &edge = iter->second;
      arrayData[src].push_back(pair<int, E>(dest, edge));
      arrayData[dest].push_back(pair<int, E>(src, edge));
    }
  }
}

template <typename W>
void makeMatrixData (vector<vector<pair<int, W>>> const &arrayData, 
    matrix<Arc<W>> &matrixData) {    
  int numV = static_cast<int>(arrayData.size());
  // remake matrix (initialization done here: predecessor_ = -1)
  matrixData.reset(numV);
  for (int i = 0; i < numV; ++i) { // for each source vertex
    vector<pair<int, W>> const &original = arrayData[i];
    typename vector<pair<int, W>>::const_iterator iter;
    for (iter = original.begin(); iter != original.end(); ++iter) {
      // setval return a reference to matrix element.
      matrixData.setval(i, iter->first)= Arc<W> (i, iter->second);
    }
    // set self-edge (path of length 0)
    W zero = W();
    matrixData.setval(i, i) = Arc<W> (i, zero);
  }
}

template <typename E>
void ripEdge (vector<vector<pair<int, E>>> const &arrayData,
    vector<vector<int>> &unweighted) {  
  int numV = static_cast<int>(arrayData.size());
  unweighted.clear();
  unweighted.resize(numV);
  for (int i = 0; i < numV; ++i) { // for each source vertex
    vector<pair<int, E>> const &original = arrayData[i];
    vector<int> &copy = unweighted[i];
    typename vector<pair<int, E>>::const_iterator iter;
    for (iter = original.begin(); iter != original.end(); ++iter) {
      copy.push_back(iter->first);
    }
  }
}

template <typename E>
void transpose (vector<vector<pair<int, E>>> const &source,
    vector<vector<pair<int, E>>> &target) {
  int numV = source.size();
  target.clear();
  target.resize(numV);
  for (int i = 0; i < numV; ++i) { // for each source vertex
    vector<pair<int, E>> const &original = source[i];
    typename vector<pair<int, E>>::const_iterator iter;
    for (iter = original.begin(); iter != original.end(); ++iter) {
      target[iter->first].push_back (pair<int, E> (i, iter->second));
    }
  }
}

//------------------------------ Algorithms ------------------------------

// For graph algorithms (priority queue)
using lib_calvin_adt::IntPq;

// arcs in result: stores total weight of paths
template <typename W>
void dijkstra(vector<vector<pair<int, W>>> const &graph, int source, 
    vector<Arc<W>> &result) {  
  int numV = static_cast<int>(graph.size()); 
  IntPq<W> pq(numV);
  vector<bool> isFinished(numV); // mark finished vertices
  result.clear();
  result.resize(numV, Arc<W>()); // predecessor_ = -1 for default
  result[source].predecessor_ = source; // my convention
  // push source vertex with priority 0.
  W zero = W(); // W() must represent 0 value (true for built-in type)
  pq.insert (source, zero); 
  while (pq.size() != 0) {
    // record current top element
    pair<int, W> const &topElem = pq.pop();
    int top = topElem.first;
    isFinished[top] = true; // mark for finished !!
    W weight = topElem.second;
    result[top].weight_ = weight;
    // relaxation
		for (size_t i = 0; i < graph[top].size(); ++i) {
      pair<int, W> const &iter = graph[top][i];
      if (isFinished[iter.first]) // do nothing if already finished
        continue;
      if (pq.insert (iter.first, iter.second + weight) == true) {
        result[iter.first].predecessor_ = top;
      }
    }
  }
}

template <typename W>
void dijkstra2(vector<vector<pair<int, W>>> const &graph, int source, 
							 vector<vector<GeneralArc<W>>> &result, size_t num) {  
	size_t const numPathsToFind = 20;
  int numV = static_cast<int>(graph.size()); 
	typedef Node<W> Node;
	Node emptyNode(num);
  IntPq<Node> pq(numV, emptyNode);
  result.clear();
  result.resize(numV);
	vector<int> numFinishedPath(numV, 0);
	Node zero = emptyNode;
	zero.relax(GeneralArc<W>(source, 0, 0)); // 0'th closest path for itself
  pq.insert(source, zero); 
  while (pq.size() != 0) {
    pair<int, Node> const &topElem = pq.pop();
    int curVertex = topElem.first;  
    Node curNode = topElem.second;
		W const & curWeight = curNode.getGeneralArc().weight_;
		result[curVertex].push_back(curNode.getGeneralArc());
		std::cout << "Node popped curVertex; " << curVertex << ", nTh: " << curNode.getNumPathFoundUntilNow() << 
			" weight = " << curNode.getWeight() << "\n";
		if (result[curVertex].size() > numPathsToFind) {
			std::cout << "dijkstra2 error1\n";
			exit(0);
		}
		for (size_t i = 0; i < graph[curVertex].size(); ++i) {
      pair<int, W> const &iter = graph[curVertex][i];
			int targetVertext = iter.first;
			W const & targetWeight = iter.second;
			if (numFinishedPath[targetVertext] == numPathsToFind) { // this vertex is finished
				continue;
			}
			Node targetNode = emptyNode;
			if (pq.hasKey(targetVertext)) {
				targetNode = pq.getPriority(targetVertext);
			}
			GeneralArc<W> arc(curVertex, curWeight + targetWeight, static_cast<int>(curNode.getNumPathFoundUntilNow()));
			targetNode.relax(arc);
      if (pq.insert(targetVertext, targetNode) == true) {
      } else {
				std::cout << "dijkstra2 error: relaxing error\n";
				exit(0);
			}
    }
		numFinishedPath[curVertex]++;
		curNode.foundPath(); 
		if (curNode.hasRemainingPaths()) { // send right back to the heap
			pq.insert(curVertex, curNode);
		}
  }
}
  
template <typename W>
void vellmanFord(vector<vector<pair<int, W>>> const &graph, 
    int source, vector<Arc<W>> &result) {
  int numV = static_cast<int>(graph.size());
  result.clear();
  result.resize (numV, Arc<W>());
  result[source].predecessor_ = source; // my convention
  W zero = W();
  result[source].weight_ = zero;
  // longest path can take numV -1 vertices
	bool finished = false;
  for (int i = 0; i < numV - 1; ++i) {
		finished = true;
    // relaxation for every edges  
    for (int src = 0; src < numV; src++) {
			for (size_t j = 0; j < graph[src].size(); j++) {
        pair<int, W> const &iter = graph[src][j];
        Arc<W> edge (src, iter.second);
        if (relax(result[src], edge, result[iter.first])) {
          finished = false;
				}
        //result[iter->first] += result[src] * edge;
      }
    }
    if (finished) { // no relaxation; finished
      break;
    }
  }
  // negative weighted cycle detection
  vector<Arc<W>> temp = result;
  for (int src = 0; src < numV; src++) {
    for (int i = 0; i < static_cast<int>(graph[src].size()); ++i) {
      pair<int, W> const &iter = graph[src][i];
      Arc<W> edge (src, iter.second);
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
void matrixApsp (matrix<Arc<W>> const &graph, 
    matrix<Arc<W>> &result) {
  int numV = graph.height();
  result.reset(graph); // optimal solution of path length 1 (base case)
  // get ceiling of lg(numV)
  int lg_V = lib_calvin_util::log<int> (numV - 1);
  cout << "matrixApsp iteration: " << lg_V << endl;
  // matrix multiplication lg_V times
  for (int i = 0; i < lg_V + 1; ++i) {
    matrix<Arc<W>> temp = result;
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
  for (int i = 0 ; i < numV; ++i) {
    Arc<W> temp = result.getval(i, i);
    if (temp.weight_ < W()) {
      cout << "matrixApsp negative weighted cycle detected!!\n";
      return;
    }
  }
}

// Slower than johnson upto E = 0.5 * V^0.5; fast in very dense graph
// There is a room for performance boost: making * and + operations of struct
// ...Arc simpler by omitting consideration for infinity (no edge). To do so, 
// ...there should a limit on possible edge weight of graph. And then, we can
// ...use a very big positive weight to represent no-edge. But it turned out 
// ...that it provides only about 10% boost, so I rejected it because it hampers
// ...design and clarity. 
template <typename W>
void floydWarshall (matrix<Arc<W>> const &graph, 
    matrix<Arc<W>> &result) {  
  int numV = graph.height();
  result.reset(graph); // copy
  for (int m = 0; m < numV; m++) { // m is the vertex that is being added
    for (int i = 0; i < numV; ++i) {
      for (int j = 0; j < numV; j++) {
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
void johnson (vector<vector<pair<int, W>>> const &graph, 
    matrix<Arc<W>> &result) {
  int numV = static_cast<int>(graph.size());
  W zero = W();
  result.reset(numV);
  // Use vellmanFord to calculate offsets 
  vector<W> offsets(numV); // offset for each vertex
  vector<vector<pair<int, W>>> tempGraph = graph;
  tempGraph.push_back(vector<pair<int, W>> (numV)); // artificial vertex
  for (int i = 0; i < numV; ++i) { 
    tempGraph[numV][i] = pair<int, W> (i, zero); 
  }
  vector<Arc<W>> tempArcs;
  vellmanFord (tempGraph, numV, tempArcs);
  for (int i = 0; i < numV; ++i) {
    offsets[i] = tempArcs[i].weight_;
  }
  // Use offsets to adjust graph to a positive weighted one 
  vector<vector<pair<int, W>>> &positiveGraph = tempGraph;
  positiveGraph.pop_back(); // remove artificial vertex
  for (int i = 0; i < numV; ++i) {
    typename vector<pair<int, W>>::iterator iter;
    for (iter = positiveGraph[i].begin(); 
        iter != positiveGraph[i].end(); ++iter) {
      // + source offset, - dest offset to edges
      iter->second += (offsets[i] - offsets[iter->first]);
    }
  }
  // Use dijkstra on transformed graph for every source vertex 
  vector<Arc<W>> SSSP_result; // SSSP result
  for (int i = 0; i < numV; ++i) {
    dijkstra (positiveGraph, i, SSSP_result);
    // copy SSSP result into matrix row
    for (int j = 0; j < numV; j++) {
      result.setval(i, j) = SSSP_result[j];
      // - source offset, + dest offset to edges
      result.setval(i, j).weight_ -= (offsets[i] - offsets[j]);
    }
  }
}

template <typename W>
bool shortestPathCheck (vector<vector<pair<int, W>>> const &graph, 
    int source, vector<Arc<W>> &solution) {  
  int numV = static_cast<int>(graph.size());
  if (solution[source].predecessor_ < 0 || solution[source].weight_ != W()) {
		cout << "input error\n";
    return false;
  }
  typename vector<pair<int, W>>::const_iterator iter;
  for (int i = 0; i < numV; ++i) { // i: source vertex
    for (iter = graph[i].begin(); iter != graph[i].end(); ++iter) {
      int src = i;
      int dest = iter->first;
      W weight = iter->second;
      if (solution[src].predecessor_ != -1 && 
          (solution[src].weight_ + weight < solution[dest].weight_
           || solution[dest].predecessor_ == -1)) {
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
void kruskal(vector<vector<pair<int, W>>> const &graph,
    set<pair<int, int>> &result) {  
  int numV = static_cast<int>(graph.size());
  int numE, curE; // curE: current num of edges in temporary MST
  W total = W(); // for debugging
  vector<WeightedEdge<W>> edges;
	lib_calvin_adt::DisjointSet sets(numV);
  result.clear();
  // Insert all edges into an array
	for (int src = 0; src < numV; src++) {
		for (int i = 0; i < static_cast<int>(graph[src].size()); ++i) {
      pair<int, W> const &iter = graph[src][i];
      // Take only half of the entire vertices.
      if (src < iter.first) {
        WeightedEdge<W> temp(src, iter.first, iter.second);
        edges.push_back(temp);
      }
    }
  }
  numE = static_cast<int>(edges.size());
  curE = 0;
  // Sorting
	lib_calvin::introSort(edges.begin(), edges.end());
  // Growing MST
  for (int i = 0; i < numE; ++i) {
    // Insert edge if it connects two sets
    if (!sets.isSameSet(edges[i].src_, edges[i].dest_)) {
      sets.unite(edges[i].src_, edges[i].dest_);
      result.insert(pair<int, int>(edges[i].src_, edges[i].dest_));
      total += edges[i].weight_;
      ++curE;
      if (curE == numV - 1) // MST is completed
        break;
    }
  }
  cout << "kruskal: total weight is " << total << endl;
  for (int i = 0; i < numV - 1; ++i) {
		if (sets.isSameSet(i, i + 1) == false) {
      cout << "kruskal fatal error!! (not connected)\n";
			exit(0);
		}
  }
}
    
template <typename W>
void prim(vector<vector<pair<int, W>>>const &graph, set<pair<int, int>> &result) {  
  int numV = static_cast<int>(graph.size()); 
  vector<int> predecessor (numV, -1); // to save predecessor vertices
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
    pair<int, W> const &topElem = pq.pop();
    int top = topElem.first;
    connected[top] = true;
    // insert in order
    if (predecessor[top] < top)
      result.insert(pair<int, int>(predecessor[top], top));
    else if (predecessor[top] > top)
      result.insert(pair<int, int>(top, predecessor[top]));
    else { // do nothing for the starting vertex
    }
    total += topElem.second;
    // relaxation
		int src = top; // rename
		for (int i = 0; i < static_cast<int>(graph[src].size()); ++i) {
      pair<int, W> const &iter = graph[src][i];
      if (connected[iter.first]) // do nothing for already connected one
        continue;
      if (pq.insert (iter.first, iter.second) == true) {
        predecessor[iter.first] = src;
      }
    }
  }
  cout << "prim: total weight is " << total << endl;
} 
} // end namespace lib_calvin_graph for definitions

#endif

