#include <utility>
#include <iostream>
#include <vector>
#include <list>

#include "vector_test.h"
#include "vector.h"
#include "stopwatch.h"
#include "array.h"
#include "blocked_array.h"
#include "container_test.h"
#include "rb_tree.h"
#include "container_test_types.h"


using std::pair;

void lib_calvin_container::vectorTest() {
	size_t const smallTestSize = 10000;
	size_t const largeTesetSize = 100000;

	vectorBasicTest();

	//vectorFunctionTest(std::vector<HeavyObject>(), "std::vector");
	vectorFunctionTest(lib_calvin_container::Array<HeavyObject>(), "lib_calvin_container::Array");
	//vectorFunctionTest(lib_calvin_container::BlockedArray<HeavyObject>(), "lib_calvin_container::BlockedArray");

	//vectorIteratorTest(lib_calvin_container::Array<pair<int, int>>(), "lib_calvin_container::Array");
	//vectorIteratorTest(lib_calvin_container::BlockedArray<pair<int, int>>(), "lib_calvin_container::BlockedArray");

	//vectorPerformanceTest(lib_calvin_container::Array<HeavyObject>(), 
		//"lib_calvin_container::Array / Heavy", smallTestSize);
	//vectorPerformanceTest(lib_calvin_container::BlockedArray<HeavyObject>(),
		//"lib_calvin_container::BlockedArray / Heavy", smallTestSize);
	//vectorPerformanceTest(std::vector<HeavyObject>(), "std::vector / Heavy", smallTestSize);
	//vectorPerformanceTest(std::list<HeavyObject>(), "std::list / Heavy", smallTestSize);
	
	//vectorPerformanceTest(lib_calvin_container::Array<int>(), 
		//"lib_calvin_container::Array / int", largeTesetSize);
	//vectorPerformanceTest(lib_calvin_container::BlockedArray<int>(), 
		//"lib_calvin_container::BlockedArray / int", largeTesetSize);
	//vectorPerformanceTest(std::vector<int>(), "std::vector / int", largeTesetSize);
	//vectorPerformanceTest(std::list<int>(), "std::list / int", largeTesetSize);

	//vectorPerformanceTest2(lib_calvin_container::Array<int>(), "lib_calvin_container::Array", testSize);
	//vectorPerformanceTest3(lib_calvin_container::Array<int>(), "lib_calvin_container::Array", testSize);

	HeavyObject::countThisObject();
}

void lib_calvin_container::vectorBasicTest() {
	Array<Array<HeavyObject>> a;
	a.resize(2);
	a[1] = Array<HeavyObject>(2);
	a.push_back(Array<HeavyObject>(3));
}

