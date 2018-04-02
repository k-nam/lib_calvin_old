#ifndef LIB_CALVIN__STRING__STRING_MATCH_H
#define LIB_CALVIN__STRING__STRING_MATCH_H

#include "abstract_string.h"
#include "vector.h"
#include "suffix_tree.h"
#include <cstdint>

namespace lib_calvin_string
{
	using std::cout;
	using std::endl;

	// Z(i) = the length of the c_string that starts at index i and matches
	// ...the prefix of pattern
	template <typename Alphabet>
	void calculateZ(lib_calvin::abstract_string<Alphabet> const &pattern, lib_calvin::vector<size_t> &record);

	// f(i) = the maximum length of a prefix of pattern which is also a 
	// ...proper suffix of substring pattern[0 i-1]
	template <typename Alphabet>
	void calculateF(lib_calvin::abstract_string<Alphabet> const &pattern, lib_calvin::vector<size_t> &record);

	// maps each Alphabet to the index in which the Alphabet first appears in the 
	// ..pattern (looking from the right-end of patttern)
	// assumes that Alphabet can be converted to unsigned int type (which is the
	// ..index in record)
	template <typename Alphabet>
	void badChar(lib_calvin::abstract_string<Alphabet> const &pattern, lib_calvin::vector<size_t> &record);

	template <typename Alphabet>
	void strongGoodSuffix(lib_calvin::abstract_string<Alphabet> const &pattern,
						  lib_calvin::vector<size_t> &record);

	// save the matching indices in the third argument
	template <typename Alphabet>
	void naiveMatch(lib_calvin::abstract_string<Alphabet> const &text,
					lib_calvin::abstract_string<Alphabet> const &pattern,
					lib_calvin::vector<size_t> &result);

	template <typename Alphabet>
	void basicMatch(lib_calvin::abstract_string<Alphabet> const &text,
					lib_calvin::abstract_string<Alphabet> const &pattern,
					lib_calvin::vector<size_t> &result);

	template <typename Alphabet>
	void kmp(lib_calvin::abstract_string<Alphabet> const &text,
			 lib_calvin::abstract_string<Alphabet> const &pattern,
			 lib_calvin::vector<size_t> &result);

	template <typename Alphabet>
	void boyerMoore(lib_calvin::abstract_string<Alphabet> const &text,
					lib_calvin::abstract_string<Alphabet> const &pattern,
					lib_calvin::vector<size_t> &result);

	template <typename Alphabet>
	void suffixTreeMatching(lib_calvin::abstract_string<Alphabet> const &text,
							lib_calvin::abstract_string<Alphabet> const &pattern,
							lib_calvin::vector<size_t> &result);

} // end namespace lib_calvin_string

  /************************ Global functions *******************************/

  // Z(k) is the length of longest substring that starts at index k, which is
  // ...also prefix of the entire c_string. 
template <typename Alphabet>
void lib_calvin_string::calculateZ(
	lib_calvin::abstract_string<Alphabet> const &pattern, lib_calvin::vector<size_t> &record) {

	size_t len = pattern.size();
	lib_calvin::vector<size_t> &Z = record;
	record.clear();
	record.resize(len, 0);
	// l(k), r(k): the leftmost and rightmost indices of the substring 
	// ...that spans most to the right and starts at index i <= k 
	size_t l = 0; // stores l(k - 1) 
	size_t r = 0; // stores r(k - 1)
	Z[0] = 0; // this value will not be used
	size_t k, m; // for indices
	for (k = 1; k < len; k++) { // k is current index in the text
		if (r < k) { // start manual matching 
			if (pattern[k] != pattern[0]) { // first character bad
				Z[k] = 0;
			} else { // first character good
				for (m = k + 1; m < len; m++) {
					if (pattern[m] != pattern[m - k])
						break;
				}
				m--; // note that this will also take care of the case when m == len
					 // Now, m is the index of last matching character
				l = k;
				r = m;
				Z[k] = m - k + 1;
			}
		} else { // use previous data
			size_t remainingMatch = Z[k - l];
			size_t remainingBox = r - k + 1;
			if (remainingMatch < remainingBox) { // match is contained in Z-block
				Z[k] = Z[k - l];
			} else if (remainingMatch == remainingBox) { // possible to have more matching
				for (m = r + 1; m < len; m++) {
					if (pattern[m] != pattern[m - k])
						break;
				}
				m--;
				// m is the index of last matching character
				l = k;
				r = m;
				Z[k] = m - k + 1;
			} else { // match is just the same as k-1
				Z[k] = r - k + 1;
			}
		}
	}
}

