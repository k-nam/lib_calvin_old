#ifndef LIB_CALVIN__STRING__BENCH_H
#define LIB_CALVIN__STRING__BENCH_H

#include <vector>
#include <string>

#include "abstract_string.h"
#include "suffix_tree.h"

namespace lib_calvin_benchmark
{
	namespace string
	{
		using namespace lib_calvin_string;

		size_t const benchNumCases = 7;
		std::vector<size_t> const benchTestSizes = { 1000*1000, 2000*1000, 4000*1000 };
		std::vector<std::string> const benchTestCase = { "string matching" };
		std::vector<size_t> const benchNumIter = { 10*1000, 100*1000, 1000*1000, 10*1000*1000 };

		std::string const unit = "sec (lower is better)";
		std::string const category = "String";

		size_t const patternLen = 16;

		enum Algorithm {
			NAIVE, Z, KMP, BOYWER, SUFFIX
		};

		static Algorithm currentAlgo;

		enum SubCategory {
			BINARY, GENE, ENG
		};

		std::string getTitle(size_t problemSize);

		std::string getSubCategory(SubCategory);

		std::vector<SubCategory> getAllSubCategories();

		std::vector<Algorithm> getAllAlgorithms();

		std::vector<std::string> getAlgorithmNamesAndTags(Algorithm);

		std::vector<std::vector<std::string>> getAlgorithmNamesAndTagsVector(std::vector<Algorithm>);
		std::vector<std::vector<std::string>> getAllNamesAndTagsVector();

		void stringBench();

		void stringBench(SubCategory);

		void stringBench(SubCategory, size_t textLen);
		

		std::vector<double> stringBenchSub(SubCategory, size_t textLen, Algorithm algo,  std::vector<size_t> numIters);

		// returns fixed-cost (only for suffix-tree) and variable-cost
		std::pair<double, double> stringBenchSub2(SubCategory, size_t textLen, Algorithm algo);

	}
}
#endif
