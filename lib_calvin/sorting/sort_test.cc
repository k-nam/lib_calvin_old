#include <iostream>

#include "sort_test.h"
#include "utility.h"
#include "random.h"
#include "pdqsort.h"
#include "bench.h"

#include "insertion_sort.h"
#include "intro_sort.h"
#include "intro_sort_parallel.h"
#include "merge_sort.h"
#include "merge_sort_parallel.h"
#include "in_place_merge_sort.h"
#include "in_place_merge_sort_parallel.h"
#include "heap_sort.h"
#include "counting_sort.h"
#include "sample_sort.h"

#include "boost/sort/sort.hpp"

void lib_calvin_sort::sortTest() {
	std::cout << "---------------- Beginning sort test -----------------\n\n";
	using namespace lib_calvin_sort;
	using namespace lib_calvin_benchmark;

	//typedef size_t ElemType;
	
	//typedef object_vector ElemType;

	typedef object_16 ElemType;
	//typedef StringStruct ElemType;
	
	//binSearchTest();
	//binSearchTest2();
	//getSamplesTest();

	//sortTest<ElemType>(boost::sort::spinsort, "spinsort");
	//sortTest<ElemType>(boost::sort::flat_stable_sort, "flat_stable_sort");
	//sortTest<ElemType>(boost::sort::pdqsort_branchless, "pdqsort");

	//sortTest<ElemType>(introSort, "introSort");
	sortTest<ElemType>(stableBlockIntroSort, "stableBlockIntroSort");
	sortTest<ElemType>(blockIntroSort, "blockIntroSort");
	//sortTest<ElemType>(pdqsort_branchless, "pdqsort_branchless");
	//sortTest<ElemType>(boost::sort::block_indirect_sort, "block_indirect_sort parallel");
	//sortTest<ElemType>(introSortParallel, "Parallel introSort");
	//sortTest<ElemType>(introSortParallelAdvanced, "Advanced parallel introSort");
	//sortTest<ElemType>(introSortParallelAdvanced2, "Advanced2 parallel introSort");
	sortTest<ElemType>(mergeSort, "mergeSort");
	//sortTest<ElemType>(mergeSort2, "mergeSort 2");
	sortTest<ElemType>(boost::sort::sample_sort, "sample_sort");
	//sortTest<ElemType>(boost::sort::parallel_stable_sort, "parallel_stable_sort");
	sortTest<ElemType>(mergeSortParallel, "Parallel mergeSort");
	//sortTest<ElemType>(inPlaceMergeSortParallel, "Parallel In-place mergesort");
	sortTest<ElemType>(heapSort, "heapSort");
	sortTest<ElemType>(inPlaceMergeSort, "inPlaceMergeSort");
	//sortTest<ElemType>(std::sort, "std::sort");
	//sortTest<ElemType>(std::stable_sort, "std::stable_sort");
	//sortTest<ElemType>(pdqsort, "pdqsort");
	

	//sortTest<ElemType>(sampleSort, "sampleSort");
	//sortTest<ElemType>(countingSort, "countingSort");
	//sortTest<ElemType>(introCountingSort, "introCountingSort");

	//sortTest2();

	//ElemType::countObjects();

	//lib_calvin_sort::fileTest();
	std::cout << "---------------- Sort test finished -----------------\n\n\n";
}

void lib_calvin_sort::sortTest2() {
	using namespace std;
	std::vector<std::string> strings{ "bb", "a", "ddd", "ee", "cccc" };
	auto comp = std::less<std::string>();
	auto lengthComp = [](string x, string y) { return x.length() < y.length(); };

	cout << "mergeSort\n";
	cout << "lexicographic order:\n";
	sortTest2Sub(lib_calvin_sort::mergeSort, strings.begin(), strings.end(), comp);
	for_each(strings.begin(), strings.end(), [](string x) { cout << x << " "; });
	cout << "\n";
	cout << "length order:\n";
	sortTest2Sub(lib_calvin_sort::mergeSort, strings.begin(), strings.end(), lengthComp);
	for_each(strings.begin(), strings.end(), [](string x) { cout << x << " "; });
	cout << "\n\n";

	cout << "introSort\n";
	cout << "lexicographic order:\n";
	sortTest2Sub(lib_calvin_sort::introSort, strings.begin(), strings.end(), comp);
	for_each(strings.begin(), strings.end(), [](string x) { cout << x << " "; });
	cout << "\n";
	cout << "length order:\n";
	sortTest2Sub(lib_calvin_sort::introSort, strings.begin(), strings.end(), lengthComp);
	for_each(strings.begin(), strings.end(), [](string x) { cout << x << " "; });
	cout << "\n\n";


}




