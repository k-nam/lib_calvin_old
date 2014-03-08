#ifndef LIB_CALVIN__UTIL__UTILITY_H
#define LIB_CALVIN__UTIL__UTILITY_H

#include <string> 
#include <iostream>
#include <cstdlib>
#include "vector.h"

#undef max
#undef min

namespace lib_calvin_misc
{
	int const L1_SIZE = 32000;
	int const L2_SIZE = 6000000;
}

namespace lib_calvin_util
{
	using std::string;
	using std::cout;
	using std::endl;
	using lib_calvin::vector;

  template <typename T>
    void printContainer (T &object, string title);
  template <typename T>
    void printContainerReverse (T &object, string title);

  template <typename T>
    void swap(T &lhs, T &rhs);

  // returns the ceiling of lg x; T should be numeric
  template <typename T>
    int log(T x);

	template <typename T>
		T const &maximum(T const &, T const &);

	template <typename T>
		T const &minimum(T const &, T const &);
} 
 
// print the elements the a container
template <typename T>
void lib_calvin_util::printContainer (T &object, string title = "") {
	cout << title << endl;
	if (object.empty())
		return;
	typename T::const_iterator pos;
	typename T::const_iterator end = object.end();
	for (pos = object.begin(); pos != end; ++pos) {
	cout << *pos << ' ';
	}
	cout << endl; 
}

template <typename T>
void lib_calvin_util::printContainerReverse (T &object, string title = "") {
	cout << title << endl;
	if (object.empty())
		return;
	for (typename T::const_reverse_iterator pos = object.rbegin(); pos != object.rend(); ++pos) {
	cout << *pos << ' ';
	}
	cout << endl; 
}

// swap 
template <typename T>
void lib_calvin_util::swap (T &lhs, T &rhs) {
  T temp = lhs;
  lhs = rhs;
  rhs = temp;
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

template <typename T>
T const &
lib_calvin_util::maximum(T const &lhs, T const &rhs) {
	if (lhs < rhs) {
		return rhs;
	} else {
		return lhs;
	}
}

template <typename T>
T const &
lib_calvin_util::minimum(T const &lhs, T const &rhs) {
	if (lhs > rhs) {
		return rhs;
	} else {
		return lhs;
	}
}

#endif