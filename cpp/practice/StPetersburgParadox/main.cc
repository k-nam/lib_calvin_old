#include <iostream>
#include <cmath>
#include "StPetersburgParadox.h"

int main() {
	std::cout << "avg return of 10^2 runs = " << getAverageResultOfGames(static_cast<int64_t>(pow(10, 2))) << "\n";
	std::cout << "avg return of 10^3 runs = " << getAverageResultOfGames(static_cast<int64_t>(pow(10, 3))) << "\n";
	std::cout << "avg return of 10^4 runs = " << getAverageResultOfGames(static_cast<int64_t>(pow(10, 4))) << "\n";
	std::cout << "avg return of 10^8 runs = " << getAverageResultOfGames(static_cast<int>(pow(10, 8))) << "\n";

}