template <typename Impl>
void lib_calvin_container::vectorFunctionTest(Impl && impl, std::string title) {
	using std::cout;
	std::cout << "Starting vector function test for: " << title << "\n";
	impl.clear();
	int const testSize = 100;
	// basic interface
	for (int i = 0; i < testSize; ++i) {
		typename Impl::value_type temp(i);
		impl.push_back(temp);
		if (impl.back() != temp) {
			cout << "vector push_back/back error\n";
			exit(0);
		}
	}
	cout << "vector push_back/back() OK!\n";

	for (int i = 0; i < testSize; ++i) {
		if (*advanceIteratorBy(impl.begin(), i) != typename Impl::value_type(i)) {
			cout << "vector random access error\n";
			exit(0);
		}
		if (impl[i] != typename Impl::value_type(i)) {
			cout << "vector random access2 error\n";
			exit(0);
		}
	}
	cout << "vector random access  OK!\n";

	for (int i = 0; i < testSize; ++i) {
		impl.pop_back();
		if (impl.size() != testSize - 1 - i) {
			cout << "vector pop_back error\n";
			exit(0);
		}
	}
	cout << "vector pop_back OK!\n";

	
	// additional interface
	// insert single element
	int numIteration = 100;
	std::vector<typename Impl::value_type> stdVector;
	for (int iter = 0; iter < numIteration; ++iter) {
		int const arraySize = rand() % testSize + 1; // prevent 0
		resetVectorForSize(impl, arraySize);
		resetVectorForSize(stdVector, arraySize);
		int const positionToInsert = (rand() % arraySize + rand() + 30) % arraySize;
		typename Impl::value_type temp(iter);
		for (int j = 0; j < arraySize; ++j) { // force reserving
			impl.insert(advanceIteratorBy(impl.begin(), positionToInsert), temp);
			stdVector.insert(advanceIteratorBy(stdVector.begin(), positionToInsert), temp);
		}
		if (!areSame(impl, stdVector)) {
			cout << "vector insert single elem error\n";
			exit(0);
		}
	}
	cout << "vector insert single OK!\n";
	
	
	// insert multiple elements
	for (int i = 0; i < numIteration; ++i) {
		int const arraySize = rand() % testSize + 1;
		resetVectorForSize(impl, arraySize);
		resetVectorForSize(stdVector, arraySize);
		int const positionToInsert = rand() % arraySize;
		int const insertionBlockLength = rand() % testSize;
		Impl insertionBlock1;
		decltype(stdVector) insertionBlock2;
		resetVectorForSize(insertionBlock1, insertionBlockLength);
		resetVectorForSize(insertionBlock2, insertionBlockLength);
		
		impl.insert(advanceIteratorBy(impl.begin(), positionToInsert), 
			insertionBlock2.begin(), insertionBlock2.end());
		stdVector.insert(advanceIteratorBy(stdVector.begin(), positionToInsert), 
			insertionBlock1.begin(), insertionBlock1.end());
			
		if (!areSame(impl, stdVector)) {
			cout << "vector insert multi elem error\n";
			exit(0);
		}
	}
	cout << "vector insert multi OK!\n";
	
	for (int i = 0; i < numIteration; ++i) {
		int const arraySize = rand() % testSize + 1; // prevent 0
		resetVectorForSize(impl, arraySize);
		resetVectorForSize(stdVector, arraySize);
		for (int i = 0; i < arraySize / 10; ++i) { 		
			int const positionToErase = rand() % impl.size();
			impl.erase(advanceIteratorBy(impl.begin(), positionToErase));
			stdVector.erase(advanceIteratorBy(stdVector.begin(), positionToErase));
		}
		if (!areSame(impl, stdVector)) {
			cout << "vector erase single elem error\n";
			exit(0);
		}
	}
	cout << "vector erase single OK!\n";

	Impl impl2(impl);
	Impl impl3;
	impl3 = impl;
	if (impl != impl2) {
		cout << "vector copy ctor error\n";
		exit(0);
	} else {
		cout << "vector copy ctor OK!\n";
	}

	if (impl != impl3) {
		cout << "vector assignment error\n";
		exit(0);
	} else {
		cout << "vector assignment OK!\n";
	}

	
	std::set<typename Impl::value_type> intSet;
	intSet.insert(typename Impl::value_type(3));
	intSet.insert(typename Impl::value_type(5));
	intSet.insert(typename Impl::value_type(7));
	
	typedef typename decltype(intSet.begin())::iterator_category tag;
	Impl impl4(intSet.begin(), intSet.end());
	std::vector<size_t> temp(intSet.begin(), intSet.end());
	if (containerEqual(impl4.begin(), impl4.end(), intSet.begin(), intSet.end())) {
		cout << "vector iterator ctor OK!\n";
	} else {
		cout << "vector iterator ctor error\n";
		exit(0);
	}

	Impl impl5 { typename Impl::value_type(3), typename Impl::value_type(5), typename Impl::value_type(7) };
	//std::vector<Impl::value_type> ss { Impl::value_type(3), Impl::value_type(5), Impl::value_type(7) };
	if (containerEqual(impl5.begin(), impl5.end(), intSet.begin(), intSet.end())) {
		cout << "vector initializer list ctor OK!\n";
	} else {
		cout << "vector initializer list ctor error\n";
		exit(0);
	}

	impl.clear();
	if (impl.size() != 0) {
		cout << "vector clear error\n";
		exit(0);
	} else {
		cout << "vector clear OK!\n";
	}
	
	numIteration = 100;
	for (int i = 0; i < numIteration; ++i) {
		int const arraySize = rand() % testSize + 1;
		resetVectorForSize(impl, arraySize);
		int index1 = rand() % arraySize;
		int index2 = rand() % arraySize;
		int indexDiff = index2 - index1;
		ptrdiff_t ptrDiff = (impl.begin() + index2) - (impl.begin() + index1);
		if (indexDiff != ptrDiff) {
			cout << "vector iterator minus operator error\n";
			exit(0);
		}
	}
	cout << "vector iterator minus operator OK!!!\n";



	cout << "vector function OK!!!\n\n";
}

