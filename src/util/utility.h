#ifndef LIB_CALVIN__UTIL__UTILITY_H
#define LIB_CALVIN__UTIL__UTILITY_H

#include <string> 
#include <iostream>
#include <cstdlib>

namespace lib_calvin_util
{
	// returns the ceiling of lg x; T should be numeric
	template <typename T>
	int log(T x);
}

// ceiling of lg x
template <typename T>
int lib_calvin_util::log(T x) {
	int lg = 0;
	double y = (double)x;
	while (y > 1) {
		y = y / 2;
		lg++;
	}
	return lg;
}

#endif