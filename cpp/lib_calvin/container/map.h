#ifndef LIB_CALVIN__CONTAINER__MAP_H
#define LIB_CALVIN__CONTAINER__MAP_H

#include "container.h"
#include "b_tree.h"
#include "b_plus_tree.h"
#include "bin_tree.h"
#include "rb_tree.h"
#include <map>

namespace lib_calvin_container
{
template <typename T1, typename T2>
class TakeFirstOfPair {
public:
	T1 const & operator()(std::pair<T1, T2> const &pair) const { return pair.first; }
};
}

namespace lib_calvin
{
	// This class makes it easy to switch among various implementations
	// We use another class for test wrapping purpose: SetAdaptor, as 
	// insert method's return type contains iterator, which can not be included
	// in the base class. 
template <typename K, typename V, typename Comp = std::less<K>,
	typename SetImpl = lib_calvin_container::BPlusTree<std::pair<K, V>, K, Comp, 
											lib_calvin_container::TakeFirstOfPair<K, V>>>
class map {
public:
	typedef K key_type;
	typedef typename SetImpl::value_type value_type;
	typedef V mapped_type;
	typedef typename SetImpl::pointer pointer;
	typedef typename SetImpl::reference reference;
		
	typedef typename SetImpl::iterator iterator;
	typedef typename SetImpl::const_iterator const_iterator;
	typedef typename SetImpl::reverse_iterator reverse_iterator;
	typedef typename SetImpl::const_reverse_iterator const_reverse_iterator;

	map<K, V, Comp, SetImpl>(): setImpl_() { }
	map(map<K, V, Comp, SetImpl> const &rhs): setImpl_(rhs.setImpl_) { }
	map(map<K, V, Comp, SetImpl> &&rhs): setImpl_(std::move(rhs.setImpl_)) { }
	map<K, V, Comp, SetImpl> & operator= (map<K, V, Comp, SetImpl> const &rhs) 
		{ setImpl_ = rhs.setImpl_; return *this; }
	map<K, V, Comp, SetImpl> & operator= (map<K, V, Comp, SetImpl> &&rhs) 
		{ setImpl_ = std::move(rhs.setImpl_); return *this; }

	size_t size() const { return setImpl_.size(); }
	iterator find(K const &key) { return setImpl_.find(key); }
	const_iterator find(K const &key) const { return setImpl_.find(key); }
	size_t count(K const &key) const { return setImpl_.count(key); }
	std::pair<iterator, bool> insert(std::pair<K, V> const &elem) 
		{ return setImpl_.insert(elem); }
	std::pair<iterator, bool> insert(std::pair<K, V> &&elem) 
		{ return setImpl_.insert(std::forward<std::pair<K, V>>(elem)); }
	// Insert range [beg, end)
	template <typename InputIterator>
	void insert(InputIterator beg, InputIterator end)
	{ setImpl_.insert(beg, end); }
	size_t erase(K const &key) { return setImpl_.erase(key); }
	mapped_type & operator[](key_type const &key)
	{ return setImpl_.insert(std::pair<K, V>(key, V())).first->second; }
	mapped_type & operator[](key_type &&key)
	{ return setImpl_.insert(std::pair<K, V>(std::forward<key_type>(key), V())).first->second; }

	void clear() { setImpl_.clear(); }
	bool empty() const { return setImpl_.empty(); }

	friend bool operator==(map const &lhs, map const &rhs)
	{ return lhs.setImpl_ == rhs.setImpl_; }
		
	friend bool operator!=(map const &lhs, map const &rhs)
	{ return lhs.setImpl_ != rhs.setImpl_; }
		
	friend bool operator< (map const &lhs, map const &rhs)
	{ return lhs.setImpl_ < rhs.setImpl_; }
		
	friend bool operator<= (map const &lhs, map const &rhs)
	{ return lhs.setImpl_ <= rhs.setImpl_; }
		
	friend bool operator> (map const &lhs, map const &rhs)
	{ return lhs.setImpl_ > rhs.setImpl_; }
		
	friend bool operator>= (map const &lhs, map const &rhs)
	{ return lhs.setImpl_ >= rhs.setImpl_; }

	const_iterator begin() const { return setImpl_.begin(); }
	const_iterator end() const { return setImpl_.end(); }
	iterator begin() { return setImpl_.begin(); }
	iterator end() { return setImpl_.end(); }

	const_reverse_iterator rbegin() const { return setImpl_.rbegin(); }
	const_reverse_iterator rend() const { return setImpl_.rend(); }
	reverse_iterator rbegin() { return setImpl_.rbegin(); }
	reverse_iterator rend() { return setImpl_.rend(); }

private:
	SetImpl setImpl_;
};
} // end namespace lib_calvin

#endif