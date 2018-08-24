#ifndef LIB_CALVIN__SORT__MERGE_SORT_H
#define LIB_CALVIN__SORT__MERGE_SORT_H

namespace lib_calvin_sort
{
	int const MERGESORT_THRESHOLD = 32;

	template <typename Iterator, typename Comparator = 
		std::less<typename std::iterator_traits<Iterator>::value_type>>
	void mergeSort(Iterator first, Iterator last, Comparator comp = Comparator());

	// Using only N/2 size temp space (from boost::spinsort)
	template <typename Iterator, typename Comparator = 
		std::less<typename std::iterator_traits<Iterator>::value_type>>
	void mergeSort2(Iterator first, Iterator last, Comparator comp = Comparator());

	template <typename Iterator, typename BufferIterator, typename Comparator>
	void mergeSort2_sub_stay(Iterator first, Iterator last, BufferIterator buffer,
		Comparator comp);
	
	template <typename Iterator, typename BufferIterator, typename Comparator>
	void mergeSort2_sub_move(Iterator first, Iterator last, BufferIterator buffer, 
		Comparator comp);

	template <typename SrcIterator, typename TargetIterator, typename Comparator>
	void merge_assign(SrcIterator input1_begin, SrcIterator input1_end, 
		SrcIterator input2_begin, SrcIterator input2_end,
		TargetIterator target, Comparator comp);

	template <typename SrcIterator, typename TargetIterator, typename Comparator>
	void merge_construct(SrcIterator input1_begin, SrcIterator input1_end,
		SrcIterator input2_begin, SrcIterator input2_end,
		TargetIterator target, Comparator comp);

	template <typename SrcIterator, typename TargetIterator, typename Comparator>
	void merge2(SrcIterator first, SrcIterator middle, SrcIterator last,
		TargetIterator target, Comparator comp);

	// Return true if insertion sort was done
	template <typename SrcIterator, typename TargetIterator, typename Comparator>
	bool mergeSortSub(SrcIterator first, SrcIterator last, TargetIterator target, Comparator comp,
		bool targetIsReal, bool alwaysAssign);
}


