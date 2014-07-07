#ifndef HACKING__RAINBOW_TABLE__RAINBOW_TABLE_H
#define HACKING__RAINBOW_TABLE__RAINBOW_TABLE_H

#include <vector>
#include <string>

namespace hacking__rainbow_table
{
	int const MIN_PASSWORD_LENGTH = 6;
	int const MAX_PASSWORD_LENGTH = 20;

	std::string hashAsString(const unsigned char hash[16]);
	std::string getReverseOfHash(const unsigned char hash[16]);
	std::string readHashOfPassword(std::string password);
	std::string getNextInChain(std::string password);
	void md5Reverse(const unsigned char hash[16], int indexInChain,
									 char *password, size_t &generatedPasswordLength);

	std::pair<std::string, std::string> md5GenerateChain(std::string password, int chainLength);
}
#endif