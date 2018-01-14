#pragma once
#include <vector>
#include <map>
#include <bitset>

class HighlightBook {	
public:
	HighlightBook(int size): marked_points_(size) { }
	int processData(std::vector<std::pair<int, int>> const &);
	// return true if highlighted more, false if erased some
	bool processInput(std::pair<int, int>);
private:
	int processWithArray(std::pair<int, int>);
private:
	std::map<int, int> blocks_;
	std::vector<bool> marked_points_;
};
