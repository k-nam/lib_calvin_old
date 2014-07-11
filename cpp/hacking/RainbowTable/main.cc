#include <iostream>
#include <string>

#include "md5.h"
#include "rainbow_table.h"
#include "rainbow_table_test.h"

int main() {
	using namespace std;
	using namespace hacking__rainbow_table;
	hashStringTest();
	getFirstAndLastTest();
	getChainTest();

	//getFirstAndLast("E:\\hacking/password_under_20digit.txt", "E:\\first_last.txt", CHAIN_LENGHTH);
	//getFirstAndLast("E:\\hacking/10k_most_common.txt", "E:\\first_last.txt", CHAIN_LENGHTH);
	
	//hash("E:\\hacking/10k_most_common.txt", "E:\\hash.txt");
	//getChain("E:\\hash.txt", "E:\\chain.txt", CHAIN_LENGHTH);

	//string password = "password";
	//Md5Hash hash = Md5Func()(password);
	//cout << "password: " << password << " hash: " << hash.toHexString() << "\n";

	getCrackedPassword("E:\\first.txt", "E:\\cracked_password.txt", CHAIN_LENGHTH);
}