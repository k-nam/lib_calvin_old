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
		return { "naive" };
	case NAIVE_TRANSPOSED:
		return { "naive with transpose" };
	case ROW_FIRST:
		return { "row first" };
	case BLOCKING:
		return { "blocking" };

	case NAIVE_MMX:
		return { "naive (MMX)", "MMX" };
	case BLOCKING_MMX:
		return { "blocking (MMX)", "MMX" };

	case RECURSIVE:
		return { "recursive", "MMX" };
	case RECURSIVE_PARALLEL:
		return { "parallel recursive", "parallel", "MMX" };
	case STRASSEN:
		return { "Strassen", "MMX" };
	case STRASSEN_PARALLEL:
		return { "parallel Strassen", "parallel", "MMX" };

	default:
		return { "getAlgorithmName error!" };
	}
}

std::vector<SubCategory> 
lib_calvin_benchmark::matrix::getAllSubCategories() {
	return std::vector<SubCategory> { OPTIMAL };
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
	case OPTIMAL:
		return "8byte (floating point) / best case performance";
	case DROP:
		return "8byte (floating point) / performance drop";
	default:
		std::cout << "getSubCategory error!";
		exit(0);
	}
}

std::vector<std::vector<std::string>>
lib_calvin_benchmark::matrix::getAlgorithmNamesAndTagsVector(std::vector<Algorithm> algorithms) {
	using namespace std;
	vector<vector<string>> algorithmNamesAndTags = {};
	
	std::for_each(algorithms.begin(), algorithms.end(),
				  [&algorithmNamesAndTags](Algorithm algo) {
		algorithmNamesAndTags.push_back(getAlgorithmNamesAndTags(algo)); });
	return algorithmNamesAndTags;
}


std::vector<std::vector<std::string>>
lib_calvin_benchmark::matrix::getAllNamesAndTagsVector() {
	return getAlgorithmNamesAndTagsVector(getAllAlgorithms());
}

void lib_calvin_benchmark::matrix::matrixBench() {

	for (size_t i = 0; i < benchNumCases; i++) {
		matrixBench(OPTIMAL, i);
	}
	matrixBench(DROP, 0);
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

	if (subCategory == OPTIMAL) {		
		for (auto algorithm : getAllAlgorithms()) {
			results.push_back({ matrixBenchSub(algorithm) });

		}
		lib_calvin_benchmark::save_bench(category, subCategoryString, getTitle(num), comment,
										 getAllNamesAndTagsVector(),
										 results, benchTestCase, unit, num);
	} else {
		std::vector<size_t> testSizes = { 400, 640, 800, 1280, 1600, 1920, 2560, 3200, 3840, 5120, 6400, 8000, 10240 };
		auto algorithms = {MKL, RECURSIVE_PARALLEL};
		auto title = getAlgorithmNamesAndTags(MKL)[0] + " vs " + getAlgorithmNamesAndTags(RECURSIVE_PARALLEL)[0];
		vector<string> testCases;
		std::for_each(testSizes.begin(), testSizes.end(), 
					  [&testCases](size_t size) { testCases.push_back(std::to_string(size)); });
		for (auto algorithm: algorithms) {
			typedef std::function<void(lib_calvin_matrix::matrix<double> const &,
									   lib_calvin_matrix::matrix<double> const &,
									   lib_calvin_matrix::matrix<double> &)> funcType;
			funcType a;
			if (algorithm == MKL) {
				a = funcType(mklMultiAdd);
			} else {
				a = funcType(recursiveMultiAddParallel<double>);
				
			}
			vector<double> result;
			for (size_t i = 0; i < testSizes.size(); i++) {
				result.push_back( matrixBenchTemplateSub(a, testSizes[i], 1) );
			}	
			results.push_back(result);
		}
		lib_calvin_benchmark::save_bench(category, subCategoryString, "MKL vs recursive parallel", comment,
										 getAlgorithmNamesAndTagsVector(algorithms),
										 results, testCases, unit, num);	}
}

double lib_calvin_benchmark::matrix::matrixBenchSub(Algorithm algo) {
	currentAlgo = algo;
	std::cout << "Now benchmarking: " << getSubCategory(currentSubCategory) << 
		" testSize: " << benchTestSizes[currentBenchNum] << 
		" algorithm: " << getAlgorithmNamesAndTags(algo)[0] << "\n";

	typedef std::function<void (lib_calvin_matrix::matrix<double> const &,
								lib_calvin_matrix::matrix<double> const &,
								lib_calvin_matrix::matrix<double> &)> funcType;
	switch (algo) {
	case MKL:
		return matrixBenchTemplateSub(funcType(mklMultiAdd));

	case NAIVE:
		return matrixBenchTemplateSub(funcType(naiveMultiAdd<double>));
	case NAIVE_TRANSPOSED:
		return matrixBenchTemplateSub(funcType(naiveMultiAdd3<double>));
	case ROW_FIRST:
		return matrixBenchTemplateSub(funcType(simpleMultiAdd<double>));
	case BLOCKING:
		return matrixBenchTemplateSub(funcType(blockedMultiAdd<double>));

	case NAIVE_MMX:
		return matrixBenchTemplateSub(funcType(naiveMultiAdd2<double>));
	case BLOCKING_MMX:
		return matrixBenchTemplateSub(funcType(blockedMultiAddMmx<double>));

	case RECURSIVE:
		return matrixBenchTemplateSub(funcType(recursiveMultiAddSingleThread<double>));
	case RECURSIVE_PARALLEL:
		return matrixBenchTemplateSub(funcType(recursiveMultiAddParallel<double>));
	case STRASSEN:
		return matrixBenchTemplateSub(funcType(strassenMultiAdd<double>));
	case STRASSEN_PARALLEL:
		return matrixBenchTemplateSub(funcType(strassenMultiAddParallel<double>));

	default:
		std::cout << "matrixBenchSub error!";
		exit(0);
	}
}

template <typename Function>
double
lib_calvin_benchmark::matrix::matrixBenchTemplateSub(std::function<Function> func) {
	size_t testSize = benchTestSizes[currentBenchNum];
	size_t numIter = benchNumIter[currentBenchNum];
	return matrixBenchTemplateSub(func, testSize, numIter);
}

template <typename Function>
double
lib_calvin_benchmark::matrix::matrixBenchTemplateSub(std::function<Function> func, size_t testSize, size_t numIter) {
	if (!(currentAlgo == MKL || currentAlgo == BLOCKING_MMX ||
		currentAlgo == RECURSIVE || currentAlgo == RECURSIVE_PARALLEL) &&
		testSize > 1500) {
		// slow algorithms taka too much time if problem is big
		return 0;
	}

	lib_calvin_matrix::matrix<double> x(testSize);
	lib_calvin_matrix::matrix<double> y(testSize);
	lib_calvin_matrix::matrix<double> z(testSize);

	lib_calvin::stopwatch watch;

	lib_calvin::random_number_generator gen;

	double best = 1000000;
	double checkSum = 0;

	for (size_t i = 0; i < numIter; i++) {
		for (size_t temp_i = 0; temp_i < testSize; temp_i++) {
			for (size_t temp_j = 0; temp_j < testSize; temp_j++) {
				x.set_val(temp_i, temp_j, static_cast<double>(gen()));
				x.set_val(temp_i, temp_j, static_cast<double>(gen()));
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
