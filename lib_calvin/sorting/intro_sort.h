#ifndef LIB_CALVIN__SORT__INTRO_SORT_H
#define LIB_CALVIN__SORT__INTRO_SORT_H

#include "insertion_sort.h"
#include "heap_sort.h"
#include "utility.h"
#include "in_place_merge_sort.h"

namespace lib_calvin_sort {

	int const INTROSORT_THRESHOLD = 24;
	typedef unsigned char blockQsortIndexType;
	size_t const blockQsortBufferSize = 192;
	size_t const cacheLineSize = 64;

	// quicksort with pivot selection with median, hoare partitioning,
	// ...and using heapsort to make worst performance n*log(n)
	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void introSort(Iterator first, Iterator last, Comparator const comp = Comparator());

	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void stableBlockIntroSort(Iterator first, Iterator last, Comparator const comp = Comparator());

	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void blockIntroSort(Iterator first, Iterator last, Comparator const comp = Comparator());
	// Returns the iterator for the first element of the right subarray
	// Caution: this assumes that input array has at least 3 elements
	template <typename Iterator, typename Comparator>
	Iterator hoarePartition(Iterator first, Iterator last, Comparator comp);

	template <typename Iterator, typename Comparator>
	inline Iterator betterPartition(Iterator first, Iterator last, Comparator comp);

	template <typename Iterator, typename Comparator>
	void introSortSub(Iterator first, Iterator last, Comparator comp, int remainingDepth);

	// Return true if insertion sort was called
	template <typename Iterator, typename Comparator>
	bool stableBlockIntroSortSub(Iterator first, Iterator last, Iterator target,
		Comparator comp, int remainingDepth, bool firstCalled, bool isSourceReal, bool inReverseOrder);

