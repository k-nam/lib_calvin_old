#include "stopwatch.h"
#include "string_test.h"
#include "string_matching.h"
#include "naive_matching.h"
#include "kmp.h"
#include "boyer_moore.h"
#include "suffix_tree.h"
#include "suffix_tree_test.h"
#include "sort.h"
#include "std_boyer_moore_horspool.h"

using namespace lib_calvin;

void lib_calvin_string::stringTest() {
	std::cout << "---------- Beginning string test -----------\n\n";
	abstractStringTest();
	matchingAlgorithmTest();
	suffixTreeTest();
	SuffixTreeTest<char>::Node::countThisObject();
	std::cout << "---------- String test finished -----------\n\n\n";
}

void lib_calvin_string::abstractStringTest() {
	abstract_string<char> a = "abc";
	a.print();
	abstract_string<char> b = "def";
	abstract_string<char> c = a + b;
	c.print();
	c += b;
	c.print();;
	abstract_string<char> d = c.substr(2, 5);
	d.print();
	std::cout << "\n";
}

void lib_calvin_string::matchingAlgorithmTest() {
	using namespace lib_calvin_string;
	size_t numIter = 1;
	typedef char Alphabet;
	std::cout << "-------------- matchingAlgorithmTest start ----------------\n\n";
	matchingTest<Alphabet>(naiveMatch, "Naive string matching");
	matchingTest<Alphabet>(basicMatch, "Basic string matching(Z-alg)");
	matchingTest<Alphabet>(kmp, "LIB_CALVIN_KMP");
	matchingTest<Alphabet>(boyerMoore, "Boyer-Moore");
	matchingTest<Alphabet>(std_boyer_moore_horspool, "std::boyer_moore_horspool_searcher");
	for (size_t i = 0; i < numIter; i++) {
		matchingTest<Alphabet>(suffixTreeMatching, "Suffix Tree");
	}
	std::cout << "--------------- matchingAlgorithmTest finished --------------\n\n";
}

// c_string class is generic with alphabet type, but we can not unit-test unless
// ... Alphabet is of numerical type. So just test with characters.
template <typename Alphabet>
void lib_calvin_string::matchingTest(void(*matchingCharAlg)
(abstract_string<Alphabet> const &text, abstract_string<Alphabet> const &pattern,
									 std::vector<size_t> &record), std::string title) {
	std::vector<size_t> record;
	std::vector<size_t> answer;
	lib_calvin::stopwatch watch;

	Alphabet alphabetSize = 2;
	size_t textLen = 1000* 1000;
	size_t patternLen = 32;
	Alphabet *pText = new Alphabet[textLen];
	Alphabet *pPattern = new Alphabet[patternLen];
	// use only small number of alphabets to make test realistic
	// don't use null character 0 (reserved for algorithms)
	for (size_t i = 0; i < textLen; ++i) {
		pText[i] = rand() % alphabetSize + '0';
	}
	abstract_string<Alphabet> text(pText, textLen);
	for (size_t iter = 0; iter < 1; iter++) {
		for (size_t i = 0; i < patternLen; ++i) {
			pPattern[i] = rand() % alphabetSize + '0';
		}
		abstract_string<Alphabet> pattern(pPattern, patternLen);
		// get right answer to compare
		naiveMatch(text, pattern, answer);

		watch.start();
		matchingCharAlg(text, pattern, record);
		watch.stop();
		cout << "Finished running: " << title << endl;
		cout << "Running time: " << watch.read() << endl;
		cout << "# of match was: " << record.size() << endl;
		// sorting is needed as suffix tree's result is not in ascending order
		lib_calvin::intro_sort(record.begin(), record.end());
		if (answer == record)
			cout << "Matching is correct.\n";
		else {
			cout << "Matching error!!\n";
			//text.print();
			cout << "Pattern was:\n";
			pattern.print();
			size_t realsize = answer.size();
			size_t wrongsize = record.size();
			cout << "Real match number : " << realsize << endl;
			cout << "Real match:\n";
			for (auto iter = answer.begin(); iter != answer.end(); ++iter) {
				std::cout << *iter << "\n";
			}
			cout << "Wrong match number : " << wrongsize << endl;
			cout << "Wrong match:\n";
			for (auto iter = record.begin(); iter != record.end(); ++iter) {
				std::cout << *iter << "\n";
			}
			cout << "There was:\n";
			text.substr(*answer.begin(), *answer.begin() + pattern.length() + 2).print();
			exit(0);
		}
	}
	delete[] pText;
	delete[] pPattern;
	cout << "\n";
}

void lib_calvin_string::palindromeTest() {

}