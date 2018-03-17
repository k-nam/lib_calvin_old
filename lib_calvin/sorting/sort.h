#ifndef LIB_CALVIN__SORT__SORT_H
#define LIB_CALVIN__SORT__SORT_H

/*
* 2008-03-08
*/

#include <iterator>
#include "stopwatch.h"
#include "utility.h"
#include <cstdlib>
#include <algorithm>
#include <inttypes.h>

#include "thread.h"
#include "factory.h"

namespace lib_calvin_sort
{

	namespace Library = std;
	using std::vector;
	using std::pair;
	using std::iterator_traits;
	using lib_calvin::Factory;
	using lib_calvin::FactoryLoader;

	int const HEAP_D = 6;
	int const INTROSORT_THRESHOLD = 24;
	int const MERGESORT_THRESHOLD = 24;
	int const L1_CACHE_SIZE = 32000;
	int const L2_CACHE_SIZE = 256000;
	int const CACHE_LINE_SIZE = 64;

	typedef unsigned char Introsort2IndexType;
	size_t const introSortBufferSize = 192;

	// Used for pointer sorting method
	template <typename T, typename Comparator>
	struct SortingPointer {
	public:
		SortingPointer(T const & value, Comparator comp) : pointer_(&value), comp_(comp) { }
		SortingPointer & operator=(SortingPointer const &rhs) { pointer_ = rhs.pointer_; return *this; }
		bool operator< (SortingPointer<T, Comparator> const &rhs) const
		{
			return comp_(*pointer_, *rhs.pointer_);
		}
		T const & operator*() const { return *pointer_; }
	private:
		T const *pointer_;
		Comparator const comp_;
	};

	// Class for storing sub-arrays to sort
	// Abandone, because we use Factory class now
	template <typename Iterator>
	class QuickSortParallelOperationQueue;


	// Argument type of quicksort for multithreaded quicksort
	template <typename Iterator, typename Comparator>
	struct QuickSortThreadArg
	{
		QuickSortThreadArg(Iterator const &first, Iterator const &last, Comparator comp, int thread_limit,
			FactoryLoader<pair<Iterator, Iterator>> *factoryLoader) :
			first_(first), last_(last), comp_(comp), thread_limit_(thread_limit),
			factoryLoader_(factoryLoader) { }
		Iterator const first_;
		Iterator const last_;
		Comparator comp_;
		int const thread_limit_;
		FactoryLoader<pair<Iterator, Iterator>> *factoryLoader_;
	};

	template <typename SrcIterator, typename TargetIterator, typename Comparator>
	struct MergeSortRThreadArg
	{
		MergeSortRThreadArg(SrcIterator const &first, SrcIterator const &last,
			TargetIterator const &target, Comparator comp, int thread_limit) :
			first_(first), last_(last), target_(target), comp_(comp), thread_limit_(thread_limit) { }
		SrcIterator const first_;
		SrcIterator const last_;
		TargetIterator const target_;
		Comparator comp_;
		int const thread_limit_;
	};

	// Thread function for multithreaded quicksort
	// Argument is supposed to be a pointer to QuickSortThreadArg
	template <typename Iterator, typename Comparator>
	void *introSortParallelSub0ThreadFunction(void *lpParam);

	template <typename Iterator, typename Comparator>
	void *introSortParallelSub1ThreadFunction(void *lpParam);

	template <typename SrcIterator, typename TargetIterator, typename Comparator>
	void *mergeSortParallelSub0ThreadFunction(void *lpParam);


	/*** auxiliary functions ***/

	// Binary search with predefined set of elements (gotten from sorting sample set)
	// Returns the index to which the given element is to belong [0, partitioners.size()]
	template <typename Iterator, typename Comparator = std::less<std::iterator_traits<Iterator>::value_type>>
	size_t binSearch(typename std::iterator_traits<Iterator>::value_type elem,
		Iterator firstPartitioner, Iterator lastPartitioner, Comparator comp = Comparator());

	// Select samples from given array. 
	// ratio = # samples / # totalElems, but only roughly (accuracy not guaranteed)
	// result will be a sorted array of different samples
	template <typename Iterator>
	void getSamples(Iterator first, Iterator last, size_t sampleSize,
		vector<typename iterator_traits<Iterator>::value_type> &result);

	template <typename T>
	void removeDuplicateElements(vector<T> const &original, vector<T> &copy);

	// This subroutine assumes that the element right before 'first' exists and
	// ..it is not greater than any element in the input array (it acts as a
	// ..sentinal)
	template <typename Iterator, typename Comparator>
	void unguardedInsertionSort(Iterator first, Iterator last, Comparator comp);

	// a maxheap for sorting
	template <typename Iterator, typename Comparator>
	void percolateDown(Iterator base, Comparator comp, int index, int size);

	// Returns the iterator for the first element of the right subarray
	// Caution: this assumes that input array has at least 3 elements
	template <typename Iterator, typename Comparator>
	Iterator hoarePartition(Iterator first, Iterator last, Comparator comp);

