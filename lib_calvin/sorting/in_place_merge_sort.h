#ifndef LIB_CALVIN__SORT__IN_PLACE_MERGE_SORT_H
#define LIB_CALVIN__SORT__IN_PLACE_MERGE_SORT_H

namespace lib_calvin_sort
{
	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void inPlaceMergeSort(Iterator first, Iterator last, Comparator const comp = Comparator());

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
	Iterator middle = first + (last - first) / 2;
	inPlaceMergeSort(first, middle, comp);
	inPlaceMergeSort(middle, last, comp);
	inPlaceMerge(first, middle, last, comp);
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::inPlaceMerge(Iterator first, Iterator middle, Iterator last, Comparator comp) {
	if (middle - first < MERGESORT_THRESHOLD || last - middle < MERGESORT_THRESHOLD) {
		insertionSort(first, last, comp);
		return;
	}
	Iterator leftMiddle = first + (middle - first) / 2;
	Iterator rightMiddle = middle + binSearch(*leftMiddle, middle, last, comp);
	exchange(leftMiddle, middle, rightMiddle);
	inPlaceMerge(first, leftMiddle, leftMiddle + (rightMiddle - middle), comp);
	inPlaceMerge(leftMiddle + (rightMiddle - middle), rightMiddle, last, comp);
	for (Iterator iter = first; iter < last - 1; iter++) {
		if (comp(*(iter + 1), *iter)) {
			std::cout << "inPlaceMerge error: " << middle - first << " " << last - middle << "\n";
			exit(0);
		}
	}
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