#ifndef LIB_CALVIN__STRING__ABSTRACT_STRING_H
#define LIB_CALVIN__STRING__ABSTRACT_STRING_H

/*
* 2008-03-11: c_string algorithms
* 2008-10-19: revised to eliminate alphabet class
*/

#include <string>
#include <iostream>
#include <cstdlib>
#include "stopwatch.h"
#include "vector.h"

namespace lib_calvin
{
	template <typename Alphabet>
	class abstract_string;
}

template <typename Alphabet>
std::ostream & operator<< (std::ostream& os, lib_calvin::abstract_string<Alphabet> const &string)
{
	for (auto character : string.vector_) {
		os << character;
	}
	return os;
}

namespace lib_calvin
{
	template <typename Alphabet>
	abstract_string<Alphabet>
		operator+ (abstract_string<Alphabet> const &, abstract_string<Alphabet> const &);

	template <typename Alphabet>
	abstract_string<Alphabet> &&
		operator+ (abstract_string<Alphabet> &&, abstract_string<Alphabet> const &);

	// Maintains its own data (value semantic) and immutable
	template <typename Alphabet = char>
	class abstract_string {
	public:
		typedef Alphabet CharType;
		abstract_string();
		abstract_string(Alphabet const &); // one letter c_string
		abstract_string(Alphabet const *instring, size_t len);
		abstract_string(Alphabet const *instring);
		abstract_string(abstract_string<Alphabet> const &);
		abstract_string(abstract_string<Alphabet> &&);
		abstract_string & operator=(abstract_string const &);
		abstract_string(std::basic_string<Alphabet> const &);
	public:
		typedef Alphabet * iterator;
		typedef Alphabet const * const_iterator;
		iterator begin() { return &(*vector_.begin()); }
		const_iterator begin() const { return &(*vector_.begin()); }
		iterator end() { return &(*vector_.end()); }
		const_iterator end() const { return &(*vector_.end()); }

	public:
		void swap(abstract_string<Alphabet> &&rhs);
		size_t size() const;
		size_t length() const;
		Alphabet const & operator[] (size_t index) const;
		Alphabet & operator[] (size_t index);
		// Lexicographical order by Alphabet
		bool operator< (abstract_string<Alphabet> const &) const;
		bool operator== (abstract_string<Alphabet> const &) const;
		bool operator!= (abstract_string<Alphabet> const &) const;
		// Does not include end index
		abstract_string<Alphabet> substr(size_t startIndex, size_t endIndex) const;
		// To the end of c_string
		abstract_string<Alphabet> substr(size_t startIndex) const;
		// reverse
		abstract_string<Alphabet> reverse() const;
		friend abstract_string<Alphabet>
			operator+<> (abstract_string<Alphabet> const &lhs, abstract_string<Alphabet> const &rhs);
		friend abstract_string<Alphabet> &&
			operator+<> (abstract_string<Alphabet> &&lhs, abstract_string<Alphabet> const &rhs);
		abstract_string & operator+= (abstract_string<Alphabet> const &rhs);
		friend std::ostream & operator<< <>(std::ostream & os, abstract_string<Alphabet> const &);
	public:
		void print() const;
		void println() const;
	private:
		abstract_string(vector<Alphabet> &&);
		void init(Alphabet const *string, size_t len);
	private:
		vector<Alphabet> vector_;
	};



	template <typename Alphabet>
	size_t getSizeOfCharSet() {
		// returns 128 for char
		return (1 << sizeof(Alphabet) * 8) >> 1;
	}

	template <typename Alphabet>
	Alphabet getNullChar() {
		return Alphabet(0);
	}

	// c_string is normal ascii string
	typedef abstract_string<char> c_string;

	// itoa function
	c_string itoa(int number);

	// Conversion methods are only for ascii c_string
	char    charOf(c_string const &); // works for 'a', '\n', ...
	int     intOfDec(c_string const &);
	int     intOfHex(c_string const &);
	float   floatOf(c_string const &);

} // end namespace lib_calvin



namespace lib_calvin
{
	/*********************** c_string <Alphabet> definition **************************/

	template <typename Alphabet>
	void abstract_string<Alphabet>::init(Alphabet const *string, size_t length) {
		vector_.resize(length);
		for (size_t i = 0; i < length; i++) {
			vector_[i] = string[i];
		}
	}

	template <typename Alphabet>
	abstract_string<Alphabet>::abstract_string() : vector_() { }

	template <typename Alphabet>
	abstract_string<Alphabet>::abstract_string(vector<Alphabet> &&vector) : vector_(std::move(vector)) { }

	template <typename Alphabet>
	abstract_string<Alphabet>::abstract_string(Alphabet const &character) {
		init(&character, 1);
	}

