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
		return { "std::sort" };
	case STD_STABLE_SORT:
		return { "std::stable_sort", "stable" };

	case LIB_CALVIN_QSORT:
		return { "lib_calvin::qsort" };
	case LIB_CALVIN_BLOCK_QSORT:
		return { "lib_calvin::block_qsort" };
	case LIB_CALVIN_MERGESORT:
		return { "lib_calvin::mergesort", "stable" };
	case LIB_CALVIN_HEAPSORT:
		return { "lib_calvin::heapsort" };

	case LIB_CALVIN_BLOCK_QSORT_PARALLEL:
		return { "lib_calvin::block_qsort_parallel", "parallel" };
	case LIB_CALVIN_MERGESORT_PARALLEL:
		return { "lib_calvin::mergesort_parallel", "stable", "parallel" };

	default:
		return { "getAlgorithmName error!" };
	}
}

std::string
lib_calvin_benchmark::sorting::getTitle(size_t num) {
	return category + " " + testCases[num] + " " + benchTitleSuffix;
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
	for (size_t i = 0; i < benchNumCases; i++) {
		sortBench(i);
	}
}

void lib_calvin_benchmark::sorting::sortBench(size_t num) {
	using namespace std;
	using namespace lib_calvin_sort;
	string category = "Sorting";
	string subCategory = "Comparison sorting";
	string comment = "block_qsort is my implementation of BlockQuickSort. pdqsort is the official one.";
	string unit = "M/s (higher is better)";
	vector<string> tags = { "sorting" };
	vector<string> testCases = { "4Byte (int)", "16Byte (key:int)", "48Byte (key:string)" };
	vector<Algorithm> algorithms{ STD_SORT, STD_STABLE_SORT,
		PDQSORT,
		LIB_CALVIN_QSORT,LIB_CALVIN_BLOCK_QSORT, LIB_CALVIN_MERGESORT, LIB_CALVIN_HEAPSORT,
		LIB_CALVIN_BLOCK_QSORT_PARALLEL, LIB_CALVIN_MERGESORT_PARALLEL
	};

	vector<vector<double>> results;
	for (auto algorithm : algorithms) {
		results.push_back(sortBenchSub(algorithm, benchTestSize[num], benchNumIter[num]));
	}

	lib_calvin_benchmark::save_bench(category, subCategory, getTitle(num), comment,
								getAlgorithmNamesAndTagsVector(algorithms), 
									 results, testCases, unit, benchOrder[num]);
}

std::vector<double>
lib_calvin_benchmark::sorting::sortBenchSub(Algorithm algo, size_t testSize, size_t numIter) {
	using namespace lib_calvin_sort;
	return std::vector<double> { 
		sortBenchSub2<int>(algo, testSize, numIter),
		sortBenchSub2<SimpleStruct>(algo, testSize, numIter),
		sortBenchSub2<StringStruct>(algo, testSize, numIter) };
}

template <typename T>
double lib_calvin_benchmark::sorting::sortBenchSub2(Algorithm algo, size_t testSize, size_t numIter) {
	using namespace lib_calvin_sort;
	using std::less;
	std::cout << "Now benchmarking: " << testSize << " " << "object size: " << sizeof(T) << 
		" " << getAlgorithmNamesAndTags(algo)[0] <<  "\n";
	switch (algo) {
	case PDQSORT:
		return sortBenchTemplateSub(pdqsort_branchless<T *, less<T>>, testSize, numIter);

	case STD_SORT:
		return sortBenchTemplateSub(std::sort<T *, less<T>>, testSize, numIter);
	case STD_STABLE_SORT:
		return sortBenchTemplateSub(std::stable_sort<T *, less<T>>, testSize, numIter);

	case LIB_CALVIN_QSORT:
		return sortBenchTemplateSub(introSort<T *, less<T>>, testSize, numIter);
	case LIB_CALVIN_BLOCK_QSORT:
		return sortBenchTemplateSub(blockIntroSort<T *, less<T>>, testSize, numIter);
	case LIB_CALVIN_MERGESORT:
		return sortBenchTemplateSub(mergeSort<T *, less<T>>, testSize, numIter);
	case LIB_CALVIN_HEAPSORT:
		return sortBenchTemplateSub(heapSort<T *, less<T>>, testSize, numIter);

	case LIB_CALVIN_BLOCK_QSORT_PARALLEL:
		return sortBenchTemplateSub(introSortParallel<T *, less<T>>, testSize, numIter);
	case LIB_CALVIN_MERGESORT_PARALLEL:
		return sortBenchTemplateSub(mergeSortParallel<T *, less<T>>, testSize, numIter);

	default:
		cout << "sortBenchSub2 error!";
		exit(0);
	}
}

template <typename T>
double
lib_calvin_benchmark::sorting::sortBenchTemplateSub(void(*sorter)(T *first, T *last, std::less<T>),
													size_t testSize, size_t numIter) {
	using namespace lib_calvin_sort;
	lib_calvin::stopwatch watch;
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
	return testSize * std::log(testSize) / min / 1000 / 1000;
}
