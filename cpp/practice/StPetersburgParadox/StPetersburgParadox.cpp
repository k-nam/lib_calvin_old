#include "StPetersburgParadox.h"
#include <random>
#include <iostream>
#include <ctime>
#include <cstdlib>

int64_t doGambleAndGetResult(std::mt19937_64 &engine) {
	int64_t earnedMoney = 1;
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

int64_t getAverageResultOfGames(int numberOfTries) {
	std::random_device randomSeed;
	std::mt19937_64 engine(randomSeed());
	int64_t totalEarnedMoney = 0;
	int64_t jackpot = 0;
	for (int i = 0; i < numberOfTries; i++) {
		int64_t earnedMoney = doGambleAndGetResult(engine);
		totalEarnedMoney += earnedMoney;
		if (earnedMoney > jackpot) {
			jackpot = earnedMoney;
		}
	}
	std::cout << "jackpot was: " << static_cast<double>(jackpot) << "\n";
	return totalEarnedMoney / numberOfTries;
}