#include "StPetersburgParadox.h"
#include <random>
#include <iostream>
#include <ctime>
#include <cstdlib>

__int64 doGambleAndGetResult(std::mt19937_64 &engine) {
	__int64 earnedMoney = 1;
	while (true) {
		if (engine() % 2 == 1) {
			earnedMoney *= 2;
		}	else {
			break;
		}
	}
	//std::cout << "result for this game: " << earnedMoney << "\n";
	return earnedMoney;
}

__int64 getAverageResultOfGames(int numberOfTries) {
	std::random_device randomSeed;
	std::mt19937_64 engine(randomSeed());
	__int64 totalEarnedMoney = 0;
	__int64 jackpot = 0;
	for (int i = 0; i < numberOfTries; i++) {
		__int64 earnedMoney = doGambleAndGetResult(engine);
		totalEarnedMoney += earnedMoney;
		if (earnedMoney > jackpot) {
			jackpot = earnedMoney;
		}
	}
	std::cout << "jackpot was: " << static_cast<double>(jackpot) << "\n";
	return totalEarnedMoney / numberOfTries;
}