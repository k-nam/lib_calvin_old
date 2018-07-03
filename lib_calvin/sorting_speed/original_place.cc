#include <iostream>

#include "original_place.h"
#include "stopwatch.h"
#include "random.h"
#include "intro_sort.h"

void original_place_test() {
	using namespace lib_calvin;
	stopwatch watch;

	int const test_size = 1000 * 1000;
	std::vector<record> test_vector(test_size);
	lib_calvin::random_number_generator g;

	for (size_t i = 0; i < test_size; i++) {
		size_t random = g();
		test_vector[i].key_ = i * 10 + random % 10;
	}
	std::shuffle(test_vector.begin(), test_vector.end(),
		std::mt19937(std::random_device()()));

	auto copy = test_vector;
	auto copy2 = test_vector;

	watch.start();
	original_place_sort(copy);
	watch.stop();
	std::cout << "original_place_sort took " << watch.read() << " sec.\n";

	watch.start();
	original_place_sort_inplace(copy2);
	watch.stop();
	std::cout << "original_place_sort_inplace took " << watch.read() << " sec.\n";

	watch.start();
	lib_calvin_sort::blockIntroSort(test_vector.begin(), test_vector.end());
	watch.stop();
	std::cout << "blockIntroSort took " << watch.read() << " sec.\n";

	if (test_vector != copy || test_vector != copy2) {
		std::cout << "error!\n";

	}
}

void original_place_sort(std::vector<record> & input) {
	auto copy = input;
	for (auto record : input) {
		size_t index = record.key_ / 10;
		copy[index] = record;
	}
	input = copy;
}

void original_place_sort_inplace(std::vector<record> & input) {
	size_t start_index = 0;	

	while (true) {
		auto record_to_insert = input[start_index];
		while (true) { 
			size_t index_to_insert = record_to_insert.key_ / 10;
			auto temp = input[index_to_insert];
			input[index_to_insert] = record_to_insert;
			record_to_insert = temp;

			if (index_to_insert == start_index) { // we got a cycle
				break;
			} 
		}
		start_index++;
		while (true) {
			if (start_index == input.size()) {
				return;
			} else if (input[start_index].key_ / 10 == start_index) {
				start_index++;
			} else {
				break;
			}
		}
	}
}