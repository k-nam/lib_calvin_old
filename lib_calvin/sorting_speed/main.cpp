// sorting_speed.cpp : Defines the entry point for the console application.
//

#include <vector>
#include <iostream>

#include "record_sorting.h"
#include "user_sorting.h"


int main()
{
	std::cout << "Beginning record_sort_test\n";
	record_sort_test();
	std::cout << "\n";


	std::cout << "Beginning user_sort_test\n";
	user_sort_test();

    return 0;
}

