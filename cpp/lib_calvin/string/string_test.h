#ifndef LIB_CALVIN__STRING__STRING_TEST_H
#define LIB_CALVIN__STRING__STRING_TEST_H

#include <iostream>
#include "abstract_string.h"

using std::cout;
using std::endl;

namespace lib_calvin_string
{
void stringTest();
void matchingTest (void (*matchingCharAlg) 
    (abstract_string<> const &text, abstract_string<> const &pattern, 
     vector<int> &record), std::string title);
}

#endif