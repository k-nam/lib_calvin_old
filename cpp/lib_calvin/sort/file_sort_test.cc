#include "file_sort_test.h"
#include "file_sort.h"
#include "sort.h"
#include "stopwatch.h"
#include <iostream>
#include <fstream>
#include <cstdint>

namespace lib_calvin_sort
{
void fileSortTest() {
	/*
	fileOpenTest();		
	diskWriteBenchmark(path + "data1");
	diskWriteBenchmark(path + "data1");
	diskWriteBenchmark(path + "data2");
	diskWriteBenchmark(path + "data3");
	diskWriteBenchmark(path + "data4");
	diskWriteBenchmark(path + "data5");
	diskWriteBenchmark(path + "data6");
	diskWriteBenchmark(path + "data7");
	diskReadBenchmark(path + "data1");	
	*/
	FileSortTest test;
	test.fileIteratorTest("data1");	
	test.sortFileTest("data1");
	test.readFileTest("data1");
}

void FileSortTest::fileOpenTest() {
	std::ofstream fout("../../../../file/data", std::ios::out | std::ios::binary);
	if (!fout) {
		std::cout << "file open failed\n";
		exit(0);
	}
	int a = 12345678;
	for (int i = 0; i < 1000; ++i) {
		fout.write((char *)&i, sizeof(int));
	}
	fout.flush();
	fout.close();
}

void FileSortTest::diskWriteBenchmark(std::string fileName) {
	using lib_calvin::stopwatch;
	stopwatch watch;
	// sequential write
	std::ofstream fout(path + fileName, std::ios::out | std::ios::binary);
	if (!fout) {
		std::cout << "file open failed\n";
		exit(0);
	}
	watch.start();		
	for (intmax_t i = 0; i < FILE_SORT_TEST_NUM_BLOCKS; ++i) {
		fout.write((char *)dataBlock, FILE_SORT_TEST_BLOCK_SIZE);
	}
	fout.flush();
	fout.close();
	watch.stop();
	std::cout << "sequential writing speed with testSize: " <<
		FILE_SORT_TEST_SIZE/1024.0/1024 << " MB, block size: " << FILE_SORT_TEST_BLOCK_SIZE/1024.0 << " KB: " <<
		FILE_SORT_TEST_SIZE/watch.read()/1024/1024 << " MB / s\n";		
}

void FileSortTest::diskReadBenchmark(std::string fileName) {		
	using lib_calvin::stopwatch;
	stopwatch watch;
	// sequential read
	std::ifstream fin(path+ fileName, std::ios::in | std::ios::binary);
	if (!fin) {
		std::cout << "file open failed\n";
		exit(0);
	}
	watch.start();
	/*
	for (intmax_t i = 0; i <= totalWords; ++i) {
		if (i % wordsInBlock == 0) {
			if (fin.eof()) {
				std::cout << "reading error;\n";
				exit(0);
			}
			fin.read((char *)dataBlock, FILE_SORT_TEST_BLOCK_SIZE);				
		}
	}
	*/
	while (fin.eof() == false) {
		fin.read((char *)dataBlock, FILE_SORT_TEST_BLOCK_SIZE);				
	}

	fin.close();
	watch.stop();
	std::cout << "sequential reading speed with testSize: " <<
		FILE_SORT_TEST_SIZE/1024.0/1024 << " MB, block size: " << FILE_SORT_TEST_BLOCK_SIZE/1024.0 << " KB: " <<
		FILE_SORT_TEST_SIZE/watch.read()/1024/1024 << " MB / s\n";
}

void FileSortTest::fileIteratorTest(std::string fileName) {
	using namespace lib_calvin_sort;
	int size = 1000000;
	int *dataArray = new int[size];
	for (int i = 0; i < size; ++i) {
		dataArray[i] = size - i;
	}
	writeToFile(dataArray, dataArray + size, path + fileName);
	std::fstream file(path + fileName, std::ios::in | std::ios::out | std::ios::binary);
	file_iterator<int> fileIter(file, 0);
	for (int i = 0; i < size; ++i) {
		if (*fileIter++ != i) {
		  std::cout << "file_iter error: disk = " << *fileIter << " memory = " << i << "\n";
			exit(0);
		} else {
			//std::cout << " i = " << i << " = " << *fileIter << "\n";
		}
	}
}

void FileSortTest::readFileTest(std::string fileName) {
	std::fstream file(path + fileName, std::ios::in | std::ios::out | std::ios::binary);
	file.seekg(0, std::ios::end);
	size_t fileSize = static_cast<size_t>(file.tellg());
	std::cout << "fileSize is: " << fileSize << "\n";
	file_iterator<int> begin(file, 0);
	file_iterator<int> end(file, fileSize/sizeof(int));
	for ( ; begin != end; ++begin) {
		std::cout << *begin << "\n";
	}
	begin.setOffset(0);
	int temp = *begin;
	std::cout << "*(begin + 1) = " << *(begin + 1) << "\n";
	*begin = (int)*(begin + 1);
	*(begin + 1) = temp;
	for ( ; begin != end; ++begin) {
		//std::cout << *begin << "\n";
	}
}

void FileSortTest::sortFileTest(std::string fileName) {
	std::fstream file(path + fileName, std::ios::in | std::ios::out | std::ios::binary);
	file.seekg(0, std::ios::end);
	size_t fileSize = static_cast<size_t>(file.tellg());
	file_iterator<int> begin(file, 0);
	file_iterator<int> end(file, fileSize/sizeof(int));
	introSort(begin, end);
}

} // end namespace