#pragma once
#include <vector>
#include <map>
#include <bitset>

//#define TEST

class HighlightBook {	
#ifdef TEST
public:
	HighlightBook(int size) : marked_points_(size) { }
#else
public:
	HighlightBook(int size) { }
#endif
	int processData(std::vector<std::pair<int, int>> const &);
	// return true if highlighted more, false if erased some
private:
	bool processInput(std::pair<int, int>);
	std::map<int, int> blocks_;

#ifdef TEST
private:
	int processWithArray(std::pair<int, int>);
	std::vector<bool> marked_points_;
#endif
};
