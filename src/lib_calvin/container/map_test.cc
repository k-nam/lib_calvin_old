#include <cstdlib>

#include "map_test.h"
#include "container_test.h"
#include "container_test_types.h"
#include "utility.h"
#include "stopwatch.h"
#include "random.h"

#include <map>
#include <unordered_map>

#include "map.h"
#include "hash_map.h"
#include "boost/unordered_map.hpp"

using lib_calvin::stopwatch;
using std::cout;
using std::endl;
using lib_calvin::vector;

void lib_calvin_container::mapTest()
{	
	using namespace lib_calvin_container;

	int const testSize = 10000;
	mapFunctionTest<lib_calvin::map<int, int>>(testSize, "lib_calvin::map");
	mapRvalueTest<lib_calvin::map<int, HeavyObjectWithMessage>>("mapRvalueTest / lib_calvin::map");
	
	int const smallSize = 10000;
	int const largeSize = 100000;
	//mapPerformanceTest<boost::unordered_map<int, int>>(performTestSize, "boost::unordered_map");
	//mapPerformanceTest<std::unordered_map<int, int>>(performTestSize, "std::unordered_map");
	
	//mapPerformanceTest<lib_calvin::map<int, int>>(largeSize, "lib_calvin::map");
	//mapPerformanceTest<std::map<int, int>>(largeSize, "std::map");
	//mapPerformanceTest<lib_calvin_container::map<HeavyObject, int>>(smallSize, "lib_calvin::map / HeavyObject->int");
	mapPerformanceTest<lib_calvin::map<int, HeavyObject>>(largeSize, "lib_calvin::map / int->HeavyObject");
	//mapPerformanceTest<std::map<HeavyObject, int>>(largeSize, "std::map / HeavyObject->int");
	mapPerformanceTest<std::map<int, HeavyObject>>(largeSize, "std::map / int->HeavyObject");
	
	mapIntegratedSpeedTest<std::map<int, int>>(smallSize, "std::map");

	mapIteratorTest<int, int>();

	//mapMemoryTest<lib_calvin::map<int, int>>("lib_calvin::map");

	HeavyObject::countThisObject();
}

