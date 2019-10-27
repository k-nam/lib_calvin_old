#ifndef LIB_CALVIN__SORT__MERGE_SORT_PARALLEL_H
#define LIB_CALVIN__SORT__MERGE_SORT_PARALLEL_H

#include "thread.h"
#include "random.h"
#include "merge_sort.h"

namespace lib_calvin_sort
{
	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void mergeSortParallel(Iterator first, Iterator last, Comparator const comp = Comparator());

	template <typename SrcIterator, typename TargetIterator, typename Comparator>
	void mergeSortParallelSub0(SrcIterator first, SrcIterator last,
							   TargetIterator target, Comparator comp, int thread_limit, bool targetReal);

	template <typename SrcIterator, typename TargetIterator, typename Comparator>
	struct MergeSortRThreadArg
	{
		MergeSortRThreadArg(SrcIterator const &first, SrcIterator const &last,
							TargetIterator const &target, Comparator comp, int thread_limit, bool targetReal) :
			first_(first), last_(last), target_(target), comp_(comp), thread_limit_(thread_limit), targetReal_(targetReal) { }
		SrcIterator const first_;
		SrcIterator const last_;
		TargetIterator const target_;
		Comparator comp_;
		int const thread_limit_;
		bool targetReal_;
	};

	template <typename SrcIterator, typename TargetIterator, typename Comparator>
	void *
		mergeSortParallelSub0ThreadFunction(void *lpParam) {
		MergeSortRThreadArg<SrcIterator, TargetIterator, Comparator> *pArg =
			(MergeSortRThreadArg<SrcIterator, TargetIterator, Comparator> *)lpParam;
		mergeSortParallelSub0(pArg->first_, pArg->last_, pArg->target_, pArg->comp_, pArg->thread_limit_, pArg->targetReal_);
		return NULL;
	}
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::mergeSortParallel(Iterator first, Iterator last, Comparator comp) {
	if (sizeof(*first)*(last - first) < L2_CACHE_SIZE / 2 ||
		last - first < PARALLEL_THRE) {
		mergeSort(first, last, comp);
		return;
	}
	size_t num = last - first;
	// additional array for operation
	typedef typename iterator_traits<Iterator>::pointer pointerType;
	typedef typename iterator_traits<Iterator>::value_type valueType;
	pointerType tempArray;
	tempArray = (pointerType) operator new (sizeof(valueType) * num);
	Iterator original = first;
	for (auto copy = tempArray; original != last; ++original, ++copy) {
		new (copy) valueType(std::move(*original));
	}
	mergeSortParallelSub0(tempArray, tempArray + num, first, comp, 3, false);
	for (size_t i = 0; i < num; i++) {
		tempArray[i].~valueType();
	}
	operator delete(tempArray);
}

template <typename SrcIterator, typename TargetIterator, typename Comparator>
void lib_calvin_sort::mergeSortParallelSub0(SrcIterator first, SrcIterator last,
											TargetIterator target, Comparator comp, int thread_limit, bool targetReal) {
	if (sizeof(*first)*(last - first) < L2_CACHE_SIZE / 2 || thread_limit <= 0) {
		size_t numStages = static_cast<size_t>(
			std::ceil(std::log2(static_cast<double>(last - first) / MERGESORT_THRESHOLD)));

		if (targetReal ^ (numStages % 2 == 0)) {
			numStages--;
		}

		mergeSortSub(first, last, target, comp, targetReal, false, numStages);
		return;
	}
	size_t num = last - first;
	TargetIterator targetLast = target + num;

	lib_calvin::random_number_generator gen;
	size_t leftSize = num / 2;

	SrcIterator middle = first + leftSize;
	TargetIterator targetMiddle = target + leftSize;
	MergeSortRThreadArg<SrcIterator, TargetIterator, Comparator> argLeft(
		target, targetMiddle, first, comp, thread_limit - 1, !targetReal);
	MergeSortRThreadArg<SrcIterator, TargetIterator, Comparator> argRight(
		targetMiddle, targetLast, middle, comp, thread_limit - 1, !targetReal);
	lib_calvin::thread_type leftThread, rightThread;
	leftThread = lib_calvin::create_thread(
		mergeSortParallelSub0ThreadFunction<SrcIterator, TargetIterator, Comparator>, &argLeft);
	rightThread = lib_calvin::create_thread(
		mergeSortParallelSub0ThreadFunction<SrcIterator, TargetIterator, Comparator>, &argRight);
	lib_calvin::wait_for_thread(leftThread);
	lib_calvin::wait_for_thread(rightThread);
	lib_calvin_sort::merge_assign(first, middle, middle, last, target, comp);
}

#endif
