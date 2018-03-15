#ifndef LIB_CALVIN__SORT__FILE_SORT_TEST_H
#define LIB_CALVIN__SORT__FILE_SORT_TEST_H

#include <string>
#include <cstdlib>

namespace lib_calvin_sort
{
	size_t const FILE_SORT_TEST_SIZE = (size_t)1024 * 1024 * 1024; // in bytes
	size_t const FILE_SORT_TEST_BLOCK_SIZE = 16; // in bytes
	size_t const FILE_SORT_TEST_NUM_BLOCKS = FILE_SORT_TEST_SIZE / FILE_SORT_TEST_BLOCK_SIZE;
	size_t const FILE_SORT_TEST_WORDS_IN_BLOCK = FILE_SORT_TEST_BLOCK_SIZE / sizeof(int);
	
	void fileSortTest();
	class FileSortTest {
	public:
		void fileOpenTest();
		void diskReadBenchmark(std::string fileName);
		void diskWriteBenchmark(std::string);
		void fileIteratorTest(std::string);
		void readFileTest(std::string);
		void sortFileTest(std::string);
	private:
		std::string path = "../../../../file/";
		int dataBlock[FILE_SORT_TEST_WORDS_IN_BLOCK];
	};
}

#endif