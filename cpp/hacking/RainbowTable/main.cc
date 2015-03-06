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
	//getFirstAndLast("D:\\dev/data/hacking/data/61M.txt", 
		//								"D:\\dev/data/hacking/data/FirstLast_61M.txt", CHAIN_LENGHTH);
	//getFirstAndLast("D:\\dev/data/hacking/data/10K.txt", 
	//								"D:\\dev/data/hacking/data/FirstLast_10K.txt", CHAIN_LENGHTH);

	
		// make test input (hash values)
//	hash("D:\\dev/data/hacking/data/10K.txt", "D:\\dev/data/hacking/test/hash.txt");
	
	// Hacking-time operations
	//getChain("D:\\dev/data/hacking/test/hash.txt", "D:\\dev/data/hacking/test/chain.txt", CHAIN_LENGHTH);
	
	// Query DB here: select each hash in chains in last column of first-last table to get first.txt

	getCrackedPassword("D:\\dev/data/hacking/test/first.txt", 
									 "D:\\dev/data/hacking/test/cracked_password.txt", CHAIN_LENGHTH);
										 
										 
}