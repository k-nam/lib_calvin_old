#ifndef LIB_CALVIN__STRING__NAIVE_MATCHING_H
#define LIB_CALVIN__STRING__NAIVE_MATCHING_H

#include "abstract_string.h"

namespace lib_calvin_string
{
	using std::cout;
	using std::endl;

	// save the matching indices in the third argument
	template <typename Alphabet>
	void naiveMatch(lib_calvin::abstract_string<Alphabet> const &text,
					lib_calvin::abstract_string<Alphabet> const &pattern,
					std::vector<size_t> &result);

} 

 
template <typename Alphabet>
void lib_calvin_string::naiveMatch(
	lib_calvin::abstract_string<Alphabet> const &text,
	lib_calvin::abstract_string<Alphabet> const &pattern, std::vector<size_t> &result)
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




#endif