#ifndef LIB_CALVIN__BOOST_GRAPH_H
#define LIB_CALVIN__BOOST_GRAPH_H

#include "vector.h"
#include "boost/graph/adjacency_list.hpp"
#include "boost/container/map.hpp"

namespace benchmark 
{
	namespace graph 
	{

		template <typename V, typename E = null_edge, typename K = V, typename ExtractKey = Identity<V>>
		class boost_graph {
		public:
			typedef boost::property <boost::vertex_all_t, V> vertex_property_t;
			typedef boost::property <boost::edge_weight_t, E> edge_property_t;
			typedef boost::adjacency_list<boost::setS, boost::setS, boost::directedS, 
				vertex_property_t, edge_property_t> Graph;
			typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
			typedef typename boost::graph_traits<Graph>::edge_descriptor edge_descriptor;
			typedef boost_graph<V, E, K, ExtractKey> self_t;

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
			/// modifying methods force enter size_to dynamic mode automatically
			// returns true when inserted, false when already existed
			bool insert_vertex(V const &src);
			virtual bool insert_edge(K const &src, K const &dest, E const &edge = E());
			// returns true when deleted, false when there was not the edge
			virtual bool remove_edge(K const &src, K const &dest);
			E const & get_edge(K const &src, K const &dest) const;
			E & get_edge(K const &src, K const &dest);
			lib_calvin::vector<std::pair<V, E>> get_vertex_edge_pairs_from(K const &src) const;
			lib_calvin::vector<V> get_vertices_to(K const &dest) const;

		private:
			Graph graph_;
			boost::container::map<K, vertex_descriptor> key_to_descriptor_;
		}; 


		template <typename V, typename E, typename K, typename ExtractKey>
		boost_graph<V, E, K, ExtractKey>::boost_graph(): graph_(), key_to_descriptor_() {
		}

		template <typename V, typename E, typename K, typename ExtractKey>
		boost_graph<V, E, K, ExtractKey>::boost_graph(self_t const &rhs):
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
	}
}



#endif