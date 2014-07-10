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



	class Md5Hash {
	public:
		Md5Hash(unsigned char const hash[20]);
		Md5Hash(std::string hexString);
		std::string toHexString() const;
		unsigned char const *getData() const;
	private:
		unsigned char hash_[20];
	};

	struct Md5Func {
		void operator()(unsigned char const *input, size_t len, unsigned char output[16]) const;
		Md5Hash operator()(std::string password) const;
	};
	struct Md5ReverseFunc {
		void operator()(unsigned char const hash[16], int indexInChain,
									char *password, size_t &generatedPasswordLength) const;
	};

	std::string getReverseOfHash(const unsigned char hash[16], int index);
	std::string readHashOfPassword(std::string password);
	std::string getNextInChain(std::string password, int index);

	std::pair<std::string, std::string> getFirstAndLast(std::string password, int chainLength);
	std::pair<std::string, std::string> getFirstAndLast2(std::string password, int chainLength);
	std::vector<std::string> getChain(std::string hash, int chainLength);
	
	void getFirstAndLast(std::string passwordFileName, std::string outFileName, int chainLength);
	void getChain(std::string hashFileName, std::string outFileName, int chainLength);
}
#endif