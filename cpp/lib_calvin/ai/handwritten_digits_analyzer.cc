#include "handwritten_digits_analyzer.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace lib_calvin_ai
{
using std::ifstream;

handwritten_digits_analyzer::handwritten_digits_analyzer() :
	neuralNetwork_(28 * 28, 10, vector<size_t>{ 50 }) {
}

void
handwritten_digits_analyzer::trainWithBinaryFile(std::string imageFileName, std::string labelFileName) {
	vector<std::pair<vector<double>, vector<double>>> trainData;
	ifstream imageFile(imageFileName);  
	ifstream labelFile(labelFileName);

	std::cout << "Checking image data file...\n";
	std::cout << "magic num: " << readOneUnsigned(imageFile) << "\n";

	size_t numItems = readOneUnsigned(imageFile);
	size_t numRows = readOneUnsigned(imageFile);
	size_t numColumns = readOneUnsigned(imageFile);

	std::cout << "# of items: " << (int)numItems << "\n";
	std::cout << "# of rows: " << numRows << "\n";
	std::cout << "# of columns: " << numColumns << "\n\n";

	std::cout << "Checking label data file...\n";
	std::cout << "magic num: " << readOneUnsigned(labelFile) << "\n";
	std::cout << "# of items: " << readOneUnsigned(labelFile) << "\n";

	std::cout << "Starting to load data.\n";
	for (size_t item = 0; item < numItems; item++) {
		vector<double> input;
		vector<double> output(10, 0.0);
		unsigned char label = readByte(labelFile);
		output[label] = 1.0;

		std::cout << "label: " << (int)label << "\n";
		for (size_t column = 0; column < numColumns; column++) {
			for (size_t row = 0; row < numRows; row++) {
				double data = static_cast<double>(readByte(imageFile));
				input.push_back(data);
				if (column == 10 && row == 10) {
					std::cout << data << " ";
				}
			}
		}
		trainData.push_back(std::make_pair(input, output));
	}
	std::cout << "Finished loading data.\n\n";
	std::cout << "Start training.\n";
	//train(trainData);
	std::cout << "End training.\n";
}

void 
handwritten_digits_analyzer::trainWithTextFile(std::string trainFileName, std::string testFileName) {
	std::cout << "Start training.\n";
	size_t trainDataSize = 100000;
	size_t testDataSize = 10000;
	//size_t part1Size = 30000;
	//size_t validationSize = 10000;

	auto trainData = readFile(trainFileName, trainDataSize);
	auto testData = readFile(testFileName, testDataSize);
	//vector<std::pair<vector<double>, vector<double>>> part1(trainData.begin(), trainData.begin() + part1Size);
	//vector<std::pair<vector<double>, vector<double>>> part2(trainData.begin() + part1Size, 
		//trainData.begin() + part1Size + validationSize);
	//train(part1, part2);
	neuralNetwork_.train(trainData, testData);
	std::cout << "End training.\n";
}

vector<std::pair<vector<double>, vector<double>>> 
handwritten_digits_analyzer::readFile(std::string fileName, size_t count) const {
	vector<std::pair<vector<double>, vector<double>>> data;
	ifstream dataFile(fileName);
	std::string line;
	std::string token;
	char delim = ',';
	std::cout << "Starting to load data: " << fileName << "\n";
	for (size_t i = 0; getline(dataFile, line) && i < count; i++) {
		vector<double> input;
		vector<double> output(10, 0.0);
		vector<int> values;
		std::stringstream stringStream(line);
		while (getline(stringStream, token, delim)) {
			values.push_back(std::stoi(token));
		}

		for (size_t i = 0; i < values.size(); i++) {
			int value = values[i];
			//std::cout << value << ",";
			if (i == 0) { // first tokoen is output
				output[static_cast<size_t>(value)] = 1.0;
			} else {
				input.push_back(static_cast<double>(value) / UCHAR_MAX);
			}
		}
		data.push_back(std::make_pair(input, output));
	}
	std::cout << "Finished loading data.\n\n";
	return data;
}

unsigned char
handwritten_digits_analyzer::readByte(std::ifstream &file) const {
	char buffer[2];
	file.read(buffer, 2);
	//std::cout << "reading " << static_cast<int>(buffer[0]) << " ";
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
	return (buffer[0]) | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24);
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
	for (int i = 0; i < 100000; i++) {
		std::cout << static_cast<int>(readByte(file)) << "\n";
	}
}

}