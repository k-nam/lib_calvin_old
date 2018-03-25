#ifndef LIB_CALVIN__SORT__INTRO_SORT_H
#define LIB_CALVIN__SORT__INTRO_SORT_H

#include <minmax.h>

namespace lib_calvin_sort {

	int const INTROSORT_THRESHOLD = 24;
	typedef unsigned char blockQsortIndexType;
	size_t const blockQsortBufferSize = 192;

	// quicksort with pivot selection with median, hoare partitioning,
	// ...and using heapsort to make worst performance n*log(n)
	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void introSort(Iterator first, Iterator last, Comparator const comp = Comparator());

	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void blockIntroSort(Iterator first, Iterator last, Comparator const comp = Comparator());
	// Returns the iterator for the first element of the right subarray
	// Caution: this assumes that input array has at least 3 elements
	template <typename Iterator, typename Comparator>
	Iterator hoarePartition(Iterator first, Iterator last, Comparator comp);

	template <typename Iterator, typename Comparator>
	Iterator betterPartition(Iterator first, Iterator last,
							 blockQsortIndexType *leftBuffer, blockQsortIndexType *rightBuffer, Comparator comp);

	template <typename Iterator, typename Comparator>
	void introSortSub(Iterator first, Iterator last, Comparator comp, int remainingDepth);

