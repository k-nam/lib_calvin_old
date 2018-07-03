#include <iostream>

#include "sort_by_group.h"
#include "stopwatch.h"
#include "random.h"

#include "merge_sort.h"
#include "intro_sort.h"

void sort_by_group_test() {
	using namespace lib_calvin;
	stopwatch watch;

	size_t const test_size = 1000 * 1000;
	size_t const group_size = 1000 * 100;
	size_t const num_groups = test_size / group_size;

	std::vector<user> test_vector(test_size);
	lib_calvin::random_number_generator g;

	for (size_t i = 0; i < test_size; i++) {
		size_t random = g();
		test_vector[i].group_ = g() % num_groups;
		test_vector[i].score_ = g();
	}

	std::shuffle(test_vector.begin(), test_vector.end(),
		std::mt19937(std::random_device()()));

	auto copy = test_vector;
	auto copy2 = test_vector;

	watch.start();
	sort_by_group_one_pass(test_vector);
	watch.stop();
	std::cout << "sort_by_group_one_pass took " << watch.read() << " sec.\n";

	watch.start();
	sort_by_group_two_pass(copy);
	watch.stop();
	std::cout << "sort_by_group_two_pass took " << watch.read() << " sec.\n";


	if (test_vector != copy) {
		std::cout << "error!\n";

	}
}

void sort_by_group_one_pass(std::vector<user> &input) {
	lib_calvin_sort::blockIntroSort(input.begin(), input.end(), user_all_compare());
}

void sort_by_group_two_pass(std::vector<user> &input) {
	lib_calvin_sort::blockIntroSort(input.begin(), input.end(), user_score_compare());
	lib_calvin_sort::mergeSort(input.begin(), input.end(), user_group_compare());

}