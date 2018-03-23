#ifndef LIB_CALVIN__SORT__MERGE_SORT_PARALLEL_H
#define LIB_CALVIN__SORT__MERGE_SORT_PARALLEL_H

#include "thread.h"

namespace lib_calvin_sort
{
	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void mergeSortParallel(Iterator first, Iterator last, Comparator const comp = Comparator());

	template <typename SrcIterator, typename TargetIterator, typename Comparator>
	void mergeSortParallelSub0(SrcIterator first, SrcIterator last,
							   TargetIterator target, Comparator comp, int thread_limit);

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

	template <typename SrcIterator, typename TargetIterator, typename Comparator>
	void *
		mergeSortParallelSub0ThreadFunction(void *lpParam) {
		MergeSortRThreadArg<SrcIterator, TargetIterator, Comparator> *pArg =
			(MergeSortRThreadArg<SrcIterator, TargetIterator, Comparator> *)lpParam;
		mergeSortParallelSub0(pArg->first_, pArg->last_, pArg->target_, pArg->comp_, pArg->thread_limit_);
		return NULL;
	}
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
	lib_calvin::thread_type leftThread, rightThread;
	leftThread = lib_calvin::create_thread(
		mergeSortParallelSub0ThreadFunction<SrcIterator, TargetIterator, Comparator>, &argLeft);
	rightThread = lib_calvin::create_thread(
		mergeSortParallelSub0ThreadFunction<SrcIterator, TargetIterator, Comparator>, &argRight);
	lib_calvin::wait_for_thread(leftThread);
	lib_calvin::wait_for_thread(rightThread);
	lib_calvin_sort::merge(first, middle, last, target, comp);
}

#endif
