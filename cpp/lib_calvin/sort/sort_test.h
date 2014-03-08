#ifndef LIB_CALVIN__SORT__SORT_TEST_H
#define LIB_CALVIN__SORT__SORT_TEST_H

#include <iostream>
#include "sort.h"

// This is test for Windows and Linux file sharing
// hello~
//
//
// 

using std::cout;
using std::endl;

namespace lib_calvin_sort
{
void sortTest();
void binSearchTest();
void binSearchTest2();
void getSamplesTest();

struct SimpleStruct {
  char operator() (int b) { return '5'; }
  int first;
  int second;
  //int x;
  //int y;
  //double x1;
  //double x2;
	//int array[10];
  int get_value() { return first; }
};

std::string getRandomString(int length);

struct StringStruct: public SimpleStruct {
	StringStruct(): value(getRandomString(3)) { 
		//std::cout << "default con\n";
	}
	StringStruct(StringStruct const &rhs): value(rhs.value) { 
		//std::cout << "copy con\n";
	}
	std::string value;
};

bool operator< (SimpleStruct const &lhs, SimpleStruct const &rhs);
bool operator== (SimpleStruct const &lhs, SimpleStruct const &rhs);

bool operator< (StringStruct const &lhs, StringStruct const &rhs);
bool operator== (StringStruct const &lhs, StringStruct const &rhs);

template <typename T>
void sortTest (void (*sortingAlg) (T *first, T *last), 
    std::string title);

template <typename Iterator>
void sortTest (void (*sortingAlgorithm)(Iterator, Iterator), 
	Iterator first, Iterator last, 
	std::string title, double &time, bool &isCorrect, bool &isStable);

} // end namespace lib_calvin_sort

#endif

