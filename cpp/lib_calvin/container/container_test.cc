#include "container_test.h"
#include "b_tree_test.h"
#include <vector>

void lib_calvin_container::containerTest()
{
	std::cout << "--------------- Beginning container test -----------------\n\n";
	vectorTest();
	setTest();
	mapTest();
	//bTreeTest();
	std::cout << "-------------- container test finished ------------------\n\n\n";
}

lib_calvin_container::HeavyObject::HeavyObject(int size) {
	init(); 
	int start = size % 3000000;
	for (int i = start; i < start + size % 10; ++i) {
		value_.push_back(i); 
	}
	dummy_[0] = size;
	dummy_[1] = size / 10;
}

lib_calvin_container::HeavyObject::~HeavyObject() {
	if (sanityCheckNumber == 22) { 
		//std::cout << "HeavyObject destructor normal\n"; 
		sanityCheckNumber = 12; 
	} else if (sanityCheckNumber == 12)	{ 
		std::cout << "HeavyObject destructor error: double delete!\n"; 
		exit(0); 
	} else {
		std::cout << "HeavyObject destructor error: no object!\n"; 
		exit(0);
	}
	objectDestroyed();
}

void lib_calvin_container::HeavyObject::init() {
	sanityCheckNumber = 22;
	objectCreated();
}

void lib_calvin_container::HeavyObject::objectCreated() {
	objectCount++;
}

void lib_calvin_container::HeavyObject::objectDestroyed() {
	objectCount--;
}

void lib_calvin_container::HeavyObject::countThisObject() {
	std::cout << "HeavyObject current count is: \"" << objectCount << "\": ";
	if (objectCount == 0) {
		std::cout << "O.K\n\n";
	} else {
		std::cout << "memory leak!!!\n";
		exit(0);
	}
}

int lib_calvin_container::HeavyObject::objectCount = 0;