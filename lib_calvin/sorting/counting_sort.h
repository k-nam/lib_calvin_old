#ifndef LIB_CALVIN__SORT__COUNTING_SORT_H
#define LIB_CALVIN__SORT__COUNTING_SORT_H

#include "intro_sort.h"

namespace lib_calvin_sort
{
	/*** Linear time sorting algorithms ***/
	// Assumes that int Iterator->get_value() is defined
	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void countingSort(Iterator first, Iterator last, Comparator const comp = Comparator());

	// Combination of introsort and counting sort
	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void introCountingSort(Iterator first, Iterator last, Comparator comp = Comparator());

	template <typename Iterator, typename Comparator>
	void introCountingSortSub(Iterator first, Iterator last, Comparator comp, int remainingDepth);


}



template <typename Iterator, typename Comparator>
void lib_calvin_sort::introCountingSort(Iterator first, Iterator last, Comparator comp) {
	introCountingSortSub(first, last, comp, lib_calvin_util::log(last - first) * 3);
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::introCountingSortSub(Iterator first, Iterator last,
										   Comparator comp, int remainingDepth) {
	blockQsortIndexType leftBuffer[blockQsortBufferSize];
	blockQsortIndexType rightBuffer[blockQsortBufferSize];
	Iterator left = betterPartition(first, last, leftBuffer, rightBuffer, comp);
	// If the current array fits into the cache, following operations on this array
	// will have high cache-hit ratio.
	if (sizeof(*first) * (last - first) < L2_CACHE_SIZE / 4 || remainingDepth == 0) {
		countingSort(first, left);
		countingSort(left, last);
	} else { // If not, go recursively for each sub-array
		introCountingSortSub(first, left, comp, remainingDepth - 1);
		introCountingSortSub(left, last, comp, remainingDepth - 1);
	}
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::countingSort(Iterator first, Iterator last, Comparator comp) {
	size_t num = last - first;
	if (num < 20) {
		insertionSort(first, last);
		return;
	}
	ptrdiff_t min = *first;
	ptrdiff_t max = min;
	for (Iterator iter = first + 1; iter < last; ++iter) {
		ptrdiff_t curValue = *iter;
		if (curValue < min) {
			min = curValue;
		} else if (curValue > max) {
			max = curValue;
		}
	}
	if (min == max)
		return;
	size_t avgBucketPop = 5;
	// at least 2
	size_t numOfBucket = num / avgBucketPop;
	// set bucketSize									 
	size_t bucketSize = (max - min) / numOfBucket;

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
		countArray[(*iter - min) / bucketSize]++;
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
			size_t bucketNum = (store - min) / bucketSize;
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

#endif