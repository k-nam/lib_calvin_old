#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

#include "matrix_bench.h"
#include "bench.h"
#include "random.h"

using lib_calvin_matrix::matrix;
using namespace lib_calvin_benchmark::matrix;

std::vector<std::string>
lib_calvin_benchmark::matrix::getAlgorithmNamesAndTags(Algorithm algo) {
	switch (algo) {
	case MKL:
		return { "MKL", "parallel", "MMX" };

	case NAIVE:
		return { "naive method" };
	case NAIVE_TRANSPOSED:
		return { "naive with transpose" };
	case ROW_FIRST:
		return { "row first method" };
	case BLOCKING:
		return { "blocking method" };

	case NAIVE_MMX:
		return { "naive method (MMX)", "MMX" };
	case BLOCKING_MMX:
		return { "blocking method (MMX)", "MMX" };

	case RECURSIVE:
		return { "recursive method", "MMX" };
	case RECURSIVE_PARALLEL:
		return { "parallel recursive", "parallel", "MMX" };
	case STRASSEN:
		return { "Strassen method", "MMX" };
	case STRASSEN_PARALLEL:
		return { "parallel Strassen", "parallel", "MMX" };

	default:
		return { "getAlgorithmName error!" };
	}
}

std::vector<SubCategory> 
lib_calvin_benchmark::matrix::getAllSubCategories() {
	return std::vector<SubCategory> { MATRIX_MULTI_DOUBLE };
}

std::vector<Algorithm> 
lib_calvin_benchmark::matrix::getAllAlgorithms() {
	return std::vector<Algorithm> { 
			MKL, 
			NAIVE, NAIVE_TRANSPOSED, ROW_FIRST, BLOCKING,
			NAIVE_MMX, BLOCKING_MMX,
			RECURSIVE, RECURSIVE_PARALLEL, STRASSEN, STRASSEN_PARALLEL };
}

std::string
lib_calvin_benchmark::matrix::getTitle(size_t num) {
	size_t problemSize = benchTestSizes[num];
	auto sizeString = std::to_string(problemSize);
	return "Multiplying " + sizeString + " x " + sizeString + " matrices";
}

std::string
lib_calvin_benchmark::matrix::getSubCategory(SubCategory subCategory) {
	switch (subCategory) {
	case MATRIX_MULTI_DOUBLE:
		return "Floating point operation (8byte)";
	case MATRIX_MULTI_USER_DEFINED_OBJECT:
		return "User defined operation (16byte)";
	default:
		std::cout << "getSubCategory error!";
		exit(0);
	}
}

std::vector<std::vector<std::string>>
lib_calvin_benchmark::matrix::getAllNamesAndTagsVector() {
	using namespace std;
	vector<vector<string>> algorithmNamesAndTags = {};
	auto allAlgos = getAllAlgorithms();
	std::for_each(allAlgos.begin(), allAlgos.end(),
				  [&algorithmNamesAndTags](Algorithm algo) {
		algorithmNamesAndTags.push_back(getAlgorithmNamesAndTags(algo)); });
	return algorithmNamesAndTags;
}

void lib_calvin_benchmark::matrix::matrixBench() {
	for (auto subCategory : getAllSubCategories()) {
		for (size_t i = 0; i < benchNumCases; i++) {
			matrixBench(subCategory, i);
		}
	}
}

void lib_calvin_benchmark::matrix::matrixBench(SubCategory subCategory, size_t num) {
	using namespace std;
	using namespace lib_calvin_matrix;

	currentSubCategory = subCategory;
	currentBenchNum = num;


	string subCategoryString = getSubCategory(subCategory);
	string comment = "";
	vector<string> testCases = { subCategoryString };
	vector<vector<double>> results;
	for (auto algorithm : getAllAlgorithms()) {
		if (subCategory == MATRIX_MULTI_DOUBLE) {
			results.push_back({ matrixBenchSub<double>(algorithm) });
		} else {
			//results.push_back({ matrixBenchSub<int>(algorithm) });
		}		
	}

	lib_calvin_benchmark::save_bench(category, subCategoryString, getTitle(num), comment,
									 getAllNamesAndTagsVector(),
									 results, benchTestCase, unit, benchOrder[num]);
}

