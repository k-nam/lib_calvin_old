#ifndef LIB_CALVIN__NEURAL_NETWORK_BENCH_H
#define LIB_CALVIN__NEURAL_NETWORK_BENCH_H

#include <vector>
#include <string>

#include "neural_network.h"

namespace lib_calvin_benchmark
{
namespace neural_network
{

size_t const benchNumCases = 7;
std::vector<std::string> const benchTestCase = { "accuracy" };

std::string const unit = "% (higher is better)";
std::string const category = "Etc.";

static size_t currentBenchNum = 0;

enum Algorithm {
	MKL,

	NAIVE,
	NAIVE_TRANSPOSED,
	ROW_FIRST,
	BLOCKING,

	NAIVE_MMX,
	BLOCKING_MMX,

	RECURSIVE,
	RECURSIVE_PARALLEL,
	STRASSEN,
	STRASSEN_PARALLEL
};

static Algorithm currentAlgo;

enum SubCategory {
	OPTIMAL,
	DROP
};

static SubCategory currentSubCategory;

std::string getTitle(size_t problemSize);

std::string getSubCategory(SubCategory);

std::vector<SubCategory> getAllSubCategories();

std::vector<Algorithm> getAllAlgorithms();

std::vector<std::string> getAlgorithmNamesAndTags(Algorithm);

std::vector<std::vector<std::string>> getAlgorithmNamesAndTagsVector(std::vector<Algorithm>);
std::vector<std::vector<std::string>> getAllNamesAndTagsVector();

void matrixBench();

void matrixBench(SubCategory, size_t currentNum);

double
matrixBenchSub(Algorithm);

template <typename Function>
double matrixBenchTemplateSub(std::function<Function>);

template <typename Function>
double matrixBenchTemplateSub(std::function<Function> func, size_t testSize, size_t numIter);

}
}
#endif
