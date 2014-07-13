#include <cstdlib>
#include <random>
#include <set>
#include <unordered_set>
#include "boost/unordered_set.hpp"
#include "boost/container/set.hpp"
#include "set_test.h"
#include "utility.h"
#include "stopwatch.h"
#include "container_test.h"
#include "bin_tree.h"
#include "b_tree.h"
#include "b_plus_tree.h"
#include "set.h"
#include "rb_tree.h"  
#include "ptr_set.h"
#include "hash_table.h"
#include "blocked_array.h"
#include "ordered_array.h"

using lib_calvin::stopwatch;
using std::cout;
using std::endl;
using std::vector;

void lib_calvin_container::setTest() {	
	//setRvalueTest(RbTree<HeavyObjectWithMessage>(), "RbTree / HeavyObject");
	//setRvalueTest(BTree<HeavyObjectWithMessage>(), "BTree / HeavyObject");
	//setRvalueTest(BPlusTree<HeavyObjectWithMessage>(), "BPlusTree / HeavyObject");

	int const testSize = 1000;
	setFunctionTest(lib_calvin::set<int>(), testSize, "lib_calvin::set");
	//setFunctionTest(BinTree<int>(), testSize, "lib_calvin_container::BinTree");
	setFunctionTest(RbTree<HeavyObject>(), testSize, "lib_calvin_container::RbTree");
	setFunctionTest(BTree<HeavyObject>(), testSize, "lib_calvin_container::BTree");
	setFunctionTest(BPlusTree<HeavyObject>(), testSize, "lib_calvin_container::BPlusTree");
	setFunctionTest(OrderedArray<HeavyObject>(), testSize, "lib_calvin_container::OrderedArray");
	//setFunctionTest(PtrSet<int>(), testSize, "lib_calvin_container::PtrSet");
	//setFunctionTest(HashTable<int>(), testSize,	"lib_calvin_container::HashTable");
	//setFunctionTest(HashTable<HeavyObject>(), testSize, "lib_calvin_container::HashTable");
	
	//setIteratorTest(RbTree<int>(), "RbTree iterator");
	//setIteratorTest(BTree<int>(), "BTree iterator");
	//setIteratorTest(HashTable<int>(), "HashTable iterator");

	int const smallSize = 1000;
	int const mediumSize = 10000;
	int const largeSize = 1000000;	
	//setPerformanceTest(std::set<int>(), largeSize, "std::set / int");
	//setPerformanceTest(boost::container::set<int>(), largeSize, "boost::set / int");
	//setPerformanceTest(BinTree<int>(), largeSize, "RbTree / int");
	//setPerformanceTest(RbTree<int>(), largeSize, "RbTree / int");
	//setPerformanceTest(BTree<int>(), largeSize, "BTree / int");
	//setPerformanceTest(BPlusTree<int>(), largeSize, "BPlusTree / int");
	
	//setPerformanceTest(OrderedArray<int>(), largeSize, "OrderedArray / int");
	//setPerformanceTest(std::unordered_set<int>(), largeSize, "std::unordered_set / int");
  //setPerformanceTest(boost::unordered_set<int>(), largeSize, "boost::unordered_set / int");
	//setPerformanceTest(HashTable<int>(), largeSize, "HashTable / int");

	setPerformanceTest(std::set<LightObject>(), largeSize, "std::set / LightObject");
	//setPerformanceTest(boost::container::set<LightObject>(), largeSize, "boost::set / LightObject");
	//setPerformanceTest(RbTree<LightObject>(), largeSize, "RbTree / LightObject");
	setPerformanceTest(BTree<LightObject>(), largeSize, "BTree / LightObject");
	//setPerformanceTest(BPlusTree<LightObject>(), largeSize, "BPlusTree / LightObject");
	//setPerformanceTest(OrderedArray<LightObject>(), largeSize, "OrderedArray / LightObject");

	setPerformanceTest(std::set<HeavyObject>(), mediumSize, "std::set / HeavyObject");
	//setPerformanceTest(boost::container::set<HeavyObject>(), mediumSize, "boost::set / HeavyObject");
	//setPerformanceTest(RbTree<HeavyObject>(), mediumSize, "RbTree / HeavyObject");
	setPerformanceTest(BTree<HeavyObject>(), mediumSize, "BTree / HeavyObject");
	//setPerformanceTest(BPlusTree<HeavyObject>(), mediumSize, "BPlusTree / HeavyObject");
	//setPerformanceTest(OrderedArray<LightObject>(), mediumSize, "OrderedArray / HeavyObject");
	//setPerformanceTest(HashTable<HeavyObject, GenericHash<HeavyObject>>(), mediumSize, "HashTable / HeavyObject");
	//setPerformanceTest(boost::unordered_set<HeavyObject, GenericHash<HeavyObject>>(), 
		//mediumSize, "boost::unordered_set / HeavyObject");
	//setPerformanceTest(std::unordered_set<HeavyObject>(), mediumSize, "std::unordered_set / HeavyObject");
	//setPerformanceTest(PtrSet<HeavyObject>(), mediumSize, "PtrSet / HeavyObject");
	
	//setIntegratedSpeedTest(std::set<LightObject>(), smallSize, "std::set");
	//setIntegratedSpeedTest(boost::container::set<LightObject>(), smallSize, "boost:set");
	//setIntegratedSpeedTest(RbTree<LightObject>(), smallSize, "RbTree");
	
	//setIntegratedSpeedTest(BTree<LightObject>(), smallSize, "BTree");
	//setIntegratedSpeedTest(BPlusTree<LightObject>(), smallSize, "BPlusTree");
	//setIntegratedSpeedTest(OrderedArray<LightObject>(), smallSize, "OrderedArray");

	//setMemoryTest(lib_calvin_container::RbTree<HeavyObject>(), "RbTree");
	//setMemoryTest<lib_calvin_container::BTree<HeavyObject>>("BTree");
	//setMemoryTest<lib_calvin_container::PtrSet<HeavyObject>>("PtrSet");
	//setMemoryTest<lib_calvin_container::link_hash_set<HeavyObject>>("link_hash_set");
	//setMemoryTest<lib_calvin_container::HashTable<HeavyObject>>("HashTable");

	randomAccessSpeedTest(largeSize);

	HeavyObject::countThisObject();
}

