#ifndef LIB_CALVIN__BOOST_STRING_ALGORITHM_H
#define LIB_CALVIN__BOOST_STRING_ALGORITHM_H


#include "boost/algorithm/searching/boyer_moore.hpp"
#include "boost/algorithm/searching/boyer_moore_horspool.hpp"
#include "boost/algorithm/searching/knuth_morris_pratt.hpp"

namespace lib_calvin_benchmark
{
namespace string
{
template <typename Alphabet>
void boost_boyer_moore_horspool(lib_calvin::abstract_string<Alphabet> const &text,
								lib_calvin::abstract_string<Alphabet> const &pattern, std::vector<size_t> &result) {

	auto matchStart = text.begin();
	while (true) {
		matchStart = boost::algorithm::boyer_moore_horspool_search(
			matchStart, text.end(), pattern.begin(), pattern.end()).first;
		if (matchStart == text.end()) {
			break;
		} else {
			result.push_back(matchStart - text.begin());
			matchStart++;
		}
	}
}

template <typename Alphabet>
void boost_boyer_moore(lib_calvin::abstract_string<Alphabet> const &text,
					   lib_calvin::abstract_string<Alphabet> const &pattern, std::vector<size_t> &result) {

	auto matchStart = text.begin();
	while (true) {
		matchStart = boost::algorithm::boyer_moore_search(
			matchStart, text.end(), pattern.begin(), pattern.end()).first;
		if (matchStart == text.end()) {
			break;
		} else {
			result.push_back(matchStart - text.begin());
			matchStart++;
		}
	}
}

template <typename Alphabet>
void boost_kmp(lib_calvin::abstract_string<Alphabet> const &text,
			   lib_calvin::abstract_string<Alphabet> const &pattern, std::vector<size_t> &result) {

	auto matchStart = text.begin();
	while (true) {
		matchStart = boost::algorithm::knuth_morris_pratt_search(
			matchStart, text.end(), pattern.begin(), pattern.end()).first;
		if (matchStart == text.end()) {
			break;
		} else {
			result.push_back(matchStart - text.begin());
			matchStart++;
		}
	}
}
}
}
#endif