#ifndef LIB_CALVIN__CONTAINER__B_PTR_TREE_H
#define LIB_CALVIN__CONTAINER__B_PTR_TREE_H

#include "utility.h"
#include "container.h"

namespace lib_calvin_container
{
// function object
template <typename T, typename K, typename ExtractKey>
class PtrExtractKey {
public:
	K operator()(T *const &ptr) const { return ExtractKey()(*ptr); }
};

template <typename T, typename K = T, typename Comp = std::less<K>, typename ExtractKey = Identity<T>,
	typename SetImpl = BTree<T *, Comp, K, PtrExtractKey<T, K, ExtractKey>>>
class PtrSet 
{
public:
	typedef T value_type;
	typedef T * pointer;
	typedef T & reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	class IteratorImpl
  {
	public:
		typedef T value_type;
		typedef T const * pointer;
		typedef T const & reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef std::bidirectional_iterator_tag iterator_category;
		reference operator*() const { return **iter_; }
		pointer operator->() const { return *iter_; }
		IteratorImpl(): iter_() { }
		IteratorImpl(typename SetImpl::const_iterator const &iter): iter_(iter) { }
		// copy ctor	
		IteratorImpl(IteratorImpl const &rhs): iter_(rhs.iter_) { }
		IteratorImpl & operator=(IteratorImpl const &rhs) { iter_ = rhs.iter_;  return *this; }	
		IteratorImpl & operator++() { ++iter_; return *this; } // prefix
		IteratorImpl & operator--() { --iter_; return *this; }
		IteratorImpl const operator++(int) { return IteratorImpl(iter_++); } // suffix			
		IteratorImpl const operator--(int) { return IteratorImpl(iter_--); } 
		bool operator==(IteratorImpl const &rhs) { return iter_ == rhs.iter_; }
		bool operator!=(IteratorImpl const &rhs) { return iter_ != rhs.iter_; }
	private:
		typename SetImpl::const_iterator iter_;
	};
	
	typedef lib_calvin_container::Iterator<IteratorImpl> iterator;
	typedef lib_calvin_container::ConstIterator<IteratorImpl> const_iterator;
	typedef lib_calvin_container::ReverseIterator<IteratorImpl> reverse_iterator;
	typedef lib_calvin_container::ConstReverseIterator<IteratorImpl> const_reverse_iterator;

public:
	PtrSet<T, K, Comp, ExtractKey, SetImpl>() { }
	// copy constructor
	PtrSet<T, K, Comp, ExtractKey, SetImpl>(
		PtrSet<T, K, Comp, ExtractKey, SetImpl> const &rhs); 
	PtrSet<T, K, Comp, ExtractKey, SetImpl> & operator= (
		PtrSet<T, K, Comp, ExtractKey, SetImpl> const &rhs); // assignment
	virtual ~PtrSet<T, K, Comp, ExtractKey, SetImpl>() { clear(); }
public:
	size_t size() const { return set_.size(); }
	const_iterator find(K const &key) const { return const_iterator(set_.find(key)); }
	iterator find(K const &key) { return iterator(set_.find(key)); }
	size_t count(K const &key) const { return set_.count(key); }
	std::pair<iterator, bool> insert(T const &);
	// Insert range [beg, end)
	template <typename InputIterator>
		void insert(InputIterator beg, InputIterator end) { 
			while(beg != end) { insert(*beg++); } }
	size_t erase(K const &key);
	void clear();
	bool empty() const { return set_.empty(); }
public:
	const_iterator begin() const { return const_iterator(set_.begin()); }
	const_iterator end() const { return const_iterator(set_.end()); }
	iterator begin() { return iterator(set_.begin()); }
	iterator end() { return iterator(set_.end()); }

	const_reverse_iterator rbegin() const { 
		return const_reverse_iterator(set_.end()); }
	const_reverse_iterator rend() const { 
		return const_reverse_iterator(set_.begin()); }
	reverse_iterator rbegin() { 
		return reverse_iterator(set_.end()); }
	reverse_iterator rend() { 
		return reverse_iterator(set_.begin()); }
public:

