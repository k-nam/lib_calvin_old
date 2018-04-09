#include <iostream>

#include "graph_bench.h"
#include "boost/graph/adjacency_list.hpp"
#include "boost_graph.h"

struct Node {
	Node() {}
	Node(size_t id): id_(id) {}
	size_t id_;
	bool operator< (Node const &rhs) const { return id_ < rhs.id_; }
};

struct Link {
	Link() {}
	Link(size_t head): head_(head) {}
	size_t head_;
	size_t length_;
};

void lib_calvin_benchmark::graph::graphBench() {
	using std::cout;
	using namespace boost;
	
	typedef property<vertex_all_t, Node> VertexProperty;
	typedef property<edge_weight_t, Link> EdgeProperty;

	typedef adjacency_list<setS, setS, directedS, VertexProperty, EdgeProperty> Graph;
	typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
	typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;

	Graph graph;
	vertex_descriptor v1 = add_vertex(graph);
	vertex_descriptor v2 = add_vertex(graph);
	edge_descriptor e1 = add_edge(v1, v2, graph).first;

	//typedef typename property_map<Graph, vertex_all_t>::type VertexPropertyMapType;
	//typedef typename property_map<Graph, edge_weight_t>::type EdgePropertyMapType;
	//VertexPropertyMapType vertex_all_map = get(vertex_all, graph);
	//EdgePropertyMapType edge_weight_map = get(edge_weight, graph);

	//property_traits<property_map<Graph, vertex_all_t>::const_type>::value_type a =
	//boost::get(vertex_all, graph, v1);

	Node b = get(vertex_all, graph, v1).m_value;
	put(vertex_all, graph, v2, Node());
	put(edge_weight, graph, e1, Link());
	num_vertices(graph);

	std::cout << "v1: " << v1 << "\n";
	std::cout << "v2: " << v2 << "\n";
	std::cout << "e1: " << e1 << "\n";	

	
	benchmark::graph::boost_graph<Node, Link> aa;
	Node v11(1);
	Node v12(2);
	Node v13(3);
	Node v14(4);

	Link e11(10);
	Link e12(11);
	Link e13(12);

	aa.insert_vertex(v11);
	aa.insert_vertex(v12);
	aa.insert_vertex(v13);
	aa.insert_vertex(v14);

	aa.get_vertex(v11);

	aa.insert_edge(v11, v12, e11);
	aa.insert_edge(v11, v13, e12);
	aa.insert_edge(v11, v14, e13);

	aa.remove_edge(v11, v14);

	aa.get_vertex_edge_pairs_from(v11);	
}