#ifndef LIB_CALVIN__MATRIX__BENCH_H
#define LIB_CALVIN__MATRIX__BENCH_H

#include <vector>
#include <string>

#include "matrix.h"

namespace lib_calvin_benchmark
{
	namespace matrix
	{
		using namespace lib_calvin_matrix;

		size_t const benchNumCases = 4;
		std::vector<size_t> const benchTestSizes = { 160, 320, 640, 1280, 2560, 5120 };
		std::vector<std::string> const benchTestCase = { "matrix multiplication" };
		std::vector<size_t> const benchNumIter = { 10, 3, 1, 1, 1, 1 };
		std::vector<int> const benchOrder = { 0, 1, 2, 3, 4, 5 };
		std::string const unit = "G/s (higher is better)";
		std::string const category = "Matrix";

		static size_t currentBenchNum = 0;

		enum Algorithm {
			MKL,

			NAIVE,
			NAIVE_TRANSPOSED,
			ROW_FIRST,
			BLOCKING,

			RECURSIVE,
			STRASSEN,
			RECURSIVE_PARALLEL,
			STRASSEN_PARALLEL
		};

		enum SubCategory {
			MATRIX_MULTI_DOUBLE,
			MATRIX_MULTI_USER_DEFINED_OBJECT
		};

		static SubCategory currentSubCategory;

		std::string getTitle(size_t problemSize);

		std::string getSubCategory(SubCategory);

		std::vector<SubCategory> getAllSubCategories();

		std::vector<Algorithm> getAllAlgorithms();

		std::vector<std::string> getAlgorithmNamesAndTags(Algorithm);



		std::vector<std::vector<std::string>> getAllNamesAndTagsVector();

		void matrixBench();

		void matrixBench(SubCategory, size_t currentNum);

		template <typename T>
		double
			matrixBenchSub(Algorithm);

		template <typename T, typename Function>
		double matrixBenchTemplateSub(std::function<Function>);

	}
}
#endif