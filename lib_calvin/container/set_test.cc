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
#include "hash_table2.h"
#include "blocked_array.h"
#include "ordered_array.h"
#include "random.h"

using lib_calvin::stopwatch;
using std::cout;
using std::endl;
using std::vector;

void lib_calvin_container::setTest() {	

	//setRvalueTest<RbTree<HeavyObjectWithMessage>>("RbTree / HeavyObject");
	//setRvalueTest<BTree<HeavyObjectWithMessage>>("BTree / HeavyObject");
	//setRvalueTest<BPlusTree<HeavyObjectWithMessage>>("BPlusTree / HeavyObject");
	
	int const testSize = 10000;
	int const smallSize = 1000;
	int const mediumSize = 100000;
	int const largeSize = 1000000;	
	typedef size_t Numeric;

	setFunctionTest<lib_calvin::set<HeavyObject>>(testSize, "lib_calvin::set");
	setFunctionTest<BinTree<HeavyObject>>(testSize, "lib_calvin_container::BinTree");
	setFunctionTest<RbTree<HeavyObject>>(testSize, "lib_calvin_container::RbTree");
	setFunctionTest<BTree<HeavyObject>>(testSize, "lib_calvin_container::BTree");
	setFunctionTest<BPlusTree<HeavyObject>>(testSize, "lib_calvin_container::BPlusTree");
	setFunctionTest<OrderedArray<HeavyObject>>(testSize, "lib_calvin_container::OrderedArray");
	//setFunctionTest(PtrSet<int>(), testSize, "lib_calvin_container::PtrSet"); // unfinished
	setFunctionTest<HashTable<Numeric>>(testSize,	"lib_calvin_container::HashTable"); // cannot iterate
	setFunctionTest<HashTable2<Numeric>>(testSize,	"lib_calvin_container::HashTable2"); // cannot iterate
	
	setFunctionTest2<BTree<Numeric>>(testSize, "lib_calvin_container::BTree");
	setFunctionTest2<BPlusTree<Numeric>>(testSize, "lib_calvin_container::BPlusTree");

	setIteratorTest<RbTree<Numeric>>("RbTree iterator");
	setIteratorTest<BTree<Numeric>>("BTree iterator");
	setIteratorTest<HashTable<Numeric>>("HashTable iterator");
	
	//setPerformanceTest<std::set<Numeric>>(largeSize, "std::set / Numeric");
	setPerformanceTest<boost::container::set<Numeric>>(largeSize, "boost::set / Numeric");
	//setPerformanceTest<BinTree<Numeric>>(largeSize, "RbTree / Numeric");
	setPerformanceTest<RbTree<Numeric>>(largeSize, "RbTree / Numeric");
	//setPerformanceTest<BTree<Numeric>>(largeSize, "BTree / Numeric");
	setPerformanceTest<BPlusTree<Numeric>>(largeSize, "BPlusTree / Numeric");
	//setPerformanceTest<OrderedArray<Numeric>>(largeSize, "OrderedArray / Numeric");
	//setPerformanceTest<std::unordered_set<Numeric>>(largeSize, "std::unordered_set / Numeric");
	setPerformanceTest<boost::unordered_set<Numeric>>(largeSize, "boost::unordered_set / Numeric");
	setPerformanceTest<HashTable<Numeric>>(largeSize, "HashTable / Numeric");
	
	//setPerformanceTest<std::set<LightObject>>(largeSize, "std::set / LightObject");
	//setPerformanceTest<boost::container::set<LightObject>>(largeSize, "boost::set / LightObject");
	//setPerformanceTest<RbTree<LightObject>>(largeSize, "RbTree / LightObject");
	//setPerformanceTest<BTree<LightObject>>(largeSize, "BTree / LightObject");
	//setPerformanceTest<BPlusTree<LightObject>>(largeSize, "BPlusTree / LightObject");
	//setPerformanceTest<OrderedArray<LightObject>>(largeSize, "OrderedArray / LightObject");
	//setPerformanceTest<lib_calvin::hash_set<LightObject, GenericHash<LightObject>>>(
	// largeSize, "hash_set / LightObject");

	//setPerformanceTest<std::set<HeavyObject>>(mediumSize, "std::set / HeavyObject");
	setPerformanceTest<boost::container::set<HeavyObject>>(mediumSize, "boost::set / HeavyObject");
	setPerformanceTest<RbTree<HeavyObject>>(mediumSize, "RbTree / HeavyObject");
	//setPerformanceTest<BTree<HeavyObject>>(mediumSize, "BTree / HeavyObject");
	setPerformanceTest<BPlusTree<HeavyObject>>(mediumSize, "BPlusTree / HeavyObject");
	//setPerformanceTest<set_ref<HeavyObject>>(mediumSize, "set_ref / HeavyObject");
	setPerformanceTest<boost::unordered_set<HeavyObject, GenericHash<HeavyObject>>>(
		mediumSize, "boost::unordered_set / HeavyObject");
	setPerformanceTest<lib_calvin::hash_set<HeavyObject, GenericHash<HeavyObject>>>(
		mediumSize, "HashTable / HeavyObject");
	//setPerformanceTest<std::unordered_set<HeavyObject>>(mediumSize, "std::unordered_set / HeavyObject");
	//setPerformanceTest<PtrSet<HeavyObject>>(mediumSize, "PtrSet / HeavyObject");
	
	//setIntegratedSpeedTest<std::set<HeavyObject>>(smallSize, "std::set");
	setIntegratedSpeedTest<boost::container::set<HeavyObject>>(smallSize, "boost:set");
	setIntegratedSpeedTest<RbTree<HeavyObject>>(smallSize, "RbTree");
	//setIntegratedSpeedTest<BTree<HeavyObject>>(smallSize, "BTree");
	setIntegratedSpeedTest<BPlusTree<HeavyObject>>(smallSize, "BPlusTree");
	//setIntegratedSpeedTest<OrderedArray<HeavyObject>>(smallSize, "OrderedArray");
	setIntegratedSpeedTest<boost::unordered_set<int>>(smallSize, "boost::unordered_set");
	setIntegratedSpeedTest<HashTable<int>>(smallSize, "HashTable");

	//setMemoryTest<std::set<std::set<HeavyObject>>>("std::set");
	//setMemoryTest<BinTree<BinTree<HeavyObject>>>("BinTree");
	//setMemoryTest<RbTree<RbTree<HeavyObject>>>("RbTree");
	//setMemoryTest<BTree<BTree<HeavyObject>>>("BTree");
	//setMemoryTest<BPlusTree<BPlusTree<HeavyObject>>>("BPlusTree");
	
	//randomAccessSpeedTest(largeSize);
	
	HeavyObject::countThisObject();

	std::cout << "set test completed\n\n";
}

