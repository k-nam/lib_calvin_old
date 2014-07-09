#include "rainbow_table.h"
#include "md5.h"
#include <random>
#include <iostream>
#include <fstream>

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
	md5Reverse2(hash, 0, password, passwordLength);
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
		password[i] = '0' + hash[i % 16] % ('z' - '0' + 1);
	}
}

void hacking__rainbow_table::md5Reverse2(const unsigned char hash[16], int indexInChain,
																				 char *password, size_t &generatedPasswordLength) {
	generatedPasswordLength = MIN_PASSWORD_LENGTH + 
														(indexInChain + hash[0]) % (MAX_PASSWORD_LENGTH - MIN_PASSWORD_LENGTH + 1);
	for (size_t i = 0; i < generatedPasswordLength; i++) {
		if (hash[i % 16] % 3 == 0) {
			password[i] = '0' + (hash[i % 16] + indexInChain) % ('9' - '0' + 1);
		} else if (hash[i % 16] % 3 == 1){
			password[i] = 'a' + (hash[i % 16] + indexInChain) % ('z' - 'a' + 1);
		} else {
			password[i] = 'A' + (hash[i % 16] + indexInChain) % ('Z' - 'A' + 1);
		}
	}
}

std::pair<std::string, std::string> 
hacking__rainbow_table::generateChain(std::string password, int chainLength) {
	char passwordBuffer[MAX_PASSWORD_LENGTH + 1];
	size_t passwordLength = password.length();
	unsigned char hashBuffer[16];

	// initial load of password buffer
	for (size_t i = 0; i < password.length(); i++) {
		passwordBuffer[i] = password[i];
	}

	for (int i = 0; i < chainLength; i++) {
		md5(reinterpret_cast<unsigned char *>(passwordBuffer), passwordLength, hashBuffer);
		md5Reverse2(hashBuffer, i, passwordBuffer, passwordLength);
		//std::cout << getReverseOfHash(hashBuffer) << "\n";
	}

	passwordBuffer[passwordLength] = 0;
	//std::cout << std::string(passwordBuffer) << "\n";
	return std::make_pair(password, std::string(passwordBuffer));
}


void 
hacking__rainbow_table::getChainListFromPasswordList(std::string passwordFileName, std::string chainFileName) {
	std::ifstream inStream(passwordFileName);
	std::ofstream outStream(chainFileName);
	std::string password;
	bool isFirstLine = true;
	while (std::getline(inStream, password)) {
		auto chain = generateChain(password, CHAIN_LENGHTH);
		if (isFirstLine) {
			isFirstLine = false;
		} else {
			outStream << "\n\n";
		}
		outStream << chain.first << "\n" << chain.second;
	}
}