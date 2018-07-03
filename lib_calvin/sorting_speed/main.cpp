// sorting_speed.cpp : Defines the entry point for the console application.
//

#include <vector>
#include <iostream>

#include "original_place.h"
#include "sort_by_group.h"


int main()
{
	std::cout << "Beginning original place test\n";
	original_place_test();
	std::cout << "\n";


	std::cout << "Beginning sort by group test\n";
	sort_by_group_test();

    return 0;
}