void lib_calvin_sort::binSearchTest() {
	vector<int> partitions;
	int partitioners[] = { 1, 3, 5, 7, 9, 11, 13 };
	for (int i = 0; i < 7; ++i) {
		partitions.push_back(partitioners[i]);
	}
	bool isCorrect = true;
	for (int i = 0; i < 15; ++i) {
		size_t index = binSearch(i, &*partitions.begin(),
			&*partitions.begin() + partitions.size());
		std::cout << "i is: " << i << " index is: " << index << std::endl;
		if (index > 0 && index < partitions.size()) {
			if (i > partitions[index - 1] && i <= partitions[index]) {
			} else {
				isCorrect = false;
			}
		} else if (index == 0) {
			if (i <= partitions[0]) {
			} else {
				isCorrect = false;
			}
		} else if (index == partitions.size()) {
			if (i > partitions[index - 1]) {
			} else {
				isCorrect = false;
			}
		}
	}
	if (isCorrect) {
		std::cout << "binSearch OK\n";
	} else {
		std::cout << "binSearch ERROR\n";
		exit(0);
	}
}

void lib_calvin_sort::binSearchTest2() {
	int arraySize = 10000000;
	int sampleSize[5] = { 4,  16, 64, 256, 1024 };
	int *pArray = new int[arraySize];
	for (int i = 0; i < arraySize; ++i) {
		pArray[i] = rand();
	}
	for (int i = 0; i < 5; ++i) {
		int *pSample = new int[sampleSize[i]];
		for (int j = 0; j < sampleSize[i]; j++) {
			pSample[j] = rand();
		}
		introSort(pSample, pSample + sampleSize[i]);
		vector<int> samples;
		for (int j = 0; j < sampleSize[i]; ++j) {
			samples.push_back(pSample[j]);
		}
		//samples.insert(samples.begin(), pSample, pSample + sampleSize[i]);
		stopwatch watch;
		watch.start();
		size_t sum = 0;
		for (int j = 0; j < arraySize; j++) {
			//binSearch(pArray[j], pSample, pSample + sampleSize[i]);
			sum += binSearch(pArray[j], samples.begin(), samples.end());
		}
		watch.stop();
		std::cout << sum << " arraySize: " << arraySize << " sampleSize: " <<
			sampleSize[i] << " binSearch took: " << watch.read() <<
			" binSearch speed is: " << arraySize / watch.read() << std::endl;
	}
}

void lib_calvin_sort::getSamplesTest() {
	int elems[] = { 12, 12, 1, 19, 3, 6, 4, 3, 10, 8, 2,5,9 };
	vector<int> result;
	getSamples(elems, elems + 12, 1, result);
	//lib_calvin_util::printContainer(result, "samples");
}

int lib_calvin_sort::SimpleStruct::objectCount_ = 0;

size_t lib_calvin_sort::SimpleStruct::defaultCtorCount_ = 0;
size_t lib_calvin_sort::SimpleStruct::copyCtorCount_ = 0;
size_t lib_calvin_sort::SimpleStruct::assignCount_ = 0;

void lib_calvin_sort::SimpleStruct::countObjects() {
	if (objectCount_ != 0) {
		std::cout << "SimpleStruct object memory leak! # was: " << objectCount_ << "\n";
		exit(0);
	} else {
		std::cout << "SimpleStruct object memory OK!\n";
	}
	std::cout << "\n";
}

bool lib_calvin_sort::operator< (SimpleStruct const &lhs, SimpleStruct const &rhs) {
	if (lhs.first < rhs.first)
		return true;
	return false;
}

bool lib_calvin_sort::operator== (SimpleStruct const &lhs, SimpleStruct const &rhs) {
	if (lhs.first == rhs.first)
		return true;
	return false;
}


bool lib_calvin_sort::operator< (StringStruct const &lhs, StringStruct const &rhs) {
	if (lhs.str_ < rhs.str_)
		return true;
	return false;
}

bool lib_calvin_sort::operator== (StringStruct const &lhs, StringStruct const &rhs) {
	if (lhs.str_ == rhs.str_)
		return true;
	return false;
}