template <typename Impl>
void lib_calvin_container::mapFunctionTest(size_t testSize, std::string title)
{
	typedef typename Impl::key_type K; 
	typedef typename Impl::mapped_type V;
	cout << "Starting map function test for " << title << "\n";
	vector<K> keyVector(testSize);
	vector<V> valueVector(testSize);
	Impl impl;
	std::map<K, V> stdMap;
	bool correct = true;
	cout << "inserting!\n"; 
	for (unsigned i = 0; i < testSize; ++i) {
		K key = rand();
		V value = rand();
		keyVector[i] = key;
		valueVector[i] = value;
		bool a = impl.insert(std::pair<K, V>(key, value)).second;
		bool b = stdMap.insert(std::pair<K, V>(key, value)).second;
		if (a != b || impl.size() != stdMap.size()) {
			correct = false;
			cout << "inserting error\n";
			exit(0);
		}
	}
	cout << "deleting!\n"; 
	for (unsigned i = 0; i < testSize/10; ++i) {
		K key = keyVector[rand() % testSize];
		size_t a = impl.erase(key);
		size_t b = stdMap.erase(key);
		if (a != b) {
			cout << "erase error\n";
			exit(0);
		}
		if (impl.size() != stdMap.size()) {
			correct = false;
			cout << "erase error:" << impl.size() << " " << stdMap.size() <<"\n";
			exit(0);
		}
	}	
	cout << "counting!\n"; 
	for (unsigned i = 0; i < testSize; ++i) {
		K key = keyVector[i];
		if (impl.count(key) != stdMap.count(key)) {
			correct = false;
			cout << "count error\n";
		}
	}

	if (correct)
		cout << title << " turned out to be O.K :-)\n";
	else
		cout << title << " turned out to be erroneous T.T\n";
	std::cout << "\n";
}
template <typename Impl>
void lib_calvin_container::mapRvalueTest(std::string title) {
	cout << "Starting map Rvalue test for " << title << "\n";
	typedef typename Impl::key_type K; 
	typedef typename Impl::mapped_type V;
	Impl impl;
	// explicit, move ctor (make_pair), move ctor (std::pair), move ctor (insert)
	impl.insert(std::make_pair(K(1), V(1))); 
	// explicit, move ctor (std::pair)
	std::pair<K, V> b(K(2), V(2)); 
	impl.insert(b); // copy ctor
	// explicit, default (operator[]), move ctor (std::pair), move ctor (insert), assignment ctor
	impl[K(3)] = V(4); 
	std::cout << "\n";
}
// performance test routine for given data array
template <typename Impl, typename Key, typename Value>
void lib_calvin_container::mapPerformanceTest_(lib_calvin::vector<std::pair<Key, Value>> const &data, 
											   unsigned n, std::string title) {
	lib_calvin::stopwatch watch;
	Impl impl;

	// Calculate worst case insert time	
	double insertTime = 0;
	for (int i = 0; i < (int)(n*0.5); ++i) {
		watch.start();
		impl.insert(data[i]);
		watch.stop();
		if (watch.read() > insertTime) {
			insertTime = watch.read();
		}
	}	
	cout << "Building MAX: " << insertTime << " sec\n";

	watch.start();
	for (int i = (int)(n*0.4); i < (int)(n*0.6); ++i) {
		impl.insert(data[i]);
	}
	watch.stop();
	cout << "Inserting: " << n*0.2 / watch.read() << " ops per sec\n";
	
	watch.start();
	auto checkSum = typename Impl::mapped_type(0);
	for (auto iter = impl.begin(); iter != impl.end(); ++iter) {
		checkSum = iter->second;
	}
	watch.stop();
	cout << "Iterating: " << n*0.6 / watch.read() << " ops per sec\n";

	size_t totalCount = 0; // for debugging
	watch.start();
	for (int i = (int)(n*0.5); i < (int)(n*0.7); ++i) {
		totalCount += impl.count(data[i].first);
	}
	watch.stop();
	cout << totalCount << " Searching: " << n*0.2 / watch.read() << " ops per sec\n";

	watch.start();
	auto temp = typename Impl::mapped_type(0);
	for (int i = (int)(n*0.1); i < (int)(n*0.7); ++i) {
		temp += impl[data[i].first];
	}
	watch.stop();
	cout << totalCount << " operator[]: " << n*0.2 / watch.read() << " ops per sec\n";

	watch.start();
	for (int i = (int)(n*0.6); i < (int)(n*0.8); ++i) {
		impl.erase(data[i].first);
	}
	watch.stop();	
	cout << "Deleting: " << n * 0.2 / watch.read() << " ops per sec\n";
	std::cout << "\n";
}

template <typename Impl>
void lib_calvin_container::mapPerformanceTest(unsigned n, std::string title)
{
	cout << "Starting map performance test for " << title << "\n";
	typedef typename Impl::key_type K;
	typedef typename Impl::mapped_type V;
	lib_calvin::vector<std::pair<K, V>> testVector(n), testVector2(n);
	for (unsigned i = 0; i < n; ++i) {
		testVector[i] = std::pair<K, V>(K(i), V(rand()));
	}	

	// Test case 1: random
	std::shuffle(testVector.begin(), testVector.end(), std::mt19937_64 (std::random_device()()));
	mapPerformanceTest_<Impl>(testVector, n, "<Random data test>");
	/*
	// Test case 2: sorted sequence
	for (unsigned i = 0; i < n; ++i) {
		testVector2[i] = std::pair<K, V>(K(i), V(rand()));
	}	
	mapPerformanceTest_(testVector2, n, "<Sorted data test>");
	*/
}

