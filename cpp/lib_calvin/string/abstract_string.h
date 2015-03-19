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

template <typename Alphabet>
class abstract_string;

template <typename Alphabet>
abstract_string<Alphabet> const
operator+ (abstract_string<Alphabet> const &, 
    abstract_string<Alphabet> const &);

// Maintains its own data (value semantic) and immutable
// Do not use 'string' for its name to avoid collision with std::string 
template <typename Alphabet = char>
class abstract_string { 
public:
	typedef Alphabet CharType;
  abstract_string();
  abstract_string(Alphabet); // one letter string
  abstract_string(Alphabet *instring, int len);
  abstract_string(abstract_string<Alphabet> const &);
  abstract_string(std::basic_string<Alphabet> const &); 
  ~abstract_string();
  int size() const { return len_; }
  Alphabet const & operator[] (int index) const { return string_[index]; } 
  Alphabet & operator[] (int index) { return string_[index]; }
  abstract_string<Alphabet> & operator= (
      abstract_string<Alphabet> const &);
  abstract_string<Alphabet> & operator+= (
      abstract_string<Alphabet> const &); // concat
  // Lexicographical order by Alphabet
  bool operator< (
      abstract_string<Alphabet> const &) const; // for set and map
  bool operator== (abstract_string<Alphabet> const &) const;
  bool operator!= (abstract_string<Alphabet> const &) const;
  // Does not include end index
  abstract_string<Alphabet> const substr(int startIndex, int endIndex) const; 
  // To the end of string
  abstract_string<Alphabet> const substr(int startIndex) const; 
  // reverse
  abstract_string<Alphabet> const reverse() const;
  friend abstract_string<Alphabet> const 
    operator+<> (abstract_string<Alphabet> const &lhs, 
        abstract_string<Alphabet> const &rhs); 
    
  void print() const;
private:
  Alphabet *string_;
  int len_;
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
/*********************** string <Alphabet> definition **************************/

template <typename Alphabet>
abstract_string<Alphabet>::abstract_string(): string_(NULL), len_(0) {
}

template <typename Alphabet>
abstract_string<Alphabet>::abstract_string(Alphabet character) {
  len_ = 1;
  string_ = new Alphabet[1];
  string_[0] = character;
}

template <typename Alphabet>
abstract_string<Alphabet>::abstract_string (Alphabet *instring, int inLen) { 
  len_ = inLen;
  string_ = new Alphabet[len_];
  for (int i = 0; i < len_; ++i)
    string_[i] = instring[i];
}

template <typename Alphabet>
abstract_string<Alphabet>::abstract_string(abstract_string<Alphabet> const &rhs) {
  len_ = rhs.len_;
  string_ = new Alphabet[len_];
  for (int i = 0; i < len_; ++i)
    string_[i] = rhs.string_[i];
}

template <typename Alphabet>
abstract_string<Alphabet>::abstract_string (
    std::basic_string<Alphabet> const &instring) { 

  len_ = static_cast<int>(instring.size());
  string_ = new Alphabet[len_];
  for (int i = 0; i < len_; ++i)
    string_[i] = instring[i];
}

template <typename Alphabet>
abstract_string<Alphabet>::~abstract_string() {
  delete string_;
}

template <typename Alphabet>
abstract_string<Alphabet> & 
abstract_string<Alphabet>::operator= (abstract_string<Alphabet> const &rhs) {
  
  if(this == &rhs)
    return *this;
  len_ = rhs.len_;
  delete[] string_;
  string_ = new Alphabet[len_];
  for (int i = 0; i < len_; ++i)
    string_[i] = rhs.string_[i];
  return *this;
}

template <typename Alphabet>
abstract_string<Alphabet> & 
abstract_string<Alphabet>::operator+= (abstract_string<Alphabet> const &rhs) {
  
  Alphabet *newstring = new Alphabet[len_ + rhs.len_];
  Alphabet *oldstring = string_;
  string_ = newstring;
  for (int i = 0; i < len_; ++i)
    string_[i] = oldstring[i];
  for (int i = 0; i < rhs.len_; ++i)
    string_[len_ + i] = rhs.string_[i];
  len_ += rhs.len_;
  delete[] oldstring;
}

template <typename Alphabet>
bool abstract_string<Alphabet>::operator< (abstract_string<Alphabet> const &rhs) const {
  bool shorter = (len_ < rhs.len_); // true if this one is shorter
  int shorterLen = (shorter) ? len_ : rhs.len_;
  for (int i = 0; i < shorterLen; ++i) {
    Alphabet a = string_[i];
    Alphabet b = rhs.string_[i];
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

template <typename Alphabet>
bool abstract_string<Alphabet>::operator== (abstract_string<Alphabet> const &rhs) const {
  
  if (len_ != rhs.size())
    return false;
  for (int i = 0; i < len_; ++i) {
    if (string_[i] != rhs.string_[i])
      return false;
  }
  return true;
}

template <typename Alphabet>
bool abstract_string<Alphabet>::operator!= (abstract_string<Alphabet> const &rhs) const {
  
  return !(operator==(rhs));
}

template <typename Alphabet>
abstract_string<Alphabet> const
abstract_string<Alphabet>::substr (int startIndex, int endIndex) const {
  abstract_string<Alphabet> substr (string_ + startIndex, endIndex - startIndex);
  return substr;
}

template <typename Alphabet>
abstract_string<Alphabet> const
abstract_string<Alphabet>::substr (int startIndex) const {
  abstract_string<Alphabet> substr (string_ + startIndex, len_ - startIndex);
  return substr;
}

template <typename Alphabet>
abstract_string<Alphabet> const
abstract_string<Alphabet>::reverse() const {
  Alphabet *pChar = new Alphabet[len_];
  for (int i = 0; i < len_; ++i) {
    pChar[i] = string_[len_ - 1 - i];
  }
  abstract_string<Alphabet> reversedstring (pChar, len_);
  return reversedstring;
}

template <typename Alphabet>
void abstract_string<Alphabet>::print() const {
  for (int i = 0; i < len_; ++i)
    cout << string_[i];
}

template <typename Alphabet>
abstract_string<Alphabet> const
operator+ (abstract_string<Alphabet> const &lhs, 
    abstract_string<Alphabet> const &rhs) {
  
  Alphabet *pChar = new Alphabet[lhs.len_ + rhs.len_];
  int j = 0;
  for (int i = 0; i < lhs.len_; ++i) {
    pChar[j++] = lhs.string_[i];
  }
  for (int i = 0; i < rhs.len_; ++i) {
    pChar[j++] = rhs.string_[i];
  }
  abstract_string<Alphabet> newstring (pChar, lhs.len_ + rhs.len_);
  return newstring;
}
} // end lib_calvin for definitions

#endif






