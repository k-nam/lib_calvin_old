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

#include "vector.h"
#include "common_thread.h"

namespace lib_calvin_sort {

namespace Library = std;
using lib_calvin::vector;
using std::pair;
using std::iterator_traits;
using namespace lib_calvin_thread;

int const heapD = 6;
int const introSort_thre = 18;
int const mergeSort_thre = 18;
int const L1_CACHE_SIZE = 32000;
int const L2_CACHE_SIZE = 256000;
int const CACHE_LINE_SIZE = 64;

// Used for pointer sorting method
template <typename T>
struct SortingPointer {
public:
	SortingPointer(T const & value): pointer_(&value) { }
	bool operator< (SortingPointer<T> const &rhs) const 
		{ return *pointer_ < *rhs.pointer_; }
	T const & operator*() const { return *pointer_; }
private:
	T const *pointer_;
};


template <typename Iterator, template <typename T> class sorter>
void pointerSorting(Iterator first, Iterator last);

template <typename Iterator>
void introSortPointerSorting(Iterator first, Iterator last) {
	pointerSorting<Iterator, IntroSort> (first, last);
}

// Test sorting algorithms with given object type T
// T should have int first and int second
// T should have operator< defined by order of first
/*
template <typename T>
class Sorter {
  public:
    Sorter();

    // num: # of elements to sort
    void sortTest (void (*sortingAlg) (T *first, T *last), std::string);
    
  private:
};*/

// Class for storing sub-arrays to sort
// Abandone, because we use Factory class now
template <typename Iterator>
class QuickSortParallelOperationQueue;

// Interface to factory only for adding work (cannot call work())
template <typename Argument>
class FactoryLoader 
{
public:
	FactoryLoader(): lock_(create_lock()), workQueue_(), 
		isClosed_(false) { }
	virtual ~FactoryLoader() { }
	void add(Argument const &); // add additional work to do
	void close() { isClosed_ = true; } // close the factory
protected:	
	lock_type lock_; 	
	vector<Argument> workQueue_;
	bool isClosed_;
private:		
};

// Wrapping class for factory pattern with multithreading support
// Simply calls Operation(Argument) for arguments in the given vector
template <typename Argument, typename Operation>
class Factory: public FactoryLoader<Argument> 
{
public:
	Factory(Operation const &op): FactoryLoader(), index_(0), op_(op) { }
	// work() does not return until all work is done AND the factory is closed
	void work(); // includes critical section	
private:	
	unsigned index_; // indicates current position
	Operation op_; // function object
};

// Argument type of quicksort for multithreaded quicksort
template <typename Iterator>
struct QuickSortThreadArg 
{
	QuickSortThreadArg(Iterator const &first, Iterator const &last, int thread_limit,
		FactoryLoader<pair<Iterator, Iterator>> *factoryLoader):
	first_(first), last_(last), thread_limit_(thread_limit),
	factoryLoader_(factoryLoader) { }
	Iterator const first_;
	Iterator const last_;
	int const thread_limit_;
	FactoryLoader<pair<Iterator, Iterator>> *factoryLoader_;
};

template <typename SrcIterator, typename TargetIterator>
struct MergeSortRThreadArg
{
	MergeSortRThreadArg(SrcIterator const &first, SrcIterator const &last, 
		TargetIterator const &target, int thread_limit):
	first_(first), last_(last), target_(target), thread_limit_(thread_limit) { }
	SrcIterator const first_;
	SrcIterator const last_;
	TargetIterator const target_;
	int const thread_limit_;
};

// Thread function for multithreaded quicksort
// Argument is supposed to be a pointer to QuickSortThreadArg
template <typename Iterator>
void *introSortParallelSub0ThreadFunction(void *lpParam);

template <typename Iterator>
void *introSortParallelSub1ThreadFunction(void *lpParam);

template <typename SrcIterator, typename TargetIterator>
void *mergeSortParallelSub0ThreadFunction(void *lpParam);

// Thread function for factory pattern
// lpParam should be a pointer to Factory<Argument, Operation>
template <typename Argument, typename Operation>
void *factoryThreadFunction(void *lpParam);

/*** auxiliary functions ***/

// Binary search with predefined set of elements (gotten from sorting sample set)
// Returns the index to which the given element is to belong [0, partitioners.size()]
template <typename elemType, typename Iterator>
size_t binSearch(elemType elem, Iterator firstPartitioner, Iterator lastPartitioner);

// Select samples from given array. 
// ratio = # samples / # totalElems, but only roughly (accuracy not guaranteed)
// result will be a sorted array of different samples
template <typename Iterator>
void getSamples(Iterator first, Iterator last, size_t sampleSize, 
								vector<typename iterator_traits<Iterator>::value_type> &result);

template <typename T>
void removeDuplicateElements(vector<T> const &original, vector<T> &copy);

template <typename Iterator> 
void insertionSort_reversed(Iterator first, Iterator last);

// This subroutine assumes that the element right before 'first' exists and
// ..it is not greater than any element in the input array (it acts as a
// ..sentinal)
template <typename Iterator> 
void unguardedInsertionSort (Iterator first, Iterator last);

// a maxheap for sorting
template <typename Iterator>
void percolateDown (Iterator base, int index, int size);

// Returns the iterator for the first element of the right subarray
// Caution: this assumes that input array has at least 3 elements
template <typename Iterator>
Iterator hoarePartition (Iterator first, Iterator last);

template <typename Iterator>
void introSortSub (Iterator first, Iterator last, int remainingDepth);

// using counting sort for subroutine
template <typename Iterator>
void introSort2Sub (Iterator first, Iterator last, int remainingDepth);

// Subroutines for introSortParallel. Proceeds partitioning and creating new
// threads for each sub-arrays until we reach the threshold (typically L2 size)
// sub-array size, 
// or the maximum number of threads (this is for ensuring we are not creating too
// many threads and ruin the system).
// And then calls another routine for pending remaining task for later stage.

// Continue creating new threads until reaching limit, and sort the remaining
// arrays completely.
template <typename Iterator>
void introSortParallelSub0 (Iterator first, Iterator last, int thread_limit);

// Continue creating new threads until reaching limit OR array size threshold,
// and pend remaining arrays to the queue
template <typename Iterator>
void introSortParallelSub1 (
	Iterator first, Iterator last, int thread_limit, 
	FactoryLoader<pair<Iterator, Iterator>> &factoryLoader);

// Singled threaded version of sub1
template <typename Iterator>
void introSortParallelSub2 (Iterator first, Iterator last,
															FactoryLoader<pair<Iterator, Iterator>> &factoryLoader);

template <typename SrcIterator, typename TargetIterator>
void merge(SrcIterator first, SrcIterator middle, SrcIterator last, 
						TargetIterator target);

template <typename Iterator>
void inPlaceMerge(Iterator first, Iterator middle, Iterator last);

template <typename Iterator>
void exchange(Iterator first, Iterator middle, Iterator last);

template <typename Iterator>
void reverse(Iterator first, Iterator last);

template <typename SrcIterator, typename TargetIterator>
void mergeSort(SrcIterator first, SrcIterator last, TargetIterator target);

template <typename SrcIterator, typename TargetIterator>
void mergeSortParallelSub0(SrcIterator first, SrcIterator last,
	TargetIterator target, int thread_limit);

template <typename Iterator>
void merge2(Iterator first, Iterator middle, Iterator last, 
    Iterator target);

template <typename Iterator>
void merge2Reversed(Iterator first, Iterator middle, Iterator last, 
    Iterator target);

template <typename Iterator>
void mergeSort2Ascending(Iterator first, Iterator last, Iterator target);

template <typename Iterator>
void mergeSort2Descending(Iterator first, Iterator last, Iterator target);


/*** comparison sorting algorithms [first, last) with operator<  ***/

template <typename Iterator> 
void insertionSort(Iterator first, Iterator last);

template <typename Iterator>
void heapSort(Iterator first, Iterator last);

// quicksort with pivot selection with median, hoare partitioning,
// ...and using heapsort to make worst performance n*log(n)
template <typename Iterator> 
void introSort(Iterator first, Iterator last);

template <typename T>
void introSort(vector<T> &elements);

// Function object version of introsort (for multi-threading)
template <typename Iterator>
class IntroSort 
{
public:
	void operator()(pair<Iterator, Iterator> const &inArray) {
		introSort(inArray.first, inArray.second);
	}
};

// Function object version of countingSort (for multi-threading)
template <typename Iterator>
class CountingSort 
{
public:
	void operator()(pair<Iterator, Iterator> const &inArray) {
		countingSort(inArray.first, inArray.second);
	}
};

// Basic multithreaded quicksort
template <typename Iterator>
void introSortParallel(Iterator first, Iterator last);

// Using blocking
template <typename Iterator>
void introSortParallelAdvanced(Iterator first, Iterator last);

// Mixing memory intensive and CPU intensive part
template <typename Iterator>
void introSortParallelAdvanced2(Iterator first, Iterator last);

template <typename Iterator>
void mergeSort(Iterator first, Iterator last);

template <typename Iterator>
void inPlaceMergeSort(Iterator first, Iterator last);

template <typename Iterator>
void mergeSortParallel(Iterator first, Iterator last);

template <typename Iterator>
void mergeSort2(Iterator first, Iterator last);

/*** Linear time sorting algorithms ***/

// Assumes that int Iterator->get_value() is defined
template <typename Iterator>
void countingSort(Iterator first, Iterator last);

// Combination of introsort and counting sort
template <typename Iterator>
void introSort2(Iterator first, Iterator last);

// Comparison sort with sampling, binary search, and buckets
// Not in-place, stable. Cache optimal. 
template <typename Iterator>
void bucketSort(Iterator first, Iterator last);

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


namespace lib_calvin_sort { // open for definitions


using lib_calvin::stopwatch;

template <typename Argument>
void FactoryLoader<Argument>::add(Argument const &newWork)
{
	acquire_lock(lock_);
	if (isClosed_) { // can not add work if closed
		release_lock(lock_);
		return;
	} else {
		workQueue_.push_back(newWork);
		release_lock(lock_);
	}
}

template <typename Argument, typename Operation>
void Factory<Argument, Operation>::work() 
{
	// data mambers should be accessed only in the critical section
	while (true) {
		acquire_lock(lock_);
		if (index_ >= workQueue_.size()) { // all work done
			if (isClosed_) { // return only when the factory is closed
				release_lock(lock_);
				return;
			} else {		
				release_lock(lock_);
				thread_yield(); // don't consume CPU time
				//Sleep(1);
			}
		} else {
			unsigned index = index_;
			++index_;
			release_lock(lock_);
			op_(workQueue_[index]);
		}
	}
}



/****************** Thread functions *****************/

template <typename Iterator>
void *
introSortParallelSub0ThreadFunction(void *lpParam) {
	QuickSortThreadArg<Iterator> *pArg = (QuickSortThreadArg<Iterator> *)lpParam;
	introSortParallelSub0(pArg->first_, pArg->last_, pArg->thread_limit_);      
	return NULL;
}

template <typename Iterator>
void *
introSortParallelSub1ThreadFunction(void *lpParam) {
	QuickSortThreadArg<Iterator> *pArg = (QuickSortThreadArg<Iterator> *)lpParam;
	introSortParallelSub1(pArg->first_, pArg->last_, pArg->thread_limit_,
		*pArg->factoryLoader_);      
	return NULL;
}

template <typename SrcIterator, typename TargetIterator>
void *
mergeSortParallelSub0ThreadFunction(void *lpParam) {
	MergeSortRThreadArg<SrcIterator, TargetIterator> *pArg = 
		(MergeSortRThreadArg<SrcIterator, TargetIterator> *)lpParam;
	mergeSortParallelSub0(pArg->first_, pArg->last_, 
		pArg->target_, pArg->thread_limit_);      
	return NULL;
}

template <typename Argument, typename Operation>
void * 
factoryThreadFunction(void *param)
{
	Factory<Argument, Operation> *factory = (Factory<Argument, Operation> *)param;
	factory->work();
	return NULL;
}

} // end namespace lib_calvin_sort for definitions

/************************* Sorting subroutines ************************/

// Pointer sorting algorithm; useful for sorting heavy objects
// We need not templatize on underlying sorting algorithm, as the algorithm itself
// ..is called only once (as opposed to the case of comparison function)
// But to avoid needless codes for specifying the type of pointer sorting algorithm,
///..I came to use template template parameter. 
template <typename Iterator, template <typename T> class sorter>
void lib_calvin_sort::pointerSorting(Iterator first, Iterator last) 
{
	typedef typename iterator_traits<Iterator>::value_type valueType;
	typedef SortingPointer<valueType> pointerType;
	size_t numElem = last - first;
	pointerType *pointerArray = 
		(pointerType *) operator new (sizeof(pointerType) * numElem);
	for (size_t i = 0; i < numElem; ++i) {
		new (pointerArray + i) pointerType(*(first + i));
	}
	sorter<pointerType *>()(std::make_pair(pointerArray, pointerArray + numElem));
	
	valueType *copyArray = (valueType *) operator new (sizeof(valueType) * numElem);
	for (size_t i = 0; i < numElem; ++i) {
		new (copyArray + i) valueType(*pointerArray[i]);
	}
	for (size_t i = 0; i < numElem; ++i) {
		*(first + i) = copyArray[i];
	}
	operator delete(pointerArray);
	operator delete(copyArray);
}

template <typename elemType, typename Iterator>
size_t lib_calvin_sort::binSearch(elemType elem, Iterator first, Iterator last) {
	//return 1;
	//static int random = 0;
	//return random++ % (partitioners.size() + 1);
	if (*first < elem == false) {
		return 0;
	} 	
	//int left = 0, right = partitioners.size() - 1, middle = (left + right) / 2;
	Iterator left = first, right = last, middle = left + (right - left) / 2;
	while (right - left > 1) {
		if (*middle < elem) {
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

template <typename Iterator>
void lib_calvin_sort::insertionSort_reversed(Iterator first, Iterator last) {
  Iterator right; // the element to insert
  Iterator p; // moving iterator
  typename iterator_traits<Iterator>::value_type store;

  for (right = first + 1; right < last; ++right) {
    store = *right;
    for (p = right - 1; p >= first; --p) {
      if (*p < store) {
        *(p + 1) = *p;
      } else {
        break;
      }
    }
    *(p + 1) = store;
  }
}

template <typename Iterator>
void lib_calvin_sort::unguardedInsertionSort(Iterator first, Iterator last) {
	typedef iterator_traits<Iterator>::value_type valueType;
  Iterator right; // the element to insert
  Iterator p; // moving iterator
  // Caution: we should start from the 'first'
  for (right = first; right < last; ++right) {
    typename iterator_traits<Iterator>::value_type store = *right;
    for (p = right - 1; /* p >= first */; --p) {
      if (store < *p) {
        *(p + 1) = (valueType)*p;
      } else {
        break;
      }
    }
    *(p + 1) = store;
  }
}

// maxheap, size: heap size, heapD > 1
template <typename Iterator>
void lib_calvin_sort::percolateDown(Iterator const base, int index, int const size) {
  // save original element and insert it back later
  typename iterator_traits<Iterator>::value_type store = *(base + index);
  while (true) {
    Iterator parent = base + index;
    Iterator max  = base + index * heapD + 1;
    if (index * heapD + heapD < size) { // all children included, most common case
      Iterator child  = max + 1;
      Iterator last   = base + index * heapD + heapD + 1;
      for ( ; child < last; ++child ) { 
        if (*max < *child)
          max = child;
      }
      // compare and swap if necessary
      if (store < *max) {
        *parent = *max; // not actually swapping now..
        index = static_cast<int>(max - base);
        continue;
      } else {
        *parent = store;
        return;
      }
    } 
    if (index * heapD + 1 >= size) { // no children included
      *parent = store;
      return;
    } else { // some children are included: rare case
      Iterator child  = max;
      Iterator last   = base + size;
      for ( ; child != last; ++child) { // iterate heapD-1 times
        if (*max < *child)
          max = child;
      }
      if (store < *max) {
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

template <typename Iterator>
Iterator lib_calvin_sort::hoarePartition(Iterator first, Iterator last) {
	// for analysis
	// int size = last - first;
	// int swapCount = 0; 
	typedef iterator_traits<Iterator>::value_type valueType;
  Iterator left, right, middle;
  using lib_calvin_util::swap;
	left = first;
  right = (last - 1);
  middle = first + (last - first) / 2;
	if (*middle < *left) {		
		valueType temp = *middle;
		*middle = (valueType)*left;
		*left = temp;
	}
  if (*right < *middle) {
    valueType temp = *middle;
		*middle = (valueType)*right;
		*right = temp;
	}
  if (*middle < *left) {
    valueType temp = *middle;
		*middle = (valueType)*left;
		*left = temp;
	}
  typename iterator_traits<Iterator>::value_type pivot = *middle;
  ++left;
  --right;
  while (true) {
		while (*left < pivot) {
      ++left;
		}
    while (pivot < *right) {
      --right;
		}
    if (!(left < right)) {
      break;
    } else {
      valueType temp = *right;
			*right = *left;
			*left = temp;
      ++left;
      --right;
			// swapCount++;
    }
  }
	// if (size >= 1000000) {
	//	 std::cout << "hoarePartition ratio = " << (double)swapCount/size <<
	//		 "\n";
	// }
	return left;
}

template <typename Iterator>
void lib_calvin_sort::introSortSub(Iterator first, Iterator last,
    int remainingDepth) {
  if (last - first < introSort_thre) {
    return;
  }
  if (remainingDepth == 0) {
    heapSort(first, last);
    return;
  }
	Iterator left = hoarePartition(first, last);
  introSortSub(first, left, remainingDepth - 1);
  introSortSub(left, last, remainingDepth - 1);
}

// Subroutine for introSort2
template <typename Iterator>
void lib_calvin_sort::introSort2Sub(Iterator first, Iterator last,
																	 int remainingDepth) 
{
	Iterator left = hoarePartition(first, last);	
	// If the current array fits into the cache, following operations on this array
	// will have high cache-hit ratio.
  if (sizeof(*first) * (last - first) < L2_CACHE_SIZE / 4 || remainingDepth == 0) {
    countingSort(first, left);
		countingSort(left, last);
	} else { // If not, go recursively for each sub-array
	  introSort2Sub(first, left, remainingDepth - 1);
		introSort2Sub(left, last, remainingDepth - 1);
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

template <typename Iterator>
void lib_calvin_sort::introSortParallelSub0(
	Iterator first, Iterator last, int thread_limit)
{
	if (sizeof(*first)*(last - first) < L2_CACHE_SIZE / 4 || thread_limit <= 0) {
		introSort(first, last);
		//introSort2(first, last);
		return;
	}
	Iterator left = hoarePartition(first, last);
	QuickSortThreadArg<Iterator> argLeft(first, left, thread_limit - 1, NULL);
	QuickSortThreadArg<Iterator> argRight(left, last, thread_limit - 1, NULL);
	lib_calvin_thread::thread_type leftThread, rightThread;
	leftThread = 
		create_thread(introSortParallelSub0ThreadFunction<Iterator>, &argLeft);
	//SetThreadAffinityMask(leftThread, 1 << 0);
	// For unbalancing left and right sub-threads, thereby letting left sub-thread
	// reach bottom, and start CPU-intensive-thread quickly. 
	//Sleep(10); 
	rightThread = 
		create_thread(introSortParallelSub0ThreadFunction<Iterator>, &argRight);
	//SetThreadAffinityMask(rightThread, 1 << 1);
	lib_calvin_thread::wait_for_thread(leftThread);
	//CloseHandle(leftThread);
	lib_calvin_thread::wait_for_thread(rightThread);		
	//CloseHandle(rightThread);
}

// multi threaded partitioning 
template <typename Iterator>
void lib_calvin_sort::introSortParallelSub1(
	Iterator first, Iterator last, int thread_limit,
	FactoryLoader<pair<Iterator, Iterator>> &factoryLoader)
{
	if (sizeof(*first)*(last - first) < L2_CACHE_SIZE || thread_limit <= 0) {
    introSortParallelSub2(first, last, factoryLoader);
    return;
  }	
	Iterator left = hoarePartition(first, last);
	QuickSortThreadArg<Iterator> argLeft(first, left, thread_limit - 1, &factoryLoader);
	QuickSortThreadArg<Iterator> argRight(left, last, thread_limit - 1, &factoryLoader);
	thread_type leftThread, rightThread;
	leftThread = 
		create_thread(introSortParallelSub1ThreadFunction<Iterator>, &argLeft);
	//SetThreadAffinityMask(leftThread, 1 << 2);
	//Sleep(10);
	rightThread = 
		create_thread(introSortParallelSub1ThreadFunction<Iterator>, &argRight);
	//SetThreadAffinityMask(leftThread, 1 << 3);
	wait_for_thread(leftThread);
	CloseHandle(leftThread);
	wait_for_thread(rightThread);	
	CloseHandle(rightThread);
}

// This subroutine supplements sub1 (single-threaded)
template <typename Iterator>
void lib_calvin_sort::introSortParallelSub2(
	Iterator first, Iterator last, 
	FactoryLoader<pair<Iterator, Iterator>> &factoryLoader)
{	
	if (sizeof(*first)*(last - first) < L2_CACHE_SIZE) {
		//std::cout << "adding to factory loader: sub2\n";
		factoryLoader.add(pair<Iterator, Iterator>(first, last));
	} else { 
		Iterator left = hoarePartition(first, last);
		introSortParallelSub2(first, left, factoryLoader);
		introSortParallelSub2(left, last, factoryLoader);
	}
}

// normal two-way merge; input size is not zero (both arrays are not empty)
template <typename SrcIterator, typename TargetIterator>
void lib_calvin_sort::merge(SrcIterator first, SrcIterator middle, SrcIterator last, 
														 TargetIterator target) {
  SrcIterator left = first, right = middle;
	TargetIterator dest = target;
	// improved merge routine than the below ordinary one: reducing one index comparison
  while (true) {
    if(*right < *left) { // insert right
			*dest++ = *right++;
      if (right == last) { // last element
        break;
			}
    } else { // insert left
      *dest++ = *left++;
      if (left == middle) {
        break;
			}
    }
  }
	/*
	while (left != middle && right != last) {
    if (*right < *left) {
			*dest++ = *right++;			
		}	else {
			*dest++ = *left++;
		}
	}*/
  if (right == last) { // right subarray empty
    while (left != middle) {
      *dest++ = *left++;
    }
  } else {
    while (right != last) { //left subarray empty
      *dest++ = *right++;
    }
  }
}

template <typename Iterator>
void lib_calvin_sort::inPlaceMerge(Iterator first, Iterator middle, Iterator last) {
	if (middle - first < mergeSort_thre || last - middle < mergeSort_thre) {
		insertionSort(first, last);
		return;
	}
	Iterator leftMiddle = first + (middle - first)/2;
	Iterator rightMiddle = middle + binSearch(*leftMiddle, middle, last);
	exchange(leftMiddle, middle, rightMiddle);
	inPlaceMerge(first, leftMiddle, leftMiddle + (rightMiddle - middle));
  inPlaceMerge(leftMiddle + (rightMiddle - middle), rightMiddle, last);
	for (Iterator iter = first; iter < last - 1; iter++) {
		if (*(iter + 1) < *iter) {
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

template <typename SrcIterator, typename TargetIterator>
void lib_calvin_sort::mergeSort(SrcIterator first, SrcIterator last, 
																	TargetIterator target) {
  size_t num = last - first;
  TargetIterator targetLast = target + num;
  if (last - first < mergeSort_thre) {
    insertionSort(target, targetLast);
    return;
  }
  SrcIterator middle = first + num / 2;
  TargetIterator targetMiddle = target + num / 2;
  mergeSort(target, targetMiddle, first);
  mergeSort(targetMiddle, targetLast, middle);
  merge(first, middle, last, target);
}

template <typename SrcIterator, typename TargetIterator>
void lib_calvin_sort::mergeSortParallelSub0(SrcIterator first, SrcIterator last, 
	TargetIterator target, int thread_limit)
{
	if (sizeof(*first)*(last - first) < L2_CACHE_SIZE / 2 || thread_limit <= 0) {
		mergeSort(first, last, target);
		return;
	}
	size_t num = last - first;
	TargetIterator targetLast = target + num;
	SrcIterator middle = first + num / 2;
  TargetIterator targetMiddle = target + num / 2;
	MergeSortRThreadArg<SrcIterator, TargetIterator> argLeft(
		target, targetMiddle, first, thread_limit - 1);
	MergeSortRThreadArg<SrcIterator, TargetIterator> argRight(
		targetMiddle, targetLast, middle, thread_limit - 1);
	lib_calvin_thread::thread_type leftThread, rightThread;
	leftThread = create_thread(
		mergeSortParallelSub0ThreadFunction<SrcIterator, TargetIterator>, &argLeft);
	rightThread = create_thread(
		mergeSortParallelSub0ThreadFunction<SrcIterator, TargetIterator>, &argRight);
	lib_calvin_thread::wait_for_thread(leftThread);
	lib_calvin_thread::wait_for_thread(rightThread);		
	merge(first, middle, last, target);
}

// input is a bitonic array, and merge in increasing order (normal order)
template <typename Iterator>
void lib_calvin_sort::merge2(Iterator first, Iterator middle, Iterator last, 
    Iterator target) {
  
  Iterator left = first, right = last - 1;
  while (true) {
    while (*right < *left) {
      *target = *right;
      ++target;
      --right;
    }
    if (right < middle) 
      break;
    // now, the left element is less than or equal to right element
    *target = *left;
    ++target;
    ++left;
    while (*left < *right) {
      *target = *left;
      ++target;
      ++left;
    }
    if (left >= middle)
      break;
  }
  if (right < middle) { // right subarray empty
    while (left < middle) {
      *target = *left;
      ++target;
      ++left;
    }
  } else { // left subarray empty
    while (right >= middle) {
      *target = *right;
      ++target;
      --right;
    }
  }
}

// input is a bitonic array, and merge in decreasing order (reverse order)
template <typename Iterator>
void lib_calvin_sort::merge2Reversed(Iterator first, Iterator middle, 
																			 Iterator last, Iterator target) {

  Iterator left = first, right = last - 1;
  target = target + (last - first - 1); // last element of target array
  while (true) {
    while (*right < *left) {
      *target = *right;
      --target;
      --right;
    }
    if (right < middle) 
      break;
    // now, the left element is less than or equal to right element
    *target = *left;
    --target;
    ++left;
    while (*left < *right) {
      *target = *left;
      --target;
      ++left;
    }
    if (left >= middle)
      break;
  }
  if (right < middle) { // right subarray empty
    while (left < middle) {
      *target = *left;
      --target;
      ++left;
    }
  } else { // left subarray empty
    while (right >= middle) {
      *target = *right;
      --target;
      --right;
    }
  }
}

template <typename Iterator>
void lib_calvin_sort::mergeSort2Ascending(Iterator first, Iterator last, 
																				Iterator target) {
  size_t num = last - first;
  Iterator targetLast = target + num;
  if (last - first < 10) {
    insertionSort (target, targetLast);
    return;
  }
  Iterator middle = first + num / 2;
  Iterator targetMiddle = target + num / 2;
  mergeSort2Ascending(target, targetMiddle, first);
  mergeSort2Descending(targetMiddle, targetLast, middle);
  merge2(first, middle, last, target);
}

template <typename Iterator>
void lib_calvin_sort::mergeSort2Descending(Iterator first, Iterator last, 
																				Iterator target) {
  size_t num = last - first;
  Iterator targetLast = target + num;
  if (last - first < 10) {
    insertionSort_reversed(target, targetLast);
    return;
  }
  Iterator middle = first + num / 2;
  Iterator targetMiddle = target + num / 2;
  mergeSort2Ascending(target, targetMiddle, first);
  mergeSort2Descending(targetMiddle, targetLast, middle);
  merge2Reversed(first, middle, last, target);
}

/******************************* Sorting methods ****************************/

// 2009-6-27: Modified loop logic to avoid making (first - 1) iterator. 
template <typename Iterator>
void lib_calvin_sort::insertionSort(Iterator first, Iterator last) {
  Iterator right; // the element to insert
  Iterator p; // moving iterator
  typedef typename iterator_traits<Iterator>::value_type elemType;
	if (first == last) {
		return;
	}
  for (right = first + 1; right < last; ++right) {
    elemType store = *right;
    p = right;
		while (true) {
			if (first < p && store < *(p - 1)) { // cannot insert into *p
				*p = (elemType)*(p - 1);
				--p;
			} else { // p == first || *(p - 1) <= store
				*p = store;
				break;
			}
    }    
  }
}

template <typename Iterator>
void lib_calvin_sort::heapSort(Iterator first, Iterator last) {
  int size = static_cast<int>(last - first); // size of heap
  // max-heapify
  for (int i = (size - 2) / heapD; i >= 0; i--) {
    percolateDown (first, i, size);
  }
  // sorting
  while (size > 1) {
    // swap first and last
    using lib_calvin_util::swap;
    swap (*first, *(first + size - 1));
    size--;
    percolateDown (first, 0, size);
  }
}

template <typename Iterator>
void lib_calvin_sort::introSort(Iterator first, Iterator last) {
  introSortSub(first, last, lib_calvin_util::log(last - first) * 3);
  if (last - first < introSort_thre) {
    insertionSort(first, last);
    return;
  } else {
    insertionSort(first, first + introSort_thre);
    unguardedInsertionSort (first + introSort_thre, last);
    return;
  }	
}

template <typename T>
void lib_calvin_sort::introSort(vector<T> &elements) {
	if (elements.empty()) {
		return;
	}
	introSort(elements.begin(), elements.end());
	// if iterator of vector container is not simple pointer, there might be room for optimization (not verified)
	//introSort(&*elements.begin(), &*(elements.end() - 1) + 1);
}

// 2009-01-26: I discovered that if the problem size is bigger than L2, multi-
// threading does not accelerate the partitioning at all, which means that the 
// bottleneck is memory bandwidth (not CPU) for that case. And if the problem lies
// within L2, we get almost maximum speedup. Therefore, The key is to mix memory 
// intensive part and CPU intensive part of the operation, to utilize both resources 
// efficiently (in the same sense as that of multi-programming in OS).
template <typename Iterator>
void lib_calvin_sort::introSortParallel(Iterator first, Iterator last) {
	introSortParallelSub0(first, last, 4);
}

// Blocking method
template <typename Iterator>
void lib_calvin_sort::introSortParallelAdvanced(Iterator first, Iterator last) 
{
	IntroSort<Iterator> sorter;
	Factory<pair<Iterator, Iterator>, IntroSort<Iterator>> factory(sorter);
	// Create many threads for partitioning large arrays
	introSortParallelSub1<Iterator>(first, last, 4, factory);
	// Create 4 threads for sorting small arrays in L2 cache
	unsigned numCores = 4;
	thread_type *handleArray = new thread_type[numCores];
	for (unsigned i = 0; i < numCores; ++i) {
		handleArray[i] = create_thread(factoryThreadFunction<pair<Iterator, Iterator>, 
				IntroSort<Iterator>>, &factory);
	}
	factory.close();
	for (unsigned i = 0; i < numCores; ++i) {
		wait_for_thread(handleArray[i]);
		//CloseHandle(handleArray[i]);
	}
	delete[] handleArray;
}

// Mixing memory bandwith and CPU intensive parts of algorithm
template <typename Iterator>
void lib_calvin_sort::introSortParallelAdvanced2(Iterator first, Iterator last) 
{
	IntroSort<Iterator> sorter;
	Factory<pair<Iterator, Iterator>, IntroSort<Iterator>> factory(sorter);
	// Create n threads for sorting small arrays in L2 cache
	unsigned numCores = 4;
	thread_type *handleArray = new thread_type[numCores];
	for (unsigned i = 0; i < numCores; ++i) {
		// We can change how to sort small sub-array simply by choosing IntroSort or
		// CountingSort below
		handleArray[i] = create_thread(factoryThreadFunction<pair<Iterator, Iterator>, 
			IntroSort<Iterator>>, &factory); 
		//SetThreadAffinityMask(handleArray[i], 1 << (i % 2));
	}
	// Create n threads for partitioning large arrays
	introSortParallelSub1(first, last, 3, factory);
	
	factory.close();
	for (unsigned i = 0; i < numCores; ++i) {
		wait_for_thread(handleArray[i]);
		//CloseHandle(handleArray[i]);
	}
	delete[] handleArray;
}

template <typename Iterator>
void lib_calvin_sort::mergeSort(Iterator first, Iterator last) {
  ptrdiff_t num = last - first;
  // additional array for operation
  typedef typename iterator_traits<Iterator>::pointer pointerType;
	typedef typename iterator_traits<Iterator>::value_type valueType;
	// prepare supplementary array
  pointerType tempArray = (pointerType)operator new (sizeof(valueType) * num);
	auto original = first;
	auto copy = tempArray;
  for ( ; original != last; ++original, ++copy) {
    new (copy) valueType(*original);
  }
	// mergesort without redundant copying
  mergeSort(tempArray, tempArray + num, first);
	// delete supplementary array
	for (ptrdiff_t i = 0; i < num; ++i) {
    tempArray[i].~valueType();
  }
  operator delete(tempArray);
}

template <typename Iterator>
void lib_calvin_sort::inPlaceMergeSort(Iterator first, Iterator last) {
	if (last - first < mergeSort_thre) {
		insertionSort(first, last);
		return;
	}
	Iterator middle = first + (last - first)/2;
	inPlaceMergeSort(first, middle);
	inPlaceMergeSort(middle, last);
	inPlaceMerge(first, middle, last);
}

template <typename Iterator>
void lib_calvin_sort::mergeSortParallel(Iterator first, Iterator last) {
  size_t num = last - first;
  // additional array for operation
  typedef typename iterator_traits<Iterator>::pointer pointerType;
	typedef typename iterator_traits<Iterator>::value_type valueType;
  pointerType tempArray;
  tempArray = (pointerType) operator new (sizeof(valueType) * num);
  Iterator original = first;
	pointerType copy = tempArray;
  for ( ; original != last; ++original, ++copy) {
    new (copy) valueType(*original);
  }
  mergeSortParallelSub0(tempArray, tempArray + num, first, 4);
  operator delete(tempArray);
}

// This one is rather messy, and turns out not to be faster than normal
// ..mergesort
template <typename Iterator>
void lib_calvin_sort::mergeSort2(Iterator first, Iterator last) {
  size_t num = last - first;
  // additional array for operation
  typedef typename iterator_traits<Iterator>::pointer Pointer;
  Pointer tempArray;
  tempArray = (Pointer) operator new 
      (sizeof(typename iterator_traits<Iterator>::value_type) * num);
  Iterator original = first, copy = tempArray;
  for ( ; original != last; ++original, ++copy) {
    *copy = *original;
  }
  mergeSort2Ascending (tempArray, tempArray + num, first);
  operator delete(tempArray);
}


template <typename Iterator>
void lib_calvin_sort::countingSort(Iterator first, Iterator last) {
  size_t num = last - first;
  if(num < 20) {
    insertionSort(first, last);
    return;
  }
  ptrdiff_t min = first->get_value();
  ptrdiff_t max = min;
  for(Iterator iter = first + 1; iter < last; ++iter) {
    ptrdiff_t curValue = iter->get_value();
    if(curValue < min) {
      min = curValue;
		} else if(curValue > max) {
      max = curValue;
		}
  }
  if(min == max)
    return;
  size_t avgBucketPop = 5;
  size_t numOfBucket = num / avgBucketPop; // at least 2
  size_t bucketSize = (max - min) / numOfBucket; // set bucketSize
  
  //std::cout << "max = " << max << "min = " << min << "\n";
	//std::cout << "num of bucket is " << numOfBucket << "\n";
	//std::cout << "bucketSize is " << bucketSize << "\n";;
  if(bucketSize == 0) { // almost all elements are equal
    introSort(first, last);
    return;
  }
  // set final numOfBucket
  numOfBucket = (max - min) / bucketSize + 1;
  size_t *countArray; // calculating num of elements for each bucket
  if((countArray = (size_t *)malloc((numOfBucket + 1)*sizeof(size_t))) == NULL) {
    std::cout << "countingSort: Malloc error.\n";
    exit(0);
  }
  size_t *startArray; // marking starting position of each bucket in the array
  if((startArray = (size_t *)malloc((numOfBucket + 1)*sizeof(size_t))) == NULL) {
    std::cout << "countingSort: Malloc error.\n";
    exit(0);
  }
  for(size_t i = 0; i <= numOfBucket; ++i)
    countArray[i] = 0;

  // counting elements for each bucket
  for(Iterator iter = first; iter < last; ++iter) 
    countArray[(iter->get_value() - min) / bucketSize]++;
  // accumulate counters to startArray
  size_t sum = 0;
  for(size_t i = 0; i <= numOfBucket; ++i) {
    startArray[i] = sum;
    sum += countArray[i];
    countArray[i] = startArray[i]; // copy back
  }
  if(startArray[numOfBucket] != num) {
    std::cout << "error\n";
    exit(0);
  }

  // put elements into right bucket.
  // countArray stores the current position to insert elements
  size_t curIndex = 0; // start with the first bucket and proceed
  while(true) {
    //cout << "piont A\n";
    while(countArray[curIndex] == startArray[curIndex + 1]) { // this bucket is done already
      size_t bucketSize = startArray[curIndex + 1] - startArray[curIndex];
      if(bucketSize > 30 ) { // if bucket size is greater than 30
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
      if(curIndex == numOfBucket) // all done
        break;
    }
    if(curIndex == numOfBucket) // all done
      break;
    // starting with the rightmost element of the bucket.
    size_t initialIndex = startArray[curIndex + 1] - 1;
    typename iterator_traits<Iterator>::value_type store;
    store = *(first + initialIndex); // store initail element
    while(true) {
      //cout << "piont B\n";
      // decide which bucket and position to put into
      size_t bucketNum = (store.get_value() - min) / bucketSize;
      size_t targetIndex = countArray[bucketNum];
      ++countArray[bucketNum];
      if(targetIndex == initialIndex) { // got around to start position
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

template <typename Iterator>
void lib_calvin_sort::introSort2(Iterator first, Iterator last) {
  introSort2Sub(first, last, lib_calvin_util::log(last - first) * 3);
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
	using lib_calvin::vector;
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