	template <typename Iterator, typename Comparator>
	Iterator betterPartition(Iterator first, Iterator last, 
		Introsort2IndexType *leftBuffer, Introsort2IndexType *rightBuffer, Comparator comp);

	template <typename Iterator, typename Comparator>
	void introSortSub(Iterator first, Iterator last, Comparator comp, int remainingDepth);
	
	// using counting sort for subroutine
	template <typename Iterator, typename Comparator>
	void introSort2Sub(Iterator first, Iterator last,
		Introsort2IndexType *leftBuffer, Introsort2IndexType *rightBuffer,
		Comparator comp, int remainingDepth);

	template <typename Iterator, typename Comparator>
	void introCountingSortSub(Iterator first, Iterator last, Comparator comp, int remainingDepth);

	// Subroutines for introSortParallel. Proceeds partitioning and creating new
	// threads for each sub-arrays until we reach the threshold (typically L2 size)
	// sub-array size, 
	// or the maximum number of threads (this is for ensuring we are not creating too
	// many threads and ruin the system).
	// And then calls another routine for pending remaining task for later stage.

	// Continue creating new threads until reaching limit, and sort the remaining
	// arrays completely.
	template <typename Iterator, typename Comparator>
	void introSortParallelSub0(Iterator first, Iterator last, Comparator comp, int thread_limit);

	// Continue creating new threads until reaching limit OR array size threshold,
	// and pend remaining arrays to the queue
	template <typename Iterator, typename Comparator>
	void introSortParallelSub1(Iterator first, Iterator last, Comparator comp, int thread_limit,
		FactoryLoader<pair<Iterator, Iterator>> &factoryLoader);

	// Singled threaded version of sub1
	template <typename Iterator, typename Comparator>
	void introSortParallelSub2(Iterator first, Iterator last, Comparator comp,
		FactoryLoader<pair<Iterator, Iterator>> &factoryLoader);

	template <typename SrcIterator, typename TargetIterator, typename Comparator>
	void merge(SrcIterator first, SrcIterator middle, SrcIterator last,
		TargetIterator target, Comparator comp);

	template <typename SrcIterator, typename TargetIterator, typename Comparator>
	void merge2(SrcIterator first, SrcIterator middle, SrcIterator last,
		TargetIterator target, Comparator comp);

	template <typename Iterator, typename Comparator>
	void inPlaceMerge(Iterator first, Iterator middle, Iterator last, Comparator comp);

	template <typename Iterator>
	void exchange(Iterator first, Iterator middle, Iterator last);

	template <typename Iterator>
	void reverse(Iterator first, Iterator last);

	template <typename SrcIterator, typename TargetIterator, typename Comparator>
	void mergeSortSub(SrcIterator first, SrcIterator last, TargetIterator target, Comparator comp);

	template <typename SrcIterator, typename TargetIterator, typename Comparator>
	void mergeSortParallelSub0(SrcIterator first, SrcIterator last,
		TargetIterator target, Comparator comp, int thread_limit);

	/*** comparison sorting algorithms [first, last) with operator<  ***/

	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void insertionSort(Iterator first, Iterator last, Comparator const comp = Comparator());

	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void unguardedInsertionSort(Iterator first, Iterator last, Comparator const comp = Comparator());

	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void heapSort(Iterator first, Iterator last, Comparator const comp = Comparator());

	// quicksort with pivot selection with median, hoare partitioning,
	// ...and using heapsort to make worst performance n*log(n)
	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void introSort(Iterator first, Iterator last, Comparator const comp = Comparator());

	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void introSort2(Iterator first, Iterator last, Comparator const comp = Comparator());

	// Basic multithreaded quicksort
	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void introSortParallel(Iterator first, Iterator last, Comparator const comp = Comparator());

	// Using blocking
	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void introSortParallelAdvanced(Iterator first, Iterator last, Comparator const comp = Comparator());

	// Mixing memory intensive and CPU intensive part
	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void introSortParallelAdvanced2(Iterator first, Iterator last, Comparator const comp = Comparator());

	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void mergeSort(Iterator first, Iterator last, Comparator comp = Comparator());

	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void inPlaceMergeSort(Iterator first, Iterator last, Comparator const comp = Comparator());

	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void mergeSortParallel(Iterator first, Iterator last, Comparator const comp = Comparator());

	/*** Linear time sorting algorithms ***/
	// Assumes that int Iterator->get_value() is defined
	template <typename Iterator>
	void countingSort(Iterator first, Iterator last);

	// Combination of introsort and counting sort
	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void introCountingSort(Iterator first, Iterator last, Comparator comp = Comparator());

	// Comparison sort with sampling, binary search, and buckets
	// Not in-place, stable. Cache optimal. 
	template <typename Iterator>
	void bucketSort(Iterator first, Iterator last);

	// Function object version of introsort (for multi-threading)
	template <typename Iterator, typename Comparator>
	class IntroSort {
	public:
		IntroSort(Comparator comp = Comparator()) : comp_(comp) { }
		void operator()(pair<Iterator, Iterator> const &inArray) {
			introSort2(inArray.first, inArray.second, comp_);
		}
	private:
		Comparator comp_;
	};

