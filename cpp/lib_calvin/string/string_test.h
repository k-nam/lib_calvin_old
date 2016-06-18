#ifndef LIB_CALVIN__STRING__STRING_TEST_H
#define LIB_CALVIN__STRING__STRING_TEST_H

#include <iostream>
#include "abstract_string.h"
#include "vector.h"

using std::cout;
using std::endl;

namespace lib_calvin_string
{
using lib_calvin::abstract_string;
void stringTest();
void abstractStringTest();

void matchingAlgorithmTest();
template <typename Alphabet>
void matchingTest (void (*matchingCharAlg) 
    (abstract_string<Alphabet> const &text, abstract_string<Alphabet> const &pattern, 
     lib_calvin::vector<size_t> &record), std::string title);

void palindromeTest();
}

#endif