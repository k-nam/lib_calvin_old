#include <iostream>
#include <string>

#include "md5.h"
#include "rainbow_table.h"
#include "rainbow_table_test.h"

int main() {
	using namespace hacking__rainbow_table;
	hashStringTest();
	getFirstAndLastTest();

	//getFirstAndLast("E:\\password.txt", "E:\\chain.txt");
	//getFirstAndLast("E:\\hacking/password_under_20digit.txt", "E:\\chain2.txt", CHAIN_LENGHTH);

}