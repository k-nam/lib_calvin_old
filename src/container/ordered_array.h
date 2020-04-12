#ifndef LIB_CALVIN__CONTAINER__ORDERED_ARRAY
#define LIB_CALVIN__CONTAINER__ORDERED_ARRAY

#include "utility.h"
#include "container.h"
#include "array.h"

namespace lib_calvin_container
{
template <typename T, typename K = T, typename Comp = std::less<K>, typename ExtractKey = Identity<T>,
	typename VectorImpl = lib_calvin_container::Array<T>>
class OrderedArray {
public:
	typedef T value_type;
	typedef T * pointer;
	typedef T & reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef typename VectorImpl::iterator iterator;
	typedef typename VectorImpl::const_iterator const_iterator;
	typedef typename VectorImpl::reverse_iterator reverse_iterator;
	typedef typename VectorImpl::const_reverse_iterator const_reverse_iterator;

	OrderedArray<T, K, Comp, ExtractKey, VectorImpl>();
	OrderedArray<T, K, Comp, ExtractKey, VectorImpl>(OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &);
	OrderedArray<T, K, Comp, ExtractKey, VectorImpl>(OrderedArray<T, K, Comp, ExtractKey, VectorImpl> &&);
	OrderedArray<T, K, Comp, ExtractKey, VectorImpl> & operator= (
		OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &);
	OrderedArray<T, K, Comp, ExtractKey, VectorImpl> & operator= (
		OrderedArray<T, K, Comp, ExtractKey, VectorImpl> &&);
	void swap(OrderedArray<T, K, Comp, ExtractKey, VectorImpl> & rhs);
	~OrderedArray<T, K, Comp, ExtractKey, VectorImpl>();

	size_t size() const;
	const_iterator find(K const &) const;
	iterator find(K const &);
	size_t count(K const &) const;
	template <typename T1> std::pair<iterator, bool> insert(T1 &&);
	template <typename InputIterator> void insert(InputIterator beg, InputIterator end);
	size_t erase(K const &);
	void clear();
	bool empty() const;

	const_iterator begin() const { return vectorImpl_.begin(); }
	const_iterator end() const { return vectorImpl_.end(); }
	iterator begin() { return vectorImpl_.begin(); }
	iterator end() { return vectorImpl_.end(); }

	const_reverse_iterator rbegin() const { return vectorImpl_.rbegin(); }
	const_reverse_iterator rend() const { return vectorImpl_.rend(); }
	reverse_iterator rbegin() { return vectorImpl_.rbegin(); }
	reverse_iterator rend() { return vectorImpl_.rend(); }

private:
	T *getElementArray() const;
	T *findPointer(K const &key) const;
private:
	VectorImpl vectorImpl_;
};

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
bool operator==(OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &lhs,
	OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &rhs);

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
bool operator!=(OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &lhs,
	OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &rhs);

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
bool operator< (OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &lhs,
	OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &rhs);

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
bool operator<= (OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &lhs,
	OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &rhs);

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
bool operator> (OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &lhs,
	OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &rhs);

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
bool operator>= (OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &lhs,
	OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &rhs);

} // lib_calvin_container

