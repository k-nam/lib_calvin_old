#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include "etc_bench.h"
#include "bench.h"
#include "stopwatch.h"

void lib_calvin_benchmark::etc::etc_bench() {
	memory_access_bench(1000*1000*10);
}

void lib_calvin_benchmark::etc::memory_access_bench(size_t testSize) {
	using namespace std;
	string category = "Etc.";
	string subCategory = "The power of parallelism";
	string title = "Random accessing " + to_string(testSize) + " elements";
	string comment = "";
	string unit = "M/s";
	size_t benchOrder = 0;
	vector<string> testCases = { "Random accessing" };
	vector<vector<double>> results;

	results.push_back(linked_list_access(testSize));
	results.push_back(pointer_array_access(testSize));
	lib_calvin_benchmark::save_bench(category, subCategory, title, comment,
									 { {"linked list"}, {"array of pointers"} },
									 results, testCases, unit, benchOrder++);
}

std::vector<double> lib_calvin_benchmark::etc::linked_list_access(size_t testSize) {
	Node *elems = new Node[testSize];
	for (size_t i = 0; i < testSize; i++) {
		elems[i].key_ = i;
		elems[i].data_[0] = 2*i;
		elems[i].data_[1] = 3*i;
		elems[i].data_[2] = 4*i;
		elems[i].next_ = nullptr;
	}
	//std::shuffle(elems, elems + testSize, std::mt19937_64(0));
	size_t primeNumber = 9973;
	size_t position = 0;
	for (size_t i = 0; i < testSize; i++) {
		size_t nextPosition = (position + primeNumber) % testSize;
		elems[position].next_ = &elems[nextPosition];
		position = nextPosition;
	}

	for (size_t i = 0; i < testSize; i++) {
		if (elems[i].next_ == nullptr) {
			std::cout << "linked_list_access\n";
			exit(0);
		}
	}

	// Iterating
	Node *curNode = &elems[0];
	size_t checksum = 0;

	lib_calvin::stopwatch watch;
	watch.start();
	for (size_t i = 0; i < testSize; i++) {
		checksum += size_t(*curNode);
		curNode = curNode->next_;
	}
	watch.stop();

	delete[] elems;
	double speed = testSize / watch.read() / 1000 / 1000;
	std::cout << "Checksum: " << checksum << " speed: " << speed << "\n";
	return { speed };
}

std::vector<double> lib_calvin_benchmark::etc::pointer_array_access(size_t testSize) {
	Node *elems = new Node[testSize];
	for (size_t i = 0; i < testSize; i++) {
		elems[i].key_ = i;
		elems[i].data_[0] = 2*i;
		elems[i].data_[1] = 3*i;
		elems[i].data_[2] = 4*i;
		elems[i].next_ = nullptr;
	}

	Node **pointerArray = new Node*[testSize];
	for (size_t i = 0; i < testSize; i++) {
		pointerArray[i] = &elems[i];
	}
	std::shuffle(pointerArray, pointerArray + testSize, std::mt19937_64(0));

	size_t checksum = 0;
	lib_calvin::stopwatch watch;
	watch.start();
	for (size_t i = 0; i < testSize; i++) {
		checksum += size_t(*pointerArray[i]);
	}
	watch.stop();

	delete[] elems;
	delete[] pointerArray;
	double speed = testSize / watch.read() / 1000 / 1000;
	std::cout << "Checksum: " << checksum << " speed: " << speed << "\n";
	return { speed };
}