template <typename Impl>
void lib_calvin_container::setFunctionTest(Impl &&impl, size_t testSize, std::string title) {
	typedef typename Impl::value_type T;
	cout << "Starting set function test for " << title << "\n";
	vector<T> testVector(testSize);
	std::set<T> stdSet;
	bool correct = true;
	cout << "inserting!\n"; 
	for (unsigned i = 0; i < testSize; ++i) {
		T temp = rand();
		testVector[i] = temp;
		std::pair<typename Impl::iterator, bool> a = impl.insert(temp);
		std::pair<std::set<T>::iterator, bool> b = stdSet.insert(temp);
		if (a.second != b.second || impl.size() != stdSet.size() || impl.count(temp) != stdSet.count(temp)) {			
			correct = false;
			cout << "inserting error\n";
			exit(0); 
		}
	}	
	
	cout << "erasing!\n"; 
	for (unsigned i = 0; i < testSize; ++i) {
		//std::cout << i << "\n";
		T temp = testVector[rand() % testSize];
		size_t a = impl.erase(temp);
		size_t b = stdSet.erase(temp);
		if (a != b) {
			cout << "erase error\n";
			exit(0);
		}
		if (impl.size() != stdSet.size()) {
			cout << "erase size error:" << impl.size() << " " << stdSet.size() <<"\n";
			exit(0);
		} 
	}		

	cout << "counting!\n"; 
	for (unsigned i = 0; i < testSize; ++i) {
		T temp = testVector[i];
		if (impl.count(temp) != stdSet.count(temp)) {
			correct = false;
			cout << "count error\n";
			exit(0);
		}
	}
	
	cout << "iterating!\n";
	auto stdIter = stdSet.begin();
	for (auto implIter = impl.begin(); implIter != impl.end(); ++implIter) {
		if (*implIter != *stdIter) {
			cout << "iterating error\n";
			exit(0);
		}
		++stdIter;
	}
	/*
	cout << "reverse iterating!\n";
	auto stdReverseIter = stdSet.rbegin();
	for (auto implReverseIter = impl.rbegin(); implReverseIter != impl.rend(); ++implReverseIter) {
		if (*implReverseIter != *stdReverseIter) {
			cout << "reverse iterating error\n";
			exit(0);
		}
		++stdReverseIter;
	}
	*/
	cout << "copying!\n"; 
	Impl copy;
	copy = impl;
	if (impl != copy) {
		cout << "assignment error\n";
		exit(0);
	} 
	Impl copy2(impl);
	if (impl != copy2) {
		cout << "copy con error\n";
		exit(0);
	} 
	for (unsigned i = 0; i < testSize; ++i) {
		T temp = testVector[i];
		if (impl.count(temp) != copy.count(temp)) {
			correct = false;
			cout << "copy error\n";
			exit(0);
		}
	}
	// erase all remaining elems
	for (unsigned i = 0; i < testSize; ++i) {
		T temp = testVector[i];
		size_t a = impl.erase(temp);
		size_t b = stdSet.erase(temp);
		if (a != b) {
			cout << "erase error2\n";
			exit(0);
		}
		if (impl.size() != stdSet.size()) {
			cout << "erase size error:" << impl.size() << " " << stdSet.size() <<"\n";
			exit(0);
		} 
	}	
	/*
	Impl table1, table2;
	table1.insert(6);	
	table1.erase(6);
	table1.insert(6);
	table1.insert(5);
	table1.insert(4);
	table1.insert(3);
	table1.insert(2);
	table1.insert(1);
	table1.erase(1);
	table1.insert(1);
	table1.erase(6);
	table2 = table1;
	lib_calvin_util::printContainer(table1, "table1");
	lib_calvin_util::printContainerReverse(table1, "table1");
	cout << "table1 size: " << table1.size() << "\n";
	lib_calvin_util::printContainer(table2, "table2");
	lib_calvin_util::printContainerReverse(table2, "table2");
	cout << "table2 size: " << table2.size() << "\n";	
	if (table1 != table2) {
		correct = false;
	}
	if (table1 < table2 || table1 > table2) {
		correct = false;
	}
	if (correct) {
		cout << title << " turned out to be O.K :-)\n";
	}	else {
		cout << title << " turned out to be erroneous T.T\n";
		exit(0);
	}
	*/
	std::cout << "\n";
}

