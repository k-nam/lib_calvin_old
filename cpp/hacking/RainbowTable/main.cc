#include <iostream>
#include <string>

#include "md5.h"
#include "rainbow_table.h"

int main() {
	using namespace hacking__rainbow_table;
	md5_self_test(1);

	std::string password = "aaa21112f";
	std::cout << "hash of " << password << " is " << hacking__rainbow_table::readHashOfPassword(password) << "\n";
	std::cout << "next of " << password << " is " << hacking__rainbow_table::getNextInChain(password) << "\n";


	getChainListFromPasswordList("E:\\password.txt", "E:\\chain.txt");

	
	//getChainListFromPasswordList("E:\\password_under_20digit.txt", "E:\\chain.txt");

}