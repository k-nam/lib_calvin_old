#ifndef LIB_CALVIN__SORT__FILE_SORT_TEST_H
#define LIB_CALVIN__SORT__FILE_SORT_TEST_H

#include <string>

namespace lib_calvin_sort
{
	void fileTest();

	void fileOpenTest();
	void diskReadBenchmark(std::string);
	void diskWriteBenchmark(std::string);

	void fileIteratorTest(std::string);
	void readFileTest(std::string);
	void sortFileTest(std::string);
}

#endif