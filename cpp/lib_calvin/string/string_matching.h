#ifndef LIB_CALVIN__STRING__STRING_MATCH_H
#define LIB_CALVIN__STRING__STRING_MATCH_H

#include "abstract_string.h"
#include "vector.h"
#include <cstdint>

namespace lib_calvin_string 
{
using lib_calvin::abstract_string;
using lib_calvin::vector;
using std::cout;
using std::endl;

// Z(i) = the length of the string that starts at index i and matches
// ...the prefix of pattern
template <typename Alphabet>
void calculateZ(abstract_string<Alphabet> const &pattern, vector<int64_t> &record);

// f(i) = the maximum length of a prefix of pattern which is also a 
// ...proper suffix of substring pattern[0 i-1]
template <typename Alphabet>
void calculateF(abstract_string<Alphabet> const &pattern, vector<int64_t> &record);

// maps each Alphabet to the index in which the Alphabet first appears in the 
// ...pattern (looking from the right-end of patttern)
// assumes that Alphabet can be converted to unsigned int type (which is the
// ...index in record)
template <typename Alphabet>
void badChar(abstract_string<Alphabet> const &pattern, vector<int64_t> &record);

template <typename Alphabet>
void strongGoodSuffix(abstract_string<Alphabet> const &pattern, 
											 vector<int64_t> &record);

// save the matching indices in the third argument
template <typename Alphabet>
void naiveMatch(abstract_string<Alphabet> const &text, 
    abstract_string<Alphabet> const &pattern, 
    vector<size_t> &result);

template <typename Alphabet>
void basicMatch(abstract_string<Alphabet> const &text, 
    abstract_string<Alphabet> const &pattern,
    vector<size_t> &result);

template <typename Alphabet>
void kmp(abstract_string<Alphabet> const &text, 
    abstract_string<Alphabet> const &pattern, 
    vector<size_t> &result);

template <typename Alphabet>
void boyerMoore(abstract_string<Alphabet> const &text, 
    abstract_string<Alphabet> const &pattern, 
    vector<size_t> &result);

} // end namespace lib_calvin_string

/************************ Global functions *******************************/

// Z(k) is the length of longest substring that starts at index k, which is
// ...also prefix of the entire string. 
template <typename Alphabet>
void lib_calvin_string::calculateZ (
    abstract_string<Alphabet> const &pattern, vector<int64_t> &record) {

  int64_t len = pattern.size();  
  vector<int64_t> &Z = record;
  record.clear();
  record.resize(len);
  // l(k), r(k): the leftmost and rightmost indices of the substring 
  // ...that spans most to the right and starts at index i <= k 
  int64_t l = 0; // stores l(k - 1) 
  int64_t r = 0; // stores r(k - 1)
  Z[0] = -1; // this value will not be used
	int64_t k, m; // for indices
  for (k = 1; k < len; k++) { // k is current index in the text
    if (r < k) { // start manual matching 
      if (pattern[k] != pattern[0]) { // first character bad
        Z[k] = 0;
      } else { // first character good
        for (m = k + 1; m < len; m++) {
          if (pattern[m] != pattern[m - k])
           break;
        }				
        m--; // note that this will also take care of the case when m == len
				// Now, m is the index of last matching character
        l = k;
        r = m;
        Z[k] = m - k + 1;
      }
    } else { // use previous data
      int64_t remainingMatch  = Z[k - l];
      int64_t remainingBox  = r - k + 1;
      if (remainingMatch < remainingBox) { // match is contained in Z-block
        Z[k] = Z[k - l];
      } else if (remainingMatch == remainingBox) { // possible to have more matching
        for (m = r + 1; m < len; m++) {
          if (pattern[m] != pattern[m - k]) 
           break;
        }
        m--;
        // m is the index of last matching character
        l = k;
        r = m;
        Z[k] = m - k + 1;
      } else { // match is just the same as k-1
        Z[k] = r - k + 1;
      }
    }
  }
}

