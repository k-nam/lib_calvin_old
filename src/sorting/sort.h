#ifndef LIB_CALVIN__SORT__SORT_H
#define LIB_CALVIN__SORT__SORT_H

// 2008-03-08

#include "stopwatch.h"
#include <cstdlib>
#include <algorithm>
#include <inttypes.h>

#include "utility.h"
#include "thread.h"
#include "factory.h"


#include "intro_sort.h"
#include "merge_sort.h"

namespace lib_calvin_sort
{
	using std::pair;
	using std::iterator_traits;	
	
	int const L1_CACHE_SIZE = 32000;
	int const L2_CACHE_SIZE = 256000;
	int const CACHE_LINE_SIZE = 64;
	int const PARALLEL_THRE = 8000;

} // end namespace lib_calvin_sort

namespace lib_calvin
{


	template <typename Iterator, typename Comparator =
		std::less<typename std::iterator_traits<Iterator>::value_type>>
		void sort(Iterator first, Iterator last, Comparator comp = Comparator()) {
		lib_calvin_sort::blockIntroSort(first, last, comp);
	}

	template <typename Iterator, typename Comparator =
		std::less<typename std::iterator_traits<Iterator>::value_type>>
		void intro_sort(Iterator first, Iterator last, Comparator comp = Comparator()) {
		lib_calvin_sort::introSort(first, last, comp);
	}

	template <typename Iterator, typename Comparator =
		std::less<typename std::iterator_traits<Iterator>::value_type>>
		void merge_sort(Iterator first, Iterator last, Comparator comp = Comparator()) {
		lib_calvin_sort::mergeSort(first, last, comp);
	}
}


#endif

