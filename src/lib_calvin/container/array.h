#ifndef LIB_CALVIN__CONTAINER__ARRAY_H
#define LIB_CALVIN__CONTAINER__ARRAY_H

#include "container.h"
#include <iostream>

namespace lib_calvin_container
{
	// Array starts with space for vector_INIT_SIZE number of elements
	size_t const VECTOR_INIT_CAPACITY = 2;
	// When Array has to reallocate, 
	// it allocates VECTOR_REALLOC_MULTIPLIER times
	// more space than before
	size_t const VECTOR_REALLOC_MULTIPLIER = 2;


	template <typename T>
	class Array {
	public:
		typedef T value_type;
		typedef T * pointer;
		typedef T & reference;
		typedef T const & const_reference;
		typedef ptrdiff_t difference_type;

		class IteratorImpl {
		public:
			typedef T value_type;
			typedef T * pointer;
			typedef T & reference;
			typedef T const & const_reference;
			typedef ptrdiff_t difference_type;
			typedef std::random_access_iterator_tag iterator_category;

			IteratorImpl() : pointer_(NULL) { }
			IteratorImpl(T *pointer) : pointer_(pointer) { }
			IteratorImpl & operator=(IteratorImpl const &rhs) { pointer_ = rhs.pointer_; return *this; }

			pointer operator->() const { return pointer_; }
			reference operator*() const { return *pointer_; }
			IteratorImpl const operator+(ptrdiff_t offset) const {
				return IteratorImpl(pointer_ + offset);
			}
			IteratorImpl const operator-(ptrdiff_t offset) const {
				return IteratorImpl(pointer_ - offset);
			}
			IteratorImpl & operator+=(ptrdiff_t offset) { pointer_ += offset; return *this; }
			IteratorImpl & operator-=(ptrdiff_t offset) { pointer_ -= offset; return *this; }
			IteratorImpl & operator++() { ++pointer_; return *this; }
			IteratorImpl & operator--() { --pointer_; return *this; }
			IteratorImpl const operator++(int) { IteratorImpl temp = *this; ++(*this); return temp; }
			IteratorImpl const operator--(int) { IteratorImpl temp = *this; --(*this); return temp; }
			difference_type operator-(IteratorImpl const &rhs) const { return pointer_ - rhs.pointer_; }

			bool operator==(IteratorImpl const &rhs) const { return pointer_ == rhs.pointer_; }
			bool operator!=(IteratorImpl const &rhs) const { return pointer_ != rhs.pointer_; }
			bool operator<(IteratorImpl const &rhs) const { return pointer_ < rhs.pointer_; }
			bool operator>(IteratorImpl const &rhs) const { return pointer_ > rhs.pointer_; }
			bool operator<=(IteratorImpl const &rhs) const { return pointer_ <= rhs.pointer_; }
			bool operator>=(IteratorImpl const &rhs) const { return pointer_ >= rhs.pointer_; }
		private:
			T * pointer_;
		};

		typedef lib_calvin_container::Iterator<IteratorImpl> iterator;
		typedef lib_calvin_container::ConstIterator<IteratorImpl> const_iterator;
		typedef lib_calvin_container::ReverseIterator<IteratorImpl> reverse_iterator;
		typedef lib_calvin_container::ConstReverseIterator<IteratorImpl> const_reverse_iterator;

		Array();
		Array(Array<T> const &rhs);
		Array(Array<T> &&rhs);
		// starts with initSize objects (using default ctor if second arg is absent)
		Array(size_t initSize, T const &value);
		Array(size_t initSize);
		template <typename InputIter>
			Array(InputIter begin, InputIter end);
		Array(std::initializer_list<T> const &);

		~Array();
		Array<T> & operator= (Array<T> const &rhs);
		Array<T> & operator= (Array<T> &&rhs);
		void swap(Array<T> &rhs);

		reference operator[] (size_t index) { return *(array_ + index); }
		const_reference operator[] (size_t index) const { return *(array_ + index); }

		template <typename T1> void push_back(T1 &&);
		void pop_back();
		reference front() { return *array_; }
		reference back() { return *(array_ + size_ - 1); }
		const_reference front() const { return *array_; }
		const_reference back() const { return *(array_ + size_ - 1); }

		// return the position of the first new elements
		template <typename T1>
		iterator insert(const_iterator const &pos, T1 &&elem);
		template <typename Iterator>
		iterator insert(const_iterator const &pos, Iterator const &begin, Iterator const &end);