// f(k) is the length of longest prefix of the string P which is also suffix of
//  P[0...k-1]   so, f(len) should be determined.
// f(k) value determines the jump length when pattern does not match at index k
//  in KMP algorithm. f(len) denotes the jump length when a match has been found.
template <typename Alphabet>
void lib_calvin_string::calculateF (
    abstract_string<Alphabet> const &pattern, vector<int64_t> &record) {

  int64_t len = pattern.size();
  int64_t k, m; // for indices
  vector<int64_t> &f = record;
  record.clear();
  record.resize(len + 1);
  f[0] = -1; // this value is not used, but convenient for below procedure
  f[1] = 0; //string of length 1 can not have proper prefix
  for (k = 2; k <= len; k++) { // include past-end char
    m = f[k - 1]; // m is the index to consider f[m] value
    while (pattern[m] != pattern[k - 1]) {
      m = f[m];
      if (m < 0) { // m is necessarily -1 now
        break;
      }
    } 
    f[k] = m + 1; // correct because m == -1 if no match
  }
}

// Be careful!!! Output is indexed in REVERSE ORDER!!!
// record[i] denotes the index (from the right) of the first occurrence of
// character i.
template <typename Alphabet>
void lib_calvin_string::badChar (
    abstract_string<Alphabet> const &pattern, vector<int64_t> &record) {
  
  record.clear();
  int64_t len = pattern.size();
  int64_t index;
  // we need to know the size of character set (Alphabet::size)
  // Characters not present in the pattern is marked as len; this will
  // ...make the shift amount appropriate
	if (sizeof(Alphabet) > 1) { // inappropriate foro this algorithm
		std::cout << "badChar error\n";
		exit(0);
	}
  record.resize(lib_calvin::getSizeOfCharSet<Alphabet>(), len); 
  for (int64_t i = len - 1; i >= 0; i--) {
    index = static_cast<size_t>(pattern[i]);
    if (record[index] == len) {
      record[index] = len - 1 - i;  // reversed index to fit Boyer-Moore
    }
  }
}

// Be careful!!! Output is indexed in REVERSE ORDER!!!
// The shift amount determined by strong good suffix rule is recorded in the
// array. Record[i] is the amount to shift (to right) if wrong character was
// detected on index i (from right).
// record[0] is meaningless.
template <typename Alphabet>
void lib_calvin_string::strongGoodSuffix (
    abstract_string<Alphabet> const &pattern, vector<int64_t> &record) {
  int64_t len = pattern.size();
  record.clear();
  record.resize(len + 1, -1); // initialize as -1
  // use Z algorithm in reverse string
  vector<int64_t> Z;
  calculateZ (pattern.reverse(), Z);
  for (int64_t i = 1; i < len; ++i) {
		// we must not overwrite; otherwise, we will get greater jump value than
		// the right one.
    if (record[Z[i]] == -1) { 
      record[Z[i]] = i;
    }
  }  
  // additional processing 
  int64_t left = len; // possible leftmost element not marked
  for (int64_t i = 1; i < len; ++i) {
    if (i + Z[i] == len) { // match goes through the end of string
      for (int64_t j = left; j > len - i; j--) {
				// we must not overwrite, as well as above.
				if (record[j] == -1) {
					record[j] = i;
				}        
      }
      left = len - i;
    }
  }
  for (int64_t k = 0; k < len + 1; k++) { // maximum shift for -1
    if (record[k] == -1)
      record[k] = len;
  }
  record[0] = -1; // not to be used (actually, this caused me trouble ^^)
}

template <typename Alphabet>
void lib_calvin_string::naiveMatch (
    abstract_string<Alphabet> const &text, 
		abstract_string<Alphabet> const &pattern, vector<size_t> &result) 
{
  result.clear();
  int64_t textLen   = text.size();
  int64_t patternLen  = pattern.size();
  int64_t k, s; // k for text index, s for pattern index
  for (k = 0; k <= textLen - patternLen; k++) { // last possible match
    for (s = 0; s < patternLen; s++) {
      if(text[k + s] != pattern[s])
        break;
    }
    if (s == patternLen) { // match detected
      result.push_back(k);
    }
  }
}

