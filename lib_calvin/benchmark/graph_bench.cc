#include <iostream>
#include <algorithm>

#include "graph_bench.h"
#include "boost/graph/adjacency_list.hpp"
#include "simple_graph.h"
#include "boost_graph.h"
#include "abstract_string.h"
#include "suffix_tree.h"

#include "bench.h"
#include "random.h"
#include "stopwatch.h"

using namespace lib_calvin_benchmark::graph;


std::string 
lib_calvin_benchmark::graph::getTitle(SubCategory sub, size_t size1, size_t size2) {
	using lib_calvin_benchmark::getSizeString;
	if (sub == GROWING_SPARSE || sub == GROWING_DENSE) {
		return "Vertices: " + getSizeString(size1) + " / Edges: " + getSizeString(size2);
	} else if (sub == SUFFIX_TREE) {
		return "Charset size: " + getSizeString(size1) + " / Text length: " + getSizeString(size2);
	} else {
		return "";
	}
}

std::string 
lib_calvin_benchmark::graph::getSubCategory(SubCategory sub) {
	if (sub == GROWING_SPARSE) {
		return "Growing / Sparse";
	} else if (sub == GROWING_DENSE) {
		return "Growing / Dense";
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
	graphBench(GROWING_SPARSE);
	graphBench(GROWING_DENSE);
	graphBench(SUFFIX_TREE);
}

void  lib_calvin_benchmark::graph::graphBench(SubCategory sub) {
	if (sub == GROWING_SPARSE) {
		std::vector<size_t> num_v_array = { 1000, 1000 * 10, 1000 * 100, 1000* 1000 };
		size_t degree = 5;
		for (size_t num_v : num_v_array) {
			basicOperationBench(sub, num_v, num_v * degree);
		}
	} else if (sub == GROWING_DENSE) {
		std::vector<size_t> num_v_array = { 1000, 2000, 4000 };
		for (size_t num_v : num_v_array) {
			basicOperationBench(sub, num_v, num_v * num_v);
		}
	} else if (sub == SUFFIX_TREE) {
		suffixTreeBench(sub, 2, 100 * 1000);
		suffixTreeBench(sub, 26, 100 * 1000);
		suffixTreeBench(sub, 2, 1000 * 1000);
		suffixTreeBench(sub, 26, 1000 * 1000);
	} else {

	}
}

// Do a run
void  lib_calvin_benchmark::graph::basicOperationBench(SubCategory sub, size_t num_v, size_t num_e) {
	static size_t benchOrder = 0;

	std::cout << "Now doing basicOperationBench subCategory: " << getSubCategory(sub) << " size: " << 
		std::to_string(num_v) << " and " << std::to_string(num_e) << "\n";

	std::vector<std::vector<double>> results;
	std::vector<std::string> testCases = { "growing" };
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
	return num_e / watch.read() / 1000 / 1000;
}


// Do a run
void  lib_calvin_benchmark::graph::suffixTreeBench(SubCategory sub, size_t alphabet_size, size_t text_len) {
	static size_t benchOrder = 0;

	std::vector<std::vector<double>> results;
	std::vector<std::string> testCases = { "building suffix tree" };
	auto algorithms = getAllAlgorithms();
	for (auto algorithm : algorithms) {
		results.push_back({ suffixTreeBenchSub(algorithm, alphabet_size, text_len) });
	}

	std::string comment = "";
	lib_calvin_benchmark::save_bench(category, getSubCategory(sub), getTitle(sub, alphabet_size, text_len), comment,
									 getAlgorithmNamesAndTagsVector(algorithms),
									 results, testCases, unit, benchOrder++);
}

double lib_calvin_benchmark::graph::suffixTreeBenchSub(Algorithm algo, size_t alphabet_size, size_t text_len) {
	std::cout << "Now doing suffixTreeBench: " << " alphabet_size: " <<
		std::to_string(alphabet_size) << " text len" << std::to_string(text_len) << "\n";

	lib_calvin::random_number_generator gen;
	char *pText = new char[text_len];

	for (size_t i = 0; i < text_len; ++i) {
		size_t number = gen();
		pText[i] = '0' + static_cast<char>(number % alphabet_size);
	}

	lib_calvin::c_string text(pText, text_len);

	lib_calvin::stopwatch watch;
	watch.start();
	if (algo == LIB_CALVIN_GRAPH) {
		lib_calvin::suffix_tree<char, lib_calvin_graph::simple_graph> tree(text);
		tree.build();
	} else if (algo == BOOST_GRAPH) {
		lib_calvin::suffix_tree<char, lib_calvin_graph::boost_graph> tree(text);
		tree.build();
	}
	watch.stop();
	return text_len / watch.read() / 1000 / 1000;
}

