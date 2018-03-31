#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

#include "string_matching.h"
#include "abstract_string.h"
#include "string_bench.h"
#include "bench.h"
#include "random.h"
#include "stopwatch.h"


using namespace lib_calvin_benchmark::string;
using namespace std;

vector<string>
lib_calvin_benchmark::string::getAlgorithmNamesAndTags(Algorithm algo) {
	switch (algo) {

	case NAIVE:
		return { "naive" };
	case Z:
		return { "z-algorithm" };
	case KMP:
		return { "KMP" };
	case BOYWER:
		return { "Boywer-Moore" };
	case SUFFIX:
		return { "suffix tree" };

	default:
		return { "getAlgorithmName error!" };
	}
}

vector<SubCategory>
lib_calvin_benchmark::string::getAllSubCategories() {
	return vector<SubCategory> { BINARY, GENE, ENG };
}

vector<Algorithm>
lib_calvin_benchmark::string::getAllAlgorithms() {
	return vector<Algorithm> { NAIVE, Z, KMP, BOYWER, SUFFIX };
}

string
lib_calvin_benchmark::string::getTitle(size_t problemSize) {
	auto sizeString = to_string(problemSize);
	return "Matching text (" + to_string(problemSize/1000/1000) +  "M) with pattern (" + to_string(patternLen) + ")";
}

string
lib_calvin_benchmark::string::getSubCategory(SubCategory subCategory) {
	switch (subCategory) {
	case BINARY:
		return "Binary string";
	case GENE:
		return "4-letter string";
	case ENG:
		return "English string (26)";
	default:
		cout << "getSubCategory error!";
		exit(0);
	}
}

vector<vector<string>>
lib_calvin_benchmark::string::getAlgorithmNamesAndTagsVector(vector<Algorithm> algorithms) {
	vector<vector<std::string>> algorithmNamesAndTags = {};

	for_each(algorithms.begin(), algorithms.end(),
				  [&algorithmNamesAndTags](Algorithm algo) {
		algorithmNamesAndTags.push_back(getAlgorithmNamesAndTags(algo)); });
	return algorithmNamesAndTags;
}


vector<vector<string>>
lib_calvin_benchmark::string::getAllNamesAndTagsVector() {
	return getAlgorithmNamesAndTagsVector(getAllAlgorithms());
}

void lib_calvin_benchmark::string::stringBench() {

	for (auto subCategory: getAllSubCategories()) {
		stringBench(subCategory);
	}
}

void lib_calvin_benchmark::string::stringBench(SubCategory subCategory) {

	for (size_t testsize : benchTestSizes) {
		stringBench(subCategory, testsize);
	}
}

void lib_calvin_benchmark::string::stringBench(SubCategory subCategory, size_t testSize) {
	auto subCategoryString = getSubCategory(subCategory);
	auto comment = "";
	vector<std::string> benchTestCase = { "string matching" };
	vector<vector<double>> results;

	static size_t order = 0;

	for (auto algorithm : getAllAlgorithms()) {
		results.push_back({ stringBenchSub(subCategory, testSize, algorithm, benchNumIter) });

	}

	lib_calvin_benchmark::save_bench(category, subCategoryString, getTitle(testSize), comment,
										getAllNamesAndTagsVector(),
										results, benchTestCase, unit, order++);	
}

vector<double> 
lib_calvin_benchmark::string::stringBenchSub(SubCategory subCategory, size_t textLen, 
											 Algorithm algo, vector<size_t> numIters) {
	cout << "Now benchmarking: " << getSubCategory(subCategory) <<
		" testSize: " << textLen <<
		" algorithm: " << getAlgorithmNamesAndTags(algo)[0] << "\n";

	vector<double> result;

	auto fixedAndVariable = stringBenchSub2(subCategory, textLen, algo);
	for (auto numIter: numIters) {
		result.push_back(fixedAndVariable.first + fixedAndVariable.second * numIter);
	}
	return result;
}

pair<double, double>
lib_calvin_benchmark::string::stringBenchSub2(SubCategory subCategory, size_t textLen,
											 Algorithm algo) {

	using namespace lib_calvin_string;
	using lib_calvin::stopwatch;

	vector<double> result;

	typedef typename lib_calvin::abstract_string<char> c_string;
	char *pText = new char[textLen];
	char *pPattern = new char[patternLen];
	lib_calvin::vector<size_t> algorithmResult;
	size_t alphabetSize = 0;

	switch (subCategory) {
	case BINARY:
		alphabetSize = 2;
		break;
	case GENE:
		alphabetSize = 4;
		break;
	case ENG:
		alphabetSize = 4;
		break;
	default:
		cout << "stringBenchSub2 error!";
		exit(0);
	}

	lib_calvin::random_number_generator gen;	
	for (size_t i = 0; i < textLen; ++i) {
		pText[i] = '0' + gen() % alphabetSize;
	}
	for (size_t i = 0; i < patternLen; i++) {
		pPattern[i] ='0' + gen() % alphabetSize;
	}

	c_string text(pText, textLen);
	c_string pattern(pPattern, patternLen);

	stopwatch watch;
	watch.start();

	switch (algo) {

	case NAIVE: {
		naiveMatch(text, pattern, algorithmResult);
		watch.stop();
		return make_pair(0, watch.read());
	}
	case Z: {
		basicMatch(text, pattern, algorithmResult);
		watch.stop();
		return make_pair(0, watch.read());
	}
	case KMP: {
		kmp(text, pattern, algorithmResult);
		watch.stop();
		return make_pair(0, watch.read());
	}
	case BOYWER: {
		boyerMoore(text, pattern, algorithmResult);
		watch.stop();
		return make_pair(0, watch.read());
	}
	case SUFFIX:{
		lib_calvin::suffix_tree<char> tree(text);
		tree.build();
		watch.stop();
		double fixed = watch.read();

		watch.start();
		tree.find_pattern(pattern);
		watch.stop();
		return make_pair(fixed, watch.read());
	}
	default:
		cout << "stringBenchSub2 error2!";
		exit(0);
	}
}

