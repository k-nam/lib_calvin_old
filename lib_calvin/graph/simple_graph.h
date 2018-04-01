#ifndef LIB_CALVIN__GRAPH__SIMPLE_GRAPH_H 
#define LIB_CALVIN__GRAPH__SIMPLE_GRAPH_H 

#include <utility>
#include "adt.h"
#include "set.h"
#include "map.h"
#include "vector.h"
#include "stopwatch.h" 
#include "utility.h" 
#include "container.h"



namespace lib_calvin_graph {

	using lib_calvin::map;
	using lib_calvin::vector;
	using lib_calvin::set;
	using std::pair;
	using std::cout;
	using std::endl;
	using lib_calvin_container::Identity;
	
	class null_edge { };
	
	// Generic directed graph.
	// Does not permit self-loop or multiple edges (will be extended later)
	// V must support '<'and '==' operator (key).
	// At least two subclasses will be derived from this: undirected graph and
	// ..network flow.
	template <typename V, typename E = null_edge, typename K = V, typename ExtractKey = Identity<V>>
	class simple_graph {
	public: // basic data access
		simple_graph();
		simple_graph(simple_graph const &);
		simple_graph & operator= (simple_graph<V, E, K, ExtractKey> const &);
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

	protected:
		lib_calvin_adt::IntIndexer<K> mapping_; // 1:1 mapping of verticex and size_tegers
		lib_calvin_container::RbTree<V, K, std::less<K>, ExtractKey> vertices_;
		//lib_calvin_container::HashTable<V, K, ExtractKey> vertices_;
		map<size_t, map<size_t, E>> outLinks_;
		map<size_t, set<size_t>> inLinks_;

	private:
		std::pair<size_t, bool> insertVertex(V const &);
		// src and dest must be present
		bool hasEdge(size_t src, size_t dest) const;
	private:
		size_t numEdges_;
	}; // end graph

} // end namespace lib_calvin_graph

  //----------------------------- graph<V, E, ExtractWeight> methods ------------------------

namespace lib_calvin_graph { // open for definitions

	template <typename V, typename E, typename K, typename ExtractKey>
	simple_graph<V, E, K, ExtractKey>::simple_graph() : numEdges_(0) { }

	template <typename V, typename E, typename K, typename ExtractKey>
	simple_graph<V, E, K, ExtractKey>::simple_graph(simple_graph const &rhs) {
		*this = rhs;
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	simple_graph<V, E, K, ExtractKey> &
		simple_graph<V, E, K, ExtractKey>::operator= (simple_graph<V, E, K, ExtractKey> const &rhs) {
		if (this == &rhs) {
			return *this;
		}
		numEdges_ = rhs.numEdges_;
		mapping_ = rhs.mapping_;
		outLinks_ = rhs.outLinks_;
		inLinks_ = rhs.inLinks_;
		return *this;
	}
	template <typename V, typename E, typename K, typename ExtractKey>
	size_t simple_graph<V, E, K, ExtractKey>::size() const {
		return mapping_.size();
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	size_t simple_graph<V, E, K, ExtractKey>::number_of_vertex() const {
		return size();
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	size_t simple_graph<V, E, K, ExtractKey>::number_of_edge() const {
		return numEdges_;
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	bool simple_graph<V, E, K, ExtractKey>::has_vertex(K vertex) const {
		return mapping_.indexOf(vertex).second == true;
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	bool simple_graph<V, E, K, ExtractKey>::has_edge(K src, K dest) const {
		auto result1 = mapping_.indexOf(src);
		auto result2 = mapping_.indexOf(dest);
		return hasEdge(result1.first, result2.first);
	}
	template <typename V, typename E, typename K, typename ExtractKey>
	V const &
		simple_graph<V, E, K, ExtractKey>::get_vertex(K const &key) const {
		return *vertices_.find(key);
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	V &
		simple_graph<V, E, K, ExtractKey>::get_vertex(K const &key) {
		return *vertices_.find(key);
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	bool simple_graph<V, E, K, ExtractKey>::hasEdge(size_t srcId, size_t destId) const {
		return outLinks_.find(srcId)->second.count(destId) != 0;
	}
	template <typename V, typename E, typename K, typename ExtractKey>
	bool simple_graph<V, E, K, ExtractKey>::insert_vertex(V const &src) {
		return insertVertex(src).second;
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	std::pair<size_t, bool>
		simple_graph<V, E, K, ExtractKey>::insertVertex(V const &src) {
		auto result = mapping_.insert(ExtractKey()(src));
		vertices_.insert(src);
		if (result.second == true) {
			outLinks_.insert(std::make_pair(result.first, map<size_t, E>()));
			inLinks_.insert(std::make_pair(result.first, set<size_t>()));
		}
		return result;
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	bool simple_graph<V, E, K, ExtractKey>::insert_edge(K const &src, K const &dest, E const &edge) {
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
			numEdges_++;
			return true;
		}
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	bool simple_graph<V, E, K, ExtractKey>::remove_edge(K const &src, K const &dest) {
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
			numEdges_--;
			return true;
		}
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	E const &
		simple_graph<V, E, K, ExtractKey>::get_edge(K const &src, K const &dest) const {
		size_t srcId = mapping_.indexOf(src).first;
		size_t destId = mapping_.indexOf(dest).first;
		return outLinks_.find(srcId)->second.find(destId)->second;
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	E &
		simple_graph<V, E, K, ExtractKey>::get_edge(K const &src, K const &dest) {
		return const_cast<E &>(const_cast<simple_graph<V, E, K, ExtractKey> const *>(this)->get_edge(src, dest));
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	vector<std::pair<V, E>>
		simple_graph<V, E, K, ExtractKey>::get_vertex_edge_pairs_from(K const &src) const {
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
		simple_graph<V, E, K, ExtractKey>::get_vertices_to(K const &dest) const {
		size_t destId = mapping_.indexOf(dest).first;
		vector<V> result;
		result.reserve(inLinks_[destId].size());
		for (auto iter = inLinks_[destId].begin(); iter != inLinks_[destId].end(); ++iter) {
			result.push_back(*vertices_.find(mapping_[*iter]));
		}
		return result;
	}

} // end namespace lib_calvin_graph


#endif