	// Function object version of countingSort (for multi-threading)
	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	class CountingSort {
	public:
		void operator()(pair<Iterator, Iterator> const &inArray, Comparator comp) {
			countingSort(inArray.first, inArray.second, Comparator());
		}
	};

	// For performance testing
	template <typename Iterator>
	void cacheTest1(Iterator first, Iterator last);
	template <typename Iterator>
	void cacheTest2(Iterator first, Iterator last);
	template <typename Iterator>
	void cacheTest3(Iterator first, Iterator last);
	template <typename Iterator>
	void cacheTest4(Iterator first, Iterator last);

} // end namespace lib_calvin_sort

namespace lib_calvin
{
	using lib_calvin_sort::introSort;
	using lib_calvin_sort::mergeSort;

	template <typename Iterator, typename Comparator =
		std::less<typename std::iterator_traits<Iterator>::value_type>>
		void sort(Iterator first, Iterator last, Comparator comp = Comparator()) {
		introSort(first, last, comp);
	}
}


namespace lib_calvin_sort
{ // open for definitions
	using lib_calvin::stopwatch;

	/****************** Thread functions *****************/

	template <typename Iterator, typename Comparator>
	void *
		introSortParallelSub0ThreadFunction(void *lpParam) {
		QuickSortThreadArg<Iterator, Comparator> *pArg = (QuickSortThreadArg<Iterator, Comparator> *)lpParam;
		introSortParallelSub0(pArg->first_, pArg->last_, pArg->comp_, pArg->thread_limit_);
		return NULL;
	}

	template <typename Iterator, typename Comparator>
	void *
		introSortParallelSub1ThreadFunction(void *lpParam) {
		QuickSortThreadArg<Iterator, Comparator> *pArg = (QuickSortThreadArg<Iterator, Comparator> *)lpParam;
		introSortParallelSub1(pArg->first_, pArg->last_, pArg->comp_, pArg->thread_limit_,
			*pArg->factoryLoader_);
		return NULL;
	}

	template <typename SrcIterator, typename TargetIterator, typename Comparator>
	void *
		mergeSortParallelSub0ThreadFunction(void *lpParam) {
		MergeSortRThreadArg<SrcIterator, TargetIterator, Comparator> *pArg =
			(MergeSortRThreadArg<SrcIterator, TargetIterator, Comparator> *)lpParam;
		mergeSortParallelSub0(pArg->first_, pArg->last_, pArg->target_, pArg->comp_, pArg->thread_limit_);
		return NULL;
	}


} // end namespace lib_calvin_sort for definitions



template <typename Iterator, typename Comparator>
size_t lib_calvin_sort::binSearch(typename std::iterator_traits<Iterator>::value_type elem,
	Iterator first, Iterator last, Comparator comp) {
	//return 1;
	//static int random = 0;
	//return random++ % (partitioners.size() + 1);
	if (comp(*first, elem) == false) {
		return 0;
	}
	//int left = 0, right = partitioners.size() - 1, middle = (left + right) / 2;
	Iterator left = first, right = last, middle = left + (right - left) / 2;
	while (right - left > 1) {
		if (comp(*middle, elem)) {
			left = middle;
		} else {
			right = middle;
		}
		middle = left + (right - left) / 2;
	}
	return right - first;
}

template <typename Iterator>
void lib_calvin_sort::getSamples(Iterator first, Iterator last, size_t sampleSize,
	vector<typename iterator_traits<Iterator>::value_type> &result) {

	typedef typename iterator_traits<Iterator>::value_type elemType;
	size_t const overSamplingRate = 20;
	ptrdiff_t size = last - first;
	size_t numberOfElementsInCacheLine = CACHE_LINE_SIZE / sizeof(elemType);
	if (numberOfElementsInCacheLine == 0) {
		numberOfElementsInCacheLine = 1;
	}
	vector<elemType> tempResult; // may contain duplicate elements
	size_t candidateSize = overSamplingRate * sampleSize;
	for (size_t resultSize = 0; resultSize < candidateSize; ) {
		ptrdiff_t startIndex = rand() % size;
		ptrdiff_t endIndex = startIndex + numberOfElementsInCacheLine;
		if (endIndex > size) {
			endIndex = size;
		}
		for (ptrdiff_t index = startIndex; index < endIndex; index++) {
			tempResult.push_back(*(first + index));
			resultSize++;
		}
	}
	// Sort and remove duplicate elements
	introSort(tempResult.begin(), tempResult.end());
	vector<elemType> tempResult2; // without duplicate elements
	removeDuplicateElements(tempResult, tempResult2);
	result.clear();
	result.reserve(sampleSize);

	for (ptrdiff_t i = overSamplingRate / 2; i < static_cast<ptrdiff_t>(tempResult2.size()); i += overSamplingRate) {
		result.push_back(tempResult2[i]);
	}
	if (result.empty()) {
		result.push_back(*first);
	}
}

