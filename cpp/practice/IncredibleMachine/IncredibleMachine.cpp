#include "stdafx.h"
#include "IncredibleMachine.h"

#include <iostream>

void IncredibleMachineTest() {
	vector<int> x, y;
	x.push_back(0); 
	x.push_back(26);
	x.push_back(100);
	y.push_back(50);
	y.push_back(26);
	y.push_back(24);

	IncredibleMachine machine;
	cout << " solution: " << machine.gravitationalAcceleration(x, y, 4) << std::endl;
	cout << " solution2: " << machine.gravitationalAcceleration2(x, y, 4) << std::endl;

	x.clear();
	y.clear();
	x.push_back(0); 
	x.push_back(100);
	y.push_back(1);
	y.push_back(0);
	cout << " solution: " << machine.gravitationalAcceleration(x, y, 7) << std::endl;
	cout << " solution2: " << machine.gravitationalAcceleration2(x, y, 7) << std::endl;
}

double solveQuadraticEquation(double a, double b, double c) {
	double solution = (-b + pow(b*b - 4*a*c, 0.5)) / (2*a);
	return solution;
}

double IncredibleMachine::gravitationalAcceleration2(
		vector<int> xCoordinates, vector<int> yCoordinates, int targetFallingTime) {

	double fallingTime = getTotalFallingTime(xCoordinates, yCoordinates, 1.0);
	double ratio = fallingTime / targetFallingTime;
	return ratio*ratio;
}

double IncredibleMachine::gravitationalAcceleration(
		vector<int> xCoordinates, vector<int> yCoordinates, int targetFallingTime) {

	double minimum = 0;
	double maximum = 1000000;
	double middle;
	while (true) {
		middle = (minimum + maximum) / 2;
		double currentFallingTime = 
			getTotalFallingTime(xCoordinates, yCoordinates, middle);
		double deviation = currentFallingTime - targetFallingTime;
		double maxDeviation = pow(10.0, -10.0);
		if (deviation < maxDeviation && deviation > -maxDeviation) {
			return middle;
		} else {
			if (deviation < 0) {
				maximum = middle;
			} else {
				minimum = middle;
			}
		}
	}
}

double IncredibleMachine::getTotalFallingTime(
		vector<int> xCoordinates, vector<int> yCoordinates, double gravitationalAccel) {
	
	double velocity = 0;
	double totalTime = 0;
	for (unsigned i = 0; i < xCoordinates.size() - 1; i++) {
		double finalVelocity = 0, fallingTime = 0;
		getSingleFallingTime(xCoordinates[i + 1] - xCoordinates[i], 
			yCoordinates[i + 1] - yCoordinates[i], finalVelocity, gravitationalAccel, 
			finalVelocity, fallingTime);
		velocity = finalVelocity;
		totalTime += fallingTime;			
	}	
	return totalTime;
}

void IncredibleMachine::getSingleFallingTime(
		int x, int y, double initialVelocity, double gravitationalAccel,
		double &finalVelocity, double &fallingTime) {
																							 
	double diagonalLength = pow(x*x + y*y, 0.5);
	double accelerationRatio = static_cast<double>(-y) / diagonalLength;
	double acceleration = accelerationRatio * gravitationalAccel; 
	fallingTime = 
		solveQuadraticEquation(acceleration / 2, initialVelocity, -diagonalLength);
	finalVelocity = initialVelocity + fallingTime*acceleration;

}




