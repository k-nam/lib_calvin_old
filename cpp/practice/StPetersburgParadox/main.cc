#include <iostream>
#include "StPetersburgParadox.h"

int main() {
	std::cout << "avg return of 100 runs = " << getAverageResultOfGames(100) << "\n";
	std::cout << "avg return of 1000 runs = " << getAverageResultOfGames(1000) << "\n";
	std::cout << "avg return of 10000 runs = " << getAverageResultOfGames(10000) << "\n";
	std::cout << "avg return of 10^8 runs = " << getAverageResultOfGames(10*10*10*10*10*10*10*10) << "\n";
}