#ifndef LIB_CALVIN__UTIL__UTILITY_H
#define LIB_CALVIN__UTIL__UTILITY_H

#include <string> 
#include <iostream>
#include <cstdlib>

namespace lib_calvin_misc
{
	int const L1_SIZE = 32000;
	int const L2_SIZE = 6000000;
}

namespace lib_calvin_util
{
using std::cout;
using std::endl;

// returns the ceiling of lg x; T should be numeric
template <typename T>
int log(T x);

} 
 
// ceiling of lg x
template <typename T>
int lib_calvin_util::log(T x) {
  int lg = 0;
  double y = (double) x;
  while (y > 1) {
    y = y / 2;
    lg++;
  }
  return lg;
}

#endif