#ifndef LIB_CALVIN__STRING__STRING_TEST_H
#define LIB_CALVIN__STRING__STRING_TEST_H

#include <iostream>
#include "abstract_string.h"


using std::cout;
using std::endl;

namespace lib_calvin_string
{

void stringTest();
void abstractStringTest();

void matchingAlgorithmTest();

template <typename Alphabet>
void matchingTest (void (*matchingCharAlg) 
    (lib_calvin::abstract_string<Alphabet> const &text, lib_calvin::abstract_string<Alphabet> const &pattern,
     std::vector<size_t> &record), std::string title);

void palindromeTest();
}

#endif