template <typename Impl>
void lib_calvin_container::mapIntegratedSpeedTest(int n, std::string title) {
	cout << "Starting map integrated test for " << title << "\n";
	cout << "size of container is " << sizeof(Impl) << "\n";
	lib_calvin::stopwatch watch;
	std::set<Impl> hostSet;
	watch.start();
	for (int i = 0; i < n; ++i) {
		Impl elem;
		for (int j = 0; j < rand() % 10000; j++) {
			elem.insert(typename Impl::value_type(j, i));
		}
		hostSet.insert(elem);
	}
	watch.stop();
	cout << "inserting (map as element): " << n / watch.read() << " ops per sec\n";

	watch.start();
	auto anotherHost = hostSet;
	watch.stop();
	cout << "copying: " << n / watch.read() << " ops per sec\n";

	lib_calvin::vector<Impl> hostVector;
	watch.start();
	hostVector.resize(n * 1000);
	watch.stop();	
	cout << "creating: " << n * 1000 / watch.read() << " ops per sec\n";

	std::vector<std::vector<Impl>> holder;
	holder.resize(n);
	watch.start();
	for (int i = 0; i < n; ++i) {
		holder[i] = std::vector<Impl>(1000);
	}
	watch.stop();	
	cout << "copying as empty: " << n * 1000 / watch.read() << " ops per sec\n";

	std::cout << "\n";
}

template <typename K, typename V>
void lib_calvin_container::mapIteratorTest()
{
	using lib_calvin::map;
	cout << "Starting map iterator test...\n";
	map<K, V> testMap;
	testMap.insert(std::pair<K, V>(K(2), V(10)));
	testMap.insert(std::pair<K, V>(K(3), V(11)));
	testMap.insert(std::pair<K, V>(K(5), V(12)));

	map<K, V> & ref = testMap;
	map<K, V> const & c_ref = testMap;

	typename map<K, V>::const_iterator c_iter;
	c_iter = c_ref.begin();
	c_iter = c_ref.end();
	c_iter = ref.begin();
	c_iter = ref.end();

	typename map<K, V>::iterator iter;
	// Below two line MUST produce compile errors
	//iter = c_ref.begin();
	//iter = c_ref.end();
	iter = ref.begin();
	iter = ref.end();

	// Should produce in-order sequence
	for (iter = ref.begin(); iter != ref.end(); iter++) {
		std::cout << iter->first << ": ";
		std::cout << iter->second << " , ";
	}
	std::cout << "\n";
	// Should produce in-order sequence
	for (auto r_iter = --ref.rend(); ; r_iter--) {
		std::cout << r_iter->first << ": ";
		std::cout << r_iter->second << " , ";
		if (r_iter == ref.rbegin()) {
			break;
		}
	}
	std::cout << "\n";
	// Should produce reverse-order sequence
	for (auto r_iter  = ref.rbegin(); r_iter != ref.rend(); ++r_iter) {
		std::cout << r_iter->first << ": ";
		std::cout << r_iter->second << " , ";
	}
	std::cout << "\n";
	// Should produce reverse-order sequence
	for (iter = --ref.end(); ; iter--) {
		std::cout << iter->first << ": ";
		std::cout << iter->second << " , ";
		if (iter == ref.begin()) {
			break;
		}
	}
	std::cout << "\n\n";
}

template <typename Impl>
void lib_calvin_container::mapMemoryTest(std::string title)
{
	typedef typename Impl::key_type K;
	typedef typename Impl::mapped_type V;
	std::cout << "\nStarting memory test for: " << title << "\n";
	for (int i = 10000000; i > 0; --i) {
		Impl impl;	
		for (int i = 0; i < 300; ++i) {
			impl.insert(std::pair<K, V>(K(i), V(i)));
		}
		Impl impl2(impl);
		for (int i = 0; i < 500; ++i) {
			impl.erase(K(i));
		}
		impl2 = impl;	
	}
}
