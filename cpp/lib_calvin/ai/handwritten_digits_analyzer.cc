#include "handwritten_digits_analyzer.h"
#include <iostream>
#include <fstream>

using std::ifstream;

handwritten_digits_analyzer::
handwritten_digits_analyzer():
	neural_network(28 * 28, 10, vector<size_t>{ 30 }) {
}

void
handwritten_digits_analyzer::trainWithFile(std::string imageFileName, std::string labelFileName) {
	vector<std::pair<vector<double>, vector<double>>> trainData;

	ifstream imageFile(imageFileName);
	ifstream labelFile(labelFileName);
	std::cout << "Checking image data file...\n";
	std::cout << "magic num: " << readOneUnsigned(imageFile) << "\n";

	size_t numItems = readOneUnsigned(imageFile);
	size_t numRows = readOneUnsigned(imageFile);
	size_t numColumns = readOneUnsigned(imageFile);

	std::cout << "# of items: " << (int)numItems  << "\n";
	std::cout << "# of rows: " << numRows << "\n";
	std::cout << "# of columns: " << numColumns << "\n\n";
	
	std::cout << "Checking label data file...\n";
	std::cout << "magic num: " << readOneUnsigned(labelFile) << "\n";
	std::cout << "# of items: " << readOneUnsigned(labelFile) << "\n";

	std::cout << "Starting to load data.\n";
	for (size_t item = 0; item < numItems; item++) {
		vector<double> input;
		vector<double> output(10);
		unsigned char label = readByte(labelFile);
		for (unsigned char i = 0; i < 10; i++) {
			if (i == label) {
				output[i] = 1.0;
			} else {
				output[i] = 0.0;
			}
		}
		for (size_t column = 0; column < numColumns; column++) {
			for (size_t row = 0; row < numRows; row++) {
				input.push_back(static_cast<double>(readByte(imageFile)) / UCHAR_MAX);
			}
		}
		trainData.push_back(std::make_pair(input, output));
	}
	std::cout << "Finished loading data.\n\n";

	std::cout << "Start training.\n";
	train(trainData);
}


unsigned char 
handwritten_digits_analyzer::readByte(std::ifstream &file) const {
	char buffer[1];
	file.read(buffer, 1);
	return static_cast<unsigned char>(buffer[0]);
}

int32_t
handwritten_digits_analyzer::readInt(std::ifstream &file) const {
	/*
	int value;
	file >> value;
	return value;*/
	char buffer[4];
	file.read(buffer, 4);
	return (buffer[0] ) | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24 );
}

uint32_t
handwritten_digits_analyzer::readOneUnsigned(ifstream &file) const {
	char buffer[4];
	file.read(buffer, 4);
	return convertFromBigEndian(buffer);
}

uint32_t 
handwritten_digits_analyzer::convertFromBigEndian(char buffer[4]) const {
	return static_cast<unsigned char>(buffer[0]) << 24 | 
		static_cast<unsigned char>(buffer[1]) << 16 | 
		static_cast<unsigned char>(buffer[2]) << 8 | 
		static_cast<unsigned char>(buffer[3]);
}

void
handwritten_digits_analyzer::showBytes(ifstream &file) const {
	for (int i = 0; i < 16; i++) {
		std::cout << static_cast<int>(readByte(file)) << "\n";
	}
}