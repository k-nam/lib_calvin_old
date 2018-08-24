#ifndef LIB_CALVIN__SORT__IN_PLACE_MERGE_SORT_PARALLEL_H
#define LIB_CALVIN__SORT__IN_PLACE_MERGE_SORT_PARALLEL_H

#include "thread.h"
#include "in_place_merge_sort.h"
#include "random.h"

namespace lib_calvin_sort
{
	template <typename Iterator, 
		typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void inPlaceMergeSortParallel(Iterator first, Iterator last, Comparator const comp = Comparator());

	template <typename SrcIterator, typename Comparator>
	void inPlaceMergeSortParallelSub0(SrcIterator first, SrcIterator last,
		Comparator comp, int thread_limit);

	template <typename SrcIterator, typename Comparator>
	struct InPlaceMergeSortRThreadArg
	{
		InPlaceMergeSortRThreadArg(SrcIterator const &first, SrcIterator const &last,
			Comparator comp, int thread_limit) :
			first_(first), last_(last), comp_(comp), thread_limit_(thread_limit) { }
		SrcIterator const first_;
		SrcIterator const last_;
		Comparator comp_;
		int const thread_limit_;
	};

	template <typename SrcIterator, typename Comparator>
	void *
		inPlaceMergeSortParallelSub0ThreadFunction(void *lpParam) {
		InPlaceMergeSortRThreadArg<SrcIterator, Comparator> *pArg =
			(InPlaceMergeSortRThreadArg<SrcIterator, Comparator> *)lpParam;
		inPlaceMergeSortParallelSub0(pArg->first_, pArg->last_, pArg->comp_,
			pArg->thread_limit_);
		return NULL;
	}
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::inPlaceMergeSortParallel(Iterator first, Iterator last, Comparator comp) {
	if (sizeof(*first)*(last - first) < L2_CACHE_SIZE / 2 ||
		last - first < PARALLEL_THRE) {
		inPlaceMergeSort(first, last, comp);
		return;
	}
	size_t num = last - first;
	inPlaceMergeSortParallelSub0(first, last, comp, 4);
}

template <typename SrcIterator, typename Comparator>
void lib_calvin_sort::inPlaceMergeSortParallelSub0(SrcIterator first, SrcIterator last, 
	Comparator comp, int thread_limit) {
	if (sizeof(*first)*(last - first) < L2_CACHE_SIZE / 2 || thread_limit <= 0) {
		inPlaceMergeSort3(first, last, comp);
		return;
	}
	size_t num = last - first;

	lib_calvin::random_number_generator gen;
	SrcIterator middle = first + num / 4 + gen() % (num / 2 );
	InPlaceMergeSortRThreadArg<SrcIterator, Comparator> argLeft(
		first, middle, comp, thread_limit - 1);
	InPlaceMergeSortRThreadArg<SrcIterator, Comparator> argRight(
		middle, last, comp, thread_limit - 1);
	lib_calvin::thread_type leftThread, rightThread;
	leftThread = lib_calvin::create_thread(
		inPlaceMergeSortParallelSub0ThreadFunction<SrcIterator, Comparator>, &argLeft);
	rightThread = lib_calvin::create_thread(
		inPlaceMergeSortParallelSub0ThreadFunction<SrcIterator, Comparator>, &argRight);
	lib_calvin::wait_for_thread(leftThread);
	lib_calvin::wait_for_thread(rightThread);
	std::inplace_merge(first, middle, last, comp);
}

#endif
