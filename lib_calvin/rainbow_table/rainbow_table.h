#ifndef HACKING__RAINBOW_TABLE__RAINBOW_TABLE_H
#define HACKING__RAINBOW_TABLE__RAINBOW_TABLE_H

#include <vector>
#include <string>

namespace hacking__rainbow_table
{
	using std::vector;
	int const MD5_HASH_LENGTH = 16;
	int const MIN_PASSWORD_LENGTH = 6;
	int const MAX_PASSWORD_LENGTH = 20;
	int const CHAIN_LENGHTH = 10;

	class Md5Hash {
	public:
		Md5Hash(Md5Hash const &rhs);
		explicit Md5Hash(unsigned char const hash[MD5_HASH_LENGTH]);
		explicit Md5Hash(std::string hexString);
		Md5Hash & operator=(Md5Hash const &rhs);
		bool operator==(Md5Hash const &rhs) const;
	public:
		std::string toHexString() const;
		unsigned char const *getData() const;
	private:
		void copyDataFrom(unsigned char const source[MD5_HASH_LENGTH]);
	private:
		unsigned char hash_[MD5_HASH_LENGTH];
	};

	struct Md5Func {
		void operator()(unsigned char const *input, size_t len, unsigned char output[16]) const;
		Md5Hash operator()(std::string password) const;
	};
	struct Md5ReverseFunc {
		void operator()(unsigned char const hash[16], int indexInChain,
									char *password, size_t &generatedPasswordLength) const;
		std::string operator()(Md5Hash hash, int indexInChain) const;
	};

	std::string readHashOfPassword(std::string password);
	std::string getNextInChain(std::string password, int index);

	std::pair<std::string, std::string> getFirstAndLast(std::string password, int chainLength);
	std::pair<std::string, std::string> getFirstAndLast2(std::string password, int chainLength);
	vector<std::string> getChain(Md5Hash hash, int chainLength);
	vector<std::string> getCrackedPassword(Md5Hash hash, std::string first, int chainLength);
	
	
	void hash(std::string passwordFileName, std::string outFileName);

	// preceding operation before hacking to build up hash data
	// password -> first_last
	void getFirstAndLast(std::string passwordFileName, std::string outFileName, int chainLength);

	// hacking operations after getting hold of hash values
	// hash -> chain
	void getChain(std::string hashFileName, std::string outFileName, int chainLength);
	// using above chain and db, we should find first (done with scala now)
	// first -> cracked_password
	void getCrackedPassword(std::string firstFileName, std::string outFileName, int chainLength);
}
#endif