	void print() const {  }
protected:
	SetImpl set_;
};

//-------------------------- PtrSet implementation ----------------------//

template <typename T, typename K, typename Comp, typename ExtractKey, 
	typename SetImpl>
PtrSet<T, K, Comp, ExtractKey, SetImpl>::PtrSet(PtrSet const &rhs): set_(rhs.set_) {
	typename SetImpl::iterator lhsIter = set_.begin();
	typename SetImpl::const_iterator rhsIter = rhs.set_.begin();
	for (; rhsIter != rhs.set_.end(); ++lhsIter, ++rhsIter) {
		*lhsIter = new T(**rhsIter);
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey, 
	typename SetImpl>
PtrSet<T, K, Comp, ExtractKey, SetImpl> &
PtrSet<T, K, Comp, ExtractKey, SetImpl>::operator=(PtrSet const &rhs) {
	if (this == &rhs) {
		return *this;
	}
	clear();
	set_ = rhs.set_;
	typename SetImpl::iterator lhsIter = set_.begin();
	typename SetImpl::const_iterator rhsIter = rhs.set_.begin();
	for (; rhsIter != rhs.set_.end(); ++lhsIter, ++rhsIter) {
		*lhsIter = new T(**rhsIter);
	}
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey, 
	typename SetImpl>
std::pair<typename PtrSet<T, K, Comp, ExtractKey, SetImpl>::iterator, bool>
PtrSet<T, K, Comp, ExtractKey, SetImpl>::insert(T const &elem) {
	std::pair<typename SetImpl::iterator, bool> result = 
		set_.insert(&const_cast<T &>(elem));
	if (result.second == true) { // inserted
		*result.first = new T(elem);
	}
	return std::pair<iterator, bool>(iterator(result.first), result.second);
}

template <typename T, typename K, typename Comp, typename ExtractKey, 
	typename SetImpl>
size_t
PtrSet<T, K, Comp, ExtractKey, SetImpl>::erase(K const &key) {
	iterator result = find(key);
	if (result != end()) { // key exists
		T *ptr = &(*result);
		set_.erase(key);
		delete ptr;		
		return 1;
	} else { 
		return 0;
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey, 
	typename SetImpl>
void 
PtrSet<T, K, Comp, ExtractKey, SetImpl>::clear() {
	vector<T *> ptrToDelete;
	for (iterator iter = begin(); iter != end(); iter++) {
		ptrToDelete.push_back(&(*iter));	
	}
	for (typename vector<T *>::iterator iter = ptrToDelete.begin(); iter != ptrToDelete.end();
			++iter) {
		delete *iter;
	}
	set_.clear();
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator==(PtrSet<T, K, Comp, ExtractKey> const &lhs, 
		PtrSet<T, K, Comp, ExtractKey> const &rhs) {
	return containerEqual(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator!=(PtrSet<T, K, Comp, ExtractKey> const &lhs, 
		PtrSet<T, K, Comp, ExtractKey> const &rhs) {
	return !(lhs == rhs);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator< (PtrSet<T, K, Comp, ExtractKey> const &lhs, 
		PtrSet<T, K, Comp, ExtractKey> const &rhs) {
	return containerLess(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator<= (PtrSet<T, K, Comp, ExtractKey> const &lhs, 
	PtrSet<T, K, Comp, ExtractKey> const &rhs) {
	return !(lhs > rhs);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator> (PtrSet<T, K, Comp, ExtractKey> const &lhs, 
	PtrSet<T, K, Comp, ExtractKey> const &rhs) {
	return rhs < lhs;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator>= (PtrSet<T, K, Comp, ExtractKey> const &lhs, 
	PtrSet<T, K, Comp, ExtractKey> const &rhs) {
	return !(lhs < rhs);
}

} // end namespace lib_calvin_container

#endif