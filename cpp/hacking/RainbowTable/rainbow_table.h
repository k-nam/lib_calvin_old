#ifndef HACKING__RAINBOW_TABLE__RAINBOW_TABLE_H
#define HACKING__RAINBOW_TABLE__RAINBOW_TABLE_H

#include <vector>
#include <string>

namespace hacking__rainbow_table
{
	int const MD5_HASH_LENGTH = 16;
	int const MIN_PASSWORD_LENGTH = 6;
	int const MAX_PASSWORD_LENGTH = 20;
	int const CHAIN_LENGHTH = 10;

	std::string hashAsString(const unsigned char hash[16]);
	std::string getReverseOfHash(const unsigned char hash[16]);
	std::string readHashOfPassword(std::string password);
	std::string getNextInChain(std::string password);
	void md5Reverse(const unsigned char hash[16], int indexInChain,
									 char *password, size_t &generatedPasswordLength);
	void md5Reverse2(const unsigned char hash[16], int indexInChain,
									char *password, size_t &generatedPasswordLength);
	std::pair<std::string, std::string> generateChain(std::string password, int chainLength);

	void getChainListFromPasswordList(std::string passwordFileName, std::string chainFileName);
}
#endif