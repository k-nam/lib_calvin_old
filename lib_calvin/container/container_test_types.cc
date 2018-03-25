#include "container_test_types.h"
#include <functional>

lib_calvin_container::HeavyObject::HeavyObject(size_t size) {
	init();
	for (size_t i = 0; i < 5; ++i) {
		value_.push_back(size % 100);
		size = std::hash<size_t>()(size);
	}
}


lib_calvin_container::HeavyObject::operator size_t() const {
	size_t hash = 0;
	for (auto number : value_) {
		hash *= 100;
		hash += number;
	}
	return hash;
}

lib_calvin_container::HeavyObject::~HeavyObject() {
	if (sanityCheckNumber == 22) {
		//std::cout << "HeavyObject destructor normal\n"; 
		sanityCheckNumber = 12;
	} else if (sanityCheckNumber == 12) {
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

std::string lib_calvin_container::getRandomString(int length) {
	char *charArray = new char[length];
	for (int i = 0; i < length; ++i) {
		charArray[i] = 'a' + rand() % 26;
	}
	std::string result(charArray, length);
	delete[] charArray;
	return result;
}

size_t lib_calvin_container::HeavyObject::objectCount = 0;