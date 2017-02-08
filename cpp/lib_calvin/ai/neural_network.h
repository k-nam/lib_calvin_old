#ifndef LIB_CALVIN__NEURAL_NETWORK__NEURAL_NETWORK_H
#define LIB_CALVIN__NEURAL_NETWORK__NEURAL_NETWORK_H

#include <vector>
#include <string>

using std::vector;

class neural_network 
{
public:
	neural_network(size_t numInput, size_t numOutput, vector<size_t> hiddenLayers);
	void train(vector<std::pair<vector<double>, vector<double>>> trainData);

private:
	struct neuron {
		// output value = G (inner product of weights and input values + bias)
		// G(x) = 1 / (1 + e^-x)
		vector<double> weights_;
		double bias_; // bias >= 0
		double mutable output_;
	};

private:
	double calculateAvgCost(vector<std::pair<vector<double>, vector<double>>> trainData) const;
	double calculateCost(vector<double> input, vector<double> output) const; // sum of square 
	// Using sigmoid
	double getOutputOfNeuron(vector<double> input, neuron const &) const;
private:
	vector<vector<neuron>> layers_; // last element is output
};

#endif

