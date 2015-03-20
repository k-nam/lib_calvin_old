#include "stopwatch.h"
#include "string_test.h"
#include "string_matching.h"
#include "suffix_tree.h"

void lib_calvin_string::stringTest() { 
	std::cout << "---------- Beginning string test -----------\n\n";
	using namespace lib_calvin_string;
  matchingTest(naiveMatch, "Naive string matching");
  matchingTest(basicMatch, "Basic string matching(Z-alg)");
  matchingTest(kmp, "KMP");
  matchingTest(boyerMoore, "Boyer-Moore");

	lib_calvin::suffix_tree<char> a("aa");
	std::cout << "---------- String test finished -----------\n\n\n";
}

// string class is generic with alphabet type, but we can not unit-test unless
// ... Alphabet is of numerical type. So just test with characters.
void lib_calvin_string::matchingTest (void (*matchingCharAlg) 
    (abstract_string<> const &text, abstract_string<> const &pattern, 
     vector<size_t> &record), std::string title) {
  vector<size_t> record;
  vector<size_t> answer;
	lib_calvin::stopwatch watch;

  size_t textLen   = 1000000;
  size_t patternLen  = 16;
  char *pText   = new char[textLen];
  char *pPattern  = new char[patternLen];
  // use only small number of alphabets to make test realistic
  // don't use null character 0 (reserved for algorithms)
  for (int i = 0; i < textLen; ++i) {
    pText[i] = rand() % 2 + 2;
  }	
  for (int i = 0; i < patternLen; ++i) {
    pPattern[i] = rand() % 2 + 2;
  }
	/*
	pPattern[0] = 2;
	pPattern[0] = 3;
	pPattern[0] = 2;
	pPattern[0] = 3;
	pPattern[0] = 4;
	pPattern[0] = 2;
	pPattern[0] = 3;
	pPattern[0] = 4;
	*/

  abstract_string<> text  (pText, textLen);
  abstract_string<> pattern (pPattern, patternLen);

  // get right answer to compare
  naiveMatch(text, pattern, answer);

  watch.start();
  matchingCharAlg(text, pattern, record);
  watch.stop();
  cout << title << endl;
  cout << "Running time: " << watch.read() << endl;
  cout << "# of match was: " << record.size() << endl;
  if (answer == record)
    cout << "Matching is correct.\n";
  else {
    cout << "Matching error!!\n";
    size_t realsize = answer.size();
    size_t wrongsize = record.size();
    cout << "Real match number : " << realsize << endl;
    cout << "Wrong match number : " << wrongsize << endl;
		exit(0);
  }
  delete[] pText;
  delete[] pPattern;
	cout << "\n";
}