	// using counting sort for subroutine
	template <typename Iterator, typename Comparator>
	void introSort2Sub(Iterator first, Iterator last,
					   blockQsortIndexType *leftBuffer, blockQsortIndexType *rightBuffer,
					   Comparator comp, int remainingDepth);


}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::introSort(Iterator first, Iterator last, Comparator comp) {
	introSortSub(first, last, comp, lib_calvin_util::log(last - first) * 3);
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::blockIntroSort(Iterator first, Iterator last, Comparator comp) {

	blockQsortIndexType leftBuffer[blockQsortBufferSize];
	blockQsortIndexType rightBuffer[blockQsortBufferSize];

	introSort2Sub(first, last, leftBuffer, rightBuffer, comp, lib_calvin_util::log(last - first) * 3);
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::introSortSub(Iterator first, Iterator last, Comparator comp,
								   int remainingDepth) {
	if (last - first < INTROSORT_THRESHOLD) {
		insertionSort(first, last, comp);
		return;
	}
	if (remainingDepth == 0) {
		//std::cout << "heapsort\n";
		heapSort(first, last, comp);
		return;
	}
	Iterator left = hoarePartition(first, last, comp);

	introSortSub(first, left, comp, remainingDepth - 1);
	introSortSub(left + 1, last, comp, remainingDepth - 1);
}

template <typename Iterator, typename Comparator>
Iterator lib_calvin_sort::hoarePartition(Iterator first, Iterator last, Comparator comp) {
	Iterator left = first;
	Iterator right = last - 1;
	Iterator middle = first + (last - first) / 2;


	if (comp(*middle, *left)) {
		std::iter_swap(middle, left);
	}
	if (comp(*right, *middle)) {
		std::iter_swap(right, middle);
	}
	if (comp(*middle, *left)) {
		std::iter_swap(middle, left);
	}

	// Leave pivot alone at the first position
	std::iter_swap(middle, left);
	auto pivot = *first;
	left++;

	// Make initial state (left and reight are to be swapped)	
	while (left < last && comp(*left, pivot)) {
		++left;
	}
	while (right > first && !comp(*right, pivot)) {
		--right;
	}
	//if (right == first) { // all elements are >= pivot
	//	return first;
	//}

	while (left < right) {
		std::iter_swap(left, right);
		++left;
		--right;
		while (comp(*left, pivot)) {
			++left;
		}
		while (!comp(*right, pivot)) {
			--right;
		}
	}

	std::iter_swap(first, left - 1);
	return left - 1;
}

template <typename Iterator, typename Comparator>
Iterator lib_calvin_sort::betterPartition(Iterator begin, Iterator end,
										  blockQsortIndexType *leftBuffer, blockQsortIndexType *rightBuffer, Comparator comp) {
	Iterator left = begin;
	Iterator right = end - 1;
	Iterator middle = begin + (end - begin) / 2;

	if (end - begin < 100) {
		if (comp(*middle, *left)) {
			std::iter_swap(middle, left);
		}
		if (comp(*right, *middle)) {
			std::iter_swap(right, middle);
		}
		if (comp(*middle, *left)) {
			std::iter_swap(middle, left);
		}

		// Leave pivot alone at the first position
		std::iter_swap(middle, left);
	} else if (end - begin < 1000) {
		insertionSort(middle, middle + 5, comp);
		std::iter_swap(left, middle + 2);
	} else {
		insertionSort(middle, middle + 9, comp);
		std::iter_swap(left, middle + 4);
	}

	auto pivot = *left;
	left++;

	// These buffers contain index of elements to be swapped
	// Buffer size MUST be a multiple of loop-unrolling count (typically 8)
	ptrdiff_t l_begin = 0;
	ptrdiff_t l_end = 0;
	ptrdiff_t r_begin = 0;
	ptrdiff_t r_end = 0;

	ptrdiff_t leftBatchSize = blockQsortBufferSize;
	ptrdiff_t rightBatchSize = blockQsortBufferSize;

	bool isLeftEmpty = true;
	bool isRightEmpty = true;
	bool mustBreak = false;

	while (true) {
		if (mustBreak) {
			break;
		}
		if (right - left + 1 < 2 * blockQsortBufferSize) {
			mustBreak = true;
			ptrdiff_t numRemainingElements = right - left + 1;
			ptrdiff_t numUnprocessed = numRemainingElements - blockQsortBufferSize;

			if (isLeftEmpty && isRightEmpty) {
				leftBatchSize = numRemainingElements / 2;
				rightBatchSize = numRemainingElements - leftBatchSize;
			} else if (isLeftEmpty) {
				leftBatchSize = numUnprocessed;
			} else {
				rightBatchSize = numUnprocessed;
			}
		} 

		if (isLeftEmpty) {
			auto tempIter = left;
			if (mustBreak) {
				for (blockQsortIndexType i = 0; i < leftBatchSize; ) {
					leftBuffer[l_end] = i++; l_end += !comp(*tempIter++, pivot);
				}
			} else {
				for (blockQsortIndexType i = 0; i < leftBatchSize; ) {
					leftBuffer[l_end] = i++; l_end += !comp(*tempIter++, pivot);
					leftBuffer[l_end] = i++; l_end += !comp(*tempIter++, pivot);
					leftBuffer[l_end] = i++; l_end += !comp(*tempIter++, pivot);
					leftBuffer[l_end] = i++; l_end += !comp(*tempIter++, pivot);
					leftBuffer[l_end] = i++; l_end += !comp(*tempIter++, pivot);
					leftBuffer[l_end] = i++; l_end += !comp(*tempIter++, pivot);
					leftBuffer[l_end] = i++; l_end += !comp(*tempIter++, pivot);
					leftBuffer[l_end] = i++; l_end += !comp(*tempIter++, pivot);
				}
			}
		}
		if (isRightEmpty) {
			auto tempIter = right;
			if (mustBreak) {
				for (blockQsortIndexType i = 0; i < rightBatchSize; i) {
					rightBuffer[r_end] = i++; r_end += comp(*tempIter--, pivot);
				}
			} else {
				for (blockQsortIndexType i = 0; i < rightBatchSize; i) {
					rightBuffer[r_end] = i++; r_end += comp(*tempIter--, pivot);
					rightBuffer[r_end] = i++; r_end += comp(*tempIter--, pivot);
					rightBuffer[r_end] = i++; r_end += comp(*tempIter--, pivot);
					rightBuffer[r_end] = i++; r_end += comp(*tempIter--, pivot);
					rightBuffer[r_end] = i++; r_end += comp(*tempIter--, pivot);
					rightBuffer[r_end] = i++; r_end += comp(*tempIter--, pivot);
					rightBuffer[r_end] = i++; r_end += comp(*tempIter--, pivot);
					rightBuffer[r_end] = i++; r_end += comp(*tempIter--, pivot);
				}
			}
		}

		// Do swap now
		ptrdiff_t swapCount = min(l_end - l_begin, r_end - r_begin);
		if (swapCount != 0) {
			ptrdiff_t l_temp = l_begin;
			ptrdiff_t l_temp_end = l_begin + swapCount;
			ptrdiff_t r_temp = r_begin;

			// Zig-zag swapping
			auto store = *(left + leftBuffer[l_temp]);
			*(left + leftBuffer[l_temp++]) = std::move(*(right - rightBuffer[r_temp]));
			while (l_temp < l_temp_end) {
				*(right - rightBuffer[r_temp++]) = std::move(*(left + leftBuffer[l_temp]));
				*(left + leftBuffer[l_temp++]) = std::move(*(right - rightBuffer[r_temp]));
			}
			*(right - rightBuffer[r_temp]) = std::move(store);
		}

		l_begin += swapCount;
		r_begin += swapCount;

		isLeftEmpty = false;
		isRightEmpty = false;

		if (l_begin == l_end) {
			isLeftEmpty = true;
			l_begin = 0;
			l_end = 0;
			left += leftBatchSize;
		}
		if (r_begin == r_end) {
			isRightEmpty = true;
			r_begin = 0;
			r_end = 0;
			right -= rightBatchSize;
		}
	}

	auto pivotPosition = left - 1;
	// All elements have been processsed, but
	// for the last non-empty buffer, we have to move all out-of-place elems to one side
	if (!isLeftEmpty) {
		for (ptrdiff_t i = l_end - 1; i >= l_begin; i--) {
			std::iter_swap(left + leftBuffer[i], right);
			right--;
		}
		pivotPosition = right;
	}
	if (!isRightEmpty) {
		for (ptrdiff_t i = r_end - 1; i >= r_begin; i--) {
			std::iter_swap(right - rightBuffer[i], left);
			left++;
		}
		pivotPosition = left - 1;
	}

	std::iter_swap(begin, pivotPosition);
	return pivotPosition;
}



template <typename Iterator, typename Comparator>
void lib_calvin_sort::introSort2Sub(Iterator first, Iterator last,
									blockQsortIndexType *leftBuffer, blockQsortIndexType *rightBuffer,
									Comparator comp, int remainingDepth) {
	if (last - first < INTROSORT_THRESHOLD) {
		insertionSort(first, last, comp);
		return;
	}
	if (remainingDepth == 0) {
		//std::cout << "heapsort\n";
		heapSort(first, last, comp);
		return;
	}
	Iterator left = betterPartition(first, last, leftBuffer, rightBuffer, comp);

	introSort2Sub(first, left, leftBuffer, rightBuffer, comp, remainingDepth - 1);
	introSort2Sub(left + 1, last, leftBuffer, rightBuffer, comp, remainingDepth - 1);
}
#endif

