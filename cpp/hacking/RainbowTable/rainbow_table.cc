#include "rainbow_table.h"
#include "md5.h"
#include <random>
#include <iostream>
#include <fstream>



hacking__rainbow_table::Md5Hash::Md5Hash(unsigned char const hash[20]) {
	for (int i =  0; i < MD5_HASH_LENGTH; i++) {
		hash_[i] = hash[i];
	}
}

hacking__rainbow_table::Md5Hash::Md5Hash(std::string stringHash) {
	//convert hexstring to len bytes of data
	//returns 0 on success, -1 on error
	//data is a buffer of at least len bytes
	//hexstring is upper or lower case hexadecimal, NOT prepended with "0x"
	const char *hexString = stringHash.c_str();
	int len = 16;
  const char *pos = hexString;
  char *endptr;
  if ((hexString[0] == '\0') || (strlen(hexString) % 2)) {
		std::cout << "Md5Hash ctor error!\n";
		return;
  }
  for(size_t count = 0; count < len; count++) {
    char buf[5] = {'0', 'x', pos[0], pos[1], 0};
    hash_[count] = static_cast<unsigned char>(strtol(buf, &endptr, 0));
    pos += 2 * sizeof(char);
    if (endptr[0] != '\0') {
        //non-hexadecimal character encountered
			std::cout << "Md5Hash ctor error2!\n";
		  return;
    }
  }
}

std::string
hacking__rainbow_table::Md5Hash::toHexString() const {
	// method for converting bit data to hex string
	static char const * const lut = "0123456789ABCDEF";
	std::string output;
	output.reserve(32);
	for (int i = 0; i < 16; i++) {
		output.push_back(lut[hash_[i] >> 4]);
		output.push_back(lut[hash_[i] & 15]);
	}
	return output;
}


unsigned char const *
hacking__rainbow_table::Md5Hash::getData() const {
	return hash_;
}

void
hacking__rainbow_table::Md5Func::operator()(
					unsigned const char *input, size_t len, unsigned char output[16]) const {
	hacking__md5::md5(input, len, output);
}

hacking__rainbow_table::Md5Hash
hacking__rainbow_table::Md5Func::operator()(std::string password) const {
	unsigned char hash[16];
	Md5Func()(reinterpret_cast<const unsigned char *>(password.c_str()), password.length(), hash);
	return Md5Hash(hash);
}

void
hacking__rainbow_table::Md5ReverseFunc::operator()(const unsigned char hash[16], int indexInChain,
									char *password, size_t &generatedPasswordLength) const {
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

std::string 
hacking__rainbow_table::getReverseOfHash(const unsigned char hash[16], int index) {
	char password[MAX_PASSWORD_LENGTH];
	size_t passwordLength;
	Md5ReverseFunc()(hash, index, password, passwordLength);
	return std::string(password, passwordLength);
}

std::string 
hacking__rainbow_table::readHashOfPassword(std::string password) {
	return Md5Func()(password).toHexString();
}

std::string 
hacking__rainbow_table::getNextInChain(std::string password, int index) {
	unsigned char hash[16];
	Md5Func()(reinterpret_cast<const unsigned char *>(password.c_str()), password.length(), hash);
	return getReverseOfHash(hash, index);
}

std::pair<std::string, std::string> 
hacking__rainbow_table::getFirstAndLast(std::string password, int chainLength) {
	char passwordBuffer[MAX_PASSWORD_LENGTH + 1];
	size_t passwordLength = password.length();
	unsigned char hashBuffer[16];

	// initial load of password buffer
	for (size_t i = 0; i < password.length(); i++) {
		passwordBuffer[i] = password[i];
	}

	for (int i = 0; i < chainLength; i++) {
		Md5Func()(reinterpret_cast<unsigned char *>(passwordBuffer), passwordLength, hashBuffer);
		Md5ReverseFunc()(hashBuffer, i, passwordBuffer, passwordLength);
		//std::cout << getReverseOfHash(hashBuffer) << "\n";
	}

	passwordBuffer[passwordLength] = 0;
	//std::cout << std::string(passwordBuffer) << "\n";
	return std::make_pair(password, std::string(passwordBuffer));
}

std::pair<std::string, std::string> 
hacking__rainbow_table::getFirstAndLast2(std::string password, int chainLength) {
	std::string last = password;
	for (int i = 0; i < chainLength; i++) {
		last = getNextInChain(last, i);
	}
	return std::make_pair(password, last);
}

void 
hacking__rainbow_table::getFirstAndLast(
							std::string passwordFileName, std::string outFileName, int chainLength) {
	std::ifstream inStream(passwordFileName);
	std::ofstream outStream(outFileName);
	std::string password;
	bool isFirstLine = true;
	while (std::getline(inStream, password)) {
		auto chain = getFirstAndLast2(password, chainLength);
		if (isFirstLine) {
			isFirstLine = false;
		} else {
			outStream << "\n\n";
		}
		outStream << chain.first << "\n" << chain.second;
	}
}

void 
hacking__rainbow_table::getChain(
							std::string hashFileName, std::string outFileName, int chainLength) {
	std::ifstream inStream(hashFileName);
	std::ofstream outStream(outFileName);
	std::string hash;
	bool isFirstLine = true;
	while (std::getline(inStream, hash)) {
		//vector<std::string> chain = 
		if (isFirstLine) {
			isFirstLine = false;
		} else {
			outStream << "\n\n";
		}
	}
}


std::vector<std::string> 
hacking__rainbow_table::getChain(std::string hash, int chainLength) {
	using namespace std;
	vector<string> chain(chainLength);
	for (int i = 0; i < chainLength; i++) {
	}
}