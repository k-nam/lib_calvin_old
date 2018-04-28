#ifndef LIB_CALVIN__STRING__KMP_H
#define LIB_CALVIN__STRING__KMP_H

#include "abstract_string.h"

namespace lib_calvin_string
{
using std::cout;
using std::endl;


// f(i) = the maximum length of a prefix of pattern which is also a 
// ...proper suffix of substring pattern[0 i-1]
template <typename Alphabet>
void calculateF(lib_calvin::abstract_string<Alphabet> const &pattern, std::vector<size_t> &record);

template <typename Alphabet>
void kmp(lib_calvin::abstract_string<Alphabet> const &text,
		 lib_calvin::abstract_string<Alphabet> const &pattern,
		 std::vector<size_t> &result);
}

// f(k) is the length of longest prefix of the c_string P which is also suffix of
//  P[0...k-1]   so, f(len) should be determined.
// f(k) value determines the jump length when pattern does not match at index k
//  in LIB_CALVIN_KMP algorithm. f(len) denotes the jump length when a match has been found.
template <typename Alphabet>
void lib_calvin_string::calculateF(
	lib_calvin::abstract_string<Alphabet> const &pattern, std::vector<size_t> &record) {
	size_t len = pattern.size();
	size_t k;
	size_t m; // for indices
	std::vector<size_t> &f = record;
	record.clear();
	record.resize(len + 1);
	f[0] = 0; // this value is not used, but convenient for below procedure
	f[1] = 0; //c_string of length 1 can not have proper prefix
	for (k = 2; k <= len; k++) { // include past-end char
		m = f[k - 1]; // m is the index to consider f[m] value
		bool didReachZero = false;
		while (pattern[m] != pattern[k - 1] ||
			(k < len && pattern[m + 1] == pattern[k])) {
			if (m != 0) {
				m = f[m];
			} else {
				didReachZero = true;
				break;
			}
		}
		if (didReachZero) {
			f[k] = 0;
		} else {
			f[k] = m + 1;
		}
	}
}

// This was hardest to micro-optimize of all codes I wrote until now.
// It seems that using goto statement is unavoidable here.
template <typename Alphabet>
void lib_calvin_string::kmp(
	lib_calvin::abstract_string<Alphabet> const &text,
	lib_calvin::abstract_string<Alphabet> const &pattern, std::vector<size_t> &result) {
	result.clear();
	size_t patternLen = pattern.size();
	size_t textLen = text.size();
	std::vector<size_t> f;
	calculateF(pattern, f);
	size_t k = 0;
	size_t s = 0;

	// about to match k (text index), s (pattern index)
AboutToMatch:
	while (true) {
		if (s == patternLen) { // match
			result.push_back(k - patternLen);
			if (k == textLen) {
				return;
			} else {
				goto Wrong;
			}
		}
		if (k == textLen) { // process all done
			return;
		}
		if (text[k] != pattern[s]) {
			goto Wrong;
		} else {
			k++;
			s++;
		}
	}
	// wrong match detected k, s
Wrong:
	if (s == 0) {
		goto FirstWrong;
	}

	s = f[s];
	if (text[k] != pattern[s]) {
		goto Wrong;
	} else {
		k++;
		s++;
		goto AboutToMatch;
	}
	// wrong match and s == 0
FirstWrong:
	k++;
	while (true) {
		if (k == textLen) {
			return;
		}
		if (text[k] == pattern[0]) {
			k++;
			s++;
			goto AboutToMatch;
		} else {
			k++;
		}
	}
}


#endif