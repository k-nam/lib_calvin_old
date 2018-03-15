#include "rainbow_table.h"
#include "md5.h"
#include "lib.h"
#include <random>
#include <iostream>
#include <fstream>

hacking__rainbow_table::Md5Hash::Md5Hash(Md5Hash const &rhs) {
	copyDataFrom(rhs.getData());
}

hacking__rainbow_table::Md5Hash::Md5Hash(unsigned char const hash[MD5_HASH_LENGTH]) {
	copyDataFrom(hash);
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

hacking__rainbow_table::Md5Hash &
hacking__rainbow_table::Md5Hash::operator=(Md5Hash const &rhs) {
	if (this != &rhs) {
		copyDataFrom(rhs.getData());
	}
	return *this;
}

bool
hacking__rainbow_table::Md5Hash::operator==(Md5Hash const &rhs) const {
	for (int i =  0; i < MD5_HASH_LENGTH; i++) {
		if (hash_[i] != rhs.getData()[i]) {
			return false;
		}
	}	
	return true;
}

void
hacking__rainbow_table::Md5Hash::copyDataFrom(unsigned char const source[MD5_HASH_LENGTH]) {
	for (int i =  0; i < MD5_HASH_LENGTH; i++) {
		hash_[i] = source[i];
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
hacking__rainbow_table::Md5ReverseFunc::operator()(Md5Hash hash, int indexInChain) const {
	char password[MAX_PASSWORD_LENGTH];
	size_t passwordLength = 0;
	Md5ReverseFunc()(hash.getData(), indexInChain, password, passwordLength);
	return std::string(password, passwordLength);
}

std::string 
hacking__rainbow_table::readHashOfPassword(std::string password) {
	return Md5Func()(password).toHexString();
}

std::string 
hacking__rainbow_table::getNextInChain(std::string password, int index) {
	Md5Hash hash = Md5Func()(password);
	return Md5ReverseFunc()(hash, index);
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

std::vector<std::string> 
hacking__rainbow_table::getChain(Md5Hash hash, int chainLength) {
	using namespace std;
	vector<string> chain;
	std::string password;
	for (int i = 0; i < chainLength; i++) {
		password = Md5ReverseFunc()(hash, i);
		for (int j = i + 1; j < chainLength; j++) {
			password = getNextInChain(password, j);
		}
		chain.push_back(password);
	}
	return chain;
}

std::vector<std::string>
hacking__rainbow_table::getCrackedPassword(Md5Hash hash, std::string first, int chainLength) {
	//std::cout << "get cracked hash: " << hash.toHexString() << " first: " << first << "\n";
	std::vector<std::string> crackedPassword;
	std::string password = first;
	for (int i = 0; i < chainLength - 1; i++) {
		//std::cout << "current password:___" << password << "___\n";
		Md5Hash currentHash = Md5Func()(password);
		//std::cout << "current hash: " << currentHash.toHexString() << "\n";
		if (currentHash == hash) {
			crackedPassword.push_back(password);
		} else {
		}
		password = Md5ReverseFunc()(currentHash, i);
	}
	return crackedPassword;
}

void 
hacking__rainbow_table::hash(std::string passwordFileName, std::string outFileName) {
	std::ifstream inStream(passwordFileName);
	std::ofstream outStream(outFileName);
	std::string line;
	while (std::getline(inStream, line)) {
		Md5Hash hash = Md5Func()(line);
		outStream << hash.toHexString();
		outStream << "\n";
	}
	inStream.close();
	outStream.close();
}

void 
hacking__rainbow_table::getFirstAndLast(
							std::string passwordFileName, std::string outFileName, int chainLength) {
	std::ifstream inStream(passwordFileName);
	std::ofstream outStream(outFileName);
	std::string password;
	while (std::getline(inStream, password)) {
		auto chain = getFirstAndLast2(password, chainLength);
		outStream << chain.first << "\t" << chain.second << '\n';
	}
	inStream.close();
	outStream.close();
}

void 
hacking__rainbow_table::getChain(
							std::string hashFileName, std::string outFileName, int chainLength) {
	std::ifstream inStream(hashFileName);
	std::ofstream outStream(outFileName);
	std::string line;
	while (std::getline(inStream, line)) {
		auto chain = getChain(Md5Hash(line), chainLength);
		outStream << line << '\t';
		for (auto password: chain) {
			outStream << password << '\t';
		}
		outStream << '\n';
	}
	inStream.close();
	outStream.close();
}


void 
hacking__rainbow_table::getCrackedPassword(
				std::string firstFileName, std::string outFileName, int chainLength) {
	using namespace std;
	ifstream inStream(firstFileName);
	ofstream outStream(outFileName);
	string line;
	unsigned char dummy[MD5_HASH_LENGTH]; // Md5Hash doesn't have default ctor
	Md5Hash hash(dummy);
	while (getline(inStream, line)) {
		if (line == "") {
			continue;
		}
		auto words = hacking__lib::split(line, '\t');
		auto iterator = words.begin();
		auto stringHex = *iterator; // first word is hex string
		hash = Md5Hash(stringHex); // read hex string
		outStream << stringHex;
		outStream << "\t";
		for (iterator++; iterator != words.end(); iterator++) {
			auto passwords = getCrackedPassword(hash, *iterator, chainLength);
			for (auto password: passwords) {
				outStream << password;
				outStream << "\t";
			}
		}
		outStream << "\n";
	}
	inStream.close();
	outStream.close();
}


