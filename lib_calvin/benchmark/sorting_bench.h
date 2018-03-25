#ifndef LIB_CALVIN__SORT__BENCH_H
#define LIB_CALVIN__SORT__BENCH_H

#include <map>
#include <vector>
#include <string>

namespace lib_calvin_benchmark
{
	namespace sorting
	{
		size_t const benchNumCases = 6;
		std::vector<size_t> const benchTestSize = { 100, 1000, 10*1000, 100*1000, 1000*1000, 10*1000*1000 };
		std::vector<std::string> const testCases = { "100", "1K", "10K", "100K", "1M", "10M" };
		std::vector<size_t> const benchNumIter = { 10000, 1000, 100, 10, 3, 1 };
		std::vector<int> const benchOrder = { 0, 1, 2, 3, 4, 5 };
		std::string const unit = "M/s (higher is better)";
		std::string const category = "Sorting";
		std::string const benchTitleSuffix = "objects";

		enum Algorithm {
			PDQSORT,

			STD_SORT,
			STD_STABLE_SORT,

			LIB_CALVIN_QSORT,
			LIB_CALVIN_BLOCK_QSORT,
			LIB_CALVIN_MERGESORT,
			LIB_CALVIN_HEAPSORT,

			LIB_CALVIN_COUNTINGSORT,
			LIB_CALVIN_BUCKETSORT,

			LIB_CALVIN_BLOCK_QSORT_PARALLEL,
			LIB_CALVIN_MERGESORT_PARALLEL
		};

		enum SubCategory {
			COMPARISON_SORT,
			LINEAR_COMPLEXITY_SORT
		};

		std::string getTitle(size_t num);

		std::vector<std::string> getAlgorithmNamesAndTags(Algorithm);

		std::vector<std::vector<std::string>> getAlgorithmNamesAndTagsVector(std::vector<Algorithm>);

		void sortBench();
		void sortBench(size_t num);

		std::vector<double>
			sortBenchSub(Algorithm, size_t testSize, size_t numIter);

		template <typename T>
		double sortBenchSub2(Algorithm, size_t testSize, size_t numIter);

		template <typename T>
		double sortBenchTemplateSub(void(*sorter)(T *first, T *last, std::less<T>), 
									size_t testSize, size_t numIter);

	}
}
#endif
