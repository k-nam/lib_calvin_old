#ifndef LIB_CALVIN__SORT__MERGE_SORT_H
#define LIB_CALVIN__SORT__MERGE_SORT_H

namespace lib_calvin_sort
{
	int const MERGESORT_THRESHOLD = 24;

	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void mergeSort(Iterator first, Iterator last, Comparator comp = Comparator());

	template <typename SrcIterator, typename TargetIterator, typename Comparator>
	void merge(SrcIterator first, SrcIterator middle, SrcIterator last,
			   TargetIterator target, Comparator comp);

	template <typename SrcIterator, typename TargetIterator, typename Comparator>
	void merge2(SrcIterator first, SrcIterator middle, SrcIterator last,
				TargetIterator target, Comparator comp);


	template <typename SrcIterator, typename TargetIterator, typename Comparator>
	void mergeSortSub(SrcIterator first, SrcIterator last, TargetIterator target, Comparator comp, bool targetIsReal);
}


template <typename Iterator, typename Comparator>
void lib_calvin_sort::mergeSort(Iterator first, Iterator last, Comparator comp) {
	using namespace std;
	ptrdiff_t num = last - first;
	// additional array for operation
	typedef typename iterator_traits<Iterator>::pointer pointerType;
	typedef typename iterator_traits<Iterator>::value_type valueType;
	// prepare supplementary array
	pointerType tempArray = (pointerType)operator new (sizeof(valueType) * num);
	for (ptrdiff_t i = 0; i < num; ++i) {
		new (tempArray + i)  valueType(std::move(*(first + i)));
	}

	// mergesort without redundant copying
	mergeSortSub(tempArray, tempArray + num, first, comp, false);
	// delete supplementary array
	for (ptrdiff_t i = 0; i < num; ++i) {
		tempArray[i].~valueType();
	}
	operator delete(tempArray);
}

template <typename SrcIterator, typename TargetIterator, typename Comparator>
void lib_calvin_sort::mergeSortSub(SrcIterator first, SrcIterator last,
								   TargetIterator target, Comparator comp, bool targetIsReal) {	
	size_t num = last - first;
	TargetIterator targetLast = target + num;
	if (last - first < MERGESORT_THRESHOLD && targetIsReal) {
		insertionSort(target, targetLast, comp);
		return;
	}

	SrcIterator middle = first + num / 2;
	TargetIterator targetMiddle = target + num / 2;
	mergeSortSub(target, targetMiddle, first, comp, !targetIsReal);
	mergeSortSub(targetMiddle, targetLast, middle, comp, !targetIsReal);
	lib_calvin_sort::merge(first, middle, last, target, comp);
}

// normal two-way merge; input size is not zero (both arrays are not empty)
template <typename SrcIterator, typename TargetIterator, typename Comparator>
void lib_calvin_sort::merge(SrcIterator first, SrcIterator middle, SrcIterator last,
							TargetIterator target, Comparator comp) {
	SrcIterator left = first, right = middle;
	TargetIterator dest = target;

	while (true) {
		if (comp(*right, *left)) {
			*dest++ = std::move(*right++);
			if (right == last) {
				break;
			}
		} else {
			*dest++ = std::move(*left++);
			if (left == middle) {
				break;
			}
		}		
	}

	if (right == last) { // right subarray empty
		while (left != middle) {
			*dest++ = std::move(*left++);
		}
	} else {
		while (right != last) { //left subarray empty
			*dest++ = std::move(*right++);
		}
	}
}

// normal two-way merge; input size is not zero (both arrays are not empty)
template <typename SrcIterator, typename TargetIterator, typename Comparator>
void lib_calvin_sort::merge2(SrcIterator first, SrcIterator middle, SrcIterator last,
							 TargetIterator target, Comparator comp) {
	// Parameter variables will be used later
	SrcIterator left = first;
	SrcIterator leftEnd = middle;
	SrcIterator	right = middle;
	SrcIterator	rightEnd = last;

	ptrdiff_t const bufferSize = 64;
	bool buffer[bufferSize];


	ptrdiff_t bufferIndexTarget = 0;

	while (true) {
		ptrdiff_t bufferIndex = 0;
		while (left < leftEnd && right < rightEnd && bufferIndex < bufferSize) {
			ptrdiff_t result = static_cast<ptrdiff_t>(comp(*right, *left));
			ptrdiff_t opposite = 1 - result;
			buffer[bufferIndex++] = opposite;
			left += opposite;
			right += result;
		}
		for (ptrdiff_t i = 0; i < bufferIndex; i++) {
			if (buffer[i]) {
				*target++ = *first++;
			} else {
				*target++ = *middle++;
			}
		}
		if (left == leftEnd || right == rightEnd) {
			break;
		}
	}

	if (right == rightEnd) { // right subarray empty
		while (left != leftEnd) {
			*target++ = std::move(*left++);
		}
	} else {
		while (right != rightEnd) { //left subarray empty
			*target++ = std::move(*right++);
		}
	}
}






#endif