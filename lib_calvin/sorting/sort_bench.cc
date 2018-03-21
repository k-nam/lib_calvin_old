#include <utility>
#include <vector>
#include <string>

#include "sort_bench.h"
#include "sort.h"
#include "sort_test.h"
#include "pdqsort.h"
#include "save_bench.h"
#include "random.h"

void
lib_calvin::sort_bench() {
	using namespace std;
	using namespace lib_calvin_sort;
	string title = "Sorting";
	string comment = "block_qsort is my implementation of BlockQuickSort. pdqsort is the official one.";
	string unit = "M/s (higher is better)";
	vector<string> tags = { "sorting" };

	vector<map<string, double>> results;
	results.push_back(sortBenchTemplate<int>());
	results.push_back(sortBenchTemplate<SimpleStruct>());
	results.push_back(sortBenchTemplate<StringStruct>());
	
	auto combinedResults = combineResults(results);

	vector<string> testCases = { "int", "16Byte (key:int)", "24Byte (key:string)" };


	lib_calvin_util::save_bench(title, comment, unit, tags, combinedResults, testCases);
}

template <typename T>
std::map<std::string, double>
lib_calvin_sort::sortBenchTemplate() {
	using namespace std;
	typedef void(*sorter)(T *first, T *last, less<T>);
	std::map<std::string, double> result;

	result["[lib_calvin] block-qsort"]		= sortBenchTemplateSub(introSort2<T *, less<T>>);
	result["std::sort"]						= sortBenchTemplateSub(std::sort<T *, less<T>>);
	result["pdqsort"]						= sortBenchTemplateSub(pdqsort_branchless<T *, less<T>>);
	result["[lib_calvin] mergesort"]		= sortBenchTemplateSub(mergeSort<T *, less<T>>);
	result["std::stable_sort"]				= sortBenchTemplateSub(std::stable_sort<T *, less<T>>);
	return result;
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