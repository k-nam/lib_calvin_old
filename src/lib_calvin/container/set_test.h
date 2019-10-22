#ifndef LIB_CALVIN__CONTAINER__SET_TEST_H
#define LIB_CALVIN__CONTAINER__SET_TEST_H

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
}

#endif
