#include "lib.h"

lib_calvin::vector<std::string>
hacking__lib::split(std::string const &line, char delim) {
	std::stringstream stringStream(line);
	std::string word;
	lib_calvin::vector<std::string> words;
	while (getline(stringStream, word, delim)) {
		words.push_back(word);
	}
	return words;
}

