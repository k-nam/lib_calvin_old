#include "StPetersburgParadox.h"
#include <random>
#include <iostream>
#include <ctime>

__int64 doGambleAndGetResult() {
	__int64 earnedMoney = 1;
	
	while (true) {
		if (std::rand() % 2 == 0) { 
			earnedMoney *= 2;
		}	else {
			break;
		}
	}
	//std::cout << "result for this game: " << earnedMoney << "\n";
	return earnedMoney;
}

__int64 getAverageResultOfGames(int numberOfTries) {
	std::srand(time(NULL));
	__int64 totalEarnedMoney = 0;
	__int64 jackpot = 0;
	for (int i = 0; i < numberOfTries; i++) {
		__int64 earnedMoney = doGambleAndGetResult();
		totalEarnedMoney += earnedMoney;
		if (earnedMoney > jackpot) {
			jackpot = earnedMoney;
		}
	}
	std::cout << "jackpot was: " << jackpot << "\n";
	return totalEarnedMoney / numberOfTries;
}