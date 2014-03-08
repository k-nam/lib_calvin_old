
#include <vector>
#include <math.h>
#include <iostream>

using std::vector;
using std::cout;

void IncredibleMachineTest();

// Return only the positive solution
double solveQuadraticEquation(double a, double b, double c);

class IncredibleMachine {
public: 
	double gravitationalAcceleration(
			vector<int> xCoordinates, vector<int> yCoordinates, int totalDuration);
	
	double gravitationalAcceleration2(
			vector<int> xCoordinates, vector<int> yCoordinates, int totalDuration);

	double getTotalFallingTime(
			vector<int> xCoordinates, vector<int> yCoordinates, double acceleration);

	void getSingleFallingTime(int x, int y, double initialVelocity, double acceleration,
			double &finalVelocity, double &fallingTime);

};