namespace lib_calvin_container
{
template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
OrderedArray<T, K, Comp, ExtractKey, VectorImpl>::OrderedArray(): vectorImpl_() { }

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
OrderedArray<T, K, Comp, ExtractKey, VectorImpl>::OrderedArray(OrderedArray const &rhs):
	vectorImpl_(rhs.vectorImpl_) { }

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
OrderedArray<T, K, Comp, ExtractKey, VectorImpl>::OrderedArray(OrderedArray &&rhs):
	vectorImpl_(std::forward<VectorImpl>(rhs.vectorImpl_)) { }

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
OrderedArray<T, K, Comp, ExtractKey, VectorImpl> &
OrderedArray<T, K, Comp, ExtractKey, VectorImpl>::operator=(
	OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &rhs) {
		vectorImpl_ = rhs.vectorImpl_;
		return *this;
}
template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
OrderedArray<T, K, Comp, ExtractKey, VectorImpl> &
OrderedArray<T, K, Comp, ExtractKey, VectorImpl>::operator= (
	OrderedArray<T, K, Comp, ExtractKey, VectorImpl> &&rhs) {
		vectorImpl_ = std::forward<VectorImpl>(rhs.vectorImpl_);
		return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
void
OrderedArray<T, K, Comp, ExtractKey, VectorImpl>::swap(OrderedArray &rhs) {
	vectorImpl_.swap(rhs.vectorImpl_);
}

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
OrderedArray<T, K, Comp, ExtractKey, VectorImpl>::~OrderedArray() { }

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
size_t
OrderedArray<T, K, Comp, ExtractKey, VectorImpl>::size() const {
	return vectorImpl_.size();
}

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
typename OrderedArray<T, K, Comp, ExtractKey, VectorImpl>::const_iterator
OrderedArray<T, K, Comp, ExtractKey, VectorImpl>::find(K const &key) const {
	return const_iterator(findPointer(key));
}

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
typename OrderedArray<T, K, Comp, ExtractKey, VectorImpl>::iterator
OrderedArray<T, K, Comp, ExtractKey, VectorImpl>::find(K const &key) {
	return iterator(findPointer(key));
}

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
size_t
OrderedArray<T, K, Comp, ExtractKey, VectorImpl>::count(K const &key) const {
	if (find(key) == end()) { // does not exist
		return 0;
	} else {
		return 1;
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
template <typename T1>
std::pair<typename OrderedArray<T, K, Comp, ExtractKey, VectorImpl>::iterator, bool>
OrderedArray<T, K, Comp, ExtractKey, VectorImpl>::insert(T1 &&elem) {
	T *elements = getElementArray();
	auto result = lib_calvin_container::getIndexToInsert<T, K, Comp, ExtractKey>(
		elements, elements + size(), ExtractKey()(elem), true);
	if (result.second == true) { // does not exist
		vectorImpl_.insert(vectorImpl_.begin() + result.first, elem);
		return std::pair<iterator, bool>(iterator(getElementArray() + result.first), true);
	} else { // exists
		return std::pair<iterator, bool>(iterator(elements + result.first), false);
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
template <typename Iterator>
void
OrderedArray<T, K, Comp, ExtractKey, VectorImpl>::insert(Iterator begin, Iterator end) {
	for ( ; begin != end; ++begin) {
		insert(*begin);
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
size_t
OrderedArray<T, K, Comp, ExtractKey, VectorImpl>::erase(K const &key) {
	T *elements = getElementArray();
	auto result = lib_calvin_container::getIndexToInsert<T, K, Comp, ExtractKey>(
		elements, elements + size(), key, true);
	if (result.second == true) { // does not exist
		return 0;
	} else {
		vectorImpl_.erase(vectorImpl_.begin() + result.first);
		return 1;
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
void
OrderedArray<T, K, Comp, ExtractKey, VectorImpl>::clear() {
	vectorImpl_.clear();
}

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
bool
OrderedArray<T, K, Comp, ExtractKey, VectorImpl>::empty() const {
	return vectorImpl_.empty();
}

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
T *
OrderedArray<T, K, Comp, ExtractKey, VectorImpl>::getElementArray() const {
	return const_cast<T *>(&(*vectorImpl_.begin()));
}

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
T *
OrderedArray<T, K, Comp, ExtractKey, VectorImpl>::findPointer(K const &key) const {
	T *elements = getElementArray();
	auto result = lib_calvin_container::getIndexToInsert<T, K, Comp, ExtractKey>(
		elements, elements + size(), key, true);
	if (result.second == true) { // does not exist
		return elements + size();
	} else {
		return elements + result.first;
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
bool operator==(OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &lhs,
		OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &rhs) {
	return containerEqual(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
bool operator!=(OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &lhs,
		OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &rhs) {
	return !(lhs == rhs);
}

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
bool operator< (OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &lhs,
		OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &rhs) {
	return containerLess(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
bool operator<= (OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &lhs,
		OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &rhs) {
	return !(lhs > rhs);
}

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
bool operator> (OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &lhs,
		OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &rhs) {
	return rhs < lhs;
}

template <typename T, typename K, typename Comp, typename ExtractKey, typename VectorImpl>
bool operator>= (OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &lhs,
		OrderedArray<T, K, Comp, ExtractKey, VectorImpl> const &rhs) {
	return !(lhs < rhs);
}

} // close definition; lib_calvin_container

#endif
