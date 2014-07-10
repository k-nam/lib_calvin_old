#include "md5.h"
#include "rainbow_table_test.h"
#include "rainbow_table.h"
#include <iostream>
#include <string>

void hacking__rainbow_table::hashStringTest() {
	using namespace std;
	hacking__md5::md5_self_test(1);
	
	std::string password = "aaa21112f";
	std::string hashString = hacking__rainbow_table::readHashOfPassword(password);
	Md5Hash hash = Md5Func()(password);
	Md5Hash hash2(hashString);
	std::cout << "hash of " << password << " is " << hash.toHexString() << "\n";
	std::cout << "hash of " << password << " is " << hash2.toHexString() << "\n";
	std::cout << "next of " << password << " is " << 
		hacking__rainbow_table::getNextInChain(password, 0) << "\n";
	
	
}

void hacking__rainbow_table::getFirstAndLastTest() {
	auto password = "abc";
	int length = 100;
	auto firstAndLast = getFirstAndLast(password, length);
	std::cout << "first and last of: " << password << ", length: " << length << "\n";
	std::cout << firstAndLast.first << "\n" << firstAndLast.second << "\n";
	firstAndLast = getFirstAndLast2(password, length);
	std::cout << firstAndLast.first << "\n" << firstAndLast.second << "\n";
}

void hacking__rainbow_table::getChainTest() {
	using namespace std;
	auto password = "abc11";
	int length = 100;
	auto chain = getChain(Md5Func()(password), length); 
	for (vector<string>::const_iterator iter = chain.begin(); iter != chain.end(); iter++) {
		std::cout << *iter << "\n";
	}
}