template <typename Impl>
void lib_calvin_container::vectorIteratorTest(Impl && impl, std::string title) {
	using std::pair;
	using std::cout;

	cout << "Starting vector iterator test for: " << title << "\n";

	Impl const &const_reference = impl;

	// vector's iterator tests
	impl.push_back(pair<int, int>(3, 1));
	impl.push_back(pair<int, int>(6, 1));
	impl.push_back(pair<int, int>(7, 1));

	typename Impl::iterator iter1 = impl.begin();
	typename Impl::iterator iter2 = impl.end();
	typename Impl::const_iterator c_iter1 = const_reference.begin();
	typename Impl::const_iterator c_iter2 = const_reference.end();
	
	c_iter1 = iter1;
	c_iter2 = iter2;

	typename Impl::reverse_iterator r_iter1 = impl.rbegin();
	typename Impl::reverse_iterator r_iter2 = impl.rend();
	typename Impl::const_reverse_iterator c_r_iter1 = const_reference.rbegin();
	typename Impl::const_reverse_iterator c_r_iter2 = const_reference.rend();

	c_r_iter1 = r_iter2;
	c_r_iter1 = iter2;
	c_r_iter2 = r_iter1;
	c_r_iter2 = iter1;

	iter1 = r_iter2;
	iter2 = r_iter1;

	int temp = 0;
	int temp2 = 0;
	for (c_r_iter1 = const_reference.rbegin(); c_r_iter1 != const_reference.rend(); ++c_r_iter1) {
		std::cout << "a's first: " << c_r_iter1->first << "\n";
		temp += c_r_iter1->first;		
		temp2 += c_r_iter1->second;
	}
	for (r_iter1 = impl.rbegin(); r_iter1 != impl.rend(); ++r_iter1) {
		std::cout << "a's first: " << r_iter1->first << "\n";
		temp += r_iter1->first;
		temp2 += r_iter1->second;
	}
	std::cout << "sum is " << temp << ", " << temp2 << "\n";

	std::cout << "size of container is " << const_reference.size() << ", size by iterator is " << 
		const_reference.end() - const_reference.begin() << "\n";

	std::cout << "\n";
}

