#include <utility>
#include <vector>
#include <string>

#include "sort_bench.h"
#include "sort.h"
#include "sort_test.h"
#include "pdqsort.h"
#include "save_bench.h"
#include "random.h"


enum Algorithm {
	PDQSORT,

	STD_SORT,
	STD_STABLE_SORT,

	LIB_CALVIN_QSORT,
	LIB_CALVIN_BLOCK_QSORT,
	LIB_CALVIN_MERGESORT,
	LIB_CALVIN_HEAPSORT,

	LIB_CALVIN_BLOCK_QSORT_PARALLEL,
	LIB_CALVIN_MERGESORT_PARALLEL
};

std::vector<std::string>
lib_calvin_sort::getAlgorithmNamesAndTags(Algorithm algo) {
	switch (algo) {
	case PDQSORT:
		return { "pdqsort", "unstable" };

	case STD_SORT:
		return { "std::sort", "std", "unstable" };
	case STD_STABLE_SORT:
		return { "std::stable_sort", "std", "stable" };

	case LIB_CALVIN_QSORT:
		return { "lib_calvin::qsort", "lib_calvin", "unstable" };
	case LIB_CALVIN_BLOCK_QSORT:
		return {"lib_calvin::block_qsort", "lib_calvin", "unstable"};
	case LIB_CALVIN_MERGESORT:
		return {"lib_calvin::mergesort", "lib_calvin", "stable" };
	case LIB_CALVIN_HEAPSORT:
		return {"lib_calvin::heapsort", "lib_calvin", "unstable" };

	case LIB_CALVIN_COUNTINGSORT:
		return { "lib_calvin::counting_sort", "lib_calvin", "stable" };
	case LIB_CALVIN_BUCKETSORT:
		return { "lib_calvin::bucket_sort", "lib_calvin", "unstable" };

	case LIB_CALVIN_BLOCK_QSORT_PARALLEL:
		return { "lib_calvin::block_qsort_parallel", "lib_calvin", "unstable", "parallel" };
	case LIB_CALVIN_MERGESORT_PARALLEL:
		return { "lib_calvin::mergesort_parallel", "lib_calvin", "stable", "parallel" };

	default:
		return { "getAlgorithmName error!" };
	}
}

std::vector<std::vector<std::string>> 
lib_calvin_sort::getAlgorithmNamesAndTagsVector(std::vector<Algorithm> algorithms) {
	using namespace std;
	vector<vector<string>> algorithmNamesAndTags = {};
	std::for_each(algorithms.begin(), algorithms.end(),
				  [&algorithmNamesAndTags](Algorithm algo) {
		algorithmNamesAndTags.push_back(getAlgorithmNamesAndTags(algo)); });
	return algorithmNamesAndTags;
}


void lib_calvin_sort::sortBench() {
	sortBenchComparison();
	sortBenchLinearComplexity();
}

void lib_calvin_sort::sortBenchComparison() {
	using namespace std;
	using namespace lib_calvin_sort;
	string category = "Sorting";
	string subCategory = "Comparison sorting";
	string title = "Sorting 1M objects";
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
		results.push_back(sortBenchSub(algorithm, COMPARISON_SORT));
	}

	lib_calvin_util::save_bench(category, subCategory, title, comment, 
								getAlgorithmNamesAndTagsVector(algorithms), results, testCases, unit);
}

void lib_calvin_sort::sortBenchLinearComplexity() {
	using namespace std;
	using namespace lib_calvin_sort;
	string category = "Sorting";
	string subCategory = "Linear complexity sorting";
	string title = "Sorting 1M integers";
	string comment = "Not so good...";
	string unit = "M/s (higher is better)";
	vector<string> tags = { "sorting" };
	vector<string> testCases = { "4Byte (int)", "8Byte (long long)" };
	vector<Algorithm> algorithms{ LIB_CALVIN_COUNTINGSORT, LIB_CALVIN_BUCKETSORT, STD_SORT };

	vector<vector<double>> results;
	for (auto algorithm : algorithms) {
		results.push_back(sortBenchSub(algorithm, LINEAR_COMPLEXITY_SORT));
	}

	lib_calvin_util::save_bench(category, subCategory, title, comment,
								getAlgorithmNamesAndTagsVector(algorithms), results, testCases, unit);
}


std::vector<double>
lib_calvin_sort::sortBenchSub(Algorithm algo, SubCategory subCategory) {
	switch (subCategory) {
	case COMPARISON_SORT:
		return std::vector<double> { sortBenchSub2<int>(algo), sortBenchSub2<SimpleStruct>(algo),
			sortBenchSub2<StringStruct>(algo) };
	case LINEAR_COMPLEXITY_SORT:
		return std::vector<double> { sortBenchSub3<int>(algo), sortBenchSub3<long long>(algo) };
	default:
		cout << "sortBenchSub error!";
		exit(0);
	}
}

template <typename T>
double lib_calvin_sort::sortBenchSub2(Algorithm algo) {
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
		return sortBenchTemplateSub(introSort2<T *, less<T>>);
	case LIB_CALVIN_MERGESORT:
		return sortBenchTemplateSub(mergeSort<T *, less<T>>);
	case LIB_CALVIN_HEAPSORT:
		return sortBenchTemplateSub(heapSort<T *, less<T>>);

	case LIB_CALVIN_BLOCK_QSORT_PARALLEL:
		return sortBenchTemplateSub(introSortParallelAdvanced2<T *, less<T>>);
	case LIB_CALVIN_MERGESORT_PARALLEL:
		return sortBenchTemplateSub(mergeSortParallel<T *, less<T>>);

	default:
		cout << "sortBenchSub2 error!";
		exit(0);
	}
}

template <typename T>
double lib_calvin_sort::sortBenchSub3(Algorithm algo) {
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
		return sortBenchTemplateSub(bucketSort<T *, less<T>>);

	default:
		cout << "sortBenchSub2 error!";
		exit(0);
	}
}

template <typename T>
double
lib_calvin_sort::sortBenchTemplateSub(void(*sorter)(T *first, T *last, std::less<T>)) {
	stopwatch watch;
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
lib_calvin_sort::combineResults(std::vector<std::map<std::string, double>> results) {
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