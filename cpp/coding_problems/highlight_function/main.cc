#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include "HighlightBook.h"

string a = R"(10 3
		2 6
		6 8
		3 4)";
// 5

string b = R"(10 10
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
// 1

string c = R"(100 5
		1 100
		1 100
		1 100
		1 100
		1 100)";
// 100

string d = R"(100 20
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
// 83

using namespace std;

void printContainer(vector<pair<int, int>> const &container);
vector<pair<int, int>> getDataFromString(string input);
int testWithRandomData();

int totalLength;
int numOps;

int main() {
	string input;
	vector<pair<int, int>> data(numOps);

	/*
	// For Paiza submission
	getline(cin, input, ' ');
	totalLength = stoi(input);
	getline(cin, input);
	numOps = stoi(input);

	for (int i = 0; i < numOps; i++) {
		int beginIndex;
		int endIndex;
		getline(cin, input, ' ');
		beginIndex = stoi(input);
		getline(cin, input);
		endIndex = stoi(input);
		data[i] = make_pair(beginIndex, endIndex);
	}
	*/
	
	data = getDataFromString(d);
	HighlightBook book(1000);
	cout << book.processData(data) << "\n";

	cout << testWithRandomData() << "\n";
}


void printContainer(vector<pair<int, int>> const &container) {
	//cout << "b" << container.size();
	for (int i = 0; i < container.size(); i++) {
		cout << container[i].first << ", " << container[i].second << "\n";
	}
}

vector<pair<int, int>>
getDataFromString(string text) {
	using namespace std;
	istringstream iss(text);
	string input;
	getline(iss, input, ' ');
	totalLength = stoi(input);
	getline(iss, input);
	numOps = stoi(input);
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
	int testIter = 10000;
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