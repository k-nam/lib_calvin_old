#include "rainbow_table.h"
#include "md5.h"
#include <random>

void hacking__rainbow_table::md5_reverse(const unsigned char hash[16], int indexInChain,
																				 char *password, int &generatedPasswordLength) {
	generatedPasswordLength = MIN_PASSWORD_LENGTH + 
														indexInChain % (MAX_PASSWORD_LENGTH - MIN_PASSWORD_LENGTH + 1);
	for (int i = 0; i < generatedPasswordLength; i++) {
		password[i] = ' ' + hash[i % 16] % ('~' - ' ' + 1);
	}
}

std::pair<std::string, std::string> 
hacking__rainbow_table::md5GenerateChain(std::string password, int chainLength) {
	char passwordBuffer[MAX_PASSWORD_LENGTH + 1];
	int passwordLength = password.length();
	unsigned char hashBuffer[16];

	// initial load of password buffer
	for (size_t i = 0; i < password.length(); i++) {
		passwordBuffer[i] = password[i];
	}

	for (int i = 0; i < chainLength; i++) {
		md5(reinterpret_cast<unsigned char *>(passwordBuffer), passwordLength, hashBuffer);
		md5_reverse(hashBuffer, i, passwordBuffer, passwordLength);
	}

	passwordBuffer[passwordLength] = '0';
	return std::make_pair(password, std::string(passwordBuffer));
}