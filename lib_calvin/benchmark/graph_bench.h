#ifndef LIB_CALVIN__GRAPH_BENCH_H
#define LIB_CALVIN__GRAPH_BENCH_H

#include <vector>
#include <string>

#include "simple_graph.h"

namespace lib_calvin_benchmark
{
	namespace graph
	{
		struct Node {
			Node(size_t key): key_(key) { }
			bool operator< (Node const &rhs) const { return key_ < rhs.key_; }
			size_t key_;
			size_t value_;
		};

		struct Link {
			size_t weight_;
			size_t distance_;
		};

		enum Algorithm {
			LIB_CALVIN_GRAPH, BOOST_GRAPH
		};

		enum SubCategory {
			GROWING_SPARSE, GROWING_DENSE, SUFFIX_TREE, DIJKSTRA
		};

		std::string const unit = "M/s (higher is better)";
		std::string const category = "Graph";

		std::string getStringFromNumber(size_t);

		std::string getTitle(SubCategory, size_t, size_t);

		std::string getSubCategory(SubCategory);

		std::vector<std::string> getAlgorithmNamesAndTags(Algorithm);

		std::vector<std::vector<std::string>> getAlgorithmNamesAndTagsVector(std::vector<Algorithm>);

		std::vector<Algorithm> getAllAlgorithms();

		std::vector<std::vector<std::string>> getAllAlgorithmNamesAndTagsVector();


		void graphBench();

		void graphBench(SubCategory);

		// Do a run
		void basicOperationBench(SubCategory, size_t num_v, size_t num_e);

		template <typename Graph>
		double basicOperationBenchTemplate(size_t num_v, size_t num_e);

		// Do a run
		void suffixTreeBench(SubCategory, size_t alphabet_size, size_t text_len);
		double suffixTreeBenchSub(Algorithm, size_t alphabet_size, size_t text_len);

	}
}

#endif
