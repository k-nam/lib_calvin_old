#include <iostream>
#include <string>

#include "md5.h"
#include "rainbow_table.h"
#include "rainbow_table_test.h"

int main() {
	using namespace hacking__rainbow_table;
	//hashStringTest();
	//getFirstAndLastTest();
	//getChainTest();

	// Pre-hacking operations
	//getFirstAndLast("D:\\dev/data/hacking/data/password_under_20digit.txt", 
		//								"D:\\dev/data/hacking/data/password_under_20digit_first_last.txt", CHAIN_LENGHTH);
	//getFirstAndLast("D:\\dev/data/hacking/data/10k_most_common.txt", 
		//								"D:\\dev/data/hacking/test/first_last.txt", CHAIN_LENGHTH);

	
	// Hacking-time operations
//	hash("D:\\dev/data/hacking/data/10k_most_common.txt", "D:\\dev/data/hacking/test/hash.txt");
	
	//getChain("D:\\dev/data/hacking/test/hash.txt", "D:\\dev/data/hacking/test/chain.txt", CHAIN_LENGHTH);
	
	// Query DB here: select each hash in chains in last column of first-last table to get first.txt
	getCrackedPassword("D:\\dev/data/hacking/test/first.txt", 
										 "D:\\dev/data/hacking/test/cracked_password.txt", CHAIN_LENGHTH);
										 
										 
}