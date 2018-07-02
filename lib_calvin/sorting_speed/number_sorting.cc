#include <iostream>

#include "number_sorting.h"
#include "stopwatch.h"
#include "random.h"
#include "intro_sort.h"

void number_sort_test() {
	using namespace lib_calvin;
	stopwatch watch;

	int const test_size = 1000 * 1000 * 10;
	std::vector<int> test_vector(test_size, 0);
	lib_calvin::random_number_generator g;

	for (size_t i = 0; i < test_size; i++) {
		size_t random = g();
		test_vector[i] = static_cast<int>(i * 10 + random % 10);
	}
	std::shuffle(test_vector.begin(), test_vector.end(),
		std::mt19937(std::random_device()()));

	auto copy = test_vector;
	auto copy2 = test_vector;

	watch.start();
	number_sort(copy);
	watch.stop();
	std::cout << "number_sort took " << watch.read() << " sec.\n";

	watch.start();
	number_sort_inplace(copy2);
	watch.stop();
	std::cout << "number_sort_inplace took " << watch.read() << " sec.\n";

	watch.start();
	lib_calvin_sort::blockIntroSort(test_vector.begin(), test_vector.end());
	watch.stop();
	std::cout << "blockIntroSort took " << watch.read() << " sec.\n";

	if (test_vector != copy || test_vector != copy2) {
		std::cout << "error!\n";

	}
}

void number_sort(std::vector<int> & input) {
	auto copy = input;
	for (int number : input) {
		size_t index = number / 10;
		//std::cout << "index was: " << index << "\n";
		copy[index] = number;
	}
	input = copy;
}

void number_sort_inplace(std::vector<int> & input) {
	size_t start_index = 0;	

	while (true) {
		int number_to_insert = input[start_index];
		while (true) { 
			size_t index_to_insert = number_to_insert / 10;
			int temp = input[index_to_insert];
			input[index_to_insert] = number_to_insert;
			number_to_insert = temp;

			if (index_to_insert == start_index) { // we got a cycle
				break;
			} 
		}
		start_index++;
		while (true) {
			if (start_index == input.size()) {
				return;
			} else if (input[start_index] / 10 == start_index) {
				start_index++;
			} else {
				break;
			}
		}
	}
}