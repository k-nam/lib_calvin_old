#ifndef LIB_CALVIN__STRING__PALINDROME_H
#define LIB_CALVIN__STRING__PALINDROME_H

#include "abstract_string.h"

namespace lib_calvin_string
{
	// return a vector of size 2n - 1, where n is the length of input string.
	// for even indices, the half of it denotes the index in the input string.
	// odd indices denotes the position between letters.
	// ex) for 'abc', 0=>'a', 1=>between 'a' &'b', 2=>'b' ...
	// the values of returned vector are the length of maximal palindrome centered
	//	  at that position. (total length, not radius)
	template <typename Alphabet>
	vector<size_t> getAllPalindromes(abstract_string<Alphabet> const &string) {

		return null;
	}

	// for testing correctness
	template <typename Alphabet>
	vector<size_t> naiveGetAllPalindromes(abstract_string<Alphabet> const &string) {
		vector<size_t> result(2 * string.length() - 1);
		for (size_t i = 0; i < result.length(); i++) {
			size_t radius = extendPalindrome(string, i, 0);
			result[i] = radius + 1;
		}
	}
	
	// try to extend palindrome whose center and radius we already know
	// if radius is 0, we are matching from the scratch
	// return the radius of maximal palindrome
	template <typename Alphabet>
	size_t extendPalindrome(abstract_string<Alphabet> const &string, size_t center, size_t radius) {
		
		// 2 because each letter is apart from each other by 2
		Alphabet const &leftmost = center - radius - 2;
		Alphabet const &rightmost = center + radius + 2;

		// special case
		if (radius == 0 && center % 2 == 1) {
			Alphabet const &leftmost = center - 1;
			Alphabet const &rightmost = center + 1;			
		} 

		size_t maximalRadius = radius;
		while (leftmost >= 0 && rightmost < 2 * (string.length() - 1)) {
			if (string[leftmost] == string[rightmost]) {
				leftmost += -2;
				rightmost += 2;
				maximalRadius += 2;
			} else {
				break;
			}
		}

		return maximalRadius;
	}
}



#endif