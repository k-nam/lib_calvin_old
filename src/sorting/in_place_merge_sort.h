#ifndef LIB_CALVIN__SORT__IN_PLACE_MERGE_SORT_H
#define LIB_CALVIN__SORT__IN_PLACE_MERGE_SORT_H

#include "merge_sort.h"

namespace lib_calvin_sort
{
	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void inPlaceMergeSort(Iterator first, Iterator last, Comparator const comp = Comparator());

	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void inPlaceMergeSort2(Iterator first, Iterator last, Comparator const comp = Comparator());

	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void inPlaceMergeSort3(Iterator first, Iterator last, Comparator const comp = Comparator());

	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void inPlaceMergeSortSub(Iterator first, Iterator last, 
							 Comparator const comp = Comparator(), ptrdiff_t threshold = 0);

	template <typename Iterator, typename Comparator>
	void inPlaceMerge(Iterator first, Iterator middle, Iterator last, Comparator comp);

	template <typename Iterator>
	void exchange(Iterator first, Iterator middle, Iterator last);

	template <typename Iterator>
	void reverse(Iterator first, Iterator last);
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::inPlaceMergeSort(Iterator first, Iterator last, Comparator comp) {
	if (last - first < MERGESORT_THRESHOLD) {
		insertionSort(first, last, comp);
		return;
	}
	double const ratio = 0.0;
	ptrdiff_t threshold = static_cast<size_t>(ratio * (last - first));
	inPlaceMergeSortSub(first, last, comp, threshold);
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::inPlaceMergeSort2(Iterator first, Iterator last, Comparator comp) {
	if (last - first < MERGESORT_THRESHOLD) {
		insertionSort(first, last, comp);
		return;
	}
	double const ratio = 0.01;
	ptrdiff_t threshold = static_cast<size_t>(ratio * (last - first));
	inPlaceMergeSortSub(first, last, comp, threshold);
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::inPlaceMergeSort3(Iterator first, Iterator last, Comparator comp) {
	if (last - first < MERGESORT_THRESHOLD) {
		insertionSort(first, last, comp);
		return;
	}
	double const ratio = 0.1;
	ptrdiff_t threshold = static_cast<size_t>(ratio * (last - first));
	inPlaceMergeSortSub(first, last, comp, threshold);
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::inPlaceMergeSortSub(Iterator first, Iterator last,
									  Comparator comp, ptrdiff_t threshold) {
	ptrdiff_t size = last - first;
	if (last - first < MERGESORT_THRESHOLD) {
		insertionSort(first, last, comp);
		return;
	}
	if (last - first < threshold) {
		mergeSort(first, last, comp);
		return;
	}
	Iterator middle = first + (last - first) / 2;
	inPlaceMergeSortSub(first, middle, comp, threshold);
	inPlaceMergeSortSub(middle, last, comp, threshold);
	inPlaceMerge(first, middle, last, comp);
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::inPlaceMerge(Iterator first, Iterator middle, Iterator last,
										  Comparator comp) {
	ptrdiff_t size = last - first;
	if (last - middle < MERGESORT_THRESHOLD || middle - first < MERGESORT_THRESHOLD) {
		insertionSort(first, last, comp);
		return;
	}
	std::inplace_merge(first, middle, last, comp);
}

template <typename Iterator>
void lib_calvin_sort::exchange(Iterator first, Iterator middle, Iterator last) {
	reverse(first, middle);
	reverse(middle, last);
	reverse(first, last);
}

template <typename Iterator>
void lib_calvin_sort::reverse(Iterator first, Iterator last) {
	Iterator left = first;
	Iterator right = last - 1;
	while (left < right) {
		std::swap(*left, *right);
		++left;
		--right;
	}
}




#endif