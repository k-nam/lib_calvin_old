#include "rainbow_table.h"
#include "md5.h"
#include <random>

std::string 
hacking__rainbow_table::hashAsString(const unsigned char hash[16]) {
	// method for converting bit data to hex string
	static const char* const lut = "0123456789ABCDEF";

	std::string output;
	output.reserve(16);
	for (int i = 0; i < 16; i++) {
		output.push_back(lut[hash[i] >> 4]);
		output.push_back(lut[hash[i] & 15]);
	}
	return output;
}

std::string 
hacking__rainbow_table::getReverseOfHash(const unsigned char hash[16]) {
	char password[MAX_PASSWORD_LENGTH];
	size_t passwordLength;
	md5Reverse(hash, 0, password, passwordLength);
	return std::string(password, passwordLength);
}

std::string 
hacking__rainbow_table::readHashOfPassword(std::string password) {
	unsigned char hash[16];
	md5(reinterpret_cast<const unsigned char *>(password.c_str()), password.length(), hash);
	return hashAsString(hash);
}

std::string 
hacking__rainbow_table::getNextInChain(std::string password) {
	unsigned char hash[16];
	md5(reinterpret_cast<const unsigned char *>(password.c_str()), password.length(), hash);
	return getReverseOfHash(hash);
}


void hacking__rainbow_table::md5Reverse(const unsigned char hash[16], int indexInChain,
																				 char *password, size_t &generatedPasswordLength) {
	generatedPasswordLength = MIN_PASSWORD_LENGTH + 
														(indexInChain + hash[0]) % (MAX_PASSWORD_LENGTH - MIN_PASSWORD_LENGTH + 1);
	for (size_t i = 0; i < generatedPasswordLength; i++) {
		password[i] = ' ' + hash[i % 16] % ('~' - ' ' + 1);
	}
}

std::pair<std::string, std::string> 
hacking__rainbow_table::md5GenerateChain(std::string password, int chainLength) {
	char passwordBuffer[MAX_PASSWORD_LENGTH + 1];
	size_t passwordLength = password.length();
	unsigned char hashBuffer[16];

	// initial load of password buffer
	for (size_t i = 0; i < password.length(); i++) {
		passwordBuffer[i] = password[i];
	}

	for (int i = 0; i < chainLength; i++) {
		md5(reinterpret_cast<unsigned char *>(passwordBuffer), passwordLength, hashBuffer);
		md5Reverse(hashBuffer, i, passwordBuffer, passwordLength);
	}

	passwordBuffer[passwordLength] = '0';
	return std::make_pair(password, std::string(passwordBuffer));
}