#ifndef LIB_CALVIN__SORT__SORT_TEST_H
#define LIB_CALVIN__SORT__SORT_TEST_H

#include <iostream>
#include "sort.h"

// This is test for Windows and Linux file sharing

using std::cout;
using std::endl;

namespace lib_calvin_sort
{
void sortTest();
void binSearchTest();
void binSearchTest2();
void getSamplesTest();

struct SimpleStruct {
	SimpleStruct() { objectCount_++; }
	SimpleStruct(SimpleStruct const &rhs): first(rhs.first), second(rhs.second) { objectCount_++; }
	~SimpleStruct() { objectCount_--; }
	static void countObjects();
  char operator() (int b) { return '5'; }
  int first;
  int second;
  //int x;
  //int y;
  //double x1;
  //double x2;
	//int array[10];
  int get_value() { return first; }
	static intmax_t objectCount_; // for checking memory leak
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

template <typename T, typename Comparator = std::less<T>>
void sortTest(void (*sortingAlg)( T *first, T *last, Comparator comp), 
							std::string title, Comparator comp = Comparator());

template <typename Iterator, typename Comparator>
void sortTest(void (*sortingAlgorithm)(Iterator, Iterator, Comparator), 
							Iterator first, Iterator last, std::string title,
							double &time, bool &isCorrect, bool &isStable, Comparator comp);


void sortTest2();

template <typename Iterator, typename Comparator>
void sortTest2Sub(void(*sortingAlgorithm)(Iterator, Iterator, Comparator),
																	 Iterator begin, Iterator end, Comparator comp);

} // end namespace lib_calvin_sort


template <typename T, typename Comparator>
void lib_calvin_sort::sortTest(void (*sortingAlg) (T *first, T *last, Comparator comp), std::string title, 
															 Comparator comp) {  
  stopwatch watch;
  bool correct  = true;
  bool stable   = true;
	size_t const numTestCases = 5;
	int arraySize[numTestCases] = { 1000, 10000, 100000, 1000000 };
	int numIteration[numTestCases] = { 100, 100, 10, 3 };
	std::srand(1232); // reset seed to give identical input to algorithms
  for (int i = 0; i < numTestCases; ++i) { 
		if (arraySize[i] == 0) {
			continue;
		}
    T *testSet = new T[arraySize[i]];		
		double min = 1000000;
		bool isTotallyCorrect = true;
		bool isTotallyStable = true;
		for (int j = 0; j < numIteration[i]; j++) {
			for (int j = 0; j < arraySize[i]; j++) {
				testSet[j].first = j / 2;
				//testSet[j].first = -j / 3; // reverse ordered input array
			}			
			std::random_shuffle(testSet, testSet + arraySize[i], lib_calvin::random_number_generator());
			for (int j = 0; j < arraySize[i]; j++) {
				testSet[j].second = j; // for testing stability
			}	
			double time = 0;
			bool isCorrect = true;
			bool isStable = true;
			sortTest(sortingAlg, testSet, testSet + arraySize[i], title, time, isCorrect, isStable, comp);
			if (time < min) {
				min = time;
			}
			if (isCorrect == false) {
				isTotallyCorrect = false;
			}
			if (isStable == false) {
				isTotallyStable = false;
			}
		}
		delete[] testSet;
		cout << title << ": ";
		if (isTotallyCorrect == true){
		} else {
			cout << "Sorting: failure!!! ";
			exit(0);
		}
		if (isTotallyStable) {
			cout << "Stable OK\n";
		} else {
			cout << "Not stable\n";
		}
		cout << sizeof(T) << "Bytes ";
		cout << arraySize[i] << " " << min << "sec " <<
			"speed: " << arraySize[i] / min << endl;
	}
	std::cout << endl;
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::sortTest(void (*sortingAlgorithm)(Iterator, Iterator, Comparator), 
							Iterator first, Iterator last, std::string title,
							double &time, bool &isCorrect, bool &isStable, Comparator comp) {
	stopwatch watch;
	ptrdiff_t size = last - first; 
  watch.start();
  sortingAlgorithm(first, last, comp);
  watch.stop();
  time = watch.read();
  // check correctness
  for (ptrdiff_t j = 0; j < size - 1; j++) {
    if (comp(*(first + (j + 1)), *(first + j))) {
      isCorrect = false;
		}
    if (*(first + j) == *(first + j + 1) && 
				(*(first + j)).second > (*(first + (j + 1))).second) {
      isStable = false;
		}
  }
} 



template <typename Iterator, typename Comparator>
void lib_calvin_sort::sortTest2Sub(void (*sortingAlgorithm)(Iterator, Iterator, Comparator),
																	 Iterator begin, Iterator end, Comparator comp) {

	sortingAlgorithm(begin, end, comp);
	
	
}

#endif