template <typename T>
double lib_calvin_benchmark::matrix::matrixBenchSub(Algorithm algo) {
	currentAlgo = algo;
	std::cout << "Now benchmarking: " << getSubCategory(currentSubCategory) << 
		" testSize: " << benchTestSizes[currentBenchNum] << 
		" algorithm: " << getAlgorithmNamesAndTags(algo)[0] << "\n";

	typedef std::function<void (lib_calvin_matrix::matrix<T> const &,
								lib_calvin_matrix::matrix<T> const &,
								lib_calvin_matrix::matrix<T> &)> funcType;
	switch (algo) {
	case MKL:
		return matrixBenchTemplateSub<T>(funcType(mklMultiAdd));

	case NAIVE:
		return matrixBenchTemplateSub<T>(funcType(naiveMultiAdd<T>));
	case NAIVE_TRANSPOSED:
		return matrixBenchTemplateSub<T>(funcType(naiveMultiAdd3<T>));
	case ROW_FIRST:
		return matrixBenchTemplateSub<T>(funcType(simpleMultiAdd<T>));
	case BLOCKING:
		return matrixBenchTemplateSub<T>(funcType(blockedMultiAdd<T>));

	case NAIVE_MMX:
		return matrixBenchTemplateSub<T>(funcType(naiveMultiAdd2<T>));
	case BLOCKING_MMX:
		return matrixBenchTemplateSub<T>(funcType(blockedMultiAddMmx<T>));

	case RECURSIVE:
		return matrixBenchTemplateSub<T>(funcType(recursiveMultiAddSingleThread<T>));
	case RECURSIVE_PARALLEL:
		return matrixBenchTemplateSub<T>(funcType(recursiveMultiAddParallel<T>));
	case STRASSEN:
		return matrixBenchTemplateSub<T>(funcType(strassenMultiAdd<T>));
	case STRASSEN_PARALLEL:
		return matrixBenchTemplateSub<T>(funcType(strassenMultiAddParallel<T>));

	default:
		std::cout << "matrixBenchSub error!";
		exit(0);
	}
}

template <typename T, typename Function>
double
lib_calvin_benchmark::matrix::matrixBenchTemplateSub(std::function<Function> func) {

	size_t testSize = benchTestSizes[currentBenchNum];
	size_t numIter = benchNumIter[currentBenchNum];

	if (!(currentAlgo == MKL || currentAlgo == BLOCKING_MMX ||
			currentAlgo == RECURSIVE || currentAlgo == RECURSIVE_PARALLEL) &&
		testSize > 1500) {
		// slow algorithms taka too much time if problem is big
		return 0;
	}

	lib_calvin_matrix::matrix<T> x = lib_calvin_matrix::matrix<T>(testSize);
	lib_calvin_matrix::matrix<T> y = lib_calvin_matrix::matrix<T>(testSize);
	lib_calvin_matrix::matrix<T> z = lib_calvin_matrix::matrix<T>(testSize);

	lib_calvin::stopwatch watch;

	lib_calvin::random_number_generator gen;

	double best = 1000000;
	double checkSum = 0;

	for (size_t i = 0; i < numIter; i++) {
		for (size_t temp_i = 0; temp_i < testSize; temp_i++) {
			for (size_t temp_j = 0; temp_j < testSize; temp_j++) {
				x.set_val(temp_i, temp_j, static_cast<T>(gen()));
				x.set_val(temp_i, temp_j, static_cast<T>(gen()));
			}
		}
		watch.start();
		func(x, y, z);
		watch.stop();

		for (size_t temp_i = 0; temp_i < testSize; temp_i++) {
			for (size_t temp_j = 0; temp_j < testSize; temp_j++) {
				checkSum += z(temp_i, temp_j);
			}
		}
		
		best = std::min(best, watch.read());
	}

	std::cout << "Record: " << best << " checksum: " << checkSum << "\n";
	return testSize * testSize * testSize / best / 1000 / 1000 / 1000;
}