// performance test routine for given data array
template <typename Impl, typename Key>
void lib_calvin_container::setPerformanceTest_(Impl &impl, std::vector<Key> &data,
			size_t n, std::string title) {
				
	std::cout << title << "\n";
	using lib_calvin::stopwatch;
	stopwatch watch;
	Impl impl2 = impl;
	watch.start();
	for (size_t i = 0; i < n*0.5; ++i) {
		//impl2.insert(data[i]);
	}
	watch.stop();
	cout << "Building: " << n*0.5 / watch.read() << " ops per sec\n";
	
	impl.clear();
	// Calculate worst case insert time	
	double insertTime = 0;
	for (size_t i = 0; i < n*0.5; ++i) {
		watch.start();
		impl.insert((data[i]));
		watch.stop();
		if (watch.read() > insertTime) {
			insertTime = watch.read();
		}
	}	
	cout << "Building MAX: " << insertTime << " sec\n";
	
	double jumpDistance = 0;
	int numIter = 10;
	int numCacheHit = 0;
	int numCacheHit2 = 0;
	int numCacheHit3 = 0;
	int numCacheHit4 = 0;
	watch.start();
	typename typename Impl::value_type iteratorCheckSum = 0;
	//int iteratorCheckSum = 0;
	for (int i = 0; i < numIter; ++i) {
		auto iter = impl.begin();
		//for (typename Impl::iterator iter = impl.begin(); iter != impl.end(); ++iter) {
		for (int j = 0; j < impl.size() - 10; j++) {
			iteratorCheckSum = *iter;
			/*
			auto copy = iter;
			auto next = ++copy;
			auto next2 = ++copy;
			auto next3 = ++copy;
			auto next4 = ++copy;
			char *currentAddress = (char *)&(*iter);
			char *nextAddress = (char *)&(*next);
			char *next2Address = (char *)&(*next2);
			char *next3Address = (char *)&(*next3);
			char *next4Address = (char *)&(*next4);
			if (std::abs(currentAddress - nextAddress) < 100) {
				numCacheHit++;
			} 
			if (std::abs(currentAddress - next2Address) < 100) {
				numCacheHit2++;
			} 
			if (std::abs(currentAddress - next3Address) < 100) {
				numCacheHit3++;
			} 
			if (std::abs(currentAddress - next4Address) < 100) {
				numCacheHit4++;
			} 
			int thisDistance = std::abs((char *)&(*iter) - (char *)&(*next));
			if (thisDistance < 100 && j < 10) {
				std::cout << "pointer distance: " << thisDistance << "\n";
			}
			//jumpDistance += thisDistance*thisDistance;
			*/
			++iter;
		}
	}
	watch.stop();
	cout << "Iterating: " << numIter*impl.size() / watch.read() << 
		" ops per sec. checksum: " << iteratorCheckSum << "\n";
	//cout << "avaerage jump distance is " << sqrt(jumpDistance / numIter*impl.size()) << "\n";
	//cout << "num cache hit " << numCacheHit << " num cache hit 2 " << numCacheHit2 << "\n";
	//cout << "num cache hit3 " << numCacheHit3 << " num cache hit 4 " << numCacheHit4 << "\n";

	watch.start();
	for (size_t i = static_cast<size_t>(n*0.4); i < static_cast<size_t>(n*0.6); ++i) {
		impl.insert((data[i])); 
	}
	watch.stop();
	cout << "Inserting: " << n*0.2 / watch.read() << " ops per sec\n";
	
	size_t totalCount = 0; // for debugging
	watch.start();
	for (size_t i = static_cast<size_t>(n*0.5); i < static_cast<size_t>(n*0.7); ++i) {
		totalCount += impl.count(data[i]);
	}
	watch.stop();
	cout << totalCount << " Searching: " << n*0.2 / watch.read() << " ops per sec\n";

	watch.start();
	for (size_t i = static_cast<size_t>(n*0.5); i < static_cast<size_t>(n*0.7); ++i) {
		impl.erase(data[i]);
	}
	watch.stop();	
	cout << "Deleting: " << n*0.2 / watch.read() << " ops per sec\n";
}