template <typename Impl>
void lib_calvin_container::setFunctionTest(size_t testSize, std::string title) {
	typedef Impl::value_type T;
	cout << "Starting set function test for " << title << "\n";
	Impl impl;
	vector<T> testVector(testSize);
	std::set<T> stdSet;
	bool correct = true;
	cout << "inserting!\n"; 
	for (unsigned i = 0; i < testSize; ++i) {	
		T temp = rand();
		testVector[i] = temp;
		std::pair<Impl::iterator, bool> a = impl.insert(temp);
		std::pair<std::set<T>::iterator, bool> b = stdSet.insert(temp);
		size_t count1 = impl.count(temp);
		size_t count2 = stdSet.count(temp);
		if (a.second != b.second || impl.size() != stdSet.size() || count1 != count2) {			
			correct = false;
			cout << "inserting error\n";
			exit(0); 
		} else {
			//cout << "inserting go\n";
		}
	}	
	
	cout << "erasing!\n"; 
	for (unsigned i = 0; i < testSize; ++i) {
		//std::cout << i << "\n";
		T temp = testVector[rand() % testSize];
		T temp2 = testVector[i]; // to ensure erasing all elements during this test
		size_t a = impl.erase(temp);
		size_t b = stdSet.erase(temp);
		size_t c = impl.erase(temp2);
		size_t d = stdSet.erase(temp2);
		if (a != b || c != d) {
			cout << "erase error\n";
			exit(0);
		}
		if (impl.size() != stdSet.size()) {
			cout << "erase size error:" << impl.size() << " " << stdSet.size() <<"\n";
			exit(0);
		} 
	}		
	// refill for next operations
	for (unsigned i = 0; i < testSize; ++i) {
		T temp = rand();
		testVector[i] = temp;
		impl.insert(temp);
		stdSet.insert(temp);
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
	
	cout << "finding!\n"; 
	for (unsigned i = 0; i < testSize; ++i) {
		T temp = testVector[i];
		bool foundInImpl = !(impl.find(temp) == impl.end());
		bool foundInStdSet = !(stdSet.find(temp) == stdSet.end());
		if (foundInImpl != foundInStdSet) {
			correct = false;
			cout << "count error1\n";
			exit(0);
		} else if (foundInImpl) {
			if (*impl.find(temp) != *stdSet.find(temp)) {
				correct = false;
				cout << "count error2\n";
				exit(0);
			}
		} else { // not found in both
		}
	}

	if (title != "lib_calvin_container::HashTable" &&
			title != "lib_calvin_container::HashTable2") { // hash table does not iterate in order
		size_t index = 0;
		cout << "iterating!\n";
		auto stdIter = stdSet.begin();
		for (auto implIter = impl.begin(); implIter != impl.end(); ++implIter) {
			if (*implIter != *stdIter) {
				cout << "iterating error\n";
				exit(0);
			}
			++stdIter;
			index++;
		}
		cout << "reverse iterating!\n";
		auto stdReverseIter = stdSet.rbegin();
		for (auto implReverseIter = impl.rbegin(); implReverseIter != impl.rend(); ++implReverseIter) {
			if (*implReverseIter != *stdReverseIter) {
				cout << "reverse iterating error\n";
				exit(0);
			}
			++stdReverseIter;
		}
	}

	cout << "copying!\n";
	Impl copy;
	copy = impl;
	Impl copy2(impl);

	if (title != "lib_calvin_container::HashTable") {
		if (impl != copy) {
			cout << "assignment error\n";
			exit(0);
		} 
		if (impl != copy2) {
			cout << "copy con error\n";
			exit(0);
		} 
	}
	for (size_t i = 0; i < testSize; ++i) {
		T temp = testVector[i];
		if (impl.count(temp) != copy.count(temp) || impl.count(temp) != copy2.count(temp)) {
			correct = false;
			cout << "assignment / copy con counting error\n";
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

	std::cout << "\n";
}

template <typename Impl>
void lib_calvin_container::setFunctionTest2(size_t testSize, std::string title) {
	typedef Impl::value_type T;
	cout << "Starting set function test2 for " << title << "\n";
	Impl impl;
	bool correct = true;
	for (unsigned i = 0; i < testSize; ++i) {	
		T temp = rand() % testSize;
		impl.insert(temp);
	}
	Impl temp = impl;
	size_t index = 0;
	for (auto iter = impl.begin(); iter != impl.end(); ++iter) {
		if (*iter != *temp.at(index)) {
			cout << "at() method error\n";
			exit(0);
		}
		if (impl.index_of(iter) != index) {
			cout << "index_of() method error\n";
			exit(0);
		}
		index++;
	}
	cout << "at() test part.1 O.K\n";

	for (unsigned i = 0; i < testSize; ++i) {	
		T temp = rand() % testSize;
		impl.erase(temp);
	}
	temp = impl;
	index = 0;
	for (auto iter = impl.begin(); iter != impl.end(); ++iter) {
		if (*iter != *temp.at(index)) {
			cout << "at() method error\n";
			exit(0);
		} 
		if (impl.index_of(iter) != index) {
			cout << "index_of() method error\n";
			exit(0);
		}
		index++;
	}		
	cout << "at() test part.2 O.K\n\n";
}


template <typename Impl>
void lib_calvin_container::setPerformanceTest(int n, std::string title) {
	cout << "Starting set performance test for " << title << "\n";
	typedef Impl::value_type T;
	std::vector<T> testVector(n), testVector2(n);
	// Test case 1: random
	for (int i = 0; i < n; ++i) {
		testVector[i] = T(i);
	}	

	std::random_shuffle(testVector.begin(), testVector.end());
	setPerformanceTest_<Impl, T>(testVector, n, "<Random data test>");
	
	// Test case 2: sorted sequence. Not a good standard for performance test because of temporal locality. 
	/*
	for (int i = 0; i < n; ++i) {
		testVector2[i] = T(i);
	}	
	setPerformanceTest_(testVector2, n, "<Sorted data test>");
	*/
	/*
	// Test case 2: reverse sorted sequence. 
	// Not a good standard for performance test because of temporal locality. 
	for (int i = 0; i < n; ++i) {
		testVector2[i] = T(-i);
	}	
	setPerformanceTest_(testVector2, n, "<Reverse sorted data test>");
	*/
	std::cout << "\n";
}

// performance test routine for given data array
template <typename Impl, typename Key>
void lib_calvin_container::setPerformanceTest_(std::vector<Key> &data,
			size_t n, std::string title) {			
	std::cout << title << "\n";
	using lib_calvin::stopwatch;
	stopwatch watch;
	Impl impl;
	watch.start();
	for (size_t i = 0; i < n*0.5; ++i) {
		impl.insert(data[i]);
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
	typename Impl::value_type iteratorCheckSum = 0;
	//int iteratorCheckSum = 0;
	for (int i = 0; i < numIter; ++i) {
		auto iter = impl.begin();
		//for (typename Impl::iterator iter = impl.begin(); iter != impl.end(); ++iter) {
		for (int j = 0; j < static_cast<int>(impl.size()) - 10; j++) {
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
void lib_calvin_container::setIntegratedSpeedTest(int n, std::string title) {
	cout << "Starting set integrated test for " << title << "\n";
	lib_calvin::stopwatch watch;
	/*
	std::set<Impl> hostSet;
	watch.start();
	for (int i = 0; i < n; ++i) {
		Impl elem;
		int begin = rand() % 100;
		int length = rand() % 1000;
		for (int j = begin; j < begin + length; j++) {
			elem.insert(Impl::value_type(j));
		}
		hostSet.insert(elem);
	}
	watch.stop();
	cout << "inserting (set as element): " << n / watch.read() << " ops per sec\n";
	
	watch.start();
	auto anotherHost = hostSet;
	watch.stop();
	cout << "copying: " << n / watch.read() << " ops per sec\n";
	*/

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
void lib_calvin_container::setIteratorTest(std::string title) {
	cout << "Starting set iterator test for: " << title << "\n";
	Impl impl;
	impl.insert(2);
	impl.insert(1);
	impl.insert(3);

	Impl & ref = impl;
	Impl const & c_ref = impl;

	Impl::const_iterator c_iter;
	c_iter = c_ref.begin();
	c_iter = c_ref.end();
	c_iter = ref.begin();
	c_iter = ref.end();

	Impl::iterator iter;
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
	for (Impl::reverse_iterator r_iter = --ref.rend(); ; r_iter--) {
		std::cout << *r_iter;
		if (r_iter == ref.rbegin()) {
			break;
		}
	}
	std::cout << "\n";
	// Should produce reverse-order sequence
	for (Impl::reverse_iterator r_iter = ref.rbegin(); r_iter != ref.rend(); ++r_iter) {
		std::cout << *r_iter;
	}
	std::cout << "\n";
	// Should produce reverse-order sequence
	for (Impl::iterator iter = --ref.end(); ; iter--) {
		std::cout << *iter;
		if (iter == ref.begin()) {
			break;
		}
	}
	std::cout << "\n\n";
}

template <typename Impl>
void lib_calvin_container::setMemoryTest(std::string title) {	
	typedef typename Impl::value_type Impl2;
	std::cout << "Starting memory test2 for " << title << "\n";
	lib_calvin::stopwatch watch;
	watch.start();
	for (int k = 0; k < 100; k++) {
		Impl tree;
		for (int i = 0; i < 100; ++i) {
			Impl2 tree2;
			for (int j = 0; j < 100; j++) {
				tree2.insert(i + j);
			}
			tree.insert(tree2);
		}
		for (int i = 0; i < 100; ++i) {
			Impl2 tree2;
			for (int j = 0; j < 100; j++) {
				tree2.insert(i + j + 1);
			}
			tree.erase(tree2);
		}
		Impl tree3(tree);
		Impl tree4 = tree;
	}
	watch.stop();
	std::cout << "time taken: " << watch.read() << "\n";
	std::cout << "setMemoryTest complete\n\n";
}

template <typename Impl>
void lib_calvin_container::setRvalueTest(std::string title) {
	std::cout << "Starting set rvalue test for: " << title << "\n";
	typedef Impl::value_type V;
	std::cout << "Test copy & assignment of container itself:\n";
	Impl impl;
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
	typedef BinTreeNode<int> ElemType;
	lib_calvin::vector<ElemType> ElemTypeArray(size, ElemType(1));
	lib_calvin::vector<size_t> indexArray(size, 0);
	for (size_t i = 1; i < size; ++i) {
		indexArray[i] = i;
	}
	std::random_shuffle(indexArray.begin(), indexArray.end());
	lib_calvin::stopwatch watch;
	int temp = 0;
	watch.start();
	for (size_t i = 0; i < size - 1; ++i) {
		temp += ElemTypeArray[indexArray[i]].getKey();
		//ElemTypeArray[indexArray[i]].next_ = (&ElemTypeArray[indexArray[i + 1]]);
	}
	watch.stop();
	std::cout << temp << " ElemType size: " << sizeof(ElemType) << " array length: " << size << 
		" random access speed: " << size / watch.read() << "\n";

	watch.start();
	ElemType *thisPointer = &ElemTypeArray[indexArray[0]];
	for (size_t i = 0; i < size - 1; ++i) {
		temp += thisPointer->getKey();
		//thisPointer = thisPointer->next_;
	}
	watch.stop();
	std::cout << temp << " ElemType size: " << sizeof(ElemType) << " array length: " << size << 
		" random access speed: " << size / watch.read() << "\n";
}