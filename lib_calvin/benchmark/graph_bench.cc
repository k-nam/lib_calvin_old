#include <iostream>
#include <algorithm>

#include "graph_bench.h"
#include "boost/graph/adjacency_list.hpp"
#include "simple_graph.h"
#include "boost_graph.h"

#include "bench.h"
#include "stopwatch.h"

using namespace lib_calvin_benchmark::graph;

std::string
lib_calvin_benchmark::graph::getStringFromNumber(size_t num) {
	if (num < 1000* 1000) {
		return std::to_string(num / 1000) + "K";
	} else {
		return std::to_string(num / 1000 / 1000) + "M";
	}
}

std::string 
lib_calvin_benchmark::graph::getTitle(SubCategory sub, size_t size1, size_t size2) {
	if (sub == BASIC_SPARSE || sub == BASIC_DENSE) {
		return "# Vertices: " + getStringFromNumber(size1) + " / # Edges: " + getStringFromNumber(size2);
	} else if (sub == SUFFIX_TREE) {
		return "Charset size: " + getStringFromNumber(size1) + " / Text length: " + getStringFromNumber(size2);
	} else {
		return "";
	}
}

std::string 
lib_calvin_benchmark::graph::getSubCategory(SubCategory sub) {
	if (sub == BASIC_SPARSE) {
		return "Basic operations / Sparse";
	} else if (sub == BASIC_DENSE) {
		return "Basic operations / Dense";
	} else {
		return "Suffix tree performance";
	}
}

std::vector<std::string> lib_calvin_benchmark::graph::getAlgorithmNamesAndTags(Algorithm algo) {
	if (algo == LIB_CALVIN_GRAPH) {
		return { "lib_calvin::graph" };
	} else if (algo == BOOST_GRAPH) {
		return { "boost::graph" };
	} else {
		return { "error" };
	}
}

std::vector<std::vector<std::string>> 
lib_calvin_benchmark::graph::getAlgorithmNamesAndTagsVector(std::vector<Algorithm> algorithms) {
	using namespace std;
	vector<vector<string>> result = {};
	std::for_each(algorithms.begin(), algorithms.end(),
				  [&result](Algorithm algo) {
		result.push_back(getAlgorithmNamesAndTags(algo)); });
	return result;
}

std::vector<Algorithm> 
lib_calvin_benchmark::graph::getAllAlgorithms() {
	return { LIB_CALVIN_GRAPH, BOOST_GRAPH };
}

std::vector<std::vector<std::string>>
lib_calvin_benchmark::graph::getAllAlgorithmNamesAndTagsVector() {
	return getAlgorithmNamesAndTagsVector(getAllAlgorithms());
}

void lib_calvin_benchmark::graph::graphBench() {
	graphBench(BASIC_SPARSE);
	graphBench(BASIC_DENSE);
	graphBench(SUFFIX_TREE);
}

void  lib_calvin_benchmark::graph::graphBench(SubCategory sub) {
	if (sub == BASIC_SPARSE) {
		basicOperationBench(sub, 1000, 5);
		basicOperationBench(sub, 1000 * 10, 5);
		basicOperationBench(sub, 1000 * 100, 5);
		basicOperationBench(sub, 1000 * 1000, 5);
	} else if (sub == BASIC_DENSE) {
		basicOperationBench(sub, 10000, 100);
		basicOperationBench(sub, 10000, 1000);
		basicOperationBench(sub, 10000, 10000);
	} else if (sub == SUFFIX_TREE) {
		suffixTreeBench(2, 100 * 1000);
		suffixTreeBench(26, 100 * 1000);
		suffixTreeBench(2, 1000 * 1000);
		suffixTreeBench(26, 1000 * 1000);
	} else {

	}
}

// Do a run
void  lib_calvin_benchmark::graph::basicOperationBench(SubCategory sub, size_t num_v, size_t num_e) {
	static size_t benchOrder = 0;

	std::vector<std::vector<double>> results;
	std::vector<std::string> testCases = { "Basic operation" };
	auto algorithms = getAllAlgorithms();

	results.push_back({ basicOperationBenchTemplate<lib_calvin_graph::simple_graph<Node, Link>>(num_v, num_e) });
	results.push_back({ basicOperationBenchTemplate<lib_calvin_graph::boost_graph<Node, Link>>(num_v, num_e) });

	
	std::string comment = "";
	lib_calvin_benchmark::save_bench(category, getSubCategory(sub), getTitle(sub, num_v, num_e), comment,
									 getAlgorithmNamesAndTagsVector(algorithms),
									 results, testCases, unit, benchOrder++);
}

template <typename Graph>
double  lib_calvin_benchmark::graph::basicOperationBenchTemplate(size_t num_v, size_t num_e) {
	std::cout << "Now doing basicOperationBench of size: " << std::to_string(num_v) << " and " << std::to_string(num_e) << "\n";
	Graph graph;
	size_t degree = num_e / num_v;

	lib_calvin::stopwatch watch;
	watch.start();
	for (size_t i = 0; i < num_v; i++) {
		graph.insert_vertex(Node(i));
	}

	size_t interval = num_v / (degree + 1);
	for (size_t i = 0; i < num_v; i++) {
		for (size_t j = 0; j < degree; j++) {			
			graph.insert_edge(Node(i), Node(i + j * interval), Link());
		}		
	}
	watch.stop();
	return watch.read();
}


// Do a run
void  lib_calvin_benchmark::graph::suffixTreeBench(size_t alphabet_size, size_t string_len) {

}