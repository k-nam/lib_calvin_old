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
void matchingTest (void (*matchingCharAlg) 
    (abstract_string<> const &text, abstract_string<> const &pattern, 
     lib_calvin::vector<size_t> &record), std::string title);
}

#endif