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
		return { "Z algorithm" };
	case KMP:
		return { "KMP" };
	case BOYER:
		return { "Boyer-Moore" };
	case SUFFIX:
		return { "suffix tree" };

	default:
		return { "getAlgorithmName error!" };
	}
}

vector<CharSet>
lib_calvin_benchmark::string::getAllCharSets() {
	return vector<CharSet> { BINARY, DNA, ENG };
}

vector<Algorithm>
lib_calvin_benchmark::string::getAllAlgorithms() {
	return vector<Algorithm> { NAIVE, Z, KMP, BOYER, SUFFIX };
}

vector<Algorithm>
lib_calvin_benchmark::string::getLinearTimeAlgorithms() {
	return vector<Algorithm> { NAIVE, Z, KMP, BOYER, SUFFIX };
}

size_t 
lib_calvin_benchmark::string::getPatternLen(CharSet charSet) {
	switch (charSet) {
	case BINARY:
		return 1000;
	case DNA:
		return 300;
	case ENG:
		return 10;
	default:
		cout << "getPatternLen error!";
		exit(0);
	}
}

string
lib_calvin_benchmark::string::getTitle(CharSet charSet) {
	return "Charset size: " +  to_string(static_cast<int>(charSet)) + 
										 " / Pattern len: " + to_string(getPatternLen(charSet));
}

string
lib_calvin_benchmark::string::getSubCategory(TextType type) {
	switch (type) {
	case RANDOM:
		return "Randomized string";
	case MANY_NEAR_MISS:
		return "Frequent near misses";
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

	for (auto type: { RANDOM, MANY_NEAR_MISS }) {
		stringBench(type);
	}
}

void lib_calvin_benchmark::string::stringBench(TextType type) {

	for (auto charSet : getAllCharSets()) {
		stringBench(type, charSet);
	}
}

void lib_calvin_benchmark::string::stringBench(TextType type, CharSet charSet) {
	auto subCategoryString = getSubCategory(type);
	auto comment = "";
	vector<vector<double>> results;

	static size_t order = 0;

	vector<Algorithm> algos;
	if (type == RANDOM) {
		algos = getAllAlgorithms();
	} else {
		algos = getLinearTimeAlgorithms();
	}

	for (auto algorithm : algos) {
		results.push_back(stringBenchSub(type, charSet, getPatternLen(charSet), algorithm));

	}

	lib_calvin_benchmark::save_bench(category, subCategoryString, getTitle(charSet), comment,
										getAlgorithmNamesAndTagsVector(algos),
										results, benchCases, unit, order++);
}

vector<double> 
lib_calvin_benchmark::string::stringBenchSub(TextType type, CharSet charSet, size_t patternLen, Algorithm algo) {
	cout << "Now benchmarking: " << getSubCategory(type) <<
		" pattern len: " << patternLen <<
		" algorithm: " << getAlgorithmNamesAndTags(algo)[0] << "\n";

	vector<double> result;
	using namespace lib_calvin_string;
	using lib_calvin::stopwatch;
	lib_calvin::random_number_generator gen;
	typedef typename lib_calvin::abstract_string<char> c_string;
	size_t const numPatterns = 10;

	size_t alphabetSize = 0;
	switch (charSet) {
	case BINARY:
		alphabetSize = 2;
		break;
	case DNA:
		alphabetSize = 4;
		break;
	case ENG:
		alphabetSize = 26;
		break;
	default:
		cout << "stringBenchSub error!";
		exit(0);
	}

	for (auto testSize: benchTestSizes) {
		size_t textLen = testSize;

		char *pText = new char[textLen];
		char *pPatterns[numPatterns];

		for (size_t i = 0; i < textLen; ++i) {
			size_t number;
			if (type == RANDOM) {
				number = gen() ;
			} else {
				number = i;
			}
			pText[i] = '0' + static_cast<char>(number % alphabetSize);
		}
		for (size_t i = 0; i < numPatterns; i++) {
			pPatterns[i] = new char[patternLen];
			for (size_t j = 0; j < patternLen; j++) {
				size_t number;
				if (type == RANDOM) {
					number = gen();
				} else {
					number = j;
				}
				pPatterns[i][j] = '0' + static_cast<char>(number % alphabetSize);				
			}
			if (type == MANY_NEAR_MISS) {
				// Prevent matching
				pPatterns[i][patternLen - 1] = pPatterns[i][patternLen - 2];
			}
		}
		c_string text(pText, textLen);

		stopwatch watch;
		

		if (algo == SUFFIX) {
			lib_calvin::suffix_tree<char> tree(text);
			tree.build();

			watch.start();
			for (size_t i = 0; i < numPatterns; i++) {
				c_string pattern(pPatterns[i], patternLen);
				tree.find_pattern(pattern);
			}
			watch.stop();
		} else {
			watch.start();
			for (size_t i = 0; i < numPatterns; i++) {
				c_string pattern(pPatterns[i], patternLen);
				lib_calvin::vector<size_t> algorithmResult;
				switch (algo) {
				case NAIVE: {
					naiveMatch(text, pattern, algorithmResult);
					break;
				}
				case Z: {
					basicMatch(text, pattern, algorithmResult);
					break;
				}
				case KMP: {
					kmp(text, pattern, algorithmResult);
					break;
				}
				case BOYER: {
					boyerMoore(text, pattern, algorithmResult);
					break;
				}
				default:
					cout << "stringBenchSub error2!";
					exit(0);
				}
			}
			watch.stop();
		}
		result.push_back(textLen / (watch.read() / numPatterns) / 1000 / 1000);


		delete[] pText;
		for (size_t i = 0; i < numPatterns; i++) {
			delete[] pPatterns[i];
		}
	}	



	return result;
}
