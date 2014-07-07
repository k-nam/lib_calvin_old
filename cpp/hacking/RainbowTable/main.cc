#include "md5.h"
#include "rainbow_table.h"
#include <iostream>

int main() {
	hacking__rainbow_table::md5_self_test(1);

	std::string password = "aaa21112f";
	std::cout << "hash of " << password << " is " << hacking__rainbow_table::readHashOfPassword(password) << "\n";
	std::cout << "next of " << password << " is " << hacking__rainbow_table::getNextInChain(password) << "\n";
}