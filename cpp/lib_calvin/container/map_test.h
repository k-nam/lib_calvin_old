#ifndef LIB_CALVIN__CONTAINER__MAP_TEST_H
#define LIB_CALVIN__CONTAINER__MAP_TEST_H

#include "vector.h"
#include <string>
#include "map.h"

namespace lib_calvin_container
{
	void mapTest();

  // Check the correctness of set implementation
	template <typename Impl>
	void mapFunctionTest(Impl &&impl, size_t testSize, std::string title);

	template <typename Impl>
	void mapRvalueTest(Impl &&impl, std::string title);

	// Test this ADT impl with N = n 
	// Assuming that element type T can be constructed with a key value
	template <typename Impl>
	void mapPerformanceTest(Impl &&impl, unsigned n, std::string title);

	template <typename Impl, typename Key, typename Value>
	void mapPerformanceTest_(
		Impl &impl, lib_calvin::vector<std::pair<Key, Value>> const &data,
			unsigned n, std::string title);

	template <typename Impl>
	void mapIntegratedSpeedTest(Impl &&impl, int n, std::string title);

	// Check the correctness of BinTree iterator
	template <typename K, typename V>
	void mapIteratorTest();

	// Check resource leak
	template <typename Impl>
	void mapMemoryTest(std::string);
}



#endif
