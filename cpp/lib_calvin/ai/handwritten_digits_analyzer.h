#ifndef LIB_CALVIN__NEURAL_NETWORK__HANDWRITTEN_DIGITS_ANALYZER_H
#define LIB_CALVIN__NEURAL_NETWORK__HANDWRITTEN_DIGITS_ANALYZER_H

#include "neural_network.h"
#include <fstream>

namespace lib_calvin_ai
{

class handwritten_digits_analyzer : public neural_network
{
public:
	handwritten_digits_analyzer();
	// labe means actual number (0-9)
	void trainWithBinaryFile(std::string imageDataFileName, std::string labelDataFileName);
	void trainWithTextFile(std::string trainFileName, std::string testFileName);
private:
	vector<std::pair<vector<double>, vector<double>>> readFile(std::string fileName, size_t count) const;
	unsigned char readByte(std::ifstream &) const;
	int32_t readInt(std::ifstream &) const;
	// some data is stored in big endian. Convert this to little endian and return the value.
	uint32_t readOneUnsigned(std::ifstream &) const;
	uint32_t convertFromBigEndian(char[4]) const;

	void showBytes(std::ifstream &) const;
};

}
#endif