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
	case STD_SORT:
		return { "std::sort", "in-place" };
	case STD_STABLE_SORT:
		return { "std::stable_sort", "stable" };

	case PDQSORT:
		return { "pdqsort", "in-place" };

	case LIB_CALVIN_QSORT:
		return { "lib_calvin::qsort", "in-place" };
	case LIB_CALVIN_BLOCK_QSORT:
		return { "lib_calvin::block_qsort", "in-place" };

	case LIB_CALVIN_MERGESORT:
		return { "lib_calvin::mergesort", "stable" };
	case LIB_CALVIN_IN_PLACE_MERGESORT:
		return { "inplace_mergesort", "stable", "in-place" };
	case LIB_CALVIN_IN_PLACE_MERGESORT2:
		return { "inplace_mergesort(+1%)", "stable", "in-place" };
	case LIB_CALVIN_IN_PLACE_MERGESORT3:
		return { "inplace_mergesort(+10%)", "stable", "in-place" };

	case LIB_CALVIN_HEAPSORT:
		return { "lib_calvin::heapsort", "in-place" };

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
	return category + " " + benchCases[num] + " " + benchTitleSuffix;
}

std::string
lib_calvin_benchmark::sorting::getSubCategory(SubCategory subCategory) {
	switch (subCategory) {
	case BYTE_4:
		return "4byte (int)";
	case BYTE_16:
		return "16byte (int)";
	case BYTE_48:
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
	std::vector<SubCategory> subCategories = { BYTE_4, BYTE_16, BYTE_48 };
	for (auto subCategory : subCategories) {
		for (size_t i = 0; i < benchNumCases; i++) {
			sortBench(i, subCategory);
		}
	}
}

void lib_calvin_benchmark::sorting::sortBench(size_t num, SubCategory subCategory) {
	using namespace std;
	using namespace lib_calvin_sort;
	string category = "Sorting";
	string subCategoryString = getSubCategory(subCategory);
	string comment = "";
	string unit = "M/s (higher is better)";
	vector<string> testCases = { subCategoryString };
	vector<Algorithm> algorithms{ STD_SORT, STD_STABLE_SORT,
		PDQSORT,
		LIB_CALVIN_QSORT,LIB_CALVIN_BLOCK_QSORT, 
		LIB_CALVIN_MERGESORT, LIB_CALVIN_IN_PLACE_MERGESORT, 
			LIB_CALVIN_IN_PLACE_MERGESORT2, LIB_CALVIN_IN_PLACE_MERGESORT3,
		LIB_CALVIN_HEAPSORT,
		LIB_CALVIN_BLOCK_QSORT_PARALLEL, LIB_CALVIN_MERGESORT_PARALLEL
	};
	vector<vector<double>> results;
	for (auto algorithm : algorithms) {
		results.push_back(sortBenchSub(algorithm, benchTestSize[num], benchNumIter[num], subCategory));
	}

	lib_calvin_benchmark::save_bench(category, subCategoryString, getTitle(num), comment,
									 getAlgorithmNamesAndTagsVector(algorithms),
									 results, testCases, unit, benchOrder[num]);
}

std::vector<double>
lib_calvin_benchmark::sorting::sortBenchSub(Algorithm algo, size_t testSize, size_t numIter,
											SubCategory subCategory) {
	using namespace lib_calvin_sort;
	std::vector<double> result;
	switch (subCategory) {
	case BYTE_4:
		return std::vector<double>{sortBenchSub2<int>(algo, testSize, numIter)};
	case BYTE_16:
		return std::vector<double>{sortBenchSub2<SimpleStruct>(algo, testSize, numIter)};
	case BYTE_48:
		return std::vector<double>{sortBenchSub2<StringStruct>(algo, testSize, numIter)};
	default:
		cout << "sortBenchSub error!";
		exit(0);
	}
}

template <typename T>
double lib_calvin_benchmark::sorting::sortBenchSub2(Algorithm algo, size_t testSize, size_t numIter) {
	using namespace lib_calvin_sort;
	using std::less;
	std::cout << "Now benchmarking: " << testSize << " " << "object size: " << sizeof(T) <<
		" " << getAlgorithmNamesAndTags(algo)[0] << "\n";
	switch (algo) {
	case STD_SORT:
		return sortBenchTemplateSub(std::sort<T *, less<T>>, testSize, numIter);
	case STD_STABLE_SORT:
		return sortBenchTemplateSub(std::stable_sort<T *, less<T>>, testSize, numIter);

	case PDQSORT:
		return sortBenchTemplateSub(pdqsort_branchless<T *, less<T>>, testSize, numIter);

	case LIB_CALVIN_QSORT:
		return sortBenchTemplateSub(introSort<T *, less<T>>, testSize, numIter);
	case LIB_CALVIN_BLOCK_QSORT:
		return sortBenchTemplateSub(blockIntroSort<T *, less<T>>, testSize, numIter);
	case LIB_CALVIN_MERGESORT:
		return sortBenchTemplateSub(mergeSort<T *, less<T>>, testSize, numIter);

	case LIB_CALVIN_IN_PLACE_MERGESORT:
		return sortBenchTemplateSub(inPlaceMergeSort<T *, less<T>>, testSize, numIter);
	case LIB_CALVIN_IN_PLACE_MERGESORT2:
		return sortBenchTemplateSub(inPlaceMergeSort2<T *, less<T>>, testSize, numIter);
	case LIB_CALVIN_IN_PLACE_MERGESORT3:
		return sortBenchTemplateSub(inPlaceMergeSort3<T *, less<T>>, testSize, numIter);

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
