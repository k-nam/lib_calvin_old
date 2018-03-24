#include <utility>
#include <vector>
#include <string>

#include "sorting_bench.h"
#include "sort.h"
#include "sort_test.h"
#include "pdqsort.h"
#include "bench.h"
#include "random.h"

std::vector<std::string>
lib_calvin_benchmark::sorting::getAlgorithmNamesAndTags(Algorithm algo) {
	switch (algo) {
	case PDQSORT:
		return { "pdqsort" };

	case STD_SORT:
		return { "std::sort", "std" };
	case STD_STABLE_SORT:
		return { "std::stable_sort", "std", "stable" };

	case LIB_CALVIN_QSORT:
		return { "lib_calvin::qsort", "lib_calvin" };
	case LIB_CALVIN_BLOCK_QSORT:
		return { "lib_calvin::block_qsort", "lib_calvin" };
	case LIB_CALVIN_MERGESORT:
		return { "lib_calvin::mergesort", "lib_calvin", "stable" };
	case LIB_CALVIN_HEAPSORT:
		return { "lib_calvin::heapsort", "lib_calvin" };

	case LIB_CALVIN_BLOCK_QSORT_PARALLEL:
		return { "lib_calvin::block_qsort_parallel", "lib_calvin", "parallel" };
	case LIB_CALVIN_MERGESORT_PARALLEL:
		return { "lib_calvin::mergesort_parallel", "lib_calvin", "stable", "parallel" };

	default:
		return { "getAlgorithmName error!" };
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
	using namespace std;
	using namespace lib_calvin_sort;
	string category = "Sorting";
	string subCategory = "Comparison sorting";
	string comment = "block_qsort is my implementation of BlockQuickSort. pdqsort is the official one.";
	string unit = "M/s (higher is better)";
	vector<string> tags = { "sorting" };
	vector<string> testCases = { "4Byte (int)", "16Byte (key:int)", "24Byte (key:string)" };
	vector<Algorithm> algorithms{ STD_SORT, STD_STABLE_SORT,
		PDQSORT,
		LIB_CALVIN_QSORT,LIB_CALVIN_BLOCK_QSORT, LIB_CALVIN_MERGESORT, LIB_CALVIN_HEAPSORT,
		LIB_CALVIN_BLOCK_QSORT_PARALLEL, LIB_CALVIN_MERGESORT_PARALLEL
	};

	vector<vector<double>> results;
	for (auto algorithm : algorithms) {
		results.push_back(sortBenchSub(algorithm));
	}

	lib_calvin_benchmark::save_bench(category, subCategory, benchTitle, comment,
								getAlgorithmNamesAndTagsVector(algorithms), results, testCases, unit, benchOrder);
}

std::vector<double>
lib_calvin_benchmark::sorting::sortBenchSub(Algorithm algo) {
	using namespace lib_calvin_sort;
	return std::vector<double> { sortBenchSub2<int>(algo), sortBenchSub2<SimpleStruct>(algo),
		sortBenchSub2<StringStruct>(algo) };
}

template <typename T>
double lib_calvin_benchmark::sorting::sortBenchSub2(Algorithm algo) {
	using namespace lib_calvin_sort;
	using std::less;
	std::cout << "Now benchmarking: " << getAlgorithmNamesAndTags(algo)[0] << "\n";
	switch (algo) {
	case PDQSORT:
		return sortBenchTemplateSub(pdqsort_branchless<T *, less<T>>);

	case STD_SORT:
		return sortBenchTemplateSub(std::sort<T *, less<T>>);
	case STD_STABLE_SORT:
		return sortBenchTemplateSub(std::stable_sort<T *, less<T>>);

	case LIB_CALVIN_QSORT:
		return sortBenchTemplateSub(introSort<T *, less<T>>);
	case LIB_CALVIN_BLOCK_QSORT:
		return sortBenchTemplateSub(blockIntroSort<T *, less<T>>);
	case LIB_CALVIN_MERGESORT:
		return sortBenchTemplateSub(mergeSort<T *, less<T>>);
	case LIB_CALVIN_HEAPSORT:
		return sortBenchTemplateSub(heapSort<T *, less<T>>);

	case LIB_CALVIN_BLOCK_QSORT_PARALLEL:
		return sortBenchTemplateSub(introSortParallel<T *, less<T>>);
	case LIB_CALVIN_MERGESORT_PARALLEL:
		return sortBenchTemplateSub(mergeSortParallel<T *, less<T>>);

	default:
		cout << "sortBenchSub2 error!";
		exit(0);
	}
}

template <typename T>
double lib_calvin_benchmark::sorting::sortBenchSub3(Algorithm algo) {
	using std::less;
	std::cout << "Now benchmarking: " << getAlgorithmNamesAndTags(algo)[0] << "\n";
	switch (algo) {
	case STD_SORT:
		return sortBenchTemplateSub(std::sort<T *, less<T>>);
	case STD_STABLE_SORT:
		return sortBenchTemplateSub(std::stable_sort<T *, less<T>>);

	case LIB_CALVIN_COUNTINGSORT:
		return sortBenchTemplateSub(countingSort<T *, less<T>>);
	case LIB_CALVIN_BUCKETSORT:
		return sortBenchTemplateSub(sampleSort<T *, less<T>>);

	default:
		cout << "sortBenchSub2 error!";
		exit(0);
	}
}

template <typename T>
double
lib_calvin_benchmark::sorting::sortBenchTemplateSub(void(*sorter)(T *first, T *last, std::less<T>)) {
	using namespace lib_calvin_sort;
	lib_calvin::stopwatch watch;
	size_t numIter = benchNumIter;
	size_t testSize = benchTestSize;
	size_t seed = 1232; // reset seed to give identical input to algorithms
	std::srand(1232); // reset seed to give identical input to algorithms

	T *testSet = static_cast<T *>(operator new(sizeof(T) * testSize));
	double min = 1000000;

	for (int i = 0; i < numIter; i++) {
		for (int j = 0; j < testSize; j++) {
			new (testSet + j) T(j);
		}
		std::random_shuffle(testSet, testSet + testSize, lib_calvin::random_number_generator());
		double time = 0;
		bool isCorrect = true;
		bool isStable = true;

		sortTest(sorter, testSet, testSet + testSize, "", time, isCorrect, isStable, std::less<T>());

		if (time < min) {
			min = time;
		}
	}
	operator delete(testSet);
	return testSize / min / 1000 / 1000;
}

std::map<std::string, std::vector<double>>
lib_calvin_benchmark::sorting::combineResults(std::vector<std::map<std::string, double>> results) {
	using namespace std;
	map<string, vector<double>> combined;
	for (auto result : results) {
		for (auto testCase : result) {
			// algorithm name => measurement
			combined[testCase.first].push_back(testCase.second);
		}
	}
	return combined;
}