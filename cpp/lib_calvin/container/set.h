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
	typedef BPlusTree<PairType, PairType, Comp, lib_calvin_container::TakeFirstOfPair<K, T *>> Impl;
	class IteratorImpl {
	public:
		typedef T value_type;
		typedef T const * pointer;
		typedef T const & reference;
		typedef std::bidirectional_iterator_tag iterator_category;
	public:
		reference operator*() const;
		pointer operator->() const;
	public:
		IteratorImpl();
		IteratorImpl(IteratorImpl const &rhs);
		IteratorImpl & operator=(IteratorImpl const &rhs);
	public:
		IteratorImpl & operator++(); // prefix
		IteratorImpl & operator--();
		IteratorImpl const operator++(int); // suffix	
		IteratorImpl const operator--(int);
	public:
		bool operator==(IteratorImpl const &rhs) const;
		bool operator!=(IteratorImpl const &rhs) const;		
	private:
		typename Impl::IteratorImpl impl_;
	};

	typedef typename T value_type;
	typedef typename T * pointer;
	typedef typename T & reference;

	typedef lib_calvin_container::Iterator<IteratorImpl> iterator;
	typedef lib_calvin_container::ConstIterator<IteratorImpl> const_iterator;
	typedef lib_calvin_container::ReverseIterator<IteratorImpl> reverse_iterator;
	typedef lib_calvin_container::ConstReverseIterator<IteratorImpl> const_reverse_iterator;

	set2(): impl_() { }
	set2(set2<T, Comp, Impl> const &rhs): impl_(rhs.impl_) { }
	set2(set2<T, Comp, Impl> &&rhs): impl_(std::move(rhs.impl_)) { }
	set2<T, Comp, Impl> & operator= (set2 const &rhs) { impl_ = rhs.impl_; return *this;}
	set2<T, Comp, Impl> & operator= (set2 &&rhs) { impl_ = std::move(rhs.impl_); return *this;}
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

	friend bool operator==(set2 const &lhs, set2 const &rhs)
	{ return lhs.impl_ == rhs.impl_; }
		
	friend bool operator!=(set2 const &lhs, set2 const &rhs)
	{ return lhs.impl_ != rhs.impl_; }
		
	friend bool operator< (set2 const &lhs, set2 const &rhs)
	{ return lhs.impl_ < rhs.impl_; }
		
	friend bool operator<= (set2 const &lhs, set2 const &rhs)
	{ return lhs.impl_ <= rhs.impl_; }
		
	friend bool operator> (set2 const &lhs, set2 const &rhs)
	{ return lhs.impl_ > rhs.impl_; }
		
	friend bool operator>= (set2 const &lhs, set2 const &rhs)
	{ return lhs.impl_ >= rhs.impl_; }

private:		
	Impl impl_;
}; // end set2

template <typename T, typename K = T, typename Comp = std::less<K>, typename ExtractKey = std::identity<T>>
set2<T, K, Comp, ExtractKey>::IteratorImpl::IteratorImpl(): impl_() { }

template <typename T, typename K = T, typename Comp = std::less<K>, typename ExtractKey = std::identity<T>>
set2<T, K, Comp, ExtractKey>::IteratorImpl::IteratorImpl(IteratorImpl const &rhs): impl_(rhs.impl_) { }

template <typename T, typename K = T, typename Comp = std::less<K>, typename ExtractKey = std::identity<T>>
typename set2<T, K, Comp, ExtractKey>::IteratorImpl &
set2<T, K, Comp, ExtractKey>::IteratorImpl::operator=(IteratorImpl const &rhs): impl_(rhs.impl_) { 
	impl_ = rhs.impl_;
}

template <typename T, typename K = T, typename Comp = std::less<K>, typename ExtractKey = std::identity<T>>
typename set2<T, K, Comp, ExtractKey>::IteratorImpl::reference
set2<T, K, Comp, ExtractKey>::IteratorImpl::operator*() const {
	PairType pair = iml_.operator*();
	return *pair.second;
}

template <typename T, typename K = T, typename Comp = std::less<K>, typename ExtractKey = std::identity<T>>
typename set2<T, K, Comp, ExtractKey>::IteratorImpl::pointer
set2<T, K, Comp, ExtractKey>::IteratorImpl::operator->() const {
	PairType pair = iml_.operator*();
	return pair.second;
}

template <typename T, typename K = T, typename Comp = std::less<K>, typename ExtractKey = std::identity<T>>
typename set2<T, K, Comp, ExtractKey>::IteratorImpl &
set2<T, K, Comp, ExtractKey>::IteratorImpl::operator++() {
	impl_.operator++();
	return *this;
}

template <typename T, typename K = T, typename Comp = std::less<K>, typename ExtractKey = std::identity<T>>
typename set2<T, K, Comp, ExtractKey>::IteratorImpl &
set2<T, K, Comp, ExtractKey>::IteratorImpl::operator--() {
	impl_.operator--();
	return *this;
}

template <typename T, typename K = T, typename Comp = std::less<K>, typename ExtractKey = std::identity<T>>
typename set2<T, K, Comp, ExtractKey>::IteratorImpl const
set2<T, K, Comp, ExtractKey>::IteratorImpl::operator++(int) {
	impl_.operator++(int);
	return *this;
}

template <typename T, typename K = T, typename Comp = std::less<K>, typename ExtractKey = std::identity<T>>
typename set2<T, K, Comp, ExtractKey>::IteratorImpl const
set2<T, K, Comp, ExtractKey>::IteratorImpl::operator--(int) {
	impl_.operator--(int);
	return *this;
}

template <typename T, typename K = T, typename Comp = std::less<K>, typename ExtractKey = std::identity<T>>
bool
set2<T, K, Comp, ExtractKey>::IteratorImpl::operator==(IteratorImpl const &rhs) const {
	return impl_ == rhs.impl_;
}

template <typename T, typename K = T, typename Comp = std::less<K>, typename ExtractKey = std::identity<T>>
bool
set2<T, K, Comp, ExtractKey>::IteratorImpl::operator!=(IteratorImpl const &rhs) const {
	return impl_ != rhs.impl_;
}
	
} // end namespace lib_calvin_container

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