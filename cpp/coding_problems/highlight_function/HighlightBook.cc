#include <set>
#include <iostream>
#include "HighlightBook.h"
using namespace std;

bool
HighlightBook::processInput(pair<int, int> input) {
	//std::cout << input.first << ", " << input.second << "\n";
	if (input.first >= input.second) {
		return false;
	}
	auto iter = blocks_.upper_bound(input.first); // first element greater than input
	decltype(blocks_)::iterator blockToExtend;
	bool toExtend = false;

	decltype(blocks_)::iterator leftBlock;

	if (iter == blocks_.begin()) { // empty or iter is strictly leftmost	
		toExtend = true;
		blockToExtend = blocks_.insert(input).first;
	} else {
		auto leftBlock = --iter;
		int leftBlockSecond = leftBlock->second;
		if (leftBlock->second >= input.second) { // included
			int leftBlockLeft = leftBlock->first;
			int leftBlockRight = leftBlock->second;
			if (leftBlockLeft == input.first) {
				blocks_.erase(leftBlock->first);
			} else {
				leftBlock->second = input.first;
			}
			if (leftBlockSecond != input.second) {
				blocks_.insert(make_pair(input.second, leftBlockSecond));
			}
		} else if (leftBlock->second > input.first) { // intersect			
			toExtend = true;
			leftBlock->second = input.second;
			blockToExtend = leftBlock;
		} else if (leftBlock->second == input.first) { // consecutive
			toExtend = true;
			leftBlock->second = input.second;
			blockToExtend = leftBlock;
		} else {
			toExtend = true;
			blockToExtend = blocks_.insert(input).first;
		}
	}

	if (toExtend) {
		auto iter = blockToExtend;
		++iter;
		while (iter != blocks_.end()) {
			if (iter->first <= blockToExtend->second) {
				if (iter->second <= blockToExtend->second) { // included
					iter = blocks_.erase(iter);
					//std::cout << "check 1\n";
				} else {
					blockToExtend->second = iter->second; // to extend
					blocks_.erase(iter);
					//std::cout << "check 2\n";
					break;
				}	
			} else {
				break;
				//std::cout << "check 3\n";
			}
		}
	}
#ifdef TEST
	// check
	for (auto iter = blocks_.begin(); iter != blocks_.end(); iter++) {
		auto temp = iter;
		temp++;
		if (temp != blocks_.end()) {
			if (iter->second >= temp->first) {
				std::cout << "check error\n";
				exit(0);
			}
		}
	}
#endif
	return toExtend;
}

int 
HighlightBook::processData(std::vector<pair<int, int>> const &data) {
	int sum = 0;
	int sum2 = 0;

	for (auto iter = data.begin(); iter != data.end(); ++iter) {
		processInput(*iter);
#ifdef TEST
		processWithArray(*iter);
#endif
	}
	for (auto iter = blocks_.begin(); iter != blocks_.end(); ++iter) {
		//std::cout << "processData: " << iter->first << ", " << iter->second << "\n";
		sum += iter->second - iter->first;
	}
#ifdef TEST
	for (auto point : marked_points_) {
		if (point) {
			sum2++;
		}
	}	

	if (sum != sum2) {
		cout << "processData Error!\n";
		exit(0);
	}
#endif
	return sum;
}

#ifdef TEST
int
HighlightBook::processWithArray(pair<int, int> input) {
	//cout << "processWithArray\n";
	bool toExtend = false;
	for (int i = input.first; i < input.second; i++) {
		if (marked_points_[i] == false) {
			toExtend = true;
			break;
		}
	}
	if (toExtend) {
		for (int i = input.first; i < input.second; i++) {
			marked_points_[i] = true;
		}
	} else {
		for (int i = input.first; i < input.second; i++) {
			marked_points_[i] = false;
		}
	}
	return 0;
}
#endif