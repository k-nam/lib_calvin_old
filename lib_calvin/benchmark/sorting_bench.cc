#include <utility>
#include <vector>
#include <string>

#include "sorting_bench.h"
#include "sort.h"
#include "sort_test.h"
#include "pdqsort.h"
#include "bench.h"
#include "random.h"
#include "boost/sort/sort.hpp"

std::vector<lib_calvin_benchmark::sorting::Algorithm>
lib_calvin_benchmark::sorting::getBenchAlgorithms() {
	using namespace lib_calvin_benchmark::sorting;
	return {
		STD_SORT,
		LIB_CALVIN_QSORT,

		ORLP_PDQSORT,
		BOOST_PDQSORT,
		LIB_CALVIN_BLOCK_QSORT,		

		STD_STABLE_SORT,
		BOOST_SPINSORT,
		LIB_CALVIN_MERGESORT,

		BOOST_FLAT_STABLE_SORT,
		LIB_CALVIN_IN_PLACE_MERGESORT,

		//LIB_CALVIN_HEAPSORT,

		BOOST_SAMPLE_SORT_PARALLEL,
		BOOST_BLOCK_INDIRECT_SORT,

		LIB_CALVIN_BLOCK_QSORT_PARALLEL,
		LIB_CALVIN_MERGESORT_PARALLEL
	};
}


std::vector<std::string>
lib_calvin_benchmark::sorting::getAlgorithmNamesAndTags(Algorithm algo) {
	switch (algo) {
	case STD_SORT:
		return { "std::sort", "in-place" };
	case STD_STABLE_SORT:
		return { "std::stable_sort", "stable" };

	case ORLP_PDQSORT:
		return { "pdqsort", "in-place" };

	case BOOST_SPINSORT:
		return { "boost::spinsort", "stable" };
	case BOOST_FLAT_STABLE_SORT:
		return { "boost::flat_stable_sort", "stable", "in-place" };
	case BOOST_PDQSORT:
		return { "boost::pdqsort", "in-place" };

	case LIB_CALVIN_QSORT:
		return { "lib_calvin::qsort", "in-place" };
	case LIB_CALVIN_BLOCK_QSORT:
		return { "lib_calvin::block_qsort", "in-place" };

	case LIB_CALVIN_MERGESORT:
		return { "lib_calvin::mergesort", "stable" };
	case LIB_CALVIN_IN_PLACE_MERGESORT:
		return { "inplace_mergesort", "stable", "in-place" };


	case LIB_CALVIN_HEAPSORT:
		return { "lib_calvin::heapsort", "in-place" };

	case BOOST_BLOCK_INDIRECT_SORT:
		return { "boost::block_indirect_sort_parallel", "in-place", "parallel" };
	case BOOST_SAMPLE_SORT_PARALLEL:
		return { "boost::sample_sort_parallel", "stable", "parallel" };

	case LIB_CALVIN_BLOCK_QSORT_PARALLEL:
		return { "block_qsort_parallel", "parallel", "in-place" };
	case LIB_CALVIN_MERGESORT_PARALLEL:
		return { "mergesort_parallel", "stable", "parallel" };

	default:
		return { "getAlgorithmName error!" };
	}
}

std::string
lib_calvin_benchmark::sorting::getTitle(size_t num) {
	return category + " " + lib_calvin_benchmark::getSizeString(benchTestSize[num]) + 
		" " + benchTitleSuffix;
}

std::string
lib_calvin_benchmark::sorting::getSubCategory(SubCategory subCategory) {
	switch (subCategory) {
	case BYTE_8:
		return "8byte (int)";
	case BYTE_16:
		return "16byte (int)";
	case BYTE_32:
		return "32byte (int)";
	case BYTE_64:
		return "64byte (int)";
	case VECTOR:
		return "48byte (string)";
	default:
		cout << "getSubCategory error!";
		exit(0);
	}
}

std::vector<std::vector<std::string>>
lib_calvin_benchmark::sorting::getAlgorithmNamesAndTagsVector(std::vector<Algorithm> algorithms) {
	using namespace std;
	vector<vector<string>> algorithmNamesAndTags = {};
	std::for_each(algorithms.begin(), algorithms.end(),
				  [&algorithmNamesAndTags](Algorithm algo) {
		algorithmNamesAndTags.push_back(getAlgorithmNamesAndTags(algo)); });
	return algorithmNamesAndTags;
}

void lib_calvin_benchmark::sorting::sortBench() {
	std::cout << "Size : " << benchTestSize.size() << "\n\n\n";
	for (size_t i = 0; i < benchTestSize.size(); i++) {
		std::cout << "Doing : " << i << "\n\n\n";
		sortBench<object_16>(i);
		sortBench<object_32>(i);
		sortBench<object_64>(i);
		sortBench<object_vector>(i);
	}
}

