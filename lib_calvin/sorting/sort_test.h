#ifndef LIB_CALVIN__SORT__SORT_TEST_H
#define LIB_CALVIN__SORT__SORT_TEST_H

#include <iostream>
#include <algorithm>
#include <vector>
#include "sort.h"
#include "stopwatch.h"
#include "container_test_types.h"


// This is test for Windows and Linux file sharing

using std::cout;
using std::endl;

namespace lib_calvin_sort
{
	using lib_calvin::stopwatch;

	void sortTest();
	void binSearchTest();
	void binSearchTest2();
	void getSamplesTest();



	struct SimpleStruct {
		SimpleStruct(int value) : first(value), second(value + 1) { 
			//defaultCtorCount_;  
			//objectCount_++; 
		}
		SimpleStruct() : first(0) { objectCount_++; }
		SimpleStruct(SimpleStruct const &rhs) : first(rhs.first), second(rhs.second) { 
			//std::cout << "ctor!\n";
			//objectCount_++; 
			//assignCount_++;
		}
		//SimpleStruct & operator=(SimpleStruct const &rhs) { 
			//first = rhs.first; 
			//second = rhs.second;
			//assignCount_++;
			//return *this;
		//}
		~SimpleStruct() { objectCount_--; }
		static void countObjects();
		char operator() (int b) { return '5'; }
		operator int() const { return first; }
		int first;
		int second;
		size_t x;
		size_t y;
		double x1;
		//double x2;
		//int array[10];

		// for checking memory leak
		static int objectCount_; 

		// for performance test		
		static size_t defaultCtorCount_; 
		static size_t copyCtorCount_;
		static size_t assignCount_;
	};

	

	struct StringStruct : public SimpleStruct {
		//StringStruct(): value(getRandomString(5)) {
			//std::cout << "default con\n";
		//}
		StringStruct(int value) : SimpleStruct(value), str_(lib_calvin_container::getRandomString(5)) {
			//std::cout << "default con\n";
		}
		StringStruct(StringStruct const &rhs): SimpleStruct(rhs), str_(rhs.str_) {
			//std::cout << "copy con\n";
		}
		StringStruct(StringStruct const &&rhs) : SimpleStruct(rhs), str_(std::move(rhs.str_)) {
			//std::cout << "copy con\n";
		}
		StringStruct & operator=(StringStruct &&rhs) {
			str_ = std::move(rhs.str_);
			return *this;
		}
		StringStruct & operator=(StringStruct const &rhs) {
			str_ = rhs.str_;
			return *this;
		}
		operator std::string() const { return str_; }
		std::string str_;
	};

	bool operator< (SimpleStruct const &lhs, SimpleStruct const &rhs);
	bool operator== (SimpleStruct const &lhs, SimpleStruct const &rhs);

	bool operator< (StringStruct const &lhs, StringStruct const &rhs);
	bool operator== (StringStruct const &lhs, StringStruct const &rhs);

	template <typename T, typename Comparator = std::less<T>>
	void sortTest(void(*sortingAlg)(T *first, T *last, Comparator comp),
		std::string title, Comparator comp = Comparator());

	template <typename Iterator, typename Comparator>
	void sortTest(void(*sortingAlgorithm)(Iterator, Iterator, Comparator),
		Iterator first, Iterator last, std::string title,
		double &time, bool &isCorrect, bool &isStable, Comparator comp);


	void sortTest2();

	template <typename Iterator, typename Comparator>
	void sortTest2Sub(void(*sortingAlgorithm)(Iterator, Iterator, Comparator),
		Iterator begin, Iterator end, Comparator comp);

} // end namespace lib_calvin_sort


template <typename T, typename Comparator>
void lib_calvin_sort::sortTest(void(*sortingAlg) (T *first, T *last, Comparator comp), std::string title,
	Comparator comp) {
	stopwatch watch;
	bool correct = true;
	bool stable = true;	
	std::vector<int> const arraySize = { 100, 1000, 10000, 1000000 };
	std::vector<int> const numIteration = { 1000, 100, 10, 3};
	size_t const numTestCases = arraySize.size();
	std::srand(1232); // reset seed to give identical input to algorithms
	for (int i = 0; i < numTestCases; ++i) {
		if (arraySize[i] == 0) {
			continue;
		}
		T *testSet = static_cast<T *>(operator new(sizeof(T) * arraySize[i]));
		double min = 1000000;
		bool isTotallyCorrect = true;
		bool isTotallyStable = true;
		size_t numCopyCtor = 0;
		size_t numAssign = 0;

		for (int j = 0; j < numIteration[i]; j++) {
			for (int j = 0; j < arraySize[i]; j++) {
				new (testSet + j) T(j);
				//testSet[j].first = -j / 3; // reverse ordered input array
			}
			std::shuffle(testSet, testSet + arraySize[i], std::mt19937(std::random_device()()));
			for (int j = 0; j < arraySize[i]; j++) {
				//testSet[j].second = j; // for testing stability
			}
			double time = 0;
			bool isCorrect = true;
			bool isStable = true;

			SimpleStruct::copyCtorCount_ = 0;
			SimpleStruct::assignCount_ = 0;
			sortTest(sortingAlg, testSet, testSet + arraySize[i], title, time, isCorrect, isStable, comp);
			numCopyCtor = SimpleStruct::copyCtorCount_;
			numAssign = SimpleStruct::assignCount_;

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
		operator delete(testSet);
		cout << title << ": ";
		if (isTotallyCorrect == true) {
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
		cout << arraySize[i] << " speed: " << arraySize[i] / min << " ( " <<
			min << " ) # default ctors: " <<
			SimpleStruct::defaultCtorCount_ / 1000000.0 << "m" << " # copy ctors: " <<
			SimpleStruct::copyCtorCount_ / 1000000.0 << "m" << " # assignment: " <<
			SimpleStruct::assignCount_ / 1000000.0 << "m" << "\n";
	}
	std::cout << endl;
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::sortTest(void(*sortingAlgorithm)(Iterator, Iterator, Comparator),
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
			//std::cout << "Sorting wrong!\n" << std::to_string(*(first + (j + 1)));
			return;
			exit(0);
		}
	}
}



template <typename Iterator, typename Comparator>
void lib_calvin_sort::sortTest2Sub(void(*sortingAlgorithm)(Iterator, Iterator, Comparator),
	Iterator begin, Iterator end, Comparator comp) {

	sortingAlgorithm(begin, end, comp);


}

#endif