	// using counting sort for subroutine
	template <typename Iterator, typename Comparator>
	void blockQsortSub(Iterator first, Iterator last, Comparator comp, int remainingDepth);


}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::introSort(Iterator first, Iterator last, Comparator comp) {
	introSortSub(first, last, comp, lib_calvin_util::log(last - first) * 3);
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::stableBlockIntroSort(Iterator first, Iterator last, Comparator comp) {
	using namespace std;
	ptrdiff_t num = last - first;
	typedef typename iterator_traits<Iterator>::pointer pointerType;
	typedef typename iterator_traits<Iterator>::value_type valueType;
	pointerType tempArray = (pointerType)operator new (sizeof(valueType) * num);
	stableBlockIntroSortSub(first, last, tempArray, comp, lib_calvin_util::log(num) * 3,
		true, true, false);

	for (ptrdiff_t i = 0; i < num; ++i) {
		tempArray[i].~valueType();
	}
	operator delete(tempArray);
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::blockIntroSort(Iterator first, Iterator last, Comparator comp) {
	blockQsortSub(first, last, comp, lib_calvin_util::log(last - first) * 3);
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
bool lib_calvin_sort::stableBlockIntroSortSub(Iterator first, Iterator last, Iterator target, Comparator comp,
	int remainingDepth, bool firstCalled, bool isSourceReal, bool inReverseOrder) {

	ptrdiff_t num = last - first;

	if ((last - first < INTROSORT_THRESHOLD || remainingDepth <= 0) && isSourceReal) {
		if (inReverseOrder) {
			reverse(first, last);
		} else {
		}
		mergeSort(first, last, comp);
		return true;
	}

	if (first + 1 >= last) {
		return false;
	}

	Iterator left = first;
	Iterator right = last - 1;
	Iterator middle = first + (last - first) / 2;
	auto pivot = first;

	if (comp(*left, *middle)) {
		if (comp(*middle, *right)) { // left, middle, right
			pivot = middle;
		} else {
			if (comp(*left, *right)) { // left, right, middle
				pivot = right;
			} else { // right, left, middle
				pivot = left;
			}
		}
	} else {
		if (comp(*left, *right)) { // middle, left, right
			pivot = left;
		} else {
			if (comp(*middle, *right)) { // middle, right, left
				pivot = right;
			} else { // right, middle, left
				pivot = middle;
			}
		}
	}

	typedef typename std::iterator_traits<Iterator>::pointer pointerType;
	typedef typename std::iterator_traits<Iterator>::value_type valueType;

	// Do stable partition
	Iterator targetBegin = target;
	Iterator targetLast = target + (last - first) - 1;


	size_t const bufferSize = 100;
	char leftBuffer[bufferSize];
	char rightBuffer[bufferSize];
	char leftBufferIndex;
	char rightBufferIndex;

	Iterator loopFirst = first;
	Iterator loopLast = last;

	if (!inReverseOrder) {
		while (loopFirst != loopLast) {
			leftBufferIndex = 0;
			rightBufferIndex = 0;

			ptrdiff_t loopSize = 0;
			if (loopLast - loopFirst >= bufferSize) {
				loopSize = bufferSize;
			} else {
				loopSize = loopLast - loopFirst;
			}
			Iterator temp = loopFirst;

			for (auto i = 0; i < loopSize; i++) {
				leftBuffer[leftBufferIndex] = i;
				rightBuffer[rightBufferIndex] = i;
				leftBufferIndex += comp(*temp, *pivot);
				rightBufferIndex += !comp(*temp, *pivot);
				temp++;
			}


			if (firstCalled) {
				for (auto i = 0; i < leftBufferIndex; i++) {
					new (&(*targetBegin++)) valueType(std::move(*(loopFirst + leftBuffer[i])));
					//targetBegin++;
				}
				for (auto i = 0; i < rightBufferIndex; i++) {
					new (&(*targetLast--)) valueType(std::move(*(loopFirst + rightBuffer[i])));
					//*targetLast--;
				}
			} else {
				for (auto i = 0; i < leftBufferIndex; i++) {
					*targetBegin++ = std::move(*(loopFirst + leftBuffer[i]));
					//targetBegin++;
				}
				for (auto i = 0; i < rightBufferIndex; i++) {
					*targetLast-- = std::move(*(loopFirst + rightBuffer[i]));
					//targetLast--;
				}
			}

			loopFirst += loopSize;
		}
	} else {
		while (loopFirst != loopLast) {
			leftBufferIndex = 0;
			rightBufferIndex = 0;

			ptrdiff_t loopSize = 0;
			if (loopLast - loopFirst >= bufferSize) {
				loopSize = bufferSize;
			} else {
				loopSize = loopLast - loopFirst;
			}
			Iterator temp = loopLast - 1;

			for (auto i = 0; i < loopSize; i++) {
				leftBuffer[leftBufferIndex] = i;
				rightBuffer[rightBufferIndex] = i;
				leftBufferIndex += comp(*temp, *pivot);
				rightBufferIndex += !comp(*temp, *pivot);
				temp--;
			}


			if (firstCalled) {
				for (auto i = 0; i < leftBufferIndex; i++) {
					new (&(*targetBegin++)) valueType(std::move(*(loopLast - 1 - leftBuffer[i])));
					//targetBegin++;
				}
				for (auto i = 0; i < rightBufferIndex; i++) {
					new (&(*targetLast--)) valueType(std::move(*(loopLast - 1 - rightBuffer[i])));
					//targetLast--;
				}
			} else {
				for (auto i = 0; i < leftBufferIndex; i++) {
					*targetBegin++ = std::move(*(loopLast - 1 - leftBuffer[i]));
					//targetBegin++;
				}
				for (auto i = 0; i < rightBufferIndex; i++) {
					*targetLast-- = std::move(*(loopLast - 1 - rightBuffer[i]));
					//targetLast--;
				}
			}

			loopLast -= loopSize;
		}
	}

	size_t leftPartitionSize = targetBegin - target;
	stableBlockIntroSortSub(target, targetBegin, first, comp, remainingDepth - 1,
		false, !isSourceReal, false);
	stableBlockIntroSortSub(targetBegin, target + (last - first), first + leftPartitionSize,
		comp, remainingDepth - 1,
		false, !isSourceReal, true);
	return false;
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
	while (right > first && comp(pivot, *right)) {
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
		while (comp(pivot, *right)) {
			--right;
		}
	}

	std::iter_swap(first, left - 1);
	return left - 1;
}

template <typename Iterator, typename Comparator>
Iterator lib_calvin_sort::betterPartition(Iterator begin, Iterator end, Comparator comp) {
	
	unsigned char leftBufferRaw[blockQsortBufferSize + cacheLineSize];
	unsigned char rightBufferRaw[blockQsortBufferSize + cacheLineSize];
	
	unsigned char *leftBuffer = leftBufferRaw - reinterpret_cast<size_t>(leftBufferRaw) % cacheLineSize
		+ cacheLineSize;
	unsigned char *rightBuffer = rightBufferRaw - reinterpret_cast<size_t>(rightBufferRaw) % cacheLineSize
		+ cacheLineSize;


	typedef typename std::iterator_traits<Iterator>::value_type T;
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

	T const &pivot = (*left);
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
	// Defense for all equal input
	bool previousWasLeft = false;

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
			previousWasLeft = true;
			Iterator tempIter = left;
			if (mustBreak) {
				for (blockQsortIndexType i = 0; i < leftBatchSize; ) {
					leftBuffer[l_end] = i++; l_end += (comp(pivot, *tempIter++));
				}
			} else {
				for (blockQsortIndexType i = 0; i < leftBatchSize; ) {
					leftBuffer[l_end] = i++; l_end += (comp(pivot, *tempIter++));
					leftBuffer[l_end] = i++; l_end += (comp(pivot, *tempIter++));
					leftBuffer[l_end] = i++; l_end += (comp(pivot, *tempIter++));
					leftBuffer[l_end] = i++; l_end += (comp(pivot, *tempIter++));
				}
			}
		}
		if (isRightEmpty) {
			previousWasLeft = false;
			auto tempIter = right;
			if (mustBreak) {
				for (blockQsortIndexType i = 0; i < rightBatchSize; i) {
					rightBuffer[r_end] = i++; r_end += comp(*tempIter--, pivot);
				}
			} else {
				for (blockQsortIndexType i = 0; i < rightBatchSize; i) {
					rightBuffer[r_end] = i++; r_end += (comp(*tempIter--, pivot));
					rightBuffer[r_end] = i++; r_end += (comp(*tempIter--, pivot));
					rightBuffer[r_end] = i++; r_end += (comp(*tempIter--, pivot));
					rightBuffer[r_end] = i++; r_end += (comp(*tempIter--, pivot));
				}
			}
		}

		// Do swap now
		ptrdiff_t swapCount = std::min(l_end - l_begin, r_end - r_begin);
		if (swapCount != 0) {

			// Zig-zag swapping			
			Iterator temp_left = left + leftBuffer[l_begin];
			Iterator temp_right = right - rightBuffer[r_begin];

			T store = std::move(*(left + leftBuffer[l_begin]));
			*temp_left = std::move(*temp_right);

			ptrdiff_t i = 1;
			for (; i < swapCount; i++) {
				temp_left = left + leftBuffer[l_begin + i];
				*temp_right = std::move(*temp_left);

				temp_right = right - rightBuffer[r_begin + i];
				*temp_left = std::move(*temp_right);
			}
			*temp_right = std::move(store);


			/*
			for (ptrdiff_t i = 0; i < swapCount; i++) {
				std::iter_swap((left + leftBuffer[l_begin + i]), (right - rightBuffer[r_begin + i]));
			}
			*/

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

	Iterator pivotPosition = left - 1;
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

	//*begin = std::move(*pivotPosition);
	//*pivotPosition = std::move(pivot);
	std::iter_swap(begin, pivotPosition);
	return pivotPosition;
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::blockQsortSub(Iterator first, Iterator last, Comparator comp, int remainingDepth) {
	if (last - first < INTROSORT_THRESHOLD) {
		insertionSort(first, last, comp);
		return;
	}
	if (remainingDepth == 0) {
		//std::cout << "heapsort\n";
		heapSort(first, last, comp);
		return;
	}
	Iterator left = betterPartition(first, last, comp);

	blockQsortSub(first, left, comp, remainingDepth - 1);
	blockQsortSub(left + 1, last, comp, remainingDepth - 1);
}

#endif