// using only calculateZ
// just find Z value for this transformed text: P$T 
// need a special character($) that is not present either in text or pattern
// using null character for this case
template <typename Alphabet>
void lib_calvin_string::basicMatch (
    abstract_string<Alphabet> const &text, 
    abstract_string<Alphabet> const &pattern,
    vector<size_t> &result) {
  
  result.clear();
  int64_t patternLen     = pattern.size();
  int64_t textLen      = text.size();
  Alphabet *pSpecialChar  = new Alphabet(0);
  abstract_string<Alphabet> newstring  = 
    pattern + abstract_string<Alphabet> (pSpecialChar, 1) + text;
  vector<int64_t> Z;
  calculateZ (newstring, Z);
  for (int64_t i = patternLen + 1; i < textLen + patternLen + 1; ++i) {
    if (Z[i] == patternLen) { // match detected
      result.push_back(i - patternLen - 1);
    }
    if (Z[i] > patternLen) {
      cout << "basicMatch error with Z algorithm\n";
      exit(0);
    }
  }
	delete pSpecialChar;
}

// This was hardest to micro-optimize of all codes I wrote until now.
// It seems that using goto statement is unavoidable here.
template <typename Alphabet>
void lib_calvin_string::kmp(
    abstract_string<Alphabet> const &text, 
		abstract_string<Alphabet> const &pattern, vector<size_t> &result) {
  result.clear();
  int64_t patternLen  = pattern.size();
  int64_t textLen   = text.size();
  vector<int64_t> f; 
  calculateF (pattern, f);
  int64_t k = 0, s = 0;
  
// about to match k (text index), s (pattern index)
AboutToMatch:
  while (true) {
    if (s == patternLen) { // match
      result.push_back(k - patternLen);
      if (k == textLen) {
        return;
			} else {
        goto Wrong;
			}
    }
    if (k == textLen) { // process all done
      return;
		}
    if (text[k] != pattern[s]) {
      goto Wrong;
		} else {
      k++;
      s++;
    }
  }
// wrong match detected k, s
Wrong:
  if (s == 0) {
    goto FirstWrong;
	}
  s = f[s];
  if (text[k] != pattern[s]) {
    goto Wrong;
	} else { 
    k++;
    s++;
    goto AboutToMatch;
  }
// wrong match and s == 0
FirstWrong:
  k++;
  while (true) {
    if (k == textLen) {
      return;
		}
    if(text[k] == pattern[0]) {
      k++;
      s++;
      goto AboutToMatch;
    } else {
      k++;
    }
  }
}

template <typename Alphabet>
void lib_calvin_string::boyerMoore (
    abstract_string<Alphabet> const &text, 
		abstract_string<Alphabet> const &pattern, vector<size_t> &result) {		
  result.clear();
  abstract_string<Alphabet> reverse = pattern.reverse();
  int64_t textLen   = text.size();
  int64_t patternLen  = pattern.size();
  // h: head in text, k: index in text, s: index in pattern (reversed)
  int64_t h = patternLen - 1, k = patternLen - 1, s = 0;
  int64_t badCharJump = 0, goodSuffixJump = 0;
	bool matched = false;
  vector<int64_t> charTable, suffixTable;
  badChar(pattern, charTable);
  strongGoodSuffix (pattern, suffixTable);
  while (h < textLen) {
    // sequntial matching
    while (true) {
      if (text[k] != reverse[s]) { // using reverse for convenient indexing
				matched = false;
        break;
			}
      k--;
      s++;
      if (s == patternLen) { // match at (k + 1)
        result.push_back (k + 1);
        badCharJump = -1; // can not use badchar when matched
        matched = true;
				break;
      }
    }
    // jump forward
		if (!matched) {
			badCharJump = charTable[static_cast<size_t>(text[k])] - s;
		}
    goodSuffixJump  = suffixTable[s];
    if (badCharJump > goodSuffixJump) {
      h += badCharJump;
		} else {
      h += goodSuffixJump;
		}
    k = h;
    s = 0;
  }
}

#endif