template <typename Impl>
void lib_calvin_container::setPerformanceTest(Impl &&impl, int n, std::string title) {
	cout << "Starting set performance test for " << title << "\n";
	typedef typename Impl::value_type T;
	std::vector<T> testVector(n), testVector2(n);
	
	// Test case 1: random
	for (int i = 0; i < n; ++i) {
		testVector[i] = T(i);
	}	
	std::random_shuffle(testVector.begin(), testVector.end());
	setPerformanceTest_(impl, testVector, n, "<Random data test>");
	
	// Test case 2: sorted sequence. Not a good standard for performance test because of temporal locality. 
	/*
	Impl impl2;
	for (int i = 0; i < n; ++i) {
		testVector2[i] = T(i);
	}	
	setPerformanceTest_(impl2, testVector2, n, "<Sorted data test>");
	*/
	/*
	// Test case 2: sorted sequence. Not a good standard for performance test because of temporal locality. 
	Impl impl2;
	for (int i = 0; i < n; ++i) {
		testVector2[i] = T(-i);
	}	
	setPerformanceTest_(impl2, testVector2, n, "<Reverse sorted data test>");
	*/
	std::cout << "\n";
}

template <typename Impl>
void lib_calvin_container::setIntegratedSpeedTest(Impl &&impl, int n, std::string title) {
	cout << "Starting set integrated test for " << title << "\n";
	lib_calvin::stopwatch watch;
	std::set<Impl> hostSet;
	watch.start();
	for (int i = 0; i < n; ++i) {
		Impl elem;
		int begin = rand() % 100;
		int length = rand() % 1000;
		for (int j = begin; j < begin + length; j++) {
			elem.insert(static_cast<Impl::value_type>(j));
		}
		hostSet.insert(elem);
	}
	watch.stop();
	cout << "inserting (set as element): " << n / watch.read() << " ops per sec\n";
	
	watch.start();
	auto anotherHost = hostSet;
	watch.stop();
	cout << "copying: " << n / watch.read() << " ops per sec\n";

	std::vector<Impl> hostVector;
	watch.start();
	hostVector.resize(n * 1000);
	watch.stop();	
	cout << "ctor as empty: " << n * 1000 / watch.read() << " ops per sec\n";

	std::vector<std::vector<Impl>> holder;
	holder.resize(n);
	watch.start();
	auto elem = std::vector<Impl>(1000);
	for (int i = 0; i < n; ++i) {
		holder[i] = elem;
	}
	watch.stop();	
	cout << "copy ctor as empty: " << n * 1000 / watch.read() << " ops per sec\n";
	
	std::cout << "\n";
}

