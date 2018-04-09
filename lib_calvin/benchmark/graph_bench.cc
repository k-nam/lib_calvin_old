#include <iostream>

#include "graph_bench.h"
#include "boost/graph/adjacency_list.hpp"
#include "boost_graph.h"

struct Node {
	size_t id_;
};

struct Link {
	size_t head_;
	size_t length_;
};

void lib_calvin_benchmark::graph::graphBench() {
	using std::cout;
	using namespace boost;

	typedef boost::property<vertex_all_t, Node> VertexProperty;
	typedef boost::property<edge_weight_t, Link> EdgeProperty;


	typedef adjacency_list<setS, setS, directedS, VertexProperty, EdgeProperty> Graph;
	typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
	typedef typename boost::graph_traits<Graph>::edge_descriptor edge_descriptor;

	Graph graph;
	vertex_descriptor v1 = add_vertex(graph);
	vertex_descriptor v2 = add_vertex(graph);
	edge_descriptor e1 = add_edge(v1, v2, graph).first;

	typedef typename property_map<Graph, vertex_all_t>::type VertexPropertyMapType;
	typedef typename property_map<Graph, edge_weight_t>::type EdgePropertyMapType;

	VertexPropertyMapType vertex_all_map = get(vertex_all, graph);
	EdgePropertyMapType edge_weight_map = get(edge_weight, graph);


	get(vertex_all_map, v1);
	put(vertex_all_map, v1, Node());
	put(edge_weight_map, e1, Link());
	num_vertices(graph);

	std::cout << "v1: " << v1 << "\n";
	std::cout << "v2: " << v2 << "\n";

	std::cout << "e1: " << e1 << "\n";



	//boost_graph<Node, Link> a;
}