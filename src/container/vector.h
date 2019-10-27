#ifndef LIB_CALVIN__CONTAINER__VECTOR_H
#define LIB_CALVIN__CONTAINER__VECTOR_H

#include "container.h"
#include "array.h"
#include "blocked_array.h"
#include <iostream>
#include <vector>

namespace lib_calvin
{
	template <typename T, typename Impl = lib_calvin_container::Array<T>>
	class vector {
	public:
		typedef typename Impl::iterator iterator;
		typedef typename Impl::const_iterator const_iterator;
		typedef typename Impl::reverse_iterator reverse_iterator;
		typedef typename Impl::const_reverse_iterator const_reverse_iterator;
		typedef typename Impl::value_type value_type;
		typedef typename Impl::pointer pointer;
		typedef typename Impl::reference reference;
		typedef typename Impl::const_reference const_reference;
		typedef typename Impl::difference_type difference_type;

		vector() : impl_() { }
		vector(vector<T, Impl> const &rhs) : impl_(rhs.impl_) { }
		vector(vector<T, Impl> &&rhs) : impl_(std::move(rhs.impl_)) { }
		vector(size_t initSize, T const &value) : impl_(initSize, value) { }
		vector(size_t initSize) : impl_(initSize) { }
		template <typename InputIter>
		vector(InputIter begin, InputIter end) : impl_(begin, end) { }
		vector(std::initializer_list<T> const &list) : impl_(list) { }

		vector<T, Impl> & operator= (vector<T, Impl> const &rhs) { impl_ = rhs.impl_; return *this; }
		vector<T, Impl> & operator= (vector<T, Impl> &&rhs) { impl_ = std::move(rhs.impl_); return *this; }
		reference operator[] (size_t index) { return impl_[index]; }
		const_reference operator[] (size_t index) const { return impl_[index]; }

		void push_back(T const &elem) { impl_.push_back(elem); }
		void push_back(T &&elem) { impl_.push_back(std::forward<T>(elem)); }
		void pop_back() { impl_.pop_back(); }
		reference front() { return impl_.front(); }
		reference back() { return impl_.back(); }
		const_reference front() const { return impl_.front(); }
		const_reference back() const { return impl_.back(); }

		// return the position of the first new elements
		iterator insert(const_iterator const &pos, T const &elem) { return impl_.insert(pos, elem); }
		iterator insert(const_iterator const &pos, const_iterator const &begin, const_iterator const &end)
		{
			return impl_.insert(pos, begin, end);
		}

		// return the position of the next element to erased one
		iterator erase(const_iterator const &pos) { return impl_.erase(pos); }
		iterator erase(const_iterator const &begin, const_iterator const &end) { return impl_.erase(begin, end); }

		size_t size() const { return impl_.size(); }
		bool empty() const { return impl_.empty(); }
		size_t capacity() const { return impl_.capacity(); }
		void reserve(size_t newCapacity) { impl_.reserve(newCapacity); }
		void resize(size_t newSize, T const &elem = T()) { impl_.resize(newSize, elem); }
		void clear() { impl_.clear(); }
		void swap(vector &rhs) { impl_.swap(rhs.impl_); }

		const_iterator begin() const { return impl_.begin(); }
		const_iterator end() const { return impl_.end(); }
		iterator begin() { return impl_.begin(); }
		iterator end() { return impl_.end(); }

		const_reverse_iterator rbegin() const { return impl_.rbegin(); }
		const_reverse_iterator rend() const { return impl_.rend(); }
		reverse_iterator rbegin() { return impl_.rbegin(); }
		reverse_iterator rend() { return impl_.rend(); }

		friend bool operator==(vector const &lhs, vector const &rhs)
		{
			return lhs.impl_ == rhs.impl_;
		}

		friend bool operator!=(vector const &lhs, vector const &rhs)
		{
			return lhs.impl_ != rhs.impl_;
		}

		friend bool operator< (vector const &lhs, vector const &rhs)
		{
			return lhs.impl_ < rhs.impl_;
		}

		friend bool operator<= (vector const &lhs, vector const &rhs)
		{
			return lhs.impl_ <= rhs.impl_;
		}

		friend bool operator> (vector const &lhs, vector const &rhs)
		{
			return lhs.impl_ > rhs.impl_;
		}

		friend bool operator>= (vector const &lhs, vector const &rhs)
		{
			return lhs.impl_ >= rhs.impl_;
		}

	private:
		Impl impl_;
	};
} // end namespace lib_calvin

#endif