template <typename Impl>
void lib_calvin_container::setIteratorTest(Impl &&impl, std::string title) {
	cout << "Starting set iterator test for: " << title << "\n";

	impl.insert(2);
	impl.insert(1);
	impl.insert(3);

	Impl & ref = impl;
	Impl const & c_ref = impl;

	typename Impl::const_iterator c_iter;
	c_iter = c_ref.begin();
	c_iter = c_ref.end();
	c_iter = ref.begin();
	c_iter = ref.end();

	typename Impl::iterator iter;
	// Below two line MUST produce compile errors
	//iter = c_ref.begin();
	//iter = c_ref.end();
	iter = ref.begin();
	iter = ref.end();
	c_iter = iter;

	// Should produce in-order sequence
	for (iter = ref.begin(); iter != ref.end(); iter++) {
		std::cout << *iter;
	}
	std::cout << "\n";
	// Should produce in-order sequence
	for (typename Impl::reverse_iterator r_iter = --ref.rend(); ; r_iter--) {
		std::cout << *r_iter;
		if (r_iter == ref.rbegin()) {
			break;
		}
	}
	std::cout << "\n";
	// Should produce reverse-order sequence
	for (typename Impl::reverse_iterator r_iter = ref.rbegin(); r_iter != ref.rend(); ++r_iter) {
		std::cout << *r_iter;
	}
	std::cout << "\n";
	// Should produce reverse-order sequence
	for (typename Impl::iterator iter = --ref.end(); ; iter--) {
		std::cout << *iter;
		if (iter == ref.begin()) {
			break;
		}
	}
	std::cout << "\n\n";
}

template <typename Impl>
void lib_calvin_container::setMemoryTest(Impl &&impl, std::string title) {
	typedef typename Impl::value_type T;
	std::cout << "Starting memory test for: " << title << "\n";
	for (int i = 1000000; i > 0; --i) {
		for (int i = 0; i < 300; ++i) {
			impl.insert(T(i));
		}
		Impl impl2(impl);
		for (int i = 0; i < 300; ++i) {
	 		impl.erase(T(i));
		}
		impl2 = impl;	
		std::cout << "memory test routine\n";
	}
}

template <typename Impl>
void lib_calvin_container::setRvalueTest(Impl &&impl, std::string title) {
	std::cout << "Starting set rvalue test for: " << title << "\n";
	typedef typename Impl::value_type V;
	std::cout << "Test copy & assignment of container itself:\n";
	Impl impl2;
	impl2 = Impl();
	Impl impl3(impl2);
	Impl impl4(std::move(impl2));

	std::cout << "Test insert method:\n";
	V temp(3);
	V temp2(5);
	impl.insert(temp); // copy ctor
	impl.insert(V(6)); // move ctor
	impl.insert(std::move(temp2)); // move ctor, move assignment (operation in a b-tree node)
	std::cout << "\n";
}

void lib_calvin_container::randomAccessSpeedTest(size_t size) {
	typedef BinTreeNode<int> object;
	lib_calvin::vector<object> objectArray(size, 1);
	lib_calvin::vector<int> indexArray(size, 0);
	for (int i = 1; i < size; ++i) {
		indexArray[i] = i;
	}
	std::random_shuffle(indexArray.begin(), indexArray.end());
	lib_calvin::stopwatch watch;
	int temp = 0;
	watch.start();
	for (int i = 0; i < size - 1; ++i) {
		temp += objectArray[indexArray[i]].getKey();
		//objectArray[indexArray[i]].next_ = (&objectArray[indexArray[i + 1]]);
	}
	watch.stop();
	std::cout << temp << " Object size: " << sizeof(object) << " array length: " << size << 
		" random access speed: " << size / watch.read() << "\n";

	watch.start();
	object *thisPointer = &objectArray[indexArray[0]];
	for (int i = 0; i < size - 1; ++i) {
		temp += thisPointer->getKey();
		//thisPointer = thisPointer->next_;
	}
	watch.stop();
	std::cout << temp << " Object size: " << sizeof(object) << " array length: " << size << 
		" random access speed: " << size / watch.read() << "\n";
}