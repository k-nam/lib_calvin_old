#ifndef LIB_CALVIN__SORT_BENCH_H
#define LIB_CALVIN__SORT_BENCH_H

#include <map>
#include <vector>
#include <string>

namespace lib_calvin_benchmark
{
	namespace sorting
	{
		size_t const benchNumCases = 4;
		size_t const kilo = 1000;
		size_t const mega = 1000*1000;
		std::vector<size_t> const benchTestSize = { kilo, 10*kilo, 100*kilo, mega };
		std::vector<std::string> const benchCases = { "1K", "10K", "100K", "1M" };
		std::vector<size_t> const benchNumIter = { 1000, 100, 10, 3};
		std::vector<int> const benchOrder = { 0, 1, 2, 3 };
		std::string const unit = "M/s (higher is better)";
		std::string const category = "Sorting";
		std::string const benchTitleSuffix = "objects";

		enum Algorithm {
			STD_SORT,
			STD_STABLE_SORT,

			PDQSORT,

			LIB_CALVIN_QSORT,
			LIB_CALVIN_BLOCK_QSORT,

			LIB_CALVIN_MERGESORT,
			LIB_CALVIN_IN_PLACE_MERGESORT,
			LIB_CALVIN_IN_PLACE_MERGESORT2,
			LIB_CALVIN_IN_PLACE_MERGESORT3,

			LIB_CALVIN_HEAPSORT,

			LIB_CALVIN_COUNTINGSORT,
			LIB_CALVIN_BUCKETSORT,

			LIB_CALVIN_BLOCK_QSORT_PARALLEL,
			LIB_CALVIN_MERGESORT_PARALLEL
		};

		enum SubCategory {
			BYTE_8, BYTE_16, BYTE_32, BYTE_64, VECTOR
		};

		std::string getTitle(size_t num);

		std::string getSubCategory(SubCategory);

		std::vector<std::string> getAlgorithmNamesAndTags(Algorithm);

		std::vector<std::vector<std::string>> getAlgorithmNamesAndTagsVector(std::vector<Algorithm>);

		void sortBench();

		template <typename T>
		void sortBench(size_t num);

		template <typename T>
		double sortBenchSub(Algorithm, size_t testSize, size_t numIter);

		template <typename T>
		double sortBenchSub2(void(*sorter)(T *first, T *last, std::less<T>), 
									size_t testSize, size_t numIter);

	}
}
#endif
