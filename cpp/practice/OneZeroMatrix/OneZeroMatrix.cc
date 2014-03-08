#include "OneZeroMatrix.h"
#include <iostream>

int one_zero_matrix::calculate(int dimension, int numberOfOnesPerLine) {
	// Variables for each matrix elements
	int *elements = new int[dimension*dimension];
	// Get the number of total cases
	int numTotalCases = 1;
	for (int i = 0; i < dimension*dimension; ++i) {
		numTotalCases *= 2;
	}
	// A number to represent a matrix
	// Each digits represents each element in the matrix
	int matrix = 0;
	int rightAnswer = 0;
	for (; matrix < numTotalCases; ++matrix) {
		//std::cout << "matrix is: " << matrix << "\n";
		for (int j = 0; j < dimension*dimension; ++j) { // fill elements
			elements[j] = (matrix >> j) & 1;
			//std::cout << "setting " << j << "'th element to " << elements[j] << "\n";
		}	
		bool isCorrect = true;
		for (int i = 0; i < dimension; ++i) { // chech i'th row
			int sum = 0;
			for (int j = dimension*i; j < dimension*(i + 1); ++j) {
				sum += elements[j];
			}
			if (sum == numberOfOnesPerLine) {
				} else {
					isCorrect = false;
					break;
				}
		}
		for (int i = 0; i < dimension; ++i) { // chech i'th column
			int sum = 0;
			for (int j = i; j < dimension*dimension; j += dimension) {
				sum += elements[j];
			}
			if (sum == numberOfOnesPerLine) {
				} else {
					isCorrect = false;
					break;
				}
		}
		if (isCorrect == true) {
			rightAnswer++;
		}
	}

	delete elements;
	return rightAnswer;	
}