template <typename T>
void lib_calvin_benchmark::sorting::sortBench(size_t num) {
	using namespace std;
	using namespace lib_calvin_sort;
	string category = "Sorting";
	string comment = "";
	string unit = "M/s (higher is better)";
	vector<string> testCases = { "comparison sorting" };
	auto algorithms = getBenchAlgorithms();
	vector<vector<double>> results;
	for (auto algorithm : algorithms) {
		results.push_back({ sortBenchSub<T>(algorithm, benchTestSize[num], benchNumIter[num]) });
	}

	lib_calvin_benchmark::save_bench(category, T::to_string(), getTitle(num), comment,
									 getAlgorithmNamesAndTagsVector(algorithms),
									 results, testCases, unit, benchOrder[num]);
}

template <typename T>
double lib_calvin_benchmark::sorting::sortBenchSub(Algorithm algo, size_t testSize, size_t numIter) {
	using namespace lib_calvin_sort;
	using std::less;
	std::cout << "Now benchmarking: " << testSize << " " << "object size: " << sizeof(T) <<
		" " << getAlgorithmNamesAndTags(algo)[0] << "\n";
	switch (algo) {
	case STD_SORT:
		return sortBenchSub2(std::sort<T *, less<T>>, testSize, numIter);
	case STD_STABLE_SORT:
		return sortBenchSub2(std::stable_sort<T *, less<T>>, testSize, numIter);

	case ORLP_PDQSORT:
		return sortBenchSub2(pdqsort_branchless<T *, less<T>>, testSize, numIter);

	case BOOST_SPINSORT:
		return sortBenchSub2(boost::sort::spinsort<T *, less<T>>, testSize, numIter);
	case BOOST_FLAT_STABLE_SORT:
		return sortBenchSub2(boost::sort::flat_stable_sort<T *, less<T>>, testSize, numIter);
	case BOOST_PDQSORT:
		return sortBenchSub2(boost::sort::pdqsort_branchless<T *, less<T>>, testSize, numIter);

	case LIB_CALVIN_QSORT:
		return sortBenchSub2(introSort<T *, less<T>>, testSize, numIter);
	case LIB_CALVIN_BLOCK_QSORT:
		return sortBenchSub2(blockIntroSort<T *, less<T>>, testSize, numIter);
	case LIB_CALVIN_MERGESORT:
		return sortBenchSub2(mergeSort<T *, less<T>>, testSize, numIter);

	case LIB_CALVIN_IN_PLACE_MERGESORT:
		return sortBenchSub2(inPlaceMergeSort<T *, less<T>>, testSize, numIter);

	case LIB_CALVIN_HEAPSORT:
		return sortBenchSub2(heapSort<T *, less<T>>, testSize, numIter);

	case BOOST_SAMPLE_SORT_PARALLEL:
		return sortBenchSub2(boost::sort::sample_sort<T *, less<T>>, testSize, numIter);
	case BOOST_BLOCK_INDIRECT_SORT:
		return sortBenchSub2(boost::sort::block_indirect_sort<T *, less<T>>, testSize, numIter);

	case LIB_CALVIN_BLOCK_QSORT_PARALLEL:
		return sortBenchSub2(introSortParallelAdvanced2<T *, less<T>>, testSize, numIter);
	case LIB_CALVIN_MERGESORT_PARALLEL:
		return sortBenchSub2(mergeSortParallel<T *, less<T>>, testSize, numIter);

	default:
		cout << "sortBenchSub error!";
		exit(0);
	}
}

template <typename T>
double
lib_calvin_benchmark::sorting::sortBenchSub2(void(*sorter)(T *first, T *last, std::less<T>),
													size_t testSize, size_t numIter) {
	using namespace lib_calvin_sort;
	lib_calvin::stopwatch watch;

	T *testSet = static_cast<T *>(operator new(sizeof(T) * testSize));
	double min = 1000000;

	for (int i = 0; i < numIter; i++) {
		for (int j = 0; j < testSize; j++) {
			new (testSet + j) T(j);
		}
		std::shuffle(testSet, testSet + testSize, std::mt19937_64(std::random_device()()));
		double time = 0;
		bool isCorrect = true;
		bool isStable = true;

		sortTest(sorter, testSet, testSet + testSize, "", time, isCorrect, isStable, std::less<T>());

		if (time < min) {
			min = time;
		}
	}
	operator delete(testSet);
	return testSize * std::log(testSize) / min / 1000 / 1000;
}
