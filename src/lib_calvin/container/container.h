#ifndef LIB_CALVIN__CONTAINER__CONTAINER_H
#define LIB_CALVIN__CONTAINER__CONTAINER_H

#include <utility>
#include <iostream>

namespace lib_calvin_container
{
	using std::ptrdiff_t;

	template <typename T>
	void moveConstructAndDestruct(T const *source, T *target, size_t number);

	template <typename T>
	void copyConstruct(T const *source, T *target, size_t number);

	template <typename T>
	void destruct(T *elements, size_t size);

	template <typename Iterator>
	Iterator advanceIteratorBy(Iterator iterator, ptrdiff_t distance);

	template <typename Iterator>
	Iterator advanceIteratorBy(Iterator iterator, ptrdiff_t distance,
		std::random_access_iterator_tag category);

	template <typename Iterator>
	Iterator advanceIteratorBy(Iterator iterator, ptrdiff_t distance,
		std::bidirectional_iterator_tag category);

	template <typename Iterator>
	Iterator advanceIteratorBy(Iterator iterator, ptrdiff_t distance,
		std::forward_iterator_tag category);


	template <typename Iterator>
	ptrdiff_t distanceBetweenIterator(Iterator begin, Iterator end);

	template <typename Iterator>
	ptrdiff_t distanceBetweenIterator(Iterator begin, Iterator end,
		std::random_access_iterator_tag category);

	template <typename Iterator>
	ptrdiff_t distanceBetweenIterator(Iterator begin, Iterator end,
		std::bidirectional_iterator_tag category);

	template <typename Iterator>
	ptrdiff_t distanceBetweenIterator(Iterator begin, Iterator end,
		std::forward_iterator_tag category);

	template <typename Iterator>
	bool containerLess(Iterator lhsBegin, Iterator lhsEnd, Iterator rhsBegin, Iterator rhsEnd);

	template <typename Iterator1, typename Iterator2>
	bool containerEqual(Iterator1 lhsBegin, Iterator1 lhsEnd, Iterator2 rhsBegin, Iterator2 rhsEnd);

	template <typename T, typename T1>
	void insertSingleElement(T *elements, size_t size, size_t index, T1&& elem);

	template <typename T, typename T1>
	void insertSingleElement(T *elements, size_t size, size_t index, T1&& elem, T *target);

	template <typename T, typename Iterator>
	void insertMultipleElement(T *source, size_t size, size_t index,
		Iterator begin, Iterator end, size_t numElementsToInsert, T *target);

	template <typename T, typename Iterator>
	void insertMultipleElement(T *elements, size_t size, size_t index,
		Iterator begin, Iterator end, size_t numElementsToInsert);

	template <typename T>
	void eraseSingleElement(T *elements, size_t size, size_t index);

	// Returns the leftmost position(index) where elem should be inserted 
	// in a sorted array, so Iterator should be a random iterator.
	template <typename T, typename K, typename Comp, typename ExtractKey>
	size_t binarySearch(T const *begin, T const *end, K const &key);

	template <typename T, typename K, typename Comp, typename ExtractKey>
	size_t sequentialSearch(T const *begin, T const *end, K const &key);

	template <typename T, typename K, typename Comp, typename ExtractKey>
	std::pair<size_t, bool>
		getIndexToInsert(T const *begin, T const *end, K const &key, bool toUseBinarySearch);

	template <typename T>
	void moveConstructAndDestruct(T *source, T *target, size_t number);

	template <typename T>
	class Identity {
	public:
		T const & operator()(T const &elem) const { return elem; }
	};

	template <typename T1, typename T2>
	class TakeFirstOfPair {
	public:
		T1 const & operator()(std::pair<T1, T2> const &pair) const { return pair.first; }
	};

	template <typename T, typename ExtractKey>
	class CompareWithKey {
	public:
		bool operator()(T const &lhs, T const  &rhs) const { return ExtractKey()(lhs) < ExtractKey()(rhs); }
	};

