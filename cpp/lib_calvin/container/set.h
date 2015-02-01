#ifndef LIB_CALVIN__CONTAINTER__SET_H
#define LIB_CALVIN__CONTAINTER__SET_H

#include "container.h"
#include "bin_tree.h"
#include "b_tree.h"
#include "b_plus_tree.h"
#include "rb_tree.h"
#include "ordered_array.h"
#include <set>

namespace lib_calvin
{
template <typename T, typename Comp = std::less<T>, 
	typename Impl = lib_calvin_container::BTree<T, Comp>>
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
} // end namespace lib_calvin

#endif