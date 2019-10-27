#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <unordered_set>

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
	//sort_by_group_bench();
	//following_link_bench();
	n_nlogn_bench();
}

void lib_calvin_benchmark::etc::memory_access_bench() {
	using namespace std;
	string category = "Etc.";
	string subCategory = "Instruction-level parallelism";
	string title = "Iterating speed (minimum working set)";
	string comment = "";
	string unit = "M/s";
	size_t benchOrder = 0;
	vector<string> testCases = { "1K", "10K", "100K", "1M", "10M" };
	vector<size_t> testSizes = { 1000, 1000 * 10, 1000 * 100, 1000 * 1000, 1000 * 1000 * 10 };
	vector<vector<double>> results;

	results.push_back(linked_list_access());
	results.push_back(pointer_array_access());
	lib_calvin_benchmark::save_bench(category, subCategory, title, comment,
									 { {"linked list"}, {"array of pointers"} },
									 results, testCases, unit, benchOrder++);
}

void lib_calvin_benchmark::etc::linear_time_sorting_bench() {
	using namespace std;
	string category = "Etc.";
	string subCategory = "Algorithm complexity (+ overhead), cache efficiency, and instruction-level parallelism";
	string title = "Sorting (final positions of elemements are known)";
	string comment = "";
	string unit = "M/s";
	size_t benchOrder = 0;
	vector<string> testCases = { "1K", "10K", "100K", "1M", "10M" };
	vector<size_t> testSizes = { 1000, 1000*10, 1000*100, 1000*1000, 1000*1000*10 };
	vector<vector<double>> results;

	results.push_back(linear_method());
	results.push_back(linear_method_inplace());
	results.push_back(block_qsort());

	lib_calvin_benchmark::save_bench(category, subCategory, title, comment,
		{ { "Linear method using O(N) space" }, { "Linear method inplace" }, { "Block qsort" } },
		results, testCases, unit, benchOrder++);
}

void lib_calvin_benchmark::etc::sort_by_group_bench() {
	using namespace std;
	string category = "Etc.";
	string subCategory = "Conditional branches";
	string title = "Sorting into " + std::to_string(numGroups) + " groups, and sorting each group";
	string comment = "";
	string unit = "M/s";
	size_t benchOrder = 0;
	vector<string> testCases = { "1K", "10K", "100K", "1M", "10M" };
	vector<size_t> testSizes = { 1000, 1000 * 10, 1000 * 100, 1000 * 1000, 1000 * 1000 * 10 };
	vector<vector<double>> results;

	results.push_back(one_pass_sorting());
	results.push_back(two_pass_sorting());

	lib_calvin_benchmark::save_bench(category, subCategory, title, comment,
		{ { "One-pass sorting" },{ "Two-pass sorting" } },
		results, testCases, unit, benchOrder++);
}

void lib_calvin_benchmark::etc::following_link_bench() {
	using namespace std;
	string category = "Etc.";
	string subCategory = "Conditional branches";
	string title = "Iterating binary tree";
	string comment = "";
	string unit = "M/s";
	size_t benchOrder = 0;
	vector<string> testCases = { "1K", "10K", "100K", "1M", "10M" };
	vector<size_t> testSizes = { 1000, 1000 * 10, 1000 * 100, 1000 * 1000, 1000 * 1000 * 10 };
	vector<vector<double>> results;

	
	results.push_back(null_pointer());
	results.push_back(null_flag());

	lib_calvin_benchmark::save_bench(category, subCategory, title, comment,
		{ { "null pointer" }, { "null flag" } },
		results, testCases, unit, benchOrder++);
}

void lib_calvin_benchmark::etc::n_nlogn_bench() {
	using namespace std;
	string category = "Etc.";
	string subCategory = "Algorithm complexity (+ overhead), cache efficiency, and instruction-level parallelism";
	string title = "O(N) vs O(Nlog(N))";
	string comment = "";
	string unit = "M/s";
	size_t benchOrder = 0;
	vector<string> testCases = { "1K", "10K", "100K", "1M", "10M" };
	vector<size_t> testSizes = { 1000, 1000 * 10, 1000 * 100, 1000 * 1000, 1000 * 1000 * 10 };
	vector<vector<double>> results;


	results.push_back(hashing());
	results.push_back(build_tree());
	results.push_back(sorting());
	results.push_back(block_qsort_int());

	lib_calvin_benchmark::save_bench(category, subCategory, title, comment,
		{ { "std::unordered_set" }, { "std::set" }, { "std::sort" }, { "lib_calvin::block_qsort" } },
		results, testCases, unit, benchOrder++);
}

