#ifndef LIB_CALVIN__GRAPH__SIMPLE_GRAPH_H
#define LIB_CALVIN__GRAPH__SIMPLE_GRAPH_H

#include <utility>
#include <map>
#include "map.h"
#include "set.h"
#include "vector.h"
#include "stopwatch.h"
#include "utility.h"
#include "container.h"
#include "rb_tree.h"
#include "graph.h"



namespace lib_calvin_graph {
	// Generic directed graph.
	// Does not permit self-loop or multiple edges (will be extended later)
	// V must support '<'and '==' operator (key).
	// At least two subclasses will be derived from this: undirected graph and
	// ..network flow.

	using lib_calvin::vector;
	using lib_calvin::set;
	using lib_calvin::btree_map;

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
		// Warning! this container must preserve positions of elems (B-tree should not be used)
		lib_calvin_container::RbTree<V, K, std::less<K>, ExtractKey> vertices_;
		//std::map<K, std::map<K, E>> outLinks_;
		map<K, map<K, std::pair<V *, E>>> outLinks_;
		//map<K, set<K>> inLinks_;

	private:
		size_t numEdges_;
	}; // end graph

} // end namespace lib_calvin_graph


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
		vertices_ = rhs.vertices_;
		outLinks_ = rhs.outLinks_;
		//inLinks_ = rhs.inLinks_;
		return *this;
	}
	template <typename V, typename E, typename K, typename ExtractKey>
	size_t simple_graph<V, E, K, ExtractKey>::size() const {
		return vertices_.size();
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	size_t simple_graph<V, E, K, ExtractKey>::number_of_vertex() const {
		return vertices_.size();
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	size_t simple_graph<V, E, K, ExtractKey>::number_of_edge() const {
		return numEdges_;
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	bool simple_graph<V, E, K, ExtractKey>::has_vertex(K vertex) const {
		return outLinks_.count(vertex) != 0;
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	bool simple_graph<V, E, K, ExtractKey>::has_edge(K src, K dest) const {
		auto iter = outLinks_.find(src);
		if (iter == outLinks_.end()) {
			return false;
		} else {
			return iter->second.count(dest) != 0;
		}
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
	bool simple_graph<V, E, K, ExtractKey>::insert_vertex(V const &src) {
		return vertices_.insert(src).second;
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	bool simple_graph<V, E, K, ExtractKey>::insert_edge(K const &src, K const &dest, E const &edge) {
		outLinks_[src][dest] = std::make_pair(& get_vertex(dest), edge);
		//inLinks_[dest].insert(src);
		numEdges_++;
		return true;
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	bool simple_graph<V, E, K, ExtractKey>::remove_edge(K const &src, K const &dest) {
		if (outLinks_[src].erase(dest) > 0) {
			//inLinks_[dest].erase(src);
			numEdges_--;
			return true;
		} else {
			return false;
		}
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	E const &
		simple_graph<V, E, K, ExtractKey>::get_edge(K const &src, K const &dest) const {

		return outLinks_.find(src)->second.find(dest)->second.second;
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	E &
		simple_graph<V, E, K, ExtractKey>::get_edge(K const &src, K const &dest) {
		return const_cast<E &>(const_cast<simple_graph<V, E, K, ExtractKey> const *>(this)->get_edge(src, dest));
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	vector<std::pair<V, E>>
		simple_graph<V, E, K, ExtractKey>::get_vertex_edge_pairs_from(K const &src) const {

		vector<std::pair<V, E>> result;
		auto iter = outLinks_.find(src);
		if (iter == outLinks_.end()) {
		} else {
			for (auto pair: iter->second) {
				result.push_back(std::make_pair(*pair.second.first, pair.second.second));
			}
		}
		return result;
	}

	/*
	template <typename V, typename E, typename K, typename ExtractKey>
	lib_calvin::vector<V>
		simple_graph<V, E, K, ExtractKey>::get_vertices_to(K const &dest) const {
		vector<V> result;
		for (auto vertex: inLinks_.find(dest)->second) {
			result.push_back(*vertices_.find(vertex));
		}
		return result;
	}*/

} // end namespace lib_calvin_graph


#endif

