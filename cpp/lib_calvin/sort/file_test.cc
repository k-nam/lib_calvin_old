#include "file_test.h"
#include "file.h"
#include "sort.h"
#include "stopwatch.h"
#include <iostream>
#include <fstream>
#include <cstdint>

namespace lib_calvin_sort
{
void fileTest() {
	std::string path = "../../../../file/";
	
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
	fileIteratorTest(path + "data1");	
	sortFileTest(path + "data1");
	readFileTest(path + "data1");
}

void fileOpenTest() {
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

int64_t const testSize = (int64_t)1024 * 1024 * 1024; // in bytes
int const blockSize = 4; // in bytes
int64_t const numBlocks = testSize / blockSize;
int const wordsInBlock = blockSize / sizeof(int);
int dataBlock[wordsInBlock];

void diskWriteBenchmark(std::string fileName) {
	using lib_calvin::stopwatch;
	stopwatch watch;
	// sequential write
	std::ofstream fout(fileName, std::ios::out | std::ios::binary);
	if (!fout) {
		std::cout << "file open failed\n";
		exit(0);
	}
	watch.start();		
	for (int64_t i = 0; i < numBlocks; ++i) {
		fout.write((char *)dataBlock, blockSize);
	}
	fout.flush();
	fout.close();
	watch.stop();
	std::cout << "sequential writing speed with testSize: " <<
		testSize/1024.0/1024 << " MB, block size: " << blockSize/1024.0 << " KB: " <<
		testSize/watch.read()/1024/1024 << " MB / s\n";		
}

void diskReadBenchmark(std::string fileName) {		
	using lib_calvin::stopwatch;
	stopwatch watch;
	// sequential read
	std::ifstream fin(fileName, std::ios::in | std::ios::binary);
	if (!fin) {
		std::cout << "file open failed\n";
		exit(0);
	}
	watch.start();
	/*
	for (int64_t i = 0; i <= totalWords; ++i) {
		if (i % wordsInBlock == 0) {
			if (fin.eof()) {
				std::cout << "reading error;\n";
				exit(0);
			}
			fin.read((char *)dataBlock, blockSize);				
		}
	}
	*/
	while (fin.eof() == false) {
		fin.read((char *)dataBlock, blockSize);				
	}

	fin.close();
	watch.stop();
	std::cout << "sequential reading speed with testSize: " <<
		testSize/1024.0/1024 << " MB, block size: " << blockSize/1024.0 << " KB: " <<
		testSize/watch.read()/1024/1024 << " MB / s\n";
}

void fileIteratorTest(std::string fileName) {
	using namespace lib_calvin_sort;
	int size = 1000000;
	int *dataArray = new int[size];
	for (int i = 0; i < size; ++i) {
		dataArray[i] = size - i;
	}
	writeToFile(dataArray, dataArray + size, fileName);
	std::fstream file(fileName, std::ios::in | std::ios::out | std::ios::binary);
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

void readFileTest(std::string fileName) {
	std::fstream file(fileName, std::ios::in | std::ios::out | std::ios::binary);
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

void sortFileTest(std::string fileName) {
	std::fstream file(fileName, std::ios::in | std::ios::out | std::ios::binary);
	file.seekg(0, std::ios::end);
	size_t fileSize = static_cast<size_t>(file.tellg());
	file_iterator<int> begin(file, 0);
	file_iterator<int> end(file, fileSize/sizeof(int));
	introSort(begin, end);
}

} // end namespace