#ifndef LIB_CALVIN__SORT__HEAP_SORT_H
#define LIB_CALVIN__SORT__HEAP_SORT_H

namespace lib_calvin_sort
{
	int const HEAP_D = 3;

	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void heapSort(Iterator first, Iterator last, Comparator const comp = Comparator());

	// a maxheap for sorting
	template <typename Iterator, typename Comparator>
	void percolateDown(Iterator base, Comparator comp, int index, int size);
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

// maxheap, size: heap size, HEAP_D > 1
template <typename Iterator, typename Comparator>
void lib_calvin_sort::percolateDown(Iterator const base, Comparator comp, int index, int const size) {
	// save original element and insert it back later
	using namespace std;
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


#endif