std::vector<double> lib_calvin_benchmark::etc::linked_list_access() {	
	std::vector<double> results;
	for (size_t i = 0; i < testSizes.size(); i++) {
		size_t testSize = testSizes[i];
		size_t numIter = numIters[i];		

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

		double min = 1000000;
		size_t checksum = 0;
		Node *curNode = &elems[0];
		for (size_t j = 0; j < numIter; j++) {
			lib_calvin::stopwatch watch;

			watch.start();
			for (size_t i = 0; i < testSize; i++) {
				checksum += size_t(*curNode);
				curNode = curNode->next_;
			}
			watch.stop();

			if (watch.read() < min) {
				min = watch.read();
			}
		}

		delete[] elems;
		double speed = testSize / min / 1000 / 1000;
		results.push_back(speed);
		std::cout << "Checksum: " << checksum << " speed: " << speed << "\n";
	}
	return results;
}

std::vector<double> lib_calvin_benchmark::etc::pointer_array_access() {

	std::vector<double> results;
	for (size_t i = 0; i < testSizes.size(); i++) {
		size_t testSize = testSizes[i];
		size_t numIter = numIters[i];

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
		
		size_t checksum = 0;
		lib_calvin::stopwatch watch;
		double min = 1000000;
		for (size_t j = 0; j < numIter; j++) {
			std::shuffle(pointerArray, pointerArray + testSize, std::mt19937_64(0));

			watch.start();
			for (size_t i = 0; i < testSize; i++) {
				checksum += size_t(*pointerArray[i]);
			}
			watch.stop();
			if (watch.read() < min) {
				min = watch.read();
			}
		}

		delete[] elems;
		delete[] pointerArray;
		double speed = testSize / min / 1000 / 1000;
		results.push_back(speed);
		std::cout << "Checksum: " << checksum << " speed: " << speed << "\n";
	}
	return results;
}


std::vector<double> lib_calvin_benchmark::etc::linear_method() {
	using namespace lib_calvin;
	stopwatch watch;

	std::vector<double> results;
	for (size_t i = 0; i < testSizes.size(); i++) {
		size_t testSize = testSizes[i];
		size_t numIter = numIters[i];		

		double min = 1000000;
		for (size_t j = 0; j < numIter; j++) {
			auto randomArray = getRandomNodeArray(testSize);

			watch.start();
			auto copy = randomArray;
			for (auto node : randomArray) {
				size_t index = node.key_ / 10;
				copy[index] = node;
			}
			randomArray = copy;
			watch.stop();
			if (watch.read() < min) {
				min = watch.read();
			}
		}
		double speed = testSize / min / 1000 / 1000;
		results.push_back(speed);
		std::cout << "linear_method speed: " << speed << "\n";

	}
	return results;
}

std::vector<double> lib_calvin_benchmark::etc::linear_method_inplace() {
	using namespace lib_calvin;
	stopwatch watch;

	std::vector<double> results;
	for (size_t i = 0; i < testSizes.size(); i++) {
		size_t testSize = testSizes[i];
		size_t numIter = numIters[i];	

		double min = 1000000;
		for (size_t j = 0; j < numIter; j++) {
			auto inputArray = getRandomNodeArray(testSize);
			auto copy = inputArray;

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
			if (watch.read() < min) {
				min = watch.read();
			}
			lib_calvin_sort::blockIntroSort(copy.begin(), copy.end());
			if (inputArray != copy) {
				std::cout << "linear_method_inplace error\n";
				exit(0);
			}
		}
		double speed = testSize / min / 1000 / 1000;
		results.push_back(speed);
		std::cout << "linear_method_inplace speed: " << speed << "\n";
	}
	return results;
}

std::vector<double> lib_calvin_benchmark::etc::block_qsort() {
	using namespace lib_calvin;
	stopwatch watch;

	std::vector<double> results;
	for (size_t i = 0; i < testSizes.size(); i++) {
		size_t testSize = testSizes[i];
		size_t numIter = numIters[i];		

		double min = 1000000;
		for (size_t j = 0; j < numIter; j++) {
			auto inputArray = getRandomNodeArray(testSize);

			watch.start();
			lib_calvin_sort::blockIntroSort(inputArray.begin(), inputArray.end());
			watch.stop();
			if (watch.read() < min) {
				min = watch.read();
			}
		}
		double speed = testSize / min / 1000 / 1000;
		results.push_back(speed);
		std::cout << "block_qsort speed: " << speed << "\n";
	}
	return results;
}