	// Interface for generic iterators
	// Note that deferencing is 'const' in this context! 
	template <typename T>
	class _IterI {
	public:
		virtual T & operator*() const = 0;
		virtual T * operator->() const = 0;
	};

	// Interface for generic const iterators
	template <typename T>
	class _ConstIterI {
	public:
		virtual T const & operator*() const = 0;
		virtual T const * operator->() const = 0;
	};

	// Interface for generic forward iterators
	template <typename IterType>
	class _ForIterI {
		virtual IterType & operator= (IterType const &) = 0; // assign
		virtual bool operator== (IterType const &) const = 0;
		virtual bool operator!= (IterType const &) const = 0;
		virtual IterType & operator++() = 0; // prefix
		virtual IterType const operator++(int) = 0; // suffix	
	};

	// Interface for generic bidirectional iterators
	template <typename IterType>
	class _BiIterI : public _ForIterI<IterType> {
		virtual IterType & operator--() = 0; // prefix
		virtual IterType const operator--(int) = 0; // suffix		
	};

	// Second attempt at re-organizing Iterator implementation
	// Impl should be normal Iterator implementation for a given container
	template <typename Impl>
	class ConstReverseIterator;

	template <typename Impl>
	class ReverseIterator;

	template <typename Impl>
	class ConstIterator {
	public:
		typedef typename Impl::value_type value_type;
		typedef value_type const * pointer;
		typedef value_type const & reference;
		typedef ptrdiff_t difference_type;
		typedef typename Impl::iterator_category iterator_category;

		ConstIterator() : impl_() { }
		ConstIterator(Impl impl) : impl_(impl) { }
		ConstIterator(ConstIterator const &rhs) : impl_(rhs.impl_) { }
		ConstIterator & operator=(ConstIterator const &rhs) { impl_ = rhs.impl_; return *this; }

		// Using only function style conversion due to member access limitation 
		// (reverse classes does not share private members...)
		// Note that we cannot decrement impl_ here, as decrementing the first
		// Iterator results in invalid range of Iterator(i.e., we should not have --begin())
		operator ConstReverseIterator<Impl>() const { return ConstReverseIterator<Impl>(impl_); }

		reference operator*() const { return impl_.operator*(); }
		pointer operator->() const { return impl_.operator->(); }

		ConstIterator const operator+(difference_type offset) const { return ConstIterator(impl_ + offset); }
		ConstIterator const operator-(difference_type offset) const { return ConstIterator(impl_ - offset); }

		ConstIterator & operator+=(difference_type offset) { impl_ += offset; return *this; }
		ConstIterator & operator-=(difference_type offset) { impl_ -= offset; return *this; }

		ConstIterator & operator++() { ++impl_; return *this; }
		ConstIterator & operator--() { --impl_; return *this; }
		ConstIterator const operator++(int) { ConstIterator temp = *this; ++(*this); return temp; }
		ConstIterator const operator--(int) { ConstIterator temp = *this; --(*this); return temp; }

		difference_type operator-(ConstIterator const &rhs) const { return impl_ - rhs.impl_; }

		bool operator==(ConstIterator const &rhs) const { return impl_ == rhs.impl_; }
		bool operator!=(ConstIterator const &rhs) const { return impl_ != rhs.impl_; }
		bool operator<(ConstIterator const &rhs) const { return impl_ < rhs.impl_; }
		bool operator<=(ConstIterator const &rhs) const { return impl_ <= rhs.impl_; }
		bool operator>(ConstIterator const &rhs) const { return impl_ > rhs.impl_; }
		bool operator>=(ConstIterator const &rhs) const { return impl_ >= rhs.impl_; }
	public:
		Impl const &getImpl() const { return impl_; }
	protected:
		Impl impl_;
	};

	template <typename Impl>
	class Iterator : public ConstIterator<Impl> {
	public:
		typedef typename Impl::value_type value_type;
		typedef value_type * pointer;
		typedef value_type & reference;
		typedef ptrdiff_t difference_type;
		typedef typename Impl::iterator_category iterator_category;
		using  ConstIterator<Impl>::impl_;

