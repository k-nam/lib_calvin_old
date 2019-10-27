#ifndef LIB_CALVIN__MATRIX_BENCH_H
#define LIB_CALVIN__MATRIX_BENCH_H

#include <vector>
#include <string>

#include "matrix.h"

namespace lib_calvin_benchmark
{
	namespace matrix
	{
		using namespace lib_calvin_matrix;

		size_t const benchNumCases = 7;
		std::vector<size_t> const benchTestSizes = { 160, 320, 640, 1280, 2560, 5120, 10240 };
		std::vector<std::string> const benchTestCase = { "multiplication" };
		std::vector<size_t> const benchNumIter = { 100, 10, 3, 1, 1, 1, 1 };

		std::string const unit = "G/s (higher is better)";
		std::string const category = "Matrix";

		static size_t currentBenchNum = 0;

		enum Algorithm {
			MKL,

			NAIVE,
			NAIVE_TRANSPOSED,
			ROW_FIRST,
			BLOCKING,

			NAIVE_MMX,
			BLOCKING_MMX,

			RECURSIVE,
			RECURSIVE_PARALLEL,
			STRASSEN,			
			STRASSEN_PARALLEL
		};

		static Algorithm currentAlgo;

		enum SubCategory {
			OPTIMAL,
			DROP
		};

		static SubCategory currentSubCategory;

		std::string getTitle(size_t problemSize);

		std::string getSubCategory(SubCategory);

		std::vector<SubCategory> getAllSubCategories();

		std::vector<Algorithm> getAllAlgorithms();

		std::vector<std::string> getAlgorithmNamesAndTags(Algorithm);

		std::vector<std::vector<std::string>> getAlgorithmNamesAndTagsVector(std::vector<Algorithm>);
		std::vector<std::vector<std::string>> getAllNamesAndTagsVector();

		void matrixBench();

		void matrixBench(SubCategory, size_t currentNum);

		double
			matrixBenchSub(Algorithm);

		template <typename Function>
		double matrixBenchTemplateSub(std::function<Function>);

		template <typename Function>
		double matrixBenchTemplateSub(std::function<Function> func, size_t testSize, size_t numIter);

	}
}
#endif