template <typename T>
void lib_calvin_sort::removeDuplicateElements(vector<T> const &original,
	vector<T> &copy) {
	copy.clear();
	copy.reserve(original.size());
	copy.push_back(original[0]);
	for (size_t i = 1; i < original.size(); ++i) {
		if (original[i - 1] < original[i]) {
			copy.push_back(original[i]);
		}
	}
}

// maxheap, size: heap size, HEAP_D > 1
template <typename Iterator, typename Comparator>
void lib_calvin_sort::percolateDown(Iterator const base, Comparator comp, int index, int const size) {
	// save original element and insert it back later
	typename iterator_traits<Iterator>::value_type store = *(base + index);
	while (true) {
		Iterator parent = base + index;
		Iterator max = base + index * HEAP_D + 1;
		if (index * HEAP_D + HEAP_D < size) { // all children included, most common case
			Iterator child = max + 1;
			Iterator last = base + index * HEAP_D + HEAP_D + 1;
			for (; child < last; ++child) {
				if (comp(*max, *child))
					max = child;
			}
			// compare and swap if necessary
			if (comp(store, *max)) {
				*parent = *max; // not actually swapping now..
				index = static_cast<int>(max - base);
				continue;
			} else {
				*parent = store;
				return;
			}
		}
		if (index * HEAP_D + 1 >= size) { // no children included
			*parent = store;
			return;
		} else { // some children are included: rare case
			Iterator child = max;
			Iterator last = base + size;
			for (; child != last; ++child) { // iterate HEAP_D-1 times
				if (comp(*max, *child))
					max = child;
			}
			if (comp(store, *max)) {
				*parent = *max;
				*max = store;
				return;
			} else {
				*parent = store;
				return;
			}
		}
	}
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
		Introsort2IndexType *leftBuffer, Introsort2IndexType *rightBuffer, Comparator comp) {
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

	ptrdiff_t leftBatchSize = introSortBufferSize;
	ptrdiff_t rightBatchSize = introSortBufferSize;

	bool isLeftEmpty = true;
	bool isRightEmpty = true;

	bool mustBreak = false;

	while (true) {
		if (mustBreak) {
			break;
		}
		if (right - left + 1 < 2 * introSortBufferSize) {
			mustBreak = true;
			ptrdiff_t numRemainingElements = right - left + 1;
			ptrdiff_t numUnprocessed = numRemainingElements - introSortBufferSize;

			if (isLeftEmpty && isRightEmpty) {
				leftBatchSize = numRemainingElements / 2;
				rightBatchSize = numRemainingElements - leftBatchSize;
			} else if (isLeftEmpty) {
				leftBatchSize = numUnprocessed;
			} else {
				rightBatchSize = numUnprocessed;
			}
		} else {
		}

		if (isLeftEmpty) {
			auto tempIter = left;
			if (mustBreak) {
				for (Introsort2IndexType i = 0; i < leftBatchSize; ) {
					leftBuffer[l_end] = i++; l_end += !comp(*tempIter++, pivot);
				}
			} else {
				for (Introsort2IndexType i = 0; i < leftBatchSize; ) {
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
				for (Introsort2IndexType i = 0; i < rightBatchSize; i) {
					rightBuffer[r_end] = i++; r_end += comp(*tempIter--, pivot);
				}
			} else {
				for (Introsort2IndexType i = 0; i < rightBatchSize; i) {
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
			
			/*
			// 1:1 Swap version
			while (l_temp < l_temp_end) {
				std::iter_swap(left + leftBuffer[l_temp++], right - rightBuffer[r_temp++]);
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
void lib_calvin_sort::introSort2Sub(Iterator first, Iterator last,
	Introsort2IndexType *leftBuffer, Introsort2IndexType *rightBuffer,
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

template <typename Iterator, typename Comparator>
void lib_calvin_sort::introCountingSortSub(Iterator first, Iterator last,
		Comparator comp, int remainingDepth) {
	Iterator left = hoarePartition(first, last, comp);
	// If the current array fits into the cache, following operations on this array
	// will have high cache-hit ratio.
	if (sizeof(*first) * (last - first) < L2_CACHE_SIZE / 4 || remainingDepth == 0) {
		countingSort(first, left);
		countingSort(left, last);
	} else { // If not, go recursively for each sub-array
		introCountingSortSub(first, left, remainingDepth - 1);
		introCountingSortSub(left, last, remainingDepth - 1);
	}
}

// 2009-01-16: verified that shared L2 cache in Q9550 plays a role in parallel
// processing. If the problem size is bigger than L2, and if we use 2-threaded
// quicksort, it helps to use two cores who does not share L2 cache (by avoiding
// conflict miss). On the other hand, it is vice versa if the problem size is
// smaller than L2. The generalization of this problem (i.e., we are using N 
// number of cores for quicksort algorithm, and some layers of cache are shared) 
// is very interesting and I figured out a tentative solution. 
// 2009-01-17: It brings quite a large performance boost if we simply run 
// multiple threads by creating separate threads for partitioned sub-arrays. But
// for maximum cache efficiency, I will try to do something like blocking - 
// when partition proceeds to certain level (typically L2 size), it stops and 
// pends the remaining task for later stage. And lastly we sort each small arrays
// sequentially, without any cache miss for L2. Of course we will use multiple
// threads in that last stage, too.

template <typename Iterator, typename Comparator>
void lib_calvin_sort::introSortParallelSub0(
	Iterator first, Iterator last, Comparator comp, int thread_limit)
{
	if (sizeof(*first)*(last - first) < L2_CACHE_SIZE / 4 || thread_limit <= 0) {
		introSort2(first, last, comp);
		return;
	}
	Iterator left = hoarePartition(first, last, comp);
	QuickSortThreadArg<Iterator, Comparator> argLeft(first, left, comp, thread_limit - 1, NULL);
	QuickSortThreadArg<Iterator, Comparator> argRight(left, last, comp, thread_limit - 1, NULL);
	lib_calvin_util::thread_type leftThread, rightThread;
	leftThread = create_thread(introSortParallelSub0ThreadFunction<Iterator, Comparator>, &argLeft);
	//SetThreadAffinityMask(leftThread, 1 << 0);
	// For unbalancing left and right sub-threads, thereby letting left sub-thread
	// reach bottom, and start CPU-intensive-thread quickly. 
	//Sleep(10); 
	rightThread =
		create_thread(introSortParallelSub0ThreadFunction<Iterator, Comparator>, &argRight);
	//SetThreadAffinityMask(rightThread, 1 << 1);
	lib_calvin_util::wait_for_thread(leftThread);
	//CloseHandle(leftThread);
	lib_calvin_util::wait_for_thread(rightThread);
	//CloseHandle(rightThread);
}

// multi threaded partitioning 
template <typename Iterator, typename Comparator>
void lib_calvin_sort::introSortParallelSub1(
	Iterator first, Iterator last, Comparator comp, int thread_limit,
	FactoryLoader<pair<Iterator, Iterator>> &factoryLoader)
{
	if (sizeof(*first)*(last - first) < L2_CACHE_SIZE || thread_limit <= 0) {
		introSortParallelSub2(first, last, comp, factoryLoader);
		return;
	}
	Iterator left = hoarePartition(first, last, comp);
	QuickSortThreadArg<Iterator, Comparator> argLeft(first, left, comp, thread_limit - 1, &factoryLoader);
	QuickSortThreadArg<Iterator, Comparator> argRight(left, last, comp, thread_limit - 1, &factoryLoader);
	lib_calvin_util::thread_type leftThread, rightThread;
	leftThread =
		create_thread(introSortParallelSub1ThreadFunction<Iterator, Comparator>, &argLeft);
	//SetThreadAffinityMask(leftThread, 1 << 2);
	//Sleep(10);
	rightThread =
		create_thread(introSortParallelSub1ThreadFunction<Iterator, Comparator>, &argRight);
	//SetThreadAffinityMask(leftThread, 1 << 3);
	wait_for_thread(leftThread);
	CloseHandle(leftThread);
	wait_for_thread(rightThread);
	CloseHandle(rightThread);
}

// This subroutine supplements sub1 (single-threaded)
template <typename Iterator, typename Comparator>
void lib_calvin_sort::introSortParallelSub2(Iterator first, Iterator last, Comparator comp,
	FactoryLoader<pair<Iterator, Iterator>> &factoryLoader) {
	if (sizeof(*first)*(last - first) < L2_CACHE_SIZE) {
		//std::cout << "adding to factory loader: sub2\n";
		factoryLoader.add(pair<Iterator, Iterator>(first, last));
	} else {
		Iterator left = hoarePartition(first, last, comp);
		introSortParallelSub2(first, left, comp, factoryLoader);
		introSortParallelSub2(left, last, comp, factoryLoader);
	}
}

// normal two-way merge; input size is not zero (both arrays are not empty)
template <typename SrcIterator, typename TargetIterator, typename Comparator>
void lib_calvin_sort::merge(SrcIterator first, SrcIterator middle, SrcIterator last,
	TargetIterator target, Comparator comp) {
	SrcIterator left = first, right = middle;
	TargetIterator dest = target;

	while (left != middle && right != last) {
		// Using bool to int conversion trick to reduce branch overhead
		// This method incurs twice as much assignment operation, but still faster
		ptrdiff_t result = static_cast<ptrdiff_t>(comp(*right, *left));
		ptrdiff_t opposite = 1 - result;
		*(dest + result) = *left;
		*(dest + opposite) = *right;
		left += opposite;
		right += result;
		dest++;

		/*
		if (comp(*right, *left)) {
		*dest++ = *right++;
		} else {
		*dest++ = *left++;
		}*/
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

template <typename SrcIterator, typename TargetIterator, typename Comparator>
void lib_calvin_sort::mergeSortSub(SrcIterator first, SrcIterator last,
	TargetIterator target, Comparator comp) {
	size_t num = last - first;
	TargetIterator targetLast = target + num;
	if (last - first < MERGESORT_THRESHOLD) {
		insertionSort(target, targetLast, comp);
		return;
	}
	SrcIterator middle = first + num / 2;
	TargetIterator targetMiddle = target + num / 2;
	mergeSortSub(target, targetMiddle, first, comp);
	mergeSortSub(targetMiddle, targetLast, middle, comp);
	lib_calvin_sort::merge(first, middle, last, target, comp);
}

template <typename SrcIterator, typename TargetIterator, typename Comparator>
void lib_calvin_sort::mergeSortParallelSub0(SrcIterator first, SrcIterator last,
	TargetIterator target, Comparator comp, int thread_limit) {
	if (sizeof(*first)*(last - first) < L2_CACHE_SIZE / 2 || thread_limit <= 0) {
		mergeSortSub(first, last, target, comp);
		return;
	}
	size_t num = last - first;
	TargetIterator targetLast = target + num;
	SrcIterator middle = first + num / 2;
	TargetIterator targetMiddle = target + num / 2;
	MergeSortRThreadArg<SrcIterator, TargetIterator, Comparator> argLeft(
		target, targetMiddle, first, comp, thread_limit - 1);
	MergeSortRThreadArg<SrcIterator, TargetIterator, Comparator> argRight(
		targetMiddle, targetLast, middle, comp, thread_limit - 1);
	lib_calvin_util::thread_type leftThread, rightThread;
	leftThread = create_thread(
		mergeSortParallelSub0ThreadFunction<SrcIterator, TargetIterator, Comparator>, &argLeft);
	rightThread = create_thread(
		mergeSortParallelSub0ThreadFunction<SrcIterator, TargetIterator, Comparator>, &argRight);
	lib_calvin_util::wait_for_thread(leftThread);
	lib_calvin_util::wait_for_thread(rightThread);
	lib_calvin_sort::merge(first, middle, last, target, comp);
}

/******************************* Sorting methods ****************************/

// 2009-6-27: Modified loop logic to avoid making (first - 1) iterator. 
template <typename Iterator, typename Comparator>
void lib_calvin_sort::insertionSort(Iterator first, Iterator last, Comparator comp) {
	for (Iterator right = first + 1; right < last; ++right) {
		if (!comp(*right, *(right - 1))) {
			continue;
		} else {
			auto store = std::move(*right);
			Iterator p = right; // candidate place to insert
			do {
				// cannot insert into *p
				*p = std::move(*(p - 1));
				p--;
			}
			while (first < p && comp(store, *(p - 1)));
			*p = std::move(store);
		}
	}
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::unguardedInsertionSort(Iterator first, Iterator last, Comparator const comp) {
	Iterator right; // the element to insert
	Iterator p; // moving iterator
	if (first == last) {
		return;
	}
	// Include first here
	for (right = first; right < last; ++right) {
		if (!comp(*right, *(right - 1))) {
			continue;
		}
		auto store = std::move(*right);
		p = right;
		while (true) {
			if (comp(store, *(p - 1))) { // cannot insert into *p
				*p = std::move(*(p - 1));
				--p;
			} else {
				*p = store;
				break;
			}
		}
	}
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::heapSort(Iterator first, Iterator last, Comparator comp) {
	int size = static_cast<int>(last - first); // size of heap
											   // max-heapify
	for (int i = (size - 2) / HEAP_D; i >= 0; i--) {
		percolateDown(first, comp, i, size);
	}
	// sorting
	while (size > 1) {
		// swap first and last
		std::iter_swap(first, (first + size - 1));
		size--;
		percolateDown(first, comp, 0, size);
	}
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::introSort(Iterator first, Iterator last, Comparator comp) {
	introSortSub(first, last, comp, lib_calvin_util::log(last - first) * 3);
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::introCountingSort(Iterator first, Iterator last, Comparator comp) {
	introCountingSortSub(first, last, comp, lib_calvin_util::log(last - first) * 3);
}

// 2009-01-26: I discovered that if the problem size is bigger than L2, multi-
// threading does not accelerate the partitioning at all, which means that the 
// bottleneck is memory bandwidth (not CPU) for that case. And if the problem lies
// within L2, we get almost maximum speedup. Therefore, The key is to mix memory 
// intensive part and CPU intensive part of the operation, to utilize both resources 
// efficiently (in the same sense as that of multi-programming in OS).
template <typename Iterator, typename Comparator>
void lib_calvin_sort::introSortParallel(Iterator first, Iterator last, Comparator comp) {
	introSortParallelSub0(first, last, comp, 4);
}

// Blocking method
template <typename Iterator, typename Comparator>
void lib_calvin_sort::introSortParallelAdvanced(Iterator first, Iterator last, Comparator comp)
{
	IntroSort<Iterator, Comparator> sorter(comp);
	Factory<pair<Iterator, Iterator>, IntroSort<Iterator, Comparator>> factory(sorter);
	// Create many threads for partitioning large arrays
	introSortParallelSub1(first, last, comp, 4, factory);
	// Create 4 threads for sorting small arrays in L2 cache
	unsigned numCores = 4;
	typedef lib_calvin_util::thread_type thread_type;
	thread_type *handleArray = new thread_type[numCores];
	for (unsigned i = 0; i < numCores; ++i) {
		handleArray[i] = create_thread(lib_calvin::factoryThreadFunction<pair<Iterator, Iterator>,
			IntroSort<Iterator, Comparator>>, &factory);
	}
	factory.close();
	for (unsigned i = 0; i < numCores; ++i) {
		wait_for_thread(handleArray[i]);
		//CloseHandle(handleArray[i]);
	}
	delete[] handleArray;
}

// Mixing memory bandwith and CPU intensive parts of algorithm
template <typename Iterator, typename Comparator>
void lib_calvin_sort::introSortParallelAdvanced2(Iterator first, Iterator last, Comparator comp)
{
	IntroSort<Iterator, Comparator> sorter(comp);
	Factory<pair<Iterator, Iterator>, IntroSort<Iterator, Comparator>> factory(sorter);
	// Create n threads for sorting small arrays in L2 cache
	unsigned numCores = 4;
	typedef lib_calvin_util::thread_type thread_type;
	thread_type *handleArray = new thread_type[numCores];
	for (unsigned i = 0; i < numCores; ++i) {
		// We can change how to sort small sub-array simply by choosing IntroSort or
		// CountingSort below
		handleArray[i] = create_thread(lib_calvin::factoryThreadFunction<pair<Iterator, Iterator>,
			IntroSort<Iterator, Comparator>>, &factory);
		//SetThreadAffinityMask(handleArray[i], 1 << (i % 2));
	}
	// Create n threads for partitioning large arrays
	introSortParallelSub1(first, last, comp, 3, factory);

	factory.close();
	for (unsigned i = 0; i < numCores; ++i) {
		wait_for_thread(handleArray[i]);
		//CloseHandle(handleArray[i]);
	}
	delete[] handleArray;
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::mergeSort(Iterator first, Iterator last, Comparator comp) {
	ptrdiff_t num = last - first;
	// additional array for operation
	typedef typename iterator_traits<Iterator>::pointer pointerType;
	typedef typename iterator_traits<Iterator>::value_type valueType;
	// prepare supplementary array
	pointerType tempArray = (pointerType)operator new (sizeof(valueType) * num);
	auto original = first;
	auto copy = tempArray;
	for (; original != last; ++original, ++copy) {
		new (copy) valueType(*original);
	}
	// mergesort without redundant copying
	mergeSortSub(tempArray, tempArray + num, first, comp);
	// delete supplementary array
	for (ptrdiff_t i = 0; i < num; ++i) {
		tempArray[i].~valueType();
	}
	operator delete(tempArray);
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
void lib_calvin_sort::mergeSortParallel(Iterator first, Iterator last, Comparator comp) {
	size_t num = last - first;
	// additional array for operation
	typedef typename iterator_traits<Iterator>::pointer pointerType;
	typedef typename iterator_traits<Iterator>::value_type valueType;
	pointerType tempArray;
	tempArray = (pointerType) operator new (sizeof(valueType) * num);
	Iterator original = first;
	for (auto copy = tempArray; original != last; ++original, ++copy) {
		new (copy) valueType(*original);
	}
	mergeSortParallelSub0(tempArray, tempArray + num, first, comp, 4);
	for (size_t i = 0; i < num; i++) {
		tempArray[i].~valueType();
	}
	operator delete(tempArray);
}

template <typename Iterator>
void lib_calvin_sort::countingSort(Iterator first, Iterator last) {
	size_t num = last - first;
	if (num < 20) {
		insertionSort(first, last);
		return;
	}
	ptrdiff_t min = first->get_value();
	ptrdiff_t max = min;
	for (Iterator iter = first + 1; iter < last; ++iter) {
		ptrdiff_t curValue = iter->get_value();
		if (curValue < min) {
			min = curValue;
		} else if (curValue > max) {
			max = curValue;
		}
	}
	if (min == max)
		return;
	size_t avgBucketPop = 5;
	size_t numOfBucket = num / avgBucketPop; // at least 2
	size_t bucketSize = (max - min) / numOfBucket; // set bucketSize

												   //std::cout << "max = " << max << "min = " << min << "\n";
												   //std::cout << "num of bucket is " << numOfBucket << "\n";
												   //std::cout << "bucketSize is " << bucketSize << "\n";;
	if (bucketSize == 0) { // almost all elements are equal
		introSort(first, last);
		return;
	}
	// set final numOfBucket
	numOfBucket = (max - min) / bucketSize + 1;
	size_t *countArray; // calculating num of elements for each bucket
	if ((countArray = (size_t *)malloc((numOfBucket + 1) * sizeof(size_t))) == NULL) {
		std::cout << "countingSort: Malloc error.\n";
		exit(0);
	}
	size_t *startArray; // marking starting position of each bucket in the array
	if ((startArray = (size_t *)malloc((numOfBucket + 1) * sizeof(size_t))) == NULL) {
		std::cout << "countingSort: Malloc error.\n";
		exit(0);
	}
	for (size_t i = 0; i <= numOfBucket; ++i)
		countArray[i] = 0;

	// counting elements for each bucket
	for (Iterator iter = first; iter < last; ++iter)
		countArray[(iter->get_value() - min) / bucketSize]++;
	// accumulate counters to startArray
	size_t sum = 0;
	for (size_t i = 0; i <= numOfBucket; ++i) {
		startArray[i] = sum;
		sum += countArray[i];
		countArray[i] = startArray[i]; // copy back
	}
	if (startArray[numOfBucket] != num) {
		std::cout << "error\n";
		exit(0);
	}

	// put elements into right bucket.
	// countArray stores the current position to insert elements
	size_t curIndex = 0; // start with the first bucket and proceed
	while (true) {
		//cout << "piont A\n";
		while (countArray[curIndex] == startArray[curIndex + 1]) { // this bucket is done already
			size_t bucketSize = startArray[curIndex + 1] - startArray[curIndex];
			if (bucketSize > 30) { // if bucket size is greater than 30
								   //quickSort(array + size*(startArray[j]), tempi, size, comp);
				heapSort(first + startArray[curIndex], first + startArray[curIndex + 1]);
			} else {
				if (curIndex == 0) {
					insertionSort(first + startArray[curIndex],
						first + startArray[curIndex + 1]);
				} else {
					unguardedInsertionSort(first + startArray[curIndex],
						first + startArray[curIndex + 1]);
				}
			}
			++curIndex;
			if (curIndex == numOfBucket) // all done
				break;
		}
		if (curIndex == numOfBucket) // all done
			break;
		// starting with the rightmost element of the bucket.
		size_t initialIndex = startArray[curIndex + 1] - 1;
		typename iterator_traits<Iterator>::value_type store;
		store = *(first + initialIndex); // store initail element
		while (true) {
			//cout << "piont B\n";
			// decide which bucket and position to put into
			size_t bucketNum = (store.get_value() - min) / bucketSize;
			size_t targetIndex = countArray[bucketNum];
			++countArray[bucketNum];
			if (targetIndex == initialIndex) { // got around to start position
				*(first + targetIndex) = store;
				break;
			} else {
				std::swap(store, *(first + targetIndex));
			}
		}
	}
	free(countArray);
	free(startArray);
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::introSort2(Iterator first, Iterator last, Comparator comp) {

	Introsort2IndexType leftBuffer[introSortBufferSize];
	Introsort2IndexType rightBuffer[introSortBufferSize];

	introSort2Sub(first, last, leftBuffer, rightBuffer, comp, lib_calvin_util::log(last - first) * 3);
}

template <typename Iterator>
void lib_calvin_sort::bucketSort(Iterator first, Iterator last) {
	typedef typename iterator_traits<Iterator>::value_type elemType;
	typedef typename iterator_traits<Iterator>::pointer pointerType;
	vector<elemType> partitioners;
	int bucketSize = L2_CACHE_SIZE / sizeof(elemType) / 2;
	size_t sampleSize = (last - first) / bucketSize;
	if (sampleSize == 0) {
		sampleSize = 1;
	}

	getSamples(first, last, sampleSize, partitioners);

	size_t numberOfPartitions = partitioners.size() + 1;
	vector<vector<elemType>> queues(numberOfPartitions);
	for (size_t i = 0; i < numberOfPartitions; ++i) {
		queues[i].reserve(bucketSize);
	}
	for (Iterator iter = first; iter < last; iter++) {
		//size_t bucketIndex = binSearch(*iter, &*partitioners.begin(), 
		//&*partitioners.begin() + partitioners.size());
		size_t bucketIndex = binSearch(*iter, partitioners.begin(), partitioners.end());
		queues[bucketIndex].push_back(*iter);
	}
	Iterator target = first;
	for (size_t i = 0; i < numberOfPartitions; ++i) {
		if (queues[i].empty())
			continue;
		mergeSort(queues[i].begin(), queues[i].end());
		for (size_t j = 0; j < queues[i].size(); j++) {
			*target++ = queues[i][j];
		}
	}
}

template <typename Iterator>
void lib_calvin_sort::cacheTest1(Iterator first, Iterator last) {
	// performance of raw array
	typedef typename iterator_traits<Iterator>::value_type elemType;
	int const ways = 1;
	int index = 0;
	elemType *queues[ways], *indices[ways];
	for (int i = 0; i < ways; ++i) {
		queues[i] = new elemType[(last - first) / ways + 1];
		indices[i] = queues[i];
	}
	for (Iterator source = first; source != last; ++source) {
		*indices[index++]++ = *source;
		if (index == ways) {
			index = 0;
		}
	}
	for (int i = 0; i < ways; ++i) {
		delete[] queues[i];
	}
}

template <typename Iterator>
void lib_calvin_sort::cacheTest2(Iterator first, Iterator last) {
	// performance of vector
	typedef typename iterator_traits<Iterator>::value_type elemType;
	int const ways = 32;
	int index = 0;
	using std::vector;
	vector<vector<elemType>> queues(ways);
	for (Iterator source = first; source != last; ++source) {
		queues[index++].push_back(*source);
		if (index == ways) {
			index = 0;
		}
	}
}

template <typename Iterator>
void lib_calvin_sort::cacheTest3(Iterator first, Iterator last) {
	hoarePartition(first, last);
}

#endif