		// return the position of the next element to erased one
		iterator erase(const_iterator const &pos);
		iterator erase(const_iterator const &begin, const_iterator const &end);

		size_t size() const { return size_; }
		bool empty() const { return size_ == 0; }
		size_t capacity() const { return capacity_; }

		void reserve(size_t numElem);

		void resize(size_t, T const &value = T());
		void clear();

		const_iterator begin() const { return const_iterator(array_); }
		const_iterator end() const { return const_iterator(array_ + size_); }
		iterator begin() { return iterator(array_); }
		iterator end() { return iterator(array_ + size_); }

		const_reverse_iterator rbegin() const { return end(); }
		const_reverse_iterator rend() const { return begin(); }
		reverse_iterator rbegin() { return end(); }
		reverse_iterator rend() { return begin(); }

	private:
		void init(size_t initSize = 0);
		void reserve_impl(size_t numElem);

	private: // member variables	
		T * array_;
		size_t size_;
		size_t capacity_;

	}; // end Array<T>

	template <typename T>
	bool operator== (Array<T> const &lhs, Array<T> const &rhs) {
		return containerEqual(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <typename T>
	bool operator!= (Array<T> const &lhs, Array<T> const &rhs) {
		return !(lhs == rhs);
	}

	template <typename T>
	bool operator< (Array<T> const &lhs, Array<T> const &rhs) {
		return containerLess(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <typename T>
	bool operator<= (Array<T> const &lhs, Array<T> const &rhs) {
		return !(rhs < lhs);
	}

	template <typename T>
	bool operator> (Array<T> const &lhs, Array<T> const &rhs) {
		return rhs < lhs;
	}

	template <typename T>
	bool operator>= (Array<T> const &lhs, Array<T> const &rhs) {
		return !(lhs < rhs);
	}



	/****************** Array<T> definitions *******************/

	template <typename T>
	Array<T>::Array() {
		init();
	}

	template <typename T>
	Array<T>::Array(Array<T> const &rhs) {
		if (rhs.empty()) {
			init();
			return;
		} else {
			init();
			*this = rhs;
		}
	}

	template <typename T>
	Array<T>::Array(Array<T> &&rhs) {
		//std::cout << "array move ctor\n";
		init();
		swap(rhs);
	}

	template <typename T>
	Array<T>::Array(size_t initSize) {
		init(initSize);
		for (size_t i = 0; i < initSize; ++i) {
			new (array_ + i) T();
		}
	}

	// Be careful with initSize == 0 case
	template <typename T>
	Array<T>::Array(size_t initSize, T const &value) {
		init(initSize);
		for (size_t i = 0; i < initSize; ++i) {
			new (array_ + i) T(value);
		}
	}

	template <typename T>
	template <typename InputIter>
	Array<T>::Array(InputIter begin, InputIter end) {
		// InputIter is an iterator. 
		init();
		for (InputIter iter = begin; iter != end; ++iter) {
			push_back(*iter);
		}
	}

	template <typename T>
	Array<T>::Array(std::initializer_list<T> const &list) {
		init();
		//std::cout << list.size() << "   \n";
		for (auto iter = list.begin(); iter != list.end(); ++iter) {
			push_back(*iter);
		}
	}

	template <typename T>
	Array<T>::~Array() {
		for (size_t i = 0; i < size_; ++i) {
			array_[i].~T();
		}
		operator delete (array_);
	}

	template <typename T>
	Array<T> &
		Array<T>::operator= (Array<T> const &rhs) {
		if (this == &rhs) {
			return *this;
		}
		if (rhs.empty()) {
			clear();
			return *this;
		}
		for (size_t i = 0; i < size_; ++i) {
			array_[i].~T();
		}

		operator delete (array_);
		array_ = (T *)operator new(sizeof(T)*rhs.capacity_);
		size_ = rhs.size_;
		capacity_ = rhs.capacity_;
		copyConstruct(rhs.array_, array_, size_);
		return *this;
	}

	template <typename T>
	Array<T> &
		Array<T>::operator= (Array<T> &&rhs) {
		//std::cout << "array move assign\n";
		if (this == &rhs) {
			return *this;
		}
		swap(rhs);
		return *this;
	}

	template <typename T>
	void Array<T>::swap(Array<T> &rhs) {
		//std::cout << "vector swap\n";

		T *tempArray = array_;
		size_t tempCapacity = capacity_;
		size_t tempSize = size_;
		array_ = rhs.array_;
		capacity_ = rhs.capacity_;
		size_ = rhs.size_;
		rhs.array_ = tempArray;
		rhs.capacity_ = tempCapacity;
		rhs.size_ = tempSize;
	}

	template <typename T>
	template <typename T1>
	void Array<T>::push_back(T1 &&elem) {
		if (size_ == capacity_) { // has to re-allocate
			reserve_impl((size_ + 1)*VECTOR_REALLOC_MULTIPLIER);
		}
		new (array_ + size_) T(std::forward<T1>(elem));
		size_++;
	}

	template <typename T>
	void Array<T>::pop_back() {
		array_[size_ - 1].~T();
		size_--;
		if (empty()) {
			clear();
		}
	}

	template <typename T>
	template <typename T1>
	typename Array<T>::iterator Array<T>::insert(const_iterator const &pos, T1 &&elem) {
		size_t insertingIndex = pos - begin();
		if (size_ == capacity_) {
			size_t newCapacity = (size_ + 1)*VECTOR_REALLOC_MULTIPLIER;
			T *newArray = (T *)operator new(sizeof(T)*newCapacity);
			insertSingleElement(array_, size_, insertingIndex, std::forward<T1>(elem), newArray);
			operator delete(array_);
			array_ = newArray;
			capacity_ = newCapacity;
		} else {
			insertSingleElement(array_, size_, insertingIndex, std::forward<T1>(elem));
		}
		size_++;
		return begin() + insertingIndex;
	}

	template <typename T>
	template <typename Iterator>
	typename Array<T>::iterator
		Array<T>::insert(const_iterator const &pos, Iterator const &first, Iterator const &last) {
		ptrdiff_t insertingIndex = pos - begin();
		size_t numElementsToInsert = distanceBetweenIterator(first, last);
		if (size_ + numElementsToInsert > capacity_) {
			size_t newCapacity = (size_ + numElementsToInsert)*VECTOR_REALLOC_MULTIPLIER;
			T *newArray = (T *)operator new(sizeof(T)*newCapacity);
			insertMultipleElement(array_, size_, insertingIndex, first, last, numElementsToInsert, newArray);
			operator delete(array_);
			array_ = newArray;
			capacity_ = newCapacity;
		} else {
			insertMultipleElement(array_, size_, insertingIndex, first, last, numElementsToInsert);
		}
		size_ += numElementsToInsert;
		return begin() + insertingIndex;
	}

	template <typename T>
	typename Array<T>::iterator
		Array<T>::erase(const_iterator const &pos) {
		size_t index = pos - begin();
		eraseSingleElement(array_, size_, index);
		size_--;
		if (empty()) {
			clear();
		}
		return begin() + index;
	}

	template <typename T>
	void Array<T>::reserve(size_t numElem) {
		reserve_impl(numElem);
	}

	// resize() does NOT free memory
	template <typename T>
	void Array<T>::resize(size_t newSize, T const &value) {
		if (newSize > size_) {
			if (newSize > capacity_) { // need to re-allocate
				reserve_impl(newSize);
			}
			for (size_t i = size_; i < newSize; ++i) { // copy construct
				new (array_ + i) T(value);
			}
		} else if (newSize < size_) {
			for (size_t i = newSize; i < size_; ++i) {
				array_[i].~T();
			}
		} else { // size_ == newSize
		}
		size_ = newSize;
	}

	// clear() frees memory
	template <typename T>
	void Array<T>::clear() {
		for (size_t i = 0; i < size_; ++i) {
			array_[i].~T();
		}
		size_ = 0;
		capacity_ = 0;
		operator delete(array_);
		array_ = nullptr;
	}

	//--------------------------- private methods ------------------------------//
	template <typename T>
	void Array<T>::reserve_impl(size_t newCapacity) {
		if (newCapacity <= capacity_) {
			return;
		}
		capacity_ = newCapacity;
		T *newArray = (T *)operator new(sizeof(T)*newCapacity);
		moveConstructAndDestruct(array_, newArray, size_);
		operator delete(array_);
		array_ = newArray;
	}

	template <typename T>
	void Array<T>::init(size_t initSize) {
		size_ = initSize;
		capacity_ = initSize;
		array_ = nullptr;
		if (initSize > 0) {
			array_ = (T *)operator new(sizeof(T)*initSize);
		}
	}

} // end namespace lib_calvin_container

#endif
