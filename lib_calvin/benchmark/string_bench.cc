#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

#include "string_bench.h"
#include "abstract_string.h"
#include "naive_matching.h"
#include "kmp.h"
#include "boyer_moore.h"
#include "boost_string_algorithm.h"
#include "std_boyer_moore_horspool.h"
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
	case BOOST_KMP:
		return { "boost::knuth_morris_pratt" };
	case LIB_CALVIN_KMP:
		return { "lib_calvin::knuth_morris_pratt" };
	case STD_BOYER_MOORE_HORSPOOL:
		return { "std::boyer_moore_horspool" };
	case BOOST_BOYER_MOORE_HORSPOOL:
		return { "boost::boyer_moore_horspool" };
	case BOOST_BOYER_MOORE:
		return { "boost::boyer_moore" };
	case LIB_CALVIN_BOYER_MOORE:
		return { "lib_calvin::boyer_moore" };
	case LIB_CALVIN_SUFFIX:
		return { "lib_calvin::suffix_tree" };

	default:
		return { "getAlgorithmName error!" };
	}
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
	case CYCLIC_TEXT_ERROR:
		return "Cyclic / wrong character in text";
	case CYCLIC_PATTERN_ERROR:
		return "Cyclic / wrong character in pattern";
	case MANY_MATCH:
		return "Frequent matches";
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

vector<Algorithm>
lib_calvin_benchmark::string::getAllAlgorithms() {
	return {
		//NAIVE, 
		BOOST_KMP, 
		LIB_CALVIN_KMP,
		//STD_BOYER_MOORE_HORSPOOL,
		//BOOST_BOYER_MOORE_HORSPOOL,
		//BOOST_BOYER_MOORE,
		//LIB_CALVIN_BOYER_MOORE,
		//LIB_CALVIN_SUFFIX 
	};
}

vector<vector<string>>
lib_calvin_benchmark::string::getAllNamesAndTagsVector() {
	return getAlgorithmNamesAndTagsVector(getAllAlgorithms());
}

void lib_calvin_benchmark::string::stringBench() {

	//for (auto type: {  RANDOM, CYCLIC_TEXT_ERROR, CYCLIC_PATTERN_ERROR }) {
	for (auto type: { CYCLIC_TEXT_ERROR }) {
		stringBench(type);
	}
}

vector<CharSet>
lib_calvin_benchmark::string::getAllCharSets() {
	//return vector<CharSet> { BINARY, DNA, ENG };
	return vector<CharSet> {BINARY };

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

	vector<Algorithm> algos  = getAllAlgorithms();

	for (auto algorithm : algos) {
		results.push_back(stringBenchSub(type, charSet, getPatternLen(charSet), algorithm));

	}

	auto testCases = lib_calvin_benchmark::getSizeStrings(benchTestSizes);
	lib_calvin_benchmark::save_bench(category, subCategoryString, getTitle(charSet), comment,
										getAlgorithmNamesAndTagsVector(algos),
										results, testCases, unit, order++);
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
	size_t const numPatterns = 50;

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

		if (type == CYCLIC_TEXT_ERROR) {
			for (size_t i = 0; i + patternLen < textLen; i += patternLen) {
				//size_t randomIndex = i + gen() % patternLen;
				pText[i + patternLen / 2]++;
			}
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
			// Prevent matching						
			if (type == CYCLIC_PATTERN_ERROR) {
				size_t randomIndex = gen() % patternLen;
				pPatterns[i][randomIndex]++;
			}
		}
		c_string text(pText, textLen);

		stopwatch watch;
		double best = 1000000;

		if (algo == LIB_CALVIN_SUFFIX) {
			lib_calvin::suffix_tree<char> tree(text);
			tree.build();
			for (size_t i = 0; i < numPatterns; i++) {
				c_string pattern(pPatterns[i], patternLen);
				watch.start();				
				tree.find_pattern(pattern);
				watch.stop();
				best = std::min(best, watch.read());
			}			
		} else {
			
			for (size_t i = 0; i < numPatterns; i++) {
				c_string pattern(pPatterns[i], patternLen);
				std::vector<size_t> algorithmResult;
				watch.start();
				switch (algo) {
				case NAIVE: {
					naiveMatch(text, pattern, algorithmResult);
					break;
				}
				case BOOST_KMP: {
					boost_kmp(text, pattern, algorithmResult);
					break;
				}
				case LIB_CALVIN_KMP: {
					kmp(text, pattern, algorithmResult);
					break;
				}
				case STD_BOYER_MOORE_HORSPOOL: {
					std_boyer_moore_horspool(text, pattern, algorithmResult);
					break;
				}
				case BOOST_BOYER_MOORE_HORSPOOL: {
					boost_boyer_moore_horspool(text, pattern, algorithmResult);
					break;
				}
				case BOOST_BOYER_MOORE: {
					boost_boyer_moore(text, pattern, algorithmResult);
					break;
				}
				case LIB_CALVIN_BOYER_MOORE: {
					boyerMoore(text, pattern, algorithmResult);
					break;
				}
				default:
					cout << "stringBenchSub error2!" << algorithmResult.size();
					exit(0);
				}
				watch.stop();
				best = std::min(best, watch.read());
			}		
		}
		cout << "Measured: " << textLen / best / 1000 / 1000 << "\n";
		result.push_back(textLen / best / 1000 / 1000);

		delete[] pText;
		for (size_t i = 0; i < numPatterns; i++) {
			delete[] pPatterns[i];
		}
	}	

	return result;
}

