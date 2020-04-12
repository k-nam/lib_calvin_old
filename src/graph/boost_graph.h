#ifndef LIB_CALVIN__BOOST_GRAPH_H
#define LIB_CALVIN__BOOST_GRAPH_H

#include <utility>
#include "vector.h"
#include "boost/graph/adjacency_list.hpp"
#include "boost/container/map.hpp"
#include "graph.h"

namespace lib_calvin_graph {
	template <typename V, typename E = null_edge, typename K = V, typename ExtractKey = Identity<V>>
	class boost_graph {
	public:
		typedef typename boost::property<boost::vertex_name_t, V> vertex_property_t;
		typedef typename boost::property<boost::edge_weight_t, E> edge_property_t;
		typedef boost::adjacency_list<boost::setS, boost::setS, boost::directedS,
			vertex_property_t, edge_property_t> graph_t;
		typedef typename boost::graph_traits<graph_t>::vertex_descriptor vertex_descriptor;
		typedef typename boost::graph_traits<graph_t>::edge_descriptor edge_descriptor;
		typedef boost_graph<V, E, K, ExtractKey> self_t;
		typedef typename boost::property_map<graph_t, boost::vertex_name_t>::type vertex_property_map_t;
		typedef typename boost::property_map<graph_t, boost::edge_weight_t>::type edge_property_map_t;

	public: // basic data access
		boost_graph();
		boost_graph(boost_graph const &);
		boost_graph & operator= (boost_graph<V, E, K, ExtractKey> const &);
	public:
		size_t size() const;
		size_t number_of_vertex() const;
		size_t number_of_edge() const;
		bool has_vertex(K const &key) const;
		bool has_edge(K const &src, K const &dest) const;
		V const & get_vertex(K const &) const;
		V & get_vertex(K const &);
	public:
		// returns true when inserted
		bool insert_vertex(V const &src);
		bool insert_edge(K const &src, K const &dest, E const &edge = E());
		// returns true when deleted
		bool remove_edge(K const &src, K const &dest);
		E const & get_edge(K const &src, K const &dest) const;
		E & get_edge(K const &src, K const &dest);
		lib_calvin::vector<std::pair<V, E>> get_vertex_edge_pairs_from(K const &src) const;
	private:
		graph_t graph_;
		boost::container::map<K, vertex_descriptor> key_to_descriptor_;
	};


	template <typename V, typename E, typename K, typename ExtractKey>
	boost_graph<V, E, K, ExtractKey>::boost_graph() : graph_(), key_to_descriptor_() {
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	boost_graph<V, E, K, ExtractKey>::boost_graph(self_t const &rhs) :
		graph_(rhs.graph_), key_to_descriptor_(rhs.key_to_descriptor_) {
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	boost_graph<V, E, K, ExtractKey> &
		boost_graph<V, E, K, ExtractKey>::operator=(self_t const &rhs) {
		graph_ = rhs.graph_;
		key_to_descriptor_ = rhs.key_to_descriptor_;
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	size_t boost_graph<V, E, K, ExtractKey>::size() const {
		return boost::num_vertices(graph_);
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	size_t boost_graph<V, E, K, ExtractKey>::number_of_vertex() const {
		return boost::num_vertices(graph_);
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	size_t boost_graph<V, E, K, ExtractKey>::number_of_edge() const {
		return boost::num_edges(graph_);
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	bool boost_graph<V, E, K, ExtractKey>::has_vertex(K const &key) const {
		return key_to_descriptor_.find(key) != key_to_descriptor_.end();
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	bool boost_graph<V, E, K, ExtractKey>::has_edge(K const &src, K const &dest) const {
		return boost::edge(key_to_descriptor_.find(src)->second,
						   key_to_descriptor_.find(dest)->second,
						   graph_).second;
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	V const & boost_graph<V, E, K, ExtractKey>::get_vertex(K const &key) const {
		return boost::get(boost::vertex_name, graph_, key_to_descriptor_.find(key)->second);
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	V & boost_graph<V, E, K, ExtractKey>::get_vertex(K const &key) {
		return const_cast<V &>(const_cast<self_t const &>(*this).get_vertex(key));
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	bool boost_graph<V, E, K, ExtractKey>::insert_vertex(V const &vertex) {
		if (key_to_descriptor_.find(ExtractKey()(vertex)) == key_to_descriptor_.end()) {
			vertex_descriptor new_vertex = boost::add_vertex(vertex, graph_);
			key_to_descriptor_[ExtractKey()(vertex)] = new_vertex;
			return true;
		} else {
			return false;
		}
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	bool boost_graph<V, E, K, ExtractKey>::insert_edge(K const &src, K const &dest, E const &edge) {
		vertex_descriptor v1 = key_to_descriptor_[src];
		vertex_descriptor v2 = key_to_descriptor_[dest];

		auto result = boost::add_edge(v1, v2, edge, graph_);
		return result.second;
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	bool boost_graph<V, E, K, ExtractKey>::remove_edge(K const &src, K const &dest) {
		if (!has_edge(src, dest)) {
			return false;
		} else {
			vertex_descriptor v1 = key_to_descriptor_[src];
			vertex_descriptor v2 = key_to_descriptor_[dest];
			boost::remove_edge(v1, v2, graph_);
			return true;
		}
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	E const & boost_graph<V, E, K, ExtractKey>::get_edge(K const &src, K const &dest) const {
		edge_descriptor e = boost::edge(key_to_descriptor_.find(src)->second,
										key_to_descriptor_.find(dest)->second,
										graph_).first;
		return boost::get(boost::edge_weight, graph_, e);
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	E & boost_graph<V, E, K, ExtractKey>::get_edge(K const &src, K const &dest) {
		return const_cast<E &>(const_cast<self_t const &>(*this).get_edge(src, dest));
	}

	template <typename V, typename E, typename K, typename ExtractKey>
	lib_calvin::vector<std::pair<V, E>>
		boost_graph<V, E, K, ExtractKey>::get_vertex_edge_pairs_from(K const &src) const {

		lib_calvin::vector<std::pair<V, E>> result;

		vertex_descriptor v = key_to_descriptor_.find(src)->second;
		auto iters = boost::out_edges(v, graph_);
		for (auto iter = iters.first; iter != iters.second; ++iter) {
			result.push_back(std::make_pair(
				boost::get(boost::vertex_name, graph_, boost::target(*iter, graph_)),
				boost::get(boost::edge_weight, graph_, *iter)
			));
		}
		return result;
	}
}




#endif