	template <typename Alphabet>
	abstract_string<Alphabet>::abstract_string(Alphabet const *string, size_t length) {
		init(string, length);
	}

	template <typename Alphabet>
	abstract_string<Alphabet>::abstract_string(Alphabet const *string) {
		for (size_t i = 0; string[i] != 0; i++) {
			vector_.push_back(string[i]);
		}
	}

	template <typename Alphabet>
	abstract_string<Alphabet>::abstract_string(abstract_string<Alphabet> const &rhs) : vector_(rhs.vector_) { }

	template <typename Alphabet>
	abstract_string<Alphabet>::abstract_string(abstract_string<Alphabet> &&rhs) {
		swap(std::move(rhs));
	}

	template <typename Alphabet>
	abstract_string<Alphabet> &
		abstract_string<Alphabet>::operator=(abstract_string<Alphabet> const &rhs) {
		if (this != &rhs) {
			vector_ = rhs.vector_;
		}
		return *this;
	}

	template <typename Alphabet>
	abstract_string<Alphabet>::abstract_string(std::basic_string<Alphabet> const &string) {
		init(string.c_str(), string.length());
	}

	template <typename Alphabet>
	void abstract_string<Alphabet>::swap(abstract_string<Alphabet> &&rhs) {
		vector_.swap(rhs.vector_);
	}

	template <typename Alphabet>
	size_t abstract_string<Alphabet>::size() const {
		return vector_.size();
	}

	template <typename Alphabet>
	size_t abstract_string<Alphabet>::length() const {
		return size();
	}

	template <typename Alphabet>
	Alphabet const & abstract_string<Alphabet>::operator[] (size_t index) const {
		return vector_[index];
	}

	template <typename Alphabet>
	Alphabet & abstract_string<Alphabet>::operator[] (size_t index) {
		return vector_[index];
	}

	template <typename Alphabet>
	bool abstract_string<Alphabet>::operator< (abstract_string<Alphabet> const &rhs) const {
		return vector_ < rhs.vector_;
	}

	template <typename Alphabet>
	bool abstract_string<Alphabet>::operator==(abstract_string<Alphabet> const &rhs) const {
		return vector_ == rhs.vector_;
	}

	template <typename Alphabet>
	bool abstract_string<Alphabet>::operator!=(abstract_string<Alphabet> const &rhs) const {
		return !(operator==(rhs));
	}

	template <typename Alphabet>
	abstract_string<Alphabet>
		abstract_string<Alphabet>::substr(size_t startIndex, size_t endIndex) const {
		return abstract_string<Alphabet>(&(*vector_.begin()) + startIndex, endIndex - startIndex);
	}

	template <typename Alphabet>
	abstract_string<Alphabet>
		abstract_string<Alphabet>::substr(size_t startIndex) const {
		return substr(startIndex, vector_.size());
	}

	template <typename Alphabet>
	abstract_string<Alphabet>
		abstract_string<Alphabet>::reverse() const {
		vector<Alphabet> reverse(vector_.size());
		for (size_t i = 0; i < vector_.size(); ++i) {
			reverse[i] = vector_[vector_.size() - 1 - i];
		}
		return abstract_string<Alphabet>(std::move(reverse));
	}

	template <typename Alphabet>
	void abstract_string<Alphabet>::print() const {
		std::cout << "<";
		for (size_t i = 0; i < size(); ++i) {
			std::cout << vector_[i];
		}
		std::cout << ">";
	}

	template <typename Alphabet>
	void abstract_string<Alphabet>::println() const {
		std::cout << "<";
		for (size_t i = 0; i < size(); ++i) {
			std::cout << vector_[i];
		}
		std::cout << ">\n";
	}

	template <typename Alphabet>
	abstract_string<Alphabet>
		operator+ (abstract_string<Alphabet> const &lhs, abstract_string<Alphabet> const &rhs) {
		if (&lhs == &rhs) {
			std::cout << "string + error\n";
			exit(0);
		}
		vector<Alphabet> vector(lhs.vector_);
		vector.insert(vector.end(), rhs.vector_.begin(), rhs.vector_.end());
		return abstract_string<Alphabet>(std::move(vector));
	}

	template <typename Alphabet>
	abstract_string<Alphabet> &&
		operator+ (abstract_string<Alphabet> &&lhs, abstract_string<Alphabet> const &rhs) {
		vector<Alphabet> &vector = lhs.vector_;
		vector.insert(vector.end(), rhs.vector_.begin(), rhs.vector_.end());
		return std::move(lhs);
	}

	template <typename Alphabet>
	abstract_string<Alphabet> &
		abstract_string<Alphabet>::operator+= (abstract_string<Alphabet> const &rhs) {
		vector_.insert(vector_.end(), rhs.vector_.begin(), rhs.vector_.end());
		return *this;
	}

} // end lib_calvin for definitions

#endif






