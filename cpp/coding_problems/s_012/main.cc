// S012 Highlight function 

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>

using namespace std;

vector<vector<string>> getTestData() {
	vector<vector<string>> data(4, vector<string>(2));

	data[0][0] = R"(10 3
	2 6
	6 8
	3 4)";
	data[0][1] = "5";

	data[1][0] = R"(10 10
	1 6
	2 5
	1 7
	1 7
	2 7
	2 8
	1 4
	2 2
	1 10
	2 10)";
	data[1][1] = "1";

	data[2][0] = R"(100 5
	1 100
	1 100
	1 100
	1 100
	1 100)";
	data[2][1] = "100";

	data[3][0] =  R"(100 20
	14 14
	53 100
	60 64
	78 82
	43 45
	6 7
	44 45
	83 98
	55 58
	34 34
	21 37
	17 73
	90 98
	55 62
	69 76
	25 73
	11 89
	92 94
	49 53
	17 17)";
	data[3][1] = "83";
	
	return data;
}

//#define TEST

class HighlightBook {
#ifdef TEST
public:
	HighlightBook(int size) : marked_points_(size) { }
#else
public:
	HighlightBook(int size) { }
#endif
	int processData(vector<pair<int, int>> const &);
	// return true if highlighted more, false if erased some
private:
	bool processInput(pair<int, int>);
	map<int, int> blocks_;

#ifdef TEST
private:
	int processWithArray(std::pair<int, int>);
	std::vector<bool> marked_points_;
#endif
};

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
	int sum2 = 0;
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

void printContainer(vector<pair<int, int>> const &container);
vector<pair<int, int>> getDataFromString(string input);
int testWithRandomData();

void printContainer(vector<pair<int, int>> const &container) {
	//cout << "b" << container.size();
	for (int i = 0; i < container.size(); i++) {
		cout << container[i].first << ", " << container[i].second << "\n";
	}
}

vector<pair<int, int>>
getDataFromString(string text) {
	istringstream iss(text);
	string input;
	getline(iss, input, ' ');
	int totalLength = stoi(input);
	getline(iss, input);
	int numOps = stoi(input);
	vector<pair<int, int>> data(numOps);

	for (int i = 0; i < numOps; i++) {
		int beginIndex;
		int endIndex;
		getline(iss, input, ' ');
		beginIndex = stoi(input);
		getline(iss, input);
		endIndex = stoi(input) + 1;
		data[i] = make_pair(beginIndex, endIndex);
	}

	return data;
}

int testWithRandomData() {
	int testSize = 1000000000;
	int testIter = 1000;
	HighlightBook book(testSize + 100);
	vector<pair<int, int>> data;
	srand(static_cast<unsigned>(time(nullptr)));
	for (int i = 0; i < testIter; i++) {
		int beginIndex;
		int length = 0;
		int pattern = 2;

		if (pattern == 2) {
			if (rand() % 1000 == 0) {
				beginIndex = (rand() * 30000 + rand()) % testSize;
				length = rand() % (testSize - beginIndex) + 1;
			} else {
				beginIndex = (i * 5) % testSize;
				length = rand() % 4;
			}
		} else if (pattern == 1) {
			beginIndex = 1;
			length = testSize - 1;
		} else if (pattern == 0) {
			beginIndex = (i * 5) % testSize;
			length = 50;
		} else if (pattern == 3) {
			beginIndex = (i * 2) % testSize;
			length = 1;
		}
		
		data.push_back(make_pair(beginIndex, beginIndex + length));
	}
	return book.processData(data);
}

int main() {
	bool forPaizaSubmission = false;

	if (forPaizaSubmission) {
		string input;
		getline(cin, input, ' ');
		int totalLength = stoi(input);
		getline(cin, input);
		int numOps = stoi(input);
		vector<pair<int, int>> data(numOps);

		for (int i = 0; i < numOps; i++) {
			int beginIndex;
			int endIndex;
			getline(cin, input, ' ');
			beginIndex = stoi(input);
			getline(cin, input);
			endIndex = stoi(input) + 1;
			data[i] = make_pair(beginIndex, endIndex);
		}
		HighlightBook book(1);
		cout << book.processData(data);
	} else {
		int maxTestDataLength = 1000;
		auto testData = getTestData();
		for (int i = 0; i < testData.size(); i++) {
			HighlightBook book(maxTestDataLength);
			if (book.processData(getDataFromString(testData[i][0])) == stoi(testData[i][1])) {
				cout << "OK\n";
			} else {
				cout << "Test case error! in : " << i << "\n";
			}
		}
		cout << "Random data test: " << testWithRandomData() << "\n";
	}
}