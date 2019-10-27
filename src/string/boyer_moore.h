#ifndef LIB_CALVIN__STRING__BOYER_MOORE_H
#define LIB_CALVIN__STRING__BOYER_MOORE_H

#include "abstract_string.h"
#include "string_matching.h"

namespace lib_calvin_string
{
using std::cout;
using std::endl;

// maps each Alphabet to the index in which the Alphabet first appears in the 
// ..pattern (looking from the right-end of patttern)
// assumes that Alphabet can be converted to unsigned int type (which is the
// ..index in record)
template <typename Alphabet>
void badChar(lib_calvin::abstract_string<Alphabet> const &pattern, std::vector<size_t> &record);

template <typename Alphabet>
void strongGoodSuffix(lib_calvin::abstract_string<Alphabet> const &pattern,
					  std::vector<size_t> &record);

template <typename Alphabet>
void boyerMoore(lib_calvin::abstract_string<Alphabet> const &text,
				lib_calvin::abstract_string<Alphabet> const &pattern,
				std::vector<size_t> &result);

}


template <typename Alphabet>
void lib_calvin_string::boyerMoore(
	lib_calvin::abstract_string<Alphabet> const &text,
	lib_calvin::abstract_string<Alphabet> const &pattern, std::vector<size_t> &result) {

	result.clear();
	size_t textLen = text.size();
	size_t patternLen = pattern.size();

	int64_t badCharJump = 0;
	int64_t	goodSuffixJump = 0;
	std::vector<size_t> charTable;
	std::vector<size_t> suffixTable;
	badChar(pattern, charTable);
	strongGoodSuffix(pattern, suffixTable);

	int64_t t = 0;
	auto textP = &text[0];
	auto textPEnd = &text[0] + textLen - patternLen;
	auto patternP = &pattern[0];
	while (textP <= textPEnd) {
		t = patternLen;
		while (textP[t - 1] == patternP[t - 1]) {
			t--;
			if (t == 0) {
				result.push_back(textP - &text[0]);
				badCharJump = 0;
				goto Match;
			}
			//std::cout << "Comparing: " << t << "\n";
		}

		badCharJump = charTable[(textP[t - 1])] - (patternLen - t);
	Match:
		if (badCharJump > 0 && badCharJump > static_cast<int64_t>(suffixTable[patternLen - t])) {
			textP += badCharJump;
		} else {
			textP += suffixTable[patternLen - t];
		}

		//std::cout << "Suffix: " << goodSuffixJump << " ";
		//std::cout << "Badchar: " <<  badCharJump << " ";
		//std::cout << "Jumping: " << std::max(goodSuffixJump, badCharJump) << "\n";
	}
}

// Be careful!!! Output is indexed in REVERSE ORDER!!!
// record[i] denotes the index (from the right) of the first occurrence of
// character i.
template <typename Alphabet>
void lib_calvin_string::badChar(
	lib_calvin::abstract_string<Alphabet> const &pattern, std::vector<size_t> &record) {

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

	// Maximum jump for characters not appearing in the pattern
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
	lib_calvin::abstract_string<Alphabet> const &pattern, std::vector<size_t> &record) {
	size_t len = pattern.size();
	record.clear();
	record.resize(len + 1, len);
	// use Z algorithm in reverse c_string
	std::vector<size_t> Z;
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
		if (i + Z[i] == len) { // match goes through the end of string
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

#endif