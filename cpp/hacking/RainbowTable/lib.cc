#include "lib.h"

std::vector<std::string>
hacking__lib::split(std::string const &line, char delim) {
	using namespace std;
	stringstream stringStream(line);
	string word;
	vector<string> words;
	while (getline(stringStream, word, delim)) {
		words.push_back(word);
	}
	return words;
}

