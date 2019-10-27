#ifndef LIB_CALVIN__STRING__STRING_MATCH_H
#define LIB_CALVIN__STRING__STRING_MATCH_H

#include "abstract_string.h"
#include "vector.h"
#include "suffix_tree.h"
#include <functional>

namespace lib_calvin_string
{
	using std::cout;
	using std::endl;

	// Z(i) = the length of the c_string that starts at index i and matches
	// ...the prefix of pattern
	template <typename Alphabet>
	void calculateZ(lib_calvin::abstract_string<Alphabet> const &pattern, std::vector<size_t> &record);

	template <typename Alphabet>
	void basicMatch(lib_calvin::abstract_string<Alphabet> const &text,
					lib_calvin::abstract_string<Alphabet> const &pattern,
					std::vector<size_t> &result);



} // end namespace lib_calvin_string

  /************************ Global functions *******************************/

  // Z(k) is the length of longest substring that starts at index k, which is
  // ...also prefix of the entire cstring. 
template <typename Alphabet>
void lib_calvin_string::calculateZ(
	lib_calvin::abstract_string<Alphabet> const &pattern, std::vector<size_t> &record) {

	size_t len = pattern.size();
	std::vector<size_t> &Z = record;
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

// using only calculateZ
// just find Z value for this transformed text: P$T 
// need a special character($) that is not present either in text or pattern
// using null character for this case
template <typename Alphabet>
void lib_calvin_string::basicMatch(lib_calvin::abstract_string<Alphabet> const &text,
								   lib_calvin::abstract_string<Alphabet> const &pattern, std::vector<size_t> &result) {
	result.clear();
	size_t patternLen = pattern.size();
	size_t textLen = text.size();
	Alphabet *pSpecialChar = new Alphabet(0);
	lib_calvin::abstract_string<Alphabet> newstring =
		pattern + lib_calvin::abstract_string<Alphabet>(pSpecialChar, 1) + text;
	std::vector<size_t> Z;
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



#endif