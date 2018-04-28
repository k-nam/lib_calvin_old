#ifndef LIB_CALVIN__STRING_BENCH_H
#define LIB_CALVIN__STRING_BENCH_H

#include <vector>
#include <string>

#include "abstract_string.h"
#include "suffix_tree.h"

namespace lib_calvin_benchmark
{
	namespace string
	{
		using namespace lib_calvin_string;

		std::vector<size_t> const benchTestSizes = { 1000*1000, 2000*1000, 4000*1000 };

		std::vector<std::string> const benchCases = { "1M", "2M", "4M"};

		std::string const unit = "M / sec (higher is better)";
		std::string const category = "String";

		enum Algorithm {
			NAIVE, Z, KMP, STD_BOYER, LIB_CALVIN_BOYER, LIB_CALVIN_SUFFIX
		};

		static Algorithm currentAlgo;

		enum CharSet {
			BINARY = 2, DNA = 4, ENG = 26
		};

		enum TextType {
			RANDOM, HEAD_MISS, TAIL_MISS
		};

		size_t getPatternLen(CharSet);

		std::string getTitle(CharSet);

		std::string getSubCategory(TextType);

		std::vector<CharSet> getAllCharSets();

		std::vector<Algorithm> getAllAlgorithms();

		std::vector<Algorithm> getLinearTimeAlgorithms();

		std::vector<std::string> getAlgorithmNamesAndTags(Algorithm);

		std::vector<std::vector<std::string>> getAlgorithmNamesAndTagsVector(std::vector<Algorithm>);
		std::vector<std::vector<std::string>> getAllNamesAndTagsVector();

		void stringBench();

		void stringBench(TextType);

		void stringBench(TextType, CharSet);
		
		std::vector<double> stringBenchSub(TextType, CharSet, size_t patternLen, Algorithm algo);

	}
}
#endif