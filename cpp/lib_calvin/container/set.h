#ifndef LIB_CALVIN__CONTAINTER__SET_H
#define LIB_CALVIN__CONTAINTER__SET_H

#include "container.h"
#include "bin_tree.h"
#include "b_tree.h"
#include "b_plus_tree.h"
#include "rb_tree.h"
#include "ordered_array.h"
#include "boost/container/set.hpp"
#include "hash_table.h"
#include <set>

namespace lib_calvin_container
{
template <typename T, typename Comp = std::less<T>,
	typename Impl = lib_calvin_container::BPlusTree<T, T, Comp, std::identity<T>>>
	//typename Impl = std::set<T>>
	//typename Impl = boost::container::set<T>>
class set 
{
public:
	typedef typename Impl::iterator iterator;
	typedef typename Impl::const_iterator const_iterator;
	typedef typename Impl::reverse_iterator reverse_iterator;
	typedef typename Impl::const_reverse_iterator const_reverse_iterator;

	typedef typename Impl::value_type value_type;
	typedef typename Impl::pointer pointer;
	typedef typename Impl::reference reference;

	set(): impl_() { }
	set(set<T, Comp, Impl> const &rhs): impl_(rhs.impl_) { }
	set(set<T, Comp, Impl> &&rhs): impl_(std::move(rhs.impl_)) { }
	set<T, Comp, Impl> & operator= (set const &rhs) { impl_ = rhs.impl_; return *this;}
	set<T, Comp, Impl> & operator= (set &&rhs) { impl_ = std::move(rhs.impl_); return *this;}
	size_t size() const { return impl_.size(); }
	iterator find(T const &key) { return impl_.find(key); }
	const_iterator find(T const &key) const { return impl_.find(key); }
	size_t count(T const &key) const { return impl_.count(key); }
	std::pair<iterator, bool> insert(T const &key) { return impl_.insert(key); }
	std::pair<iterator, bool> insert(T &&key) { return impl_.insert(std::forward<T>(key)); }
	template <typename InputIterator>
	void insert(InputIterator beg, InputIterator end)
		{ impl_.insert(beg, end); } // Insert range [beg, end)
	size_t erase(T const &key) { return impl_.erase(key); }
	void clear() { impl_.clear(); }
	bool empty() const { return impl_.empty(); }

	const_iterator begin() const { return impl_.begin(); }
	const_iterator end() const { return impl_.end(); }
	iterator begin() { return impl_.begin(); }
	iterator end() { return impl_.end(); }

	const_reverse_iterator rbegin() const { return impl_.rbegin(); }
	const_reverse_iterator rend() const { return impl_.rend(); }
	reverse_iterator rbegin() { return impl_.rbegin(); }
	reverse_iterator rend() { return impl_.rend(); }

	friend bool operator==(set const &lhs, set const &rhs)
	{ return lhs.impl_ == rhs.impl_; }
		
	friend bool operator!=(set const &lhs, set const &rhs)
	{ return lhs.impl_ != rhs.impl_; }
		
	friend bool operator< (set const &lhs, set const &rhs)
	{ return lhs.impl_ < rhs.impl_; }
		
	friend bool operator<= (set const &lhs, set const &rhs)
	{ return lhs.impl_ <= rhs.impl_; }
		
	friend bool operator> (set const &lhs, set const &rhs)
	{ return lhs.impl_ > rhs.impl_; }
		
