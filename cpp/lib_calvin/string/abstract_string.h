#ifndef LIB_CALVIN__STRING__ABSTRACT_STRING_H
#define LIB_CALVIN__STRING__ABSTRACT_STRING_H

/*
 * 2008-03-11: string algorithms
 * 2008-10-19: revised to eliminate alphabet class
 */

#include <string>
#include <iostream>
#include <deque>
#include <cstdlib>
#include "stopwatch.h"

namespace lib_calvin 
{
using std::deque;

template <typename Char>
class abstract_string;

template <typename Char>
abstract_string<Char> const
operator+ (abstract_string<Char> const &, 
    abstract_string<Char> const &);

// Maintains its own data (value semantic) and immutable
// Do not use 'string' for its name to avoid collision with std::string 
template <typename Char = char>
class abstract_string { 
public:
	typedef Char CharType;
  abstract_string();
  abstract_string(Char); // one letter string
  abstract_string(Char *instring, int len);
  abstract_string(abstract_string<Char> const &);
  abstract_string(std::basic_string<Char> const &); 
  ~abstract_string();
  int size() const { return len_; }
  Char const & operator[] (int index) const { return string_[index]; } 
  Char & operator[] (int index) { return string_[index]; }
  abstract_string<Char> & operator= (
      abstract_string<Char> const &);
  abstract_string<Char> & operator+= (
      abstract_string<Char> const &); // concat
  // Lexicographical order by Char
  bool operator< (
      abstract_string<Char> const &) const; // for set and map
  bool operator== (abstract_string<Char> const &) const;
  bool operator!= (abstract_string<Char> const &) const;
  // Does not include end index
  abstract_string<Char> const substr(int startIndex, int endIndex) const; 
  // To the end of string
  abstract_string<Char> const substr(int startIndex) const; 
  // reverse
  abstract_string<Char> const reverse() const;
  friend abstract_string<Char> const 
    operator+<> (abstract_string<Char> const &lhs, 
        abstract_string<Char> const &rhs); 
    
  void print() const;
private:
  Char *string_;
  int len_;
};

template <typename Char>
size_t getSizeOfCharSet() {
	// returns 128 for char
	return (1 << sizeof(Char) * 8) >> 1;
}

template <typename Char>
Char getNullChar() {
	return Char(0);
}

// string is normal ascii string
typedef abstract_string<char> string;

// itoa function
string itoa(int number);

// Conversion methods are only for ascii string
char    charOf(string const &); // works for 'a', '\n', ...
int     intOfDec(string const &);
int     intOfHex(string const &);
float   floatOf(string const &); 

} // end namespace lib_calvin

namespace lib_calvin 
{ 
/*********************** string <Char> definition **************************/

template <typename Char>
abstract_string<Char>::abstract_string(): string_(NULL), len_(0) {
}

template <typename Char>
abstract_string<Char>::abstract_string(Char character) {
  len_ = 1;
  string_ = new Char[1];
  string_[0] = character;
}

template <typename Char>
abstract_string<Char>::abstract_string (Char *instring, int inLen) { 
  len_ = inLen;
  string_ = new Char[len_];
  for (int i = 0; i < len_; ++i)
    string_[i] = instring[i];
}

template <typename Char>
abstract_string<Char>::abstract_string(abstract_string<Char> const &rhs) {
  len_ = rhs.len_;
  string_ = new Char[len_];
  for (int i = 0; i < len_; ++i)
    string_[i] = rhs.string_[i];
}

template <typename Char>
abstract_string<Char>::abstract_string (
    std::basic_string<Char> const &instring) { 

  len_ = static_cast<int>(instring.size());
  string_ = new Char[len_];
  for (int i = 0; i < len_; ++i)
    string_[i] = instring[i];
}

template <typename Char>
abstract_string<Char>::~abstract_string() {
  delete string_;
}

template <typename Char>
abstract_string<Char> & 
abstract_string<Char>::operator= (abstract_string<Char> const &rhs) {
  
  if(this == &rhs)
    return *this;
  len_ = rhs.len_;
  delete[] string_;
  string_ = new Char[len_];
  for (int i = 0; i < len_; ++i)
    string_[i] = rhs.string_[i];
  return *this;
}

template <typename Char>
abstract_string<Char> & 
abstract_string<Char>::operator+= (abstract_string<Char> const &rhs) {
  
  Char *newstring = new Char[len_ + rhs.len_];
  Char *oldstring = string_;
  string_ = newstring;
  for (int i = 0; i < len_; ++i)
    string_[i] = oldstring[i];
  for (int i = 0; i < rhs.len_; ++i)
    string_[len_ + i] = rhs.string_[i];
  len_ += rhs.len_;
  delete[] oldstring;
}

template <typename Char>
bool abstract_string<Char>::operator< (abstract_string<Char> const &rhs) const {
  bool shorter = (len_ < rhs.len_); // true if this one is shorter
  int shorterLen = (shorter) ? len_ : rhs.len_;
  for (int i = 0; i < shorterLen; ++i) {
    Char a = string_[i];
    Char b = rhs.string_[i];
    if (a < b)
      return true;
    if (b < a)
      return false;
  }
  // All chars are the same, then determine by length
  if (shorter)
    return true;
  return false; // same length will return false
}

template <typename Char>
bool abstract_string<Char>::operator== (abstract_string<Char> const &rhs) const {
  
  if (len_ != rhs.size())
    return false;
  for (int i = 0; i < len_; ++i) {
    if (string_[i] != rhs.string_[i])
      return false;
  }
  return true;
}

template <typename Char>
bool abstract_string<Char>::operator!= (abstract_string<Char> const &rhs) const {
  
  return !(operator==(rhs));
}

template <typename Char>
abstract_string<Char> const
abstract_string<Char>::substr (int startIndex, int endIndex) const {
  abstract_string<Char> substr (string_ + startIndex, endIndex - startIndex);
  return substr;
}

template <typename Char>
abstract_string<Char> const
abstract_string<Char>::substr (int startIndex) const {
  abstract_string<Char> substr (string_ + startIndex, len_ - startIndex);
  return substr;
}

template <typename Char>
abstract_string<Char> const
abstract_string<Char>::reverse() const {
  Char *pChar = new Char[len_];
  for (int i = 0; i < len_; ++i) {
    pChar[i] = string_[len_ - 1 - i];
  }
  abstract_string<Char> reversedstring (pChar, len_);
  return reversedstring;
}

template <typename Char>
void abstract_string<Char>::print() const {
  for (int i = 0; i < len_; ++i)
    cout << string_[i];
}

template <typename Char>
abstract_string<Char> const
operator+ (abstract_string<Char> const &lhs, 
    abstract_string<Char> const &rhs) {
  
  Char *pChar = new Char[lhs.len_ + rhs.len_];
  int j = 0;
  for (int i = 0; i < lhs.len_; ++i) {
    pChar[j++] = lhs.string_[i];
  }
  for (int i = 0; i < rhs.len_; ++i) {
    pChar[j++] = rhs.string_[i];
  }
  abstract_string<Char> newstring (pChar, lhs.len_ + rhs.len_);
  return newstring;
}
} // end lib_calvin for definitions

#endif






