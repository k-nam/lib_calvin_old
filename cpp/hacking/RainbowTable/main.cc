#include <iostream>
#include <string>

#include "md5.h"
#include "rainbow_table.h"
#include "rainbow_table_test.h"

int main() {
	using namespace hacking__rainbow_table;
	hashStringTest();
	getFirstAndLastTest();
	getChainTest();

	//getFirstAndLast("E:\\hacking/password_under_20digit.txt", "E:\\first_last.txt", CHAIN_LENGHTH);
	//getFirstAndLast("E:\\hash.txt", "E:\\chain.txt", CHAIN_LENGHTH);
	hash("E:\\password.txt", "E:\\hash.txt");
	getFirstAndLast("E:\\password.txt", "E:\\first_last.txt", CHAIN_LENGHTH);
	getChain("E:\\hash.txt", "E:\\chain.txt", CHAIN_LENGHTH);
}