	friend bool operator>= (set const &lhs, set const &rhs)
	{ return lhs.impl_ >= rhs.impl_; }

private:		
	Impl impl_;
};


template <typename T, typename K = T, typename Comp = std::less<K>, typename ExtractKey = std::identity<T>>
class set2 
{
public:
	typedef std::pair<K, T *> PairType;
	typedef BPlusTree<PairType, K, Comp, lib_calvin_container::TakeFirstOfPair<K, T *>> TreeImpl;
	class const_iterator {
	public:
		typedef T value_type;
		typedef T * pointer;
		typedef T & reference;
		typedef std::bidirectional_iterator_tag iterator_category;
	public:
		reference operator*() const;
		pointer operator->() const;
	public:
		const_iterator();
		const_iterator(const_iterator const &rhs);
		const_iterator(typename TreeImpl::const_iterator const &rhs);
		const_iterator & operator=(const_iterator const &rhs);
	public:
		const_iterator & operator++(); // prefix
		const_iterator & operator--();
		const_iterator const operator++(int); // suffix	
		const_iterator const operator--(int);
	public:
		bool operator==(const_iterator const &rhs) const;
		bool operator!=(const_iterator const &rhs) const;		
	private:
		typename TreeImpl::const_iterator impl_;
	};
	class iterator {
	public:
		typedef T value_type;
		typedef T * pointer;
		typedef T & reference;
		typedef std::bidirectional_iterator_tag iterator_category;
	public:
		reference operator*() const;
		pointer operator->() const;
	public:
		iterator();
		iterator(iterator const &rhs);
		iterator(typename TreeImpl::iterator const &rhs);
		iterator & operator=(iterator const &rhs);
	public:
		iterator & operator++(); // prefix
		iterator & operator--();
		iterator const operator++(int); // suffix	
		iterator const operator--(int);
	public:
		bool operator==(iterator const &rhs) const;
		bool operator!=(iterator const &rhs) const;		
	private:
		typename TreeImpl::iterator impl_;
	};
	class const_reverse_iterator {
	public:
		typedef T value_type;
		typedef T * pointer;
		typedef T & reference;
		typedef std::bidirectional_iterator_tag iterator_category;
	public:
		reference operator*() const;
		pointer operator->() const;
	public:
		const_reverse_iterator();
		const_reverse_iterator(const_reverse_iterator const &rhs);
		const_reverse_iterator(typename TreeImpl::const_reverse_iterator const &rhs);
		const_reverse_iterator & operator=(const_reverse_iterator const &rhs);
	public:
		const_reverse_iterator & operator++(); // prefix
		const_reverse_iterator & operator--();
		const_reverse_iterator const operator++(int); // suffix	
		const_reverse_iterator const operator--(int);
	public:
		bool operator==(const_reverse_iterator const &rhs) const;
		bool operator!=(const_reverse_iterator const &rhs) const;		
	private:
		typename TreeImpl::const_reverse_iterator impl_;
	};
	class reverse_iterator {
	public:
		typedef T value_type;
		typedef T * pointer;
		typedef T & reference;
		typedef std::bidirectional_iterator_tag iterator_category;
	public:
		reference operator*() const;
		pointer operator->() const;
	public:
		reverse_iterator();
		reverse_iterator(reverse_iterator const &rhs);
		reverse_iterator(typename TreeImpl::reverse_iterator const &rhs);
		reverse_iterator & operator=(reverse_iterator const &rhs);
	public:
		reverse_iterator & operator++(); // prefix
		reverse_iterator & operator--();
		reverse_iterator const operator++(int); // suffix	
		reverse_iterator const operator--(int);
	public:
		bool operator==(reverse_iterator const &rhs) const;
		bool operator!=(reverse_iterator const &rhs) const;		
	private:
		typename TreeImpl::reverse_iterator impl_;
	};
	typedef typename T value_type;
	typedef typename T * pointer;
	typedef typename T & reference;


	set2();
	set2(set2<T, K, Comp, ExtractKey> const &rhs);
	set2(set2<T, K, Comp, ExtractKey> &&rhs);
	~set2();
	set2<T, K, Comp, ExtractKey> & operator= (set2 const &rhs);
	//set2<T, K, Comp, ExtractKey> & operator= (set2 &&rhs);
	size_t size() const;
	iterator find(K const &key);
	const_iterator find(K const &key) const;
	size_t count(K const &key) const;
	std::pair<iterator, bool> insert(T const &elem);
	template <typename InputIterator>
	void insert(InputIterator beg, InputIterator end);
	size_t erase(K const &key);
	void clear();
	bool empty() const;
public:
	const_iterator begin() const { return impl_.begin(); }
	const_iterator end() const { return impl_.end(); }
	iterator begin() { return impl_.begin(); }
	iterator end() { return impl_.end(); }

	const_reverse_iterator rbegin() const { return impl_.rbegin(); }
	const_reverse_iterator rend() const { return impl_.rend(); }
	reverse_iterator rbegin() { return impl_.rbegin(); }
	reverse_iterator rend() { return impl_.rend(); }