// f(k) is the length of longest prefix of the c_string P which is also suffix of
//  P[0...k-1]   so, f(len) should be determined.
// f(k) value determines the jump length when pattern does not match at index k
//  in KMP algorithm. f(len) denotes the jump length when a match has been found.
template <typename Alphabet>
void lib_calvin_string::calculateF(
	lib_calvin::abstract_string<Alphabet> const &pattern, lib_calvin::vector<size_t> &record) {
	size_t len = pattern.size();
	size_t k;
	size_t m; // for indices
	lib_calvin::vector<size_t> &f = record;
	record.clear();
	record.resize(len + 1);
	f[0] = 0; // this value is not used, but convenient for below procedure
	f[1] = 0; //c_string of length 1 can not have proper prefix
	for (k = 2; k <= len; k++) { // include past-end char
		m = f[k - 1]; // m is the index to consider f[m] value
		bool didReachZero = false;
		while (pattern[m] != pattern[k - 1]) {
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

// Be careful!!! Output is indexed in REVERSE ORDER!!!
// record[i] denotes the index (from the right) of the first occurrence of
// character i.
template <typename Alphabet>
void lib_calvin_string::badChar(
	lib_calvin::abstract_string<Alphabet> const &pattern, lib_calvin::vector<size_t> &record) {

	record.clear();
	size_t len = pattern.size();
	size_t index;
	// we need to know the size of character set (Alphabet::size)
	// Characters not present in the pattern is marked as len; this will
	// ...make the shift amount appropriate
	if (sizeof(Alphabet) > 1) { // inappropriate for this algorithm
		std::cout << "badChar for large charset\n";
		//exit(0);
	}
	record.resize(lib_calvin::getSizeOfCharSet<Alphabet>(), len);
	for (size_t i = 0; i < len; i++) {
		index = static_cast<size_t>(pattern[i]);
		record[index] = len - 1 - i;  // reversed index to fit Boyer-Moore
	}
}

// Be careful!!! Output is indexed in REVERSE ORDER!!!
// The shift amount determined by strong good suffix rule is recorded in the
// array. Record[i] is the amount to shift (to right) if wrong character was
// detected on index i (from right).
template <typename Alphabet>
void lib_calvin_string::strongGoodSuffix(
	lib_calvin::abstract_string<Alphabet> const &pattern, lib_calvin::vector<size_t> &record) {
	size_t len = pattern.size();
	record.clear();
	record.resize(len + 1, len);
	// use Z algorithm in reverse c_string
	lib_calvin::vector<size_t> Z;
	calculateZ(pattern.reverse(), Z);
	for (size_t i = 1; i < len; ++i) {
		// we must not overwrite; otherwise, we will get greater jump value than
		// the right one.
		if (record[Z[i]] == len) {
			record[Z[i]] = i;
		}
	}
	// additional processing 
	size_t left = len; // possible leftmost element not marked
	for (size_t i = 1; i < len; ++i) {
		if (i + Z[i] == len) { // match goes through the end of c_string
			for (size_t j = left; j > len - i; j--) {
				// we must not overwrite, like above.
				if (record[j] == len) {
					record[j] = i;
				}
			}
			left = len - i;
		}
	}
	for (size_t k = 0; k < len + 1; k++) { // maximum shift for -1
		if (record[k] == len) {
			record[k] = len;
		}
	}
	// not to be used, but the right answer is 1 
	record[0] = 1;
}

template <typename Alphabet>
void lib_calvin_string::naiveMatch(
	lib_calvin::abstract_string<Alphabet> const &text,
	lib_calvin::abstract_string<Alphabet> const &pattern, lib_calvin::vector<size_t> &result)
{
	result.clear();
	size_t textLen = text.size();
	size_t patternLen = pattern.size();
	size_t k, s; // k for text index, s for pattern index
	for (k = 0; k <= textLen - patternLen; k++) { // last possible match
		for (s = 0; s < patternLen; s++) {
			if (text[k + s] != pattern[s])
				break;
		}
		if (s == patternLen) { // match detected
			result.push_back(k);
		}
	}
}

// using only calculateZ
// just find Z value for this transformed text: P$T 
// need a special character($) that is not present either in text or pattern
// using null character for this case
template <typename Alphabet>
void lib_calvin_string::basicMatch(lib_calvin::abstract_string<Alphabet> const &text,
								   lib_calvin::abstract_string<Alphabet> const &pattern, lib_calvin::vector<size_t> &result) {
	result.clear();
	size_t patternLen = pattern.size();
	size_t textLen = text.size();
	Alphabet *pSpecialChar = new Alphabet(0);
	lib_calvin::abstract_string<Alphabet> newstring =
		pattern + lib_calvin::abstract_string<Alphabet>(pSpecialChar, 1) + text;
	lib_calvin::vector<size_t> Z;
	calculateZ(newstring, Z);
	for (size_t i = patternLen + 1; i < textLen + patternLen + 1; ++i) {
		if (Z[i] == patternLen) { // match detected
			result.push_back(i - patternLen - 1);
		}
		if (Z[i] > patternLen) {
			cout << "basicMatch error with Z algorithm\n";
			exit(0);
		}
	}
	delete pSpecialChar;
}

// This was hardest to micro-optimize of all codes I wrote until now.
// It seems that using goto statement is unavoidable here.
template <typename Alphabet>
void lib_calvin_string::kmp(
	lib_calvin::abstract_string<Alphabet> const &text,
	lib_calvin::abstract_string<Alphabet> const &pattern, lib_calvin::vector<size_t> &result) {
	result.clear();
	size_t patternLen = pattern.size();
	size_t textLen = text.size();
	lib_calvin::vector<size_t> f;
	calculateF(pattern, f);
	size_t k = 0, s = 0;

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

template <typename Alphabet>
void lib_calvin_string::boyerMoore(
	lib_calvin::abstract_string<Alphabet> const &text,
	lib_calvin::abstract_string<Alphabet> const &pattern, lib_calvin::vector<size_t> &result) {
	result.clear();
	lib_calvin::abstract_string<Alphabet> reverse = pattern.reverse();
	size_t textLen = text.size();
	size_t patternLen = pattern.size();
	// h: head in text, k: index in text, s: index in pattern (reversed)
	size_t h = patternLen - 1, k = patternLen - 1, s = 0;
	size_t badCharJump = 0, goodSuffixJump = 0;
	bool matched = false;
	lib_calvin::vector<size_t> charTable;
	lib_calvin::vector<size_t> suffixTable;
	badChar(pattern, charTable);
	strongGoodSuffix(pattern, suffixTable);
	while (h < textLen) {
		// sequntial matching
		while (true) {
			if (text[k] != reverse[s]) { // using reverse for convenient indexing
				matched = false;
				break;
			}
			k--;
			s++;
			if (s == patternLen) { // match at (k + 1)
				result.push_back(k + 1);
				badCharJump = 0; // can not use badchar when matched
				matched = true;
				break;
			}
		}
		// jump forward
		if (!matched) {
			badCharJump = charTable[static_cast<size_t>(text[k])];
			if (badCharJump > s) {
				badCharJump -= s;
			} else {
				badCharJump = 0;
			}
		}
		goodSuffixJump = suffixTable[s];
		if (badCharJump > goodSuffixJump) {
			h += badCharJump;
		} else {
			h += goodSuffixJump;
		}
		k = h;
		s = 0;
	}
}

template <typename Alphabet>
void lib_calvin_string::suffixTreeMatching(lib_calvin::abstract_string<Alphabet> const &text,
										   lib_calvin::abstract_string<Alphabet> const &pattern, lib_calvin::vector<size_t> &result) {
	lib_calvin::suffix_tree<Alphabet> tree(text);
	// return value of find_pattern is (textId, startIndex) pair
	tree.build();
	auto temp = tree.find_pattern(pattern);
	for (auto iter = temp.begin(); iter != temp.end(); ++iter) {
		result.push_back(iter->second);
	}
}


#endif