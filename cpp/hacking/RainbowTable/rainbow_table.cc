#include "rainbow_table.h"

void hacking__rainbow_table::reverse(const unsigned char hash[16], unsigned char password[20]) {
	for (int i = 0; i < 20; i++) {
		password[i] = hash[i % 16];
	}
}