	friend bool operator==(set2 const &lhs, set2 const &rhs)
	{ return containerEqual(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }
		
	friend bool operator!=(set2 const &lhs, set2 const &rhs)
	{ return !(lhs == rhs); }
		
	friend bool operator< (set2 const &lhs, set2 const &rhs)
	{ return containerLess(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }
		
	friend bool operator<= (set2 const &lhs, set2 const &rhs)
	{ return !(rhs < lhs); }
		
	friend bool operator> (set2 const &lhs, set2 const &rhs)
	{ return containerEqual(rhs.begin(), rhs.end(), lhs.begin(), lhs.end()); }
		
	friend bool operator>= (set2 const &lhs, set2 const &rhs)
	{ return !(lhs < rhs); }
private:
	void copyObjectsFrom(set2 const &rhs);
	void deleteObjects();
private:		
	TreeImpl impl_;
}; // end set2


template <typename T, typename K, typename Comp, typename ExtractKey >
set2<T, K, Comp, ExtractKey>::set2(): impl_() { 

}

template <typename T, typename K, typename Comp, typename ExtractKey >
set2<T, K, Comp, ExtractKey>::set2(set2<T, K, Comp, ExtractKey> const &rhs): impl_(rhs.impl_) { 
	copyObjectsFrom(rhs);
}

template <typename T, typename K, typename Comp, typename ExtractKey >
set2<T, K, Comp, ExtractKey>::set2(set2<T, K, Comp, ExtractKey> &&rhs): impl_() { 
	impl_.swap(rhs.impl_);
}

template <typename T, typename K, typename Comp, typename ExtractKey >
set2<T, K, Comp, ExtractKey>::~set2() {
	deleteObjects();
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey> &
set2<T, K, Comp, ExtractKey>::operator=(set2 const &rhs) {
	if (this != &rhs) {
		deleteObjects();
		impl_ = rhs.impl_;
		copyObjectsFrom(rhs);
	}
	return *this;
}

/*
template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey> &
set2<T, K, Comp, ExtractKey>::operator=(set2 &&rhs) {
	if (this != &rhs) {
		impl_.swap(rhs.impl_);
	}
	return *this;
}
*/

template <typename T, typename K, typename Comp, typename ExtractKey >
size_t
set2<T, K, Comp, ExtractKey>::size() const {
	return impl_.size();
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::iterator
set2<T, K, Comp, ExtractKey>::find(K const &key) {
	return iterator(impl_.find(key));
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::const_iterator
set2<T, K, Comp, ExtractKey>::find(K const &key) const {
	return const_iterator(impl_.find(key));
}

template <typename T, typename K, typename Comp, typename ExtractKey >
size_t
set2<T, K, Comp, ExtractKey>::count(K const &key) const {
	return impl_.count(key);
}

template <typename T, typename K, typename Comp, typename ExtractKey >
std::pair<typename set2<T, K, Comp, ExtractKey>::iterator, bool>
set2<T, K, Comp, ExtractKey>::insert(T const &elem) {
	auto result = impl_.insert(std::make_pair(ExtractKey()(elem), nullptr));
	if (result.second) {
		result.first->second = new T(elem);
	}
	return std::make_pair(iterator(result.first), result.second);
}

template <typename T, typename K, typename Comp, typename ExtractKey >
template <typename InputIterator>
void set2<T, K, Comp, ExtractKey>::insert(InputIterator beg, InputIterator end) { 
	// Insert range [beg, end)
	for (InputIterator iter = beg; iter != end; ++iter) {
		insert(*iter);
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey >
size_t
set2<T, K, Comp, ExtractKey>::erase(K const &key) { 
	auto result = impl_.find(key); 
	if (result != impl_.end()) {
		delete result->second;
		return impl_.erase(key);
	} else {
		return 0;
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey >
void
set2<T, K, Comp, ExtractKey>::clear() { 
	deleteObjects();
	impl_.clear(); 
}

template <typename T, typename K, typename Comp, typename ExtractKey >
bool
set2<T, K, Comp, ExtractKey>::empty() const { 
	return impl_.empty(); 
}

template <typename T, typename K, typename Comp, typename ExtractKey >
void
set2<T, K, Comp, ExtractKey>::copyObjectsFrom(set2 const &rhs) {
	auto sourceIter = rhs.impl_.begin();
	auto targetIter = impl_.begin();
	for ( ; sourceIter != rhs.impl_.end(); ++sourceIter, ++targetIter) {
		targetIter->second = new T(*sourceIter->second);
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey >
void
set2<T, K, Comp, ExtractKey>::deleteObjects() {
	for (auto iter = impl_.begin() ; iter != impl_.end(); ++iter) {
		delete iter->second;
	}
}


} // end namespace lib_calvin_container

// set2 iterator
namespace lib_calvin_container
{
template <typename T, typename K, typename Comp, typename ExtractKey >
set2<T, K, Comp, ExtractKey>::const_iterator::const_iterator(): impl_() { }

template <typename T, typename K, typename Comp, typename ExtractKey >
set2<T, K, Comp, ExtractKey>::const_iterator::const_iterator(const_iterator const &rhs): impl_(rhs.impl_) { }

template <typename T, typename K, typename Comp, typename ExtractKey >
set2<T, K, Comp, ExtractKey>::const_iterator::const_iterator(typename TreeImpl::const_iterator const &impl): impl_(impl) { }

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::const_iterator &
set2<T, K, Comp, ExtractKey>::const_iterator::operator=(const_iterator const &rhs) { 
	impl_ = rhs.impl_;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::const_iterator::reference
set2<T, K, Comp, ExtractKey>::const_iterator::operator*() const {
	PairType pair = impl_.operator*();
	return *pair.second;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::const_iterator::pointer
set2<T, K, Comp, ExtractKey>::const_iterator::operator->() const {
	PairType pair = impl_.operator*();
	return pair.second;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::const_iterator &
set2<T, K, Comp, ExtractKey>::const_iterator::operator++() {
	impl_.operator++();
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::const_iterator &
set2<T, K, Comp, ExtractKey>::const_iterator::operator--() {
	impl_.operator--();
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::const_iterator const
set2<T, K, Comp, ExtractKey>::const_iterator::operator++(int) {
	auto copy = *this;
	operator++();
	return copy;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::const_iterator const
set2<T, K, Comp, ExtractKey>::const_iterator::operator--(int) {
	auto copy = *this;
	operator--();
	return copy;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
bool
set2<T, K, Comp, ExtractKey>::const_iterator::operator==(const_iterator const &rhs) const {
	return impl_ == rhs.impl_;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
bool
set2<T, K, Comp, ExtractKey>::const_iterator::operator!=(const_iterator const &rhs) const {
	return impl_ != rhs.impl_;
}
	
template <typename T, typename K, typename Comp, typename ExtractKey >
set2<T, K, Comp, ExtractKey>::iterator::iterator(): impl_() { }

template <typename T, typename K, typename Comp, typename ExtractKey >
set2<T, K, Comp, ExtractKey>::iterator::iterator(iterator const &rhs): impl_(rhs.impl_) { }

template <typename T, typename K, typename Comp, typename ExtractKey >
set2<T, K, Comp, ExtractKey>::iterator::iterator(typename TreeImpl::iterator const &impl): impl_(impl) { }

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::iterator &
set2<T, K, Comp, ExtractKey>::iterator::operator=(iterator const &rhs) { 
	impl_ = rhs.impl_;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::iterator::reference
set2<T, K, Comp, ExtractKey>::iterator::operator*() const {
	PairType pair = impl_.operator*();
	return *pair.second;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::iterator::pointer
set2<T, K, Comp, ExtractKey>::iterator::operator->() const {
	PairType pair = impl_.operator*();
	return pair.second;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::iterator &
set2<T, K, Comp, ExtractKey>::iterator::operator++() {
	impl_.operator++();
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::iterator &
set2<T, K, Comp, ExtractKey>::iterator::operator--() {
	impl_.operator--();
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::iterator const
set2<T, K, Comp, ExtractKey>::iterator::operator++(int) {
	impl_.operator++(int);
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::iterator const
set2<T, K, Comp, ExtractKey>::iterator::operator--(int) {
	impl_.operator--(int);
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
bool
set2<T, K, Comp, ExtractKey>::iterator::operator==(iterator const &rhs) const {
	return impl_ == rhs.impl_;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
bool
set2<T, K, Comp, ExtractKey>::iterator::operator!=(iterator const &rhs) const {
	return impl_.operator!=(rhs.impl_);
}

template <typename T, typename K, typename Comp, typename ExtractKey >
set2<T, K, Comp, ExtractKey>::const_reverse_iterator::const_reverse_iterator(): impl_() { }

template <typename T, typename K, typename Comp, typename ExtractKey >
set2<T, K, Comp, ExtractKey>::const_reverse_iterator::const_reverse_iterator(const_reverse_iterator const &rhs): impl_(rhs.impl_) { }

template <typename T, typename K, typename Comp, typename ExtractKey >
set2<T, K, Comp, ExtractKey>::const_reverse_iterator::const_reverse_iterator(typename TreeImpl::const_reverse_iterator const &impl): impl_(impl) { }

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::const_reverse_iterator &
set2<T, K, Comp, ExtractKey>::const_reverse_iterator::operator=(const_reverse_iterator const &rhs) { 
	impl_ = rhs.impl_;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::const_reverse_iterator::reference
set2<T, K, Comp, ExtractKey>::const_reverse_iterator::operator*() const {
	PairType pair = impl_.operator*();
	return *pair.second;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::const_reverse_iterator::pointer
set2<T, K, Comp, ExtractKey>::const_reverse_iterator::operator->() const {
	PairType pair = impl_.operator*();
	return pair.second;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::const_reverse_iterator &
set2<T, K, Comp, ExtractKey>::const_reverse_iterator::operator++() {
	impl_.operator++();
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::const_reverse_iterator &
set2<T, K, Comp, ExtractKey>::const_reverse_iterator::operator--() {
	impl_.operator--();
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::const_reverse_iterator const
set2<T, K, Comp, ExtractKey>::const_reverse_iterator::operator++(int) {
	impl_.operator++(int);
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::const_reverse_iterator const
set2<T, K, Comp, ExtractKey>::const_reverse_iterator::operator--(int) {
	impl_.operator--(int);
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
bool
set2<T, K, Comp, ExtractKey>::const_reverse_iterator::operator==(const_reverse_iterator const &rhs) const {
	return impl_ == rhs.impl_;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
bool
set2<T, K, Comp, ExtractKey>::const_reverse_iterator::operator!=(const_reverse_iterator const &rhs) const {
	return impl_ != rhs.impl_;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
set2<T, K, Comp, ExtractKey>::reverse_iterator::reverse_iterator(): impl_() { }

template <typename T, typename K, typename Comp, typename ExtractKey >
set2<T, K, Comp, ExtractKey>::reverse_iterator::reverse_iterator(reverse_iterator const &rhs): impl_(rhs.impl_) { }

template <typename T, typename K, typename Comp, typename ExtractKey >
set2<T, K, Comp, ExtractKey>::reverse_iterator::reverse_iterator(typename TreeImpl::reverse_iterator const &impl): impl_(impl) { }

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::reverse_iterator &
set2<T, K, Comp, ExtractKey>::reverse_iterator::operator=(reverse_iterator const &rhs) { 
	impl_ = rhs.impl_;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::reverse_iterator::reference
set2<T, K, Comp, ExtractKey>::reverse_iterator::operator*() const {
	PairType pair = impl_.operator*();
	return *pair.second;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::reverse_iterator::pointer
set2<T, K, Comp, ExtractKey>::reverse_iterator::operator->() const {
	PairType pair = impl_.operator*();
	return pair.second;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::reverse_iterator &
set2<T, K, Comp, ExtractKey>::reverse_iterator::operator++() {
	impl_.operator++();
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::reverse_iterator &
set2<T, K, Comp, ExtractKey>::reverse_iterator::operator--() {
	impl_.operator--();
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::reverse_iterator const
set2<T, K, Comp, ExtractKey>::reverse_iterator::operator++(int) {
	impl_.operator++(int);
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
typename set2<T, K, Comp, ExtractKey>::reverse_iterator const
set2<T, K, Comp, ExtractKey>::reverse_iterator::operator--(int) {
	impl_.operator--(int);
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
bool
set2<T, K, Comp, ExtractKey>::reverse_iterator::operator==(reverse_iterator const &rhs) const {
	return impl_ == rhs.impl_;
}

template <typename T, typename K, typename Comp, typename ExtractKey >
bool
set2<T, K, Comp, ExtractKey>::reverse_iterator::operator!=(reverse_iterator const &rhs) const {
	return impl_ != rhs.impl_;
}
}

namespace lib_calvin
{
	template <typename T>
	class set: public lib_calvin_container::set<T> { };

	template <typename T, typename HashFunc>
	class hash_set: 
		public lib_calvin_container::set<T, std::less<T>, 
			lib_calvin_container::HashTable<T, T, std::identity<T>, HashFunc>> { };

} // end namespace lib_calvin
#endif