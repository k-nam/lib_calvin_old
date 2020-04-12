#ifndef LIB_CALVIN__SORT__INTRO_SORT_PARALLEL_H
#define LIB_CALVIN__SORT__INTRO_SORT_PARALLEL_H

#include <utility>

#include "thread.h"
#include "factory.h"

namespace lib_calvin_sort {
	using lib_calvin::Factory;
	using lib_calvin::FactoryLoader;
	using std::pair;

	// Basic multithreaded quicksort
	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void introSortParallel(Iterator first, Iterator last, Comparator const comp = Comparator());

	// Using blocking
	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void introSortParallelAdvanced(Iterator first, Iterator last, Comparator const comp = Comparator());

	// Mixing memory intensive and CPU intensive part
	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void introSortParallelAdvanced2(Iterator first, Iterator last, Comparator const comp = Comparator());

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
	void introSortParallelSub2(Iterator first, Iterator last, Comparator comp, int remainingDepth,
							   FactoryLoader<pair<Iterator, Iterator>> &factoryLoader);

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

	// Function object version of introsort (for multi-threading)
	template <typename Iterator, typename Comparator>
	class IntroSort {
	public:
		IntroSort(Comparator comp = Comparator()) : comp_(comp) { }
		void operator()(pair<Iterator, Iterator> const &inArray) {
			blockIntroSort(inArray.first, inArray.second, comp_);
		}
	private:
		Comparator comp_;
	};

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
}

// 2009 - 01 - 26: I discovered that if the problem size is bigger than L2, multi -
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
	using std::pair;
	IntroSort<Iterator, Comparator> sorter(comp);
	typedef Factory<pair<Iterator, Iterator>, IntroSort<Iterator, Comparator>> factoryType;
	factoryType factory(sorter);
	// Create many threads for partitioning large arrays
	introSortParallelSub1(first, last, comp, 4, factory);
	// Create 4 threads for sorting small arrays in L2 cache
	unsigned numCores = 6;
	typedef lib_calvin::thread_type thread_type;
	thread_type *handleArray = new thread_type[numCores];
	for (unsigned i = 0; i < numCores; ++i) {
		handleArray[i] = create_thread(lib_calvin::factoryThreadFunction<pair<Iterator, Iterator>,
									   IntroSort<Iterator, Comparator>>, &factory);
	}
	factory.close();
	for (unsigned i = 0; i < numCores; ++i) {
		lib_calvin::wait_for_thread(handleArray[i]);
		//CloseHandle(handleArray[i]);
	}
	delete[] handleArray;
}

// Mixing memory bandwith and CPU intensive parts of algorithm
template <typename Iterator, typename Comparator>
void lib_calvin_sort::introSortParallelAdvanced2(Iterator first, Iterator last, Comparator comp)
{
	IntroSort<Iterator, Comparator> sorter(comp);
	typedef Factory<pair<Iterator, Iterator>, IntroSort<Iterator, Comparator>> factoryType;
	factoryType factory(sorter);
	// Create n threads for sorting small arrays in L2 cache
	unsigned numCores = 4;
	typedef lib_calvin::thread_type thread_type;
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
		lib_calvin::wait_for_thread(handleArray[i]);
		//CloseHandle(handleArray[i]);
	}
	delete[] handleArray;
}

// 2009 - 01 - 16: verified that shared L2 cache in Q9550 plays a role in parallel
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
		blockIntroSort(first, last, comp);
		return;
	}
	Iterator left = betterPartition(first, last, comp);
	QuickSortThreadArg<Iterator, Comparator> argLeft(first, left, comp, thread_limit - 1, NULL);
	QuickSortThreadArg<Iterator, Comparator> argRight(left, last, comp, thread_limit - 1, NULL);
	lib_calvin::thread_type leftThread, rightThread;
	leftThread = lib_calvin::create_thread(introSortParallelSub0ThreadFunction<Iterator, Comparator>, &argLeft);
	//SetThreadAffinityMask(leftThread, 1 << 0);
	// For unbalancing left and right sub-threads, thereby letting left sub-thread
	// reach bottom, and start CPU-intensive-thread quickly.
	//Sleep(10);
	rightThread =
		lib_calvin::create_thread(introSortParallelSub0ThreadFunction<Iterator, Comparator>, &argRight);
	//SetThreadAffinityMask(rightThread, 1 << 1);
	lib_calvin::wait_for_thread(leftThread);
	//CloseHandle(leftThread);
	lib_calvin::wait_for_thread(rightThread);
	//CloseHandle(rightThread);
}

// multi threaded partitioning
template <typename Iterator, typename Comparator>
void lib_calvin_sort::introSortParallelSub1(
	Iterator first, Iterator last, Comparator comp, int thread_limit,
	FactoryLoader<pair<Iterator, Iterator>> &factoryLoader)
{
	if (sizeof(*first)*(last - first) < L2_CACHE_SIZE || thread_limit <= 0) {
		introSortParallelSub2(first, last, comp, lib_calvin_util::log(last - first) * 3,
			factoryLoader);
		return;
	}
	Iterator left = betterPartition(first, last, comp);
	QuickSortThreadArg<Iterator, Comparator> argLeft(first, left, comp, thread_limit - 1, &factoryLoader);
	QuickSortThreadArg<Iterator, Comparator> argRight(left, last, comp, thread_limit - 1, &factoryLoader);
	lib_calvin::thread_type leftThread, rightThread;
	leftThread =
		lib_calvin::create_thread(introSortParallelSub1ThreadFunction<Iterator, Comparator>, &argLeft);
	//SetThreadAffinityMask(leftThread, 1 << 2);
	//Sleep(10);
	rightThread =
		lib_calvin::create_thread(introSortParallelSub1ThreadFunction<Iterator, Comparator>, &argRight);
	//SetThreadAffinityMask(leftThread, 1 << 3);
	lib_calvin::wait_for_thread(leftThread);
	// CloseHandle(leftThread);
	lib_calvin::wait_for_thread(rightThread);
	// CloseHandle(rightThread);
}

// This subroutine supplements sub1 (single-threaded)
template <typename Iterator, typename Comparator>
void lib_calvin_sort::introSortParallelSub2(Iterator first, Iterator last, Comparator comp, int remainingDepth,
											FactoryLoader<pair<Iterator, Iterator>> &factoryLoader) {
	if (sizeof(*first)*(last - first) < L2_CACHE_SIZE / 2 || remainingDepth == 0) {
		//std::cout << "adding to factory loader: sub2\n";
		factoryLoader.add(pair<Iterator, Iterator>(first, last));
	} else {
		Iterator left = betterPartition(first, last, comp);
		introSortParallelSub2(first, left, comp, remainingDepth - 1, factoryLoader);
		introSortParallelSub2(left, last, comp, remainingDepth - 1, factoryLoader);
	}
}

#endif


