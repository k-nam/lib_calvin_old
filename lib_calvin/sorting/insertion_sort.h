#ifndef LIB_CALVIN__SORT__INSERTION_SORT_H
#define LIB_CALVIN__SORT__INSERTION_SORT_H

namespace lib_calvin_sort
{
	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void insertionSort(Iterator first, Iterator last, Comparator const comp = Comparator());

	// This subroutine assumes that the element right before 'first' exists and
	// ..it is not greater than any element in the input array (it acts as a
	// ..sentinal)
	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void unguardedInsertionSort(Iterator first, Iterator last, Comparator const comp = Comparator());
}


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
			} while (first < p && comp(store, *(p - 1)));
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


#endif