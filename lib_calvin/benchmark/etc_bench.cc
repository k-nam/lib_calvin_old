#include <vector>
#include <string>
#include <algorithm>
#include <random>

#include "random.h"
#include "etc_bench.h"
#include "bench.h"
#include "stopwatch.h"
#include "intro_sort.h"
#include "merge_sort.h"


using namespace lib_calvin_benchmark::etc;

void lib_calvin_benchmark::etc::etc_bench() {
	//memory_access_bench();
	//linear_time_sorting_bench();
	sort_by_group_bench();
}

void lib_calvin_benchmark::etc::memory_access_bench() {
	using namespace std;
	string category = "Etc.";
	string subCategory = "Parallelism";
	string title = "Random access speed";
	string comment = "";
	string unit = "M/s";
	size_t benchOrder = 0;
	vector<string> testCases = { "1K", "10K", "100K", "1M", "10M" };
	vector<size_t> testSizes = { 1000, 1000 * 10, 1000 * 100, 1000 * 1000, 1000 * 1000 * 10 };
	vector<vector<double>> results;

	results.push_back(linked_list_access(testSizes));
	results.push_back(pointer_array_access(testSizes));
	lib_calvin_benchmark::save_bench(category, subCategory, title, comment,
									 { {"linked list"}, {"array of pointers"} },
									 results, testCases, unit, benchOrder++);
}

void lib_calvin_benchmark::etc::linear_time_sorting_bench() {
	using namespace std;
	string category = "Etc.";
	string subCategory = "Algorithm complexity (+ overhead), cache efficiency, and parallelism";
	string title = "Sorting elements with special property";
	string comment = "";
	string unit = "M/s";
	size_t benchOrder = 0;
	vector<string> testCases = { "1K", "10K", "100K", "1M", "10M" };
	vector<size_t> testSizes = { 1000, 1000*10, 1000*100, 1000*1000, 1000*1000*10 };
	vector<vector<double>> results;

	results.push_back(linear_method(testSizes));
	results.push_back(linear_method_inplace(testSizes));
	results.push_back(block_qsort(testSizes));

	lib_calvin_benchmark::save_bench(category, subCategory, title, comment,
		{ { "Linear method using O(N) space" }, { "Linear method inplace" }, { "Block qsort" } },
		results, testCases, unit, benchOrder++);
}

void lib_calvin_benchmark::etc::sort_by_group_bench() {
	using namespace std;
	string category = "Etc.";
	string subCategory = "Conditional branches";
	string title = "Sorting by group of " + std::to_string(numGroups);
	string comment = "";
	string unit = "M/s";
	size_t benchOrder = 0;
	vector<string> testCases = { "1K", "10K", "100K", "1M", "10M" };
	vector<size_t> testSizes = { 1000, 1000 * 10, 1000 * 100, 1000 * 1000, 1000 * 1000 * 10 };
	vector<vector<double>> results;

	results.push_back(one_pass_sorting(testSizes));
	results.push_back(two_pass_sorting(testSizes));

	lib_calvin_benchmark::save_bench(category, subCategory, title, comment,
		{ { "One-pass sorting" },{ "Two-pass sorting" } },
		results, testCases, unit, benchOrder++);
}


std::vector<double> lib_calvin_benchmark::etc::linked_list_access(std::vector<size_t> testSizes) {
	
	std::vector<double> results;
	for (size_t testSize : testSizes) {
		Node *elems = new Node[testSize];
		for (size_t i = 0; i < testSize; i++) {
			elems[i].key_ = i;
			elems[i].data_[0] = 2 * i;
			elems[i].data_[1] = 3 * i;
			elems[i].data_[2] = 4 * i;
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
		results.push_back(speed);
		std::cout << "Checksum: " << checksum << " speed: " << speed << "\n";
	}
	return results;
}

std::vector<double> lib_calvin_benchmark::etc::pointer_array_access(std::vector<size_t> testSizes) {

	std::vector<double> results;
	for (size_t testSize : testSizes) {
		Node *elems = new Node[testSize];
		for (size_t i = 0; i < testSize; i++) {
			elems[i].key_ = i;
			elems[i].data_[0] = 2 * i;
			elems[i].data_[1] = 3 * i;
			elems[i].data_[2] = 4 * i;
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
		results.push_back(speed);
		std::cout << "Checksum: " << checksum << " speed: " << speed << "\n";
	}
	return results;
}


std::vector<double> lib_calvin_benchmark::etc::linear_method(std::vector<size_t> testSizes) {
	using namespace lib_calvin;
	stopwatch watch;

	std::vector<double> results;
	for (size_t testSize : testSizes) {
		auto randomArray = getRandomNodeArray(testSize);

		watch.start();
		auto copy = randomArray;
		for (auto node : randomArray) {
			size_t index = node.key_ / 10;
			copy[index] = node;
		}
		randomArray = copy;
		watch.stop();
		results.push_back(testSize / watch.read() / 1000 / 1000);
	}
	return results;
}

std::vector<double> lib_calvin_benchmark::etc::linear_method_inplace(std::vector<size_t> testSizes) {
	using namespace lib_calvin;
	stopwatch watch;

	std::vector<double> results;
	for (size_t testSize : testSizes) {
		auto inputArray = getRandomNodeArray(testSize);

		watch.start();

		size_t start_index = 0;
		while (start_index < inputArray.size()) {
			auto node_to_insert = inputArray[start_index];
			while (true) {
				size_t index_to_insert = node_to_insert.key_ / 10;
				auto temp = inputArray[index_to_insert];
				inputArray[index_to_insert] = node_to_insert;
				node_to_insert = temp;

				if (index_to_insert == start_index) { // we got a cycle
					break;
				}
			}
			start_index++;
			while (true) {
				if (start_index == inputArray.size()) {
					break;;
				} else if (inputArray[start_index].key_ / 10 == start_index) {
					start_index++;
				} else {
					break;
				}
			}
		}

		watch.stop();
		results.push_back(testSize / watch.read() / 1000 / 1000);
	}
	return results;
}

std::vector<double> lib_calvin_benchmark::etc::block_qsort(std::vector<size_t> testSizes) {
	using namespace lib_calvin;
	stopwatch watch;

	std::vector<double> results;
	for (size_t testSize : testSizes) {
		auto inputArray = getRandomNodeArray(testSize);

		watch.start();
		lib_calvin_sort::blockIntroSort(inputArray.begin(), inputArray.end());
		watch.stop();

		results.push_back(testSize / watch.read() / 1000 / 1000);
	}
	return results;
}

std::vector<double> lib_calvin_benchmark::etc::one_pass_sorting(std::vector<size_t> testSizes) {
	using namespace lib_calvin;
	stopwatch watch;

	std::vector<double> results;
	for (size_t testSize : testSizes) {
		auto inputArray = getRandomNodeArray(testSize);

		watch.start();

		lib_calvin_sort::blockIntroSort(inputArray.begin(), inputArray.end(), all_compare());

		watch.stop();
		results.push_back(testSize / watch.read() / 1000 / 1000);
	}
	return results;
}

std::vector<double> lib_calvin_benchmark::etc::two_pass_sorting(std::vector<size_t> testSizes) {
	using namespace lib_calvin;
	stopwatch watch;

	std::vector<double> results;
	for (size_t testSize : testSizes) {
		auto inputArray = getRandomNodeArray(testSize);

		watch.start();

		lib_calvin_sort::blockIntroSort(inputArray.begin(), inputArray.end(), key_compare());
		lib_calvin_sort::mergeSort(inputArray.begin(), inputArray.end(), group_compare());

		watch.stop();
		results.push_back(testSize / watch.read() / 1000 / 1000);
	}
	return results;
}

std::vector<lib_calvin_benchmark::etc::Node> lib_calvin_benchmark::etc::getRandomNodeArray(size_t size) {
	using namespace lib_calvin;

	std::vector<Node> test_vector(size);
	lib_calvin::random_number_generator g;

	for (size_t i = 0; i < size; i++) {
		size_t random = g();
		test_vector[i].key_ = i * 10 + random % 10;
		test_vector[i].group_ = i / numGroups;
	}
	std::shuffle(test_vector.begin(), test_vector.end(),
		std::mt19937(std::random_device()()));

	return test_vector;
}