std::vector<double> lib_calvin_benchmark::etc::one_pass_sorting() {
	using namespace lib_calvin;
	stopwatch watch;

	std::vector<double> results;
	for (size_t i = 0; i < testSizes.size(); i++) {
		size_t testSize = testSizes[i];
		size_t numIter = numIters[i];

		double min = 1000000;
		for (size_t j = 0; j < numIter; j++) {
			auto inputArray = getRandomNodeArray(testSize);
			watch.start();
			lib_calvin_sort::blockIntroSort(inputArray.begin(), inputArray.end(), all_compare());
			watch.stop();

			if (watch.read() < min) {
				min = watch.read();
			}
		}

		double speed = testSize / min / 1000 / 1000;
		results.push_back(speed);
		std::cout << "one_pass_sorting speed: " << speed << "\n";
	}
	return results;
}

std::vector<double> lib_calvin_benchmark::etc::two_pass_sorting() {
	using namespace lib_calvin;
	stopwatch watch;

	std::vector<double> results;
	for (size_t i = 0; i < testSizes.size(); i++) {
		size_t testSize = testSizes[i];
		size_t numIter = numIters[i];

		double min = 1000000;
		for (size_t j = 0; j < numIter; j++) {
			auto inputArray = getRandomNodeArray(testSize);
			auto copy = inputArray;

			watch.start();

			lib_calvin_sort::blockIntroSort(inputArray.begin(), inputArray.end(), key_compare());
			lib_calvin_sort::mergeSort(inputArray.begin(), inputArray.end(), group_compare());

			watch.stop();

			if (watch.read() < min) {
				min = watch.read();
			}

			lib_calvin_sort::blockIntroSort(copy.begin(), copy.end(), all_compare());
			if (inputArray != copy) {
				std::cout << "two_pass_sorting error\n";
				exit(0);
			}
		}

		double speed = testSize / min / 1000 / 1000;
		results.push_back(speed);
		std::cout << "two_pass_sorting speed: " << speed << "\n";
	}
	return results;
}

std::vector<double> lib_calvin_benchmark::etc::null_pointer() {
	std::vector<double> results;
	for (size_t i = 0; i < testSizes.size(); i++) {
		size_t testSize = testSizes[i];
		size_t numIter = numIters[i];

		Node *null_node = new Node();
		null_node->is_null = true;

		Node **chains = new Node *[testSize];
		for (size_t i = 0; i < testSize; i++) {
			Node * node1 = new Node();
			Node * node2 = new Node();
			node1->next_ = node2;
			node2->next_ = nullptr;
			node1->is_null = false;
			node2->is_null = true;

			chains[i] = node1;
		}

		double min = 1000000;
		size_t checksum = 0;		
		for (size_t j = 0; j < numIter; j++) {
			lib_calvin::stopwatch watch;

			watch.start();
			
			size_t index = 0;
			for (size_t i = 0; i < testSize; i++) {
				auto curNode = chains[index];
				
				while (curNode) {
					checksum += size_t(*curNode);
					curNode = curNode->next_;
				}
				
				index = (index + 9973) % testSize;
			}

			watch.stop();

			if (watch.read() < min) {
				min = watch.read();
			}
		}

		for (size_t i = 0; i < testSize; i++) {
			delete chains[i]->next_;
			delete chains[i];
		}

		double speed = testSize / min / 1000 / 1000;
		results.push_back(speed);
		std::cout << "null_pointer Checksum: " << checksum << " speed: " << speed << "\n";
	}
	return results;
}

std::vector<double> lib_calvin_benchmark::etc::null_flag() {
	std::vector<double> results;
	for (size_t i = 0; i < testSizes.size(); i++) {
		size_t testSize = testSizes[i];
		size_t numIter = numIters[i];

		Node **chains = new Node *[testSize];
		for (size_t i = 0; i < testSize; i++) {
			Node * node1 = new Node();
			Node * node2 = new Node();
			node1->next_ = node2;
			node2->next_ = nullptr; 
			node1->is_null = false;
			node2->is_null = true;

			chains[i] = node1;
		}

		double min = 1000000;
		size_t checksum = 0;
		for (size_t j = 0; j < numIter; j++) {
			lib_calvin::stopwatch watch;

			watch.start();

			size_t index = 0;
			for (size_t i = 0; i < testSize; i++) {
				auto curNode = chains[index];

				while (!curNode->is_null) {
					checksum += size_t(*curNode);
					curNode = curNode->next_;
				}
				index = (index + 9973) % testSize;
			}

			watch.stop();

			if (watch.read() < min) {
				min = watch.read();
			}
		}

		for (size_t i = 0; i < testSize; i++) {
			delete chains[i]->next_;
			delete chains[i];
		}

		double speed = testSize / min / 1000 / 1000;
		results.push_back(speed);
		std::cout << "null flag Checksum: " << checksum << " speed: " << speed << "\n";
	}
	return results;
}