template <typename Impl>
void lib_calvin_container::vectorPerformanceTest(Impl && impl, std::string title, size_t testSize) {
	using std::cout;
	cout << "Starting vector performance test for: " << title << "\n";
	cout << "size of container is " << sizeof(impl) << "\n";
	typedef typename Impl::value_type T;

	lib_calvin::stopwatch watch;
	watch.start();
	for (size_t i = 0; i < testSize; ++i) {
		impl.push_back(T(static_cast<int>(i)));
	}
	watch.stop();
	cout << "push_back speed is: " << testSize / watch.read() << "/s\n";

	typename Impl::value_type temp = 0;
	watch.start();
	for (auto iter = impl.begin(); iter != impl.end(); ++iter) {
		temp = *iter;
	}
	watch.stop();
	cout << temp << " sequential access speed is: " << 
		testSize / watch.read() << "/s\n";
	/*
	int const numIterationForRandomAccess = static_cast<int>(testSize) / 10;
	watch.start();
	for (size_t i = 0; i < numIterationForRandomAccess; ++i) {
		temp = *advanceIteratorBy(impl.begin(), rand() % impl.size());
	}
	watch.stop();
	cout << temp << " random access speed is: " << 
		numIterationForRandomAccess / watch.read() << "/s\n";
	*/
	int const numIterationForSingleInsertion = static_cast<int>(testSize) / 3;
	double totalTime = 0;
	for (int i = 0; i < numIterationForSingleInsertion; ++i) {
		auto pos = advanceIteratorBy(impl.begin(), rand() % impl.size());
		watch.start();
		impl.insert(pos, rand());
		watch.stop();
		totalTime += watch.read();
	}
	cout << temp << " single insertion speed is: " << numIterationForSingleInsertion / totalTime << "/s\n";
	/*
	totalTime = 0;
	for (int i = 0; i < numIterationForSingleInsertion; ++i) {
		auto pos = advanceIteratorBy(impl.begin(), rand() % impl.size());
		watch.start();
		impl.erase(pos);
		watch.stop();
		totalTime += watch.read();
	}
	cout << temp << " single erase speed is: " << numIterationForSingleInsertion / totalTime << "/s\n";	
	*/
	/*
	std::vector<Impl::value_type> valuesToInsert;
	resetVectorForSize(valuesToInsert, impl.size() / 10);
	int const numIterationForMultiInsertion = 10;
	watch.start();
	for (int i = 0; i < numIterationForMultiInsertion; ++i) {
		impl.insert(advanceIteratorBy(impl.begin(), rand() % impl.size()), 
									valuesToInsert.begin(), advanceIteratorBy(valuesToInsert.begin(), rand() % valuesToInsert.size()));
	}
	watch.stop();
	cout << temp << " multiple insertion speed is: " << numIterationForMultiInsertion / watch.read() << "/s\n";
	*/
	
	watch.start();
	for (auto iter = impl.begin(); iter != impl.end(); ++iter) {
		temp = *iter;
	}
	watch.stop();
	cout << temp << " sequential access after modification, speed is: " << 
		testSize / watch.read() << "/s\n";

	cout << "\n";
}

template <typename Impl>
void lib_calvin_container::vectorPerformanceTest2(Impl && impl, std::string title, size_t testSize) {
	using lib_calvin::vector;
  using std::cout;
	cout << "Starting vector performance test 2 for: " << title << "\n";

	lib_calvin::stopwatch watch;
	std::vector<std::vector<int>> v;
	watch.start();
	for (size_t i = 0; i < testSize; ++i) {
		v.push_back(std::vector<int>(3, (int)i));
	}
	watch.stop();
	cout << "push_back speed is: " << testSize / watch.read() << "/s\n";

	watch.start();
	int temp = 0;
	for (size_t i = 0; i < testSize; ++i) {
		temp += v[i][1];
	}
	watch.stop();
	cout << temp << "sequential access speed is: " << 
		testSize / watch.read() << "/s\n";
	cout << "\n";
}

template <typename Impl>
void lib_calvin_container::vectorPerformanceTest3(Impl && impl, std::string title, size_t testSize) {
	// Comparison with naked pointer
	using std::cout;
	cout << "Starting vector performance test 3 (for simple pointer) for: " << title << "\n";
	size_t size = testSize;
	int *p = new int[size];
	for (size_t i = 0; i < size; ++i) {
		p[i] = (int)i; 
	}
	int temp = 0;
	lib_calvin::stopwatch watch;
	watch.start();
	for (size_t i = 0; i < size; ++i) {
		temp += p[i];
	}
	watch.stop();
	delete[] p;
	std::cout << temp << "pointer sequential access speed is " << 
		size / watch.read() << "\n";
	cout << "\n";
}

template <typename T1, typename T2>
bool lib_calvin_container::areSame(T1 const &lhs, T2 const &rhs) {
	if (lhs.size() != rhs.size()) {
		return false;
	}
	size_t index = 0;
	auto lhsIter = lhs.begin();
	auto rhsIter = rhs.begin();
	for ( ; lhsIter != lhs.end(); ++lhsIter, ++rhsIter) {
		index++;
		if (*lhsIter != *rhsIter) {
			return false;
		}
	}
	return true;
}

template <typename Impl> 
void lib_calvin_container::resetVectorForSize(Impl &impl, size_t size) {
	impl.resize(size);
	int i = 0;
	auto iter = impl.begin();
	for ( ; iter != impl.end(); ++i, ++iter) {
		*iter = typename Impl::value_type(i);
	}

}