		Iterator() : ConstIterator<Impl>() { }
		Iterator(Impl impl) : ConstIterator<Impl>(impl) { }
		Iterator(Iterator const &rhs) : ConstIterator<Impl>(rhs) { }
		Iterator & operator=(Iterator const &rhs) { ConstIterator<Impl>::operator=(rhs); return *this; }

		operator ReverseIterator<Impl>() const { return ReverseIterator<Impl>(impl_); }
		operator ConstReverseIterator<Impl>() const { return ConstReverseIterator<Impl>(impl_); }

		reference operator*() const { return const_cast<reference>(ConstIterator<Impl>::operator*()); }
		pointer operator->() const { return const_cast<pointer>(ConstIterator<Impl>::operator->()); }

		Iterator const operator+(difference_type offset) const { return Iterator(impl_ + offset); }
		Iterator const operator-(difference_type offset) const { return Iterator(impl_ - offset); }

		Iterator & operator+=(difference_type offset) { impl_ += offset; return *this; }
		Iterator & operator-=(difference_type offset) { impl_ -= offset; return *this; }

		Iterator & operator++() { ConstIterator<Impl>::operator++(); return *this; }
		Iterator & operator--() { ConstIterator<Impl>::operator--(); return *this; }
		Iterator const operator++(int) { Iterator temp = *this; ++(*this); return temp; }
		Iterator const operator--(int) { Iterator temp = *this; --(*this); return temp; }

		difference_type operator-(Iterator const &rhs) const { return impl_ - rhs.impl_; }
	};

	template <typename Impl>
	class ConstReverseIterator {
	public:
		typedef typename std::iterator_traits<Impl>::value_type value_type;
		typedef value_type const * pointer;
		typedef value_type const & reference;
		typedef ptrdiff_t difference_type;
		typedef typename std::iterator_traits<Impl>::iterator_category iterator_category;

		ConstReverseIterator() : impl_() { }
		ConstReverseIterator(Impl impl) : impl_(impl) { }
		ConstReverseIterator(ConstReverseIterator const &rhs) : impl_(rhs.impl_) { }
		ConstReverseIterator & operator=(ConstReverseIterator const &rhs) { impl_ = rhs.impl_; return *this; }

		operator ConstIterator<Impl>() const { return ConstIterator<Impl>(impl_); }

		// Dereferencing is quite expensive for reverse iterators, but we can't avoid it
		// as we cannot have --begin()
		reference operator*() const { Impl copy(impl_); return (--copy).operator*(); }
		pointer operator->() const { Impl copy(impl_); return (--copy).operator->(); }

		ConstReverseIterator const operator+(difference_type offset) const { return ConstReverseIterator(impl_ - offset); }
		ConstReverseIterator const operator-(difference_type offset) const { return ConstReverseIterator(impl_ + offset); }

		ConstReverseIterator & operator+=(difference_type offset) { impl_ -= offset; return *this; }
		ConstReverseIterator & operator-=(difference_type offset) { impl_ += offset; return *this; }

		ConstReverseIterator & operator++() { --impl_; return *this; }
		ConstReverseIterator & operator--() { ++impl_; return *this; }
		ConstReverseIterator const operator++(int) { ConstReverseIterator temp = *this; ++(*this); return temp; }
		ConstReverseIterator const operator--(int) { ConstReverseIterator temp = *this; --(*this); return temp; }

		difference_type operator-(ConstReverseIterator const &rhs) const { return impl_ - rhs.impl_; }

		bool operator==(ConstReverseIterator const &rhs) const { return impl_ == rhs.impl_; }
		bool operator!=(ConstReverseIterator const &rhs) const { return impl_ != rhs.impl_; }
		bool operator<(ConstReverseIterator const &rhs) const { return impl_ > rhs.impl_; }
		bool operator<=(ConstReverseIterator const &rhs) const { return impl_ >= rhs.impl_; }
		bool operator>(ConstReverseIterator const &rhs) const { return impl_ < rhs.impl_; }
		bool operator>=(ConstReverseIterator const &rhs) const { return impl_ <= rhs.impl_; }
	public:
		Impl const &getImpl() const { return impl_; }
	protected:
		Impl impl_;
	};