template <typename Iterator, typename Comparator>
void lib_calvin_sort::mergeSort(Iterator first, Iterator last, Comparator comp) {
	ptrdiff_t num = last - first;
	// additional array for operation
	typedef typename std::iterator_traits<Iterator>::pointer pointerType;
	typedef typename std::iterator_traits<Iterator>::value_type valueType;
	// prepare supplementary array
	pointerType tempArray = (pointerType)operator new (sizeof(valueType) * num);
	for (ptrdiff_t i = 0; i < num; ++i) {
		//new (tempArray + i)  valueType(std::move(*(first + i)));
	}

	// mergesort without redundant copying
	bool endedImmediately = mergeSortSub(tempArray, tempArray + num, first, comp, true, false);
	// delete supplementary array
	if (!endedImmediately) {
		for (ptrdiff_t i = 0; i < num; ++i) {
			tempArray[i].~valueType();
		}
	}
	operator delete(tempArray);
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::mergeSort2(Iterator first, Iterator last, Comparator comp) {
	ptrdiff_t num = last - first;
	if (num < 2 * MERGESORT_THRESHOLD) {
		insertionSort(first, last, comp);
		return;
	}
	typedef typename std::iterator_traits<Iterator>::pointer pointerType;
	typedef typename std::iterator_traits<Iterator>::value_type valueType;

	// Make left subarray the larger one, and I will use that space as buffer
	ptrdiff_t leftSize = num - num / 2;
	pointerType tempArray = (pointerType)operator new (sizeof(valueType) * leftSize);
	for (ptrdiff_t i = 0; i < leftSize; ++i) {
		//new (tempArray + i)  valueType(std::move(*(first + i)));
	}

	// If num is odd, the last element of tempArray will be un-constructed after the first 
	//  mergeSortSub call
	if (num % 2 == 1) {
		new (tempArray + leftSize - 1)  valueType(*first);
	}	

	Iterator middle = first + leftSize;
	mergeSortSub(tempArray, tempArray + leftSize, middle, comp, true, false);
	mergeSortSub(first, first + leftSize, tempArray, comp, false, true);

	merge_assign(tempArray, tempArray + leftSize, middle, last, first, comp);

	for (ptrdiff_t i = 0; i < leftSize; ++i) {
		tempArray[i].~valueType();
	}	
	operator delete(tempArray);
}

template <typename Iterator, typename BufferIterator, typename Comparator>
void lib_calvin_sort::mergeSort2_sub_stay(Iterator first, Iterator last, BufferIterator buffer, 
	Comparator comp) {
	if (last - first < MERGESORT_THRESHOLD) {
		insertionSort(first, last, comp);
		return;
	}
	ptrdiff_t num = last - first;
	ptrdiff_t leftSize = num / 2;
	Iterator middle = first + leftSize;
	mergeSort2_sub_stay(middle, last, buffer, comp);
	mergeSort2_sub_move(first, middle, buffer, comp);
	merge_assign(middle, last, buffer, buffer + leftSize, first, comp);
}

template <typename Iterator, typename BufferIterator, typename Comparator>
void lib_calvin_sort::mergeSort2_sub_move(Iterator first, Iterator last, BufferIterator buffer, 
	Comparator comp) {
	if (last - first < MERGESORT_THRESHOLD) {
		insertionSort(first, last, comp);
		for (Iterator iter = first; iter < last; ++iter, ++buffer) {
			*buffer = std::move(*iter);
		}
		return;
	}
	ptrdiff_t num = last - first;
	ptrdiff_t leftSize = num / 2;
	Iterator middle = first + leftSize;
	mergeSort2_sub_stay(first, middle, buffer, comp);
	mergeSort2_sub_move(middle, last, buffer + leftSize, comp);
	merge_assign(first, middle, buffer + leftSize, buffer + num, buffer, comp);
}

template <typename SrcIterator, typename TargetIterator, typename Comparator>
bool lib_calvin_sort::mergeSortSub(SrcIterator first, SrcIterator last,
	TargetIterator target, Comparator comp,
	bool targetIsReal, bool alwaysAssign) {
	size_t num = last - first;
	SrcIterator middle = first + num / 2;
	TargetIterator targetLast = target + num;

	if (last - first < 2 * MERGESORT_THRESHOLD && !targetIsReal) {
		insertionSort(first, middle, comp);
		insertionSort(middle, last, comp);
		if (alwaysAssign) {
			lib_calvin_sort::merge_assign(first, middle, middle, last, target, comp);
		} else {
			lib_calvin_sort::merge_construct(first, middle, middle, last, target, comp);
		}		
		return true;
	}
		
	TargetIterator targetMiddle = target + num / 2;
	mergeSortSub(target, targetMiddle, first, comp, !targetIsReal, alwaysAssign);
	mergeSortSub(targetMiddle, targetLast, middle, comp, !targetIsReal, alwaysAssign);
	lib_calvin_sort::merge_assign(first, middle, middle, last, target, comp);
	return false;
}

// normal two-way merge; input size is not zero (both arrays are not empty)
template <typename SrcIterator, typename TargetIterator, typename Comparator>
void lib_calvin_sort::merge_assign(SrcIterator left, SrcIterator input1_end,
	SrcIterator right, SrcIterator input2_end,
	TargetIterator target, Comparator comp) {

	
	while (true) {
		if (comp(*right, *left)) {
			*target++ = std::move(*right++);
			if (right == input2_end) {
				break;
			}
		} else {
			*target++ = std::move(*left++);
			if (left == input1_end) {
				break;
			}
		}
	}

	/*
	while (left != input1_end && right != input2_end) {
		if (comp(*right, *left)) {
			*target++ = std::move(*right++);

		} else {
			*target++ = std::move(*left++);

		}
	}*/

	if (right == input2_end) { // right subarray empty
		while (left != input1_end) {
			*target++ = std::move(*left++);
		}
	} else {
		while (right != input2_end) { //left subarray empty
			*target++ = std::move(*right++);
		}
	}
}

template <typename SrcIterator, typename TargetIterator, typename Comparator>
void lib_calvin_sort::merge_construct(SrcIterator left, SrcIterator input1_end,
	SrcIterator right, SrcIterator input2_end,
	TargetIterator target, Comparator comp) {

	typedef typename std::iterator_traits<SrcIterator>::pointer pointerType;
	typedef typename std::iterator_traits<SrcIterator>::value_type valueType;

	while (true) {
		if (comp(*right, *left)) {
			new (&(*target++)) valueType(std::move(*right++));
			if (right == input2_end) {
				break;
			}
		} else {
			new (&(*target++)) valueType(std::move(*left++));
			if (left == input1_end) {
				break;
			}
		}
	}

	if (right == input2_end) { // right subarray empty
		while (left != input1_end) {
			new (&(*target++)) valueType(std::move(*left++));
		}
	} else {
		while (right != input2_end) { //left subarray empty
			new (&(*target++)) valueType(std::move(*right++));
		}
	}
}

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






#endif