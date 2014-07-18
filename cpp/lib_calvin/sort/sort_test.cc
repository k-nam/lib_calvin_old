
#include "sort_test.h"
#include "utility.h"
#include <iostream>

void lib_calvin_sort::sortTest() {
	std::cout << "\n---------- Beginning sort test -----------\n";
	typedef SimpleStruct ElemType;
	//binSearchTest();
	//binSearchTest2();
	getSamplesTest();

	//sortTest<ElemType>(cacheTest1, "cacheTest1");
	//sortTest<ElemType>(cacheTest2, "cacheTest2");
	//sortTest<ElemType>(cacheTest3, "cacheTest3");
	
  sortTest<ElemType>(introSortPointerSorting, "introSort pointer");
	sortTest<ElemType>(introSort, "introSort");
	sortTest<ElemType>(introSortParallel, "Parallel introSort");	
	sortTest<ElemType>(introSortParallelAdvanced, "Advanced parallel introSort");	
	sortTest<ElemType>(introSortParallelAdvanced2, "Advanced2 parallel introSort");
	sortTest<ElemType>(mergeSort, "mergeSort");
	sortTest<ElemType>(mergeSortParallel, "Parallel mergeSort");
	sortTest<ElemType>(bucketSort, "bucketSort");
	sortTest<ElemType>(heapSort,  "heapSort");
	sortTest<ElemType>(inPlaceMergeSort, "inPlaceMergeSort");
	sortTest<ElemType>(mergeSort2, "mergeSort2");
	sortTest<ElemType>(std::sort, "std::sort");
	sortTest<ElemType>(std::stable_sort, "std::stable_sort");
	
	sortTest<ElemType>(countingSort, "countingSort");
	sortTest<ElemType>(introSort2, "introSort+");
}

template <typename T>
void lib_calvin_sort::sortTest (void (*sortingAlg) (T *first, T *last), 
    std::string title) {  
	size_t const numTestCases = 4;
  stopwatch watch;
  bool correct  = true;
  bool stable   = true;
	int arraySize[] = { 1000, 10000, 100000, 1000000 };
	int numIteration[] = { 100, 100, 10, 3 };
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
			std::random_shuffle(testSet, testSet + arraySize[i]);
			for (int j = 0; j < arraySize[i]; j++) {
				testSet[j].second = j; // for testing stability
			}	
			double time = 0;
			bool isCorrect = true;
			bool isStable = true;
			sortTest(sortingAlg, testSet, testSet + arraySize[i], title,
				time, isCorrect, isStable);
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

template <typename Iterator>
void lib_calvin_sort::sortTest (void (*sortingAlgorithm)(Iterator, Iterator), 
		Iterator first, Iterator last, std::string title,
		double &time, bool &isCorrect, bool &isStable) {
	stopwatch watch;
	ptrdiff_t size = last - first; 
  watch.start();
  sortingAlgorithm(first, last);
  watch.stop();
  time = watch.read();
  // check correctness
  for (ptrdiff_t j = 0; j < size - 1; j++) {
    if (*(first + (j + 1)) < *(first + j)) {
      isCorrect = false;
		}
    if (*(first + j) == *(first + j + 1) && 
				(*(first + j)).second > (*(first + (j + 1))).second) {
      isStable = false;
		}
  }
}

std::string lib_calvin_sort::getRandomString(int length) {
	char *charArray = new char[length];
	for (int i = 0; i < length; ++i) {
		charArray[i] = 'a' + rand() % 26;
	}
	std::string result(charArray, length);
	delete[] charArray;
	return result;
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
			if (i > partitions[index- 1] && i <= partitions[index]) {
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
	lib_calvin_util::printContainer(result, "samples");
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
	if (lhs.value < rhs.value)
		return true;
	return false;
}

bool lib_calvin_sort::operator== (StringStruct const &lhs, StringStruct const &rhs) {
	if (lhs.value == rhs.value)
		return true;
	return false;
}


