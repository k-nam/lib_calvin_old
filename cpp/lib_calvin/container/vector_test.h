#ifndef LIB_CALVIN__CONTAINER__VECTOR_TEST_H
#define LIB_CALVIN__CONTAINER__VECTOR_TEST_H

#include "vector.h"
#include <string>
#include <utility>

namespace lib_calvin_container 
{

void vectorTest();

void vectorBasicTest();

template <typename Impl>
void vectorFunctionTest(Impl && impl, std::string title);

template <typename Impl>
void vectorIteratorTest(Impl && impl, std::string title);

template <typename Impl>
void vectorPerformanceTest(Impl && impl, std::string title, size_t testSize);

template <typename Impl>
void vectorPerformanceTest2(Impl && impl, std::string title, size_t testSize);

template <typename Impl>
void vectorPerformanceTest3(Impl && impl, std::string title, size_t testSize);

template <typename T1, typename T2>
bool areSame(T1 const &lhs, T2 const &rhs);

template <typename Impl> 
void resetVectorForSize(Impl &impl, size_t size);

}

#endif