	template <typename Impl>
	class ReverseIterator : public ConstReverseIterator<Impl> {
	public:
		typedef typename std::iterator_traits<Impl>::value_type value_type;
		typedef value_type * pointer;
		typedef value_type & reference;
		typedef ptrdiff_t difference_type;
		typedef typename std::iterator_traits<Impl>::iterator_category iterator_category;
		using ConstReverseIterator<Impl>::impl_;

		ReverseIterator() : ConstReverseIterator<Impl>() { }
		ReverseIterator(Impl impl) : ConstReverseIterator<Impl>(impl) { }
		ReverseIterator(ReverseIterator const &rhs) : ConstReverseIterator<Impl>(rhs) { }
		ReverseIterator & operator=(ReverseIterator const &rhs) { ConstReverseIterator<Impl>::operator=(rhs); return *this; }

		operator Iterator<Impl>() const { return Iterator<Impl>(impl_); }
		operator ConstIterator<Impl>() const { return ConstIterator<Impl>(impl_); }

		reference operator*() const { return const_cast<reference>(ConstReverseIterator<Impl>::operator*()); }
		pointer operator->() const { return const_cast<pointer>(ConstReverseIterator<Impl>::operator->()); }

		ReverseIterator const operator+(difference_type offset) const { return ReverseIterator(impl_ + offset); }
		ReverseIterator const operator-(difference_type offset) const { return ReverseIterator(impl_ - offset); }

		ReverseIterator & operator+=(difference_type offset) { impl_ -= offset; return *this; }
		ReverseIterator & operator-=(difference_type offset) { impl_ += offset; return *this; }

		ReverseIterator & operator++() { ConstReverseIterator<Impl>::operator++(); return *this; }
		ReverseIterator & operator--() { ConstReverseIterator<Impl>::operator--(); return *this; }
		ReverseIterator const operator++(int) { ReverseIterator temp = *this; ++(*this); return temp; }
		ReverseIterator const operator--(int) { ReverseIterator temp = *this; --(*this); return temp; }
	};

	template <typename T>
	class BTreeTest;
}


// capacity of elements array is assumed
template <typename T, typename T1>
void lib_calvin_container::insertSingleElement(T *elements, size_t size, size_t index, T1&& elem) {
	if (size == index) { // add to last. also takes care when size is zero.
		new (elements + index) T(std::forward<T1>(elem));
		return;
	}
	new (elements + size) T(std::move(elements[size - 1]));
	for (size_t i = size - 1; i > index; i--) { // assuming size is non-zero
		elements[i].~T();
		new (elements + i)  T(std::move(elements[i - 1]));
	}
	elements[index].~T();
	new (elements + index) T(std::forward<T1>(elem));
}

// put result in the target array (source array is treated rvalue)
template <typename T, typename T1>
void lib_calvin_container::insertSingleElement(T *elements, size_t size, size_t index, T1&& elem, T *target) {
	moveConstructAndDestruct(elements, target, index);
	new (target + index) T(std::forward<T1>(elem));
	moveConstructAndDestruct(elements + index, target + index + 1, size - index);
}

template <typename T, typename Iterator>
void lib_calvin_container::insertMultipleElement(T *elements, size_t size, size_t index,
	Iterator begin, Iterator end, size_t numElementsToInsert) {
	// when moving the right part of existing array, we distinguish object construction and assign
	size_t numRightPartElementsToAssign;
	size_t numRightPartElementsToConstruct;
	size_t numInsertingBlockElementsToAssign;
	size_t numInsertingBlockElementsToConstruct;
	if (index + numElementsToInsert < size) {
		numRightPartElementsToAssign = size - (index + numElementsToInsert);
		numRightPartElementsToConstruct = numElementsToInsert;
		numInsertingBlockElementsToAssign = numElementsToInsert;
		numInsertingBlockElementsToConstruct = 0;
	} else { // all right-part elements should be constructed
		numRightPartElementsToAssign = 0;
		numRightPartElementsToConstruct = size - index;
		numInsertingBlockElementsToAssign = size - index;
		numInsertingBlockElementsToConstruct = index + numElementsToInsert - size;
	}
	// construct and assign right part
	size_t operatingIndex = size + numElementsToInsert - 1;
	for (size_t i = 0; i < numRightPartElementsToConstruct; ++i, operatingIndex--) {
		new (elements + operatingIndex) T(std::move(elements[operatingIndex - numElementsToInsert]));
	}
	for (size_t i = 0; i < numRightPartElementsToAssign; ++i, operatingIndex--) {
		elements[operatingIndex] = std::move(elements[operatingIndex - numElementsToInsert]);
	}
	// assign and construct new elements
	operatingIndex = index;
	auto operatingIter = begin;
	for (size_t i = 0; i < numInsertingBlockElementsToAssign; ++i, operatingIndex++, ++operatingIter) {
		elements[operatingIndex] = *operatingIter;
	}
	for (size_t i = 0; i < numInsertingBlockElementsToConstruct; ++i, operatingIndex++, ++operatingIter) {
		new (elements + operatingIndex) T(*operatingIter);
	}
}

template <typename T, typename Iterator>
void lib_calvin_container::insertMultipleElement(T *elements, size_t size, size_t index,
	Iterator begin, Iterator end, size_t numElementsToInsert, T *target) {
	moveConstructAndDestruct(elements, target, index);
	auto iter = begin;
	size_t insertOffset = 0;
	for (; iter != end; ++insertOffset, ++iter) {
		new (target + index + insertOffset) T(*iter);
	}
	moveConstructAndDestruct(elements + index, target + index + numElementsToInsert, size - index);
}

template <typename T>
void lib_calvin_container::eraseSingleElement(T *elements, size_t size, size_t index) {
	for (size_t i = index; i < size - 1; ++i) { // pull from right
		elements[i] = std::move(elements[i + 1]);
	}
	elements[size - 1].~T();
}

template <typename T>
void lib_calvin_container::moveConstructAndDestruct(T *source, T *target, size_t number) {
	for (size_t i = 0; i < number; ++i) {
		new (target + i) T(std::move(source[i]));
		source[i].~T();
	}
}

template <typename T>
void lib_calvin_container::copyConstruct(T const *source, T *target, size_t number) {
	for (size_t i = 0; i < number; ++i) {
		new (target + i) T(source[i]);
	}
}

template <typename T>
void lib_calvin_container::destruct(T *elements, size_t size) {
	for (size_t i = 0; i < size; ++i) {
		elements[i].~T();
	}
}

// This leads to lower_bound position
template <typename T, typename K, typename Comp, typename ExtractKey>
size_t lib_calvin_container::binarySearch(T const *begin, T const *end, K const &key) {	
	T const *left = begin;
	T const *right = end;
	T const *mid = begin + (end - begin) / 2;

	// Invariant: the answer is in range [left, right]
	while (true) {
		if (right == left) {
			return static_cast<size_t>(right - begin);
		}

		if (Comp()(ExtractKey()(*mid), key)) {
			left = mid + 1;
		} else {
			right = mid;
		}
		mid = left + (right - left) / 2;
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
size_t lib_calvin_container::sequentialSearch(T const *begin, T const *end, K const &key) {
	// Should return the same value with binarySearch algorithm above
	T const *temp = begin;
	while (temp < end) {
		if (Comp()(ExtractKey()(*temp), key) == false) {
			return static_cast<size_t>(temp - begin);
		} else {
			++temp;
		}
	}
	return static_cast<size_t>(end - begin);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
std::pair<size_t, bool>
lib_calvin_container::getIndexToInsert(T const *begin, T const *end, K const &key, bool toUseBinarySearch) {
	size_t indexToInsert = 0;
	if (toUseBinarySearch) {
		indexToInsert = binarySearch<T, K, Comp, ExtractKey>(
			begin, end, key);
	} else {
		indexToInsert = sequentialSearch<T, K, Comp, ExtractKey>(
			begin, end, key);
	}
	size_t size = end - begin;
	if (indexToInsert != size && Comp()(key, ExtractKey()(*(begin + indexToInsert))) == false) {
		// already exists
		return std::pair<size_t, bool>(indexToInsert, false);
	} else {
		return std::pair<size_t, bool>(indexToInsert, true);
	}
}

template <typename Iterator1, typename Iterator2>
bool lib_calvin_container::containerEqual(Iterator1 lhsBegin, Iterator1 lhsEnd,
	Iterator2 rhsBegin, Iterator2 rhsEnd) {
	while (true) {
		if (lhsBegin == lhsEnd) {
			if (rhsBegin == rhsEnd) {
				return true;
			} else {
				return false;
			}
		} else {
			if (rhsBegin == rhsEnd) {
				return false;
			} else {
				if (*lhsBegin == *rhsBegin) {
					++lhsBegin;
					++rhsBegin;
				} else {
					return false;
				}
			}
		}
	}
}

template <typename Iterator>
bool lib_calvin_container::containerLess(Iterator lhsBegin, Iterator lhsEnd,
	Iterator rhsBegin, Iterator rhsEnd)
{
	while (true) {
		if (lhsBegin == lhsEnd) {
			if (rhsBegin == rhsEnd) {
				return false;
			} else {
				return true;
			}
		} else {
			if (rhsBegin == rhsEnd) {
				return false;
			} else {
				if (*lhsBegin < *rhsBegin) {
					return true;
				} else if (*rhsBegin < *lhsBegin) {
					return false;
				} else {
					++lhsBegin;
					++rhsBegin;
				}
			}
		}
	}
}


template <typename Iterator>
Iterator lib_calvin_container::advanceIteratorBy(Iterator iterator, ptrdiff_t distance) {
	typename std::iterator_traits<Iterator>::iterator_category category;
	return advanceIteratorBy(iterator, distance, category);
}

template <typename Iterator>
Iterator lib_calvin_container::advanceIteratorBy(Iterator iterator, ptrdiff_t distance,
	std::random_access_iterator_tag category) {
	return iterator + distance;
}

template <typename Iterator>
Iterator lib_calvin_container::advanceIteratorBy(Iterator iterator, ptrdiff_t distance,
	std::bidirectional_iterator_tag category) {
	if (distance > 0) {
		for (ptrdiff_t i = 0; i < distance; ++i) {
			++iterator;
		}
	} else {
		for (ptrdiff_t i = 0; i > distance; i--) {
			--iterator;
		}
	}
	return iterator;
}

template <typename Iterator>
Iterator lib_calvin_container::advanceIteratorBy(Iterator iterator, ptrdiff_t distance,
	std::forward_iterator_tag category) {
	std::cout << "advanceIteratorBy function error\n";
	exit(0);
}

template <typename Iterator>
std::ptrdiff_t lib_calvin_container::distanceBetweenIterator(Iterator begin, Iterator end) {
	typename std::iterator_traits<Iterator>::iterator_category category;
	return distanceBetweenIterator(begin, end, category);
}

template <typename Iterator>
std::ptrdiff_t lib_calvin_container::distanceBetweenIterator(Iterator begin, Iterator end, 
	std::random_access_iterator_tag category) {
	return end - begin;
}

template <typename Iterator>
std::ptrdiff_t lib_calvin_container::distanceBetweenIterator(Iterator begin, Iterator end, 
	std::bidirectional_iterator_tag category) {
	size_t distance = 0;
	while (begin != end) {
		distance++;
	}
	return distance;
}

template <typename Iterator>
std::ptrdiff_t lib_calvin_container::distanceBetweenIterator(Iterator begin, Iterator end, 
	std::forward_iterator_tag category) {
	std::cout << "distanceBetweenIterator function error\n";
	exit(0);
}





#endif
