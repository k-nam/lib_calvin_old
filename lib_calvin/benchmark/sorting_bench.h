#ifndef LIB_CALVIN__SORT__BENCH_H
#define LIB_CALVIN__SORT__BENCH_H

#include <map>
#include <vector>
#include <string>

namespace lib_calvin_benchmark
{
	namespace sorting
	{
		size_t const benchTestSize = 1000 * 1000;
		size_t const benchNumIter = 3;
		std::string const benchTitle = "Sorting 10M objects";
		int const benchOrder = 5;

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

		std::vector<std::string> getAlgorithmNamesAndTags(Algorithm);

		std::vector<std::vector<std::string>> getAlgorithmNamesAndTagsVector(std::vector<Algorithm>);

		void sortBench();

		std::vector<double>
			sortBenchSub(Algorithm);

		template <typename T>
		double sortBenchSub2(Algorithm);

		template <typename T>
		double sortBenchSub3(Algorithm);

		template <typename T>
		double sortBenchTemplateSub(void(*sorter)(T *first, T *last, std::less<T>));

		std::map<std::string, std::vector<double>>
			combineResults(std::vector<std::map<std::string, double>>);
	}
}
#endif
