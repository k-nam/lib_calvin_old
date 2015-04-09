#ifndef LIB_CALVIN__CONTAINER__SET_TEST_H
#define LIB_CALVIN__CONTAINER__SET_TEST_H

#include <cstdlib>
#include <random>
#include <set>
#include <unordered_set>
#include "boost/unordered_set.hpp"
#include "boost/container/set.hpp"

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


namespace lib_calvin_container
{
	void setTest();

	// Check the correctness of set implementation
	template <typename Impl>
	void setFunctionTest(size_t testSize, std::string title);

	template <typename Impl>
	void setFunctionTest2(size_t testSize, std::string title);

	// Test this ADT impl with N = n 
	// Assuming that element type T can be constructed with a key value
	template <typename Impl>
	void setPerformanceTest(int n, std::string title);

	template <typename Impl, typename Key>
	void setPerformanceTest_(std::vector<Key> &data, size_t n, std::string title);

	template <typename Impl>
	void setIntegratedSpeedTest(int n, std::string title);

	// Check the correctness of BinTree iterator
	template <typename Impl>
	void setIteratorTest(std::string title);

	// Check resource leak
	template <typename Impl>
	void setMemoryTest(std::string);
	template <typename Impl>
	void setMemoryTest(std::string title);

	template <typename Impl>
	void setRvalueTest(std::string);

	void randomAccessSpeedTest(int size);
}

#endif
