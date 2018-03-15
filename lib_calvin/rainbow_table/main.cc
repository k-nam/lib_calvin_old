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
	getFirstAndLast("../../../data/rainbow_table/password/password_10k.txt", 
									"../../../data/rainbow_table/test/first_last_10k.txt", CHAIN_LENGHTH);	

	// Assume I stole some hashes of real users
	hash("../../../data/password/password_10k.txt", "../../../data/rainbow_table/test/hash.txt");

	// Hacking-time operations		
	getChain("../../../data/rainbow_table/test/hash.txt", "../../../data/rainbow_table/test/chain.txt", CHAIN_LENGHTH);
	
	// Query DB needed here: 
	// Find all hashes in chain.txt which matches column 'last' of first-last table to get first.txt

	// Get plaintext password
	getCrackedPassword("../../../data/rainbow_table/test/first.txt", 
									 "../../../data/rainbow_table/test/cracked_password.txt", CHAIN_LENGHTH);
										 									 
}