std::vector<double> lib_calvin_benchmark::etc::hashing() {
	using namespace lib_calvin;
	stopwatch watch;

	std::vector<double> results;
	for (size_t i = 0; i < testSizes.size(); i++) {
		size_t testSize = testSizes[i];
		size_t numIter = numIters[i];

		double min = 1000000;
		for (size_t j = 0; j < numIter; j++) {
			auto inputArray = getRandomIntArray(testSize);
			std::unordered_set<size_t> hashSet;

			watch.start();

			for (size_t i = 0; i < testSize; i++) {
				hashSet.insert(inputArray[i]);
			}

			watch.stop();

			if (watch.read() < min) {
				min = watch.read();
			}
		}

		double speed = testSize / min / 1000 / 1000;
		results.push_back(speed);
		std::cout << "hashing speed: " << speed << "\n";
	}
	return results;
}

std::vector<double> lib_calvin_benchmark::etc::build_tree() {
	using namespace lib_calvin;
	stopwatch watch;

	std::vector<double> results;
	for (size_t i = 0; i < testSizes.size(); i++) {
		size_t testSize = testSizes[i];
		size_t numIter = numIters[i];

		double min = 1000000;
		for (size_t j = 0; j < numIter; j++) {
			auto inputArray = getRandomIntArray(testSize);
			std::set<size_t> treeSet;

			watch.start();

			for (size_t i = 0; i < testSize; i++) {
				treeSet.insert(inputArray[i]);
			}

			watch.stop();

			if (watch.read() < min) {
				min = watch.read();
			}
		}

		double speed = testSize / min / 1000 / 1000;
		results.push_back(speed);
		std::cout << "tree speed: " << speed << "\n";
	}
	return results;
}

std::vector<double> lib_calvin_benchmark::etc::sorting() {
	using namespace lib_calvin;
	stopwatch watch;

	std::vector<double> results;
	for (size_t i = 0; i < testSizes.size(); i++) {
		size_t testSize = testSizes[i];
		size_t numIter = numIters[i];

		double min = 1000000;
		for (size_t j = 0; j < numIter; j++) {
			auto inputArray = getRandomIntArray(testSize);
			auto copy = inputArray;

			watch.start();

			std::sort(inputArray.begin(), inputArray.end());

			watch.stop();

			if (watch.read() < min) {
				min = watch.read();
			}
		}

		double speed = testSize / min / 1000 / 1000;
		results.push_back(speed);
		std::cout << "tree speed: " << speed << "\n";
	}
	return results;
}

std::vector<double> lib_calvin_benchmark::etc::block_qsort_int() {
	using namespace lib_calvin;
	stopwatch watch;

	std::vector<double> results;
	for (size_t i = 0; i < testSizes.size(); i++) {
		size_t testSize = testSizes[i];
		size_t numIter = numIters[i];

		double min = 1000000;
		for (size_t j = 0; j < numIter; j++) {
			auto inputArray = getRandomIntArray(testSize);
			auto copy = inputArray;

			watch.start();

			lib_calvin_sort::blockIntroSort(inputArray.begin(), inputArray.end());

			watch.stop();

			if (watch.read() < min) {
				min = watch.read();
			}
		}

		double speed = testSize / min / 1000 / 1000;
		results.push_back(speed);
		std::cout << "tree speed: " << speed << "\n";
	}
	return results;
}

std::vector<size_t> lib_calvin_benchmark::etc::getRandomIntArray(size_t size) {
	using namespace lib_calvin;
	std::vector<size_t> test_vector(size);
	for (size_t i = 0; i < size; i++) {
		test_vector[i] = i;
	}
	std::shuffle(test_vector.begin(), test_vector.end(),
		std::mt19937(std::random_device()()));
	return test_vector;
}

std::vector<lib_calvin_benchmark::etc::Node> lib_calvin_benchmark::etc::getRandomNodeArray(size_t size) {
	using namespace lib_calvin;

	std::vector<Node> test_vector(size);
	lib_calvin::random_number_generator g;

	for (size_t i = 0; i < size; i++) {
		size_t random = g();
		test_vector[i].key_ = i * 10; // multiply 10 to use it in linear_method
		test_vector[i].group_ = i % numGroups;
	}
	std::shuffle(test_vector.begin(), test_vector.end(),
		std::mt19937(std::random_device()()));

	return test_vector;
}
