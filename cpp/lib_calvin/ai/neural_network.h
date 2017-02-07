#ifndef LIB_CALVIN__NEURAL_NETWORK__NEURAL_NETWORK_H
#define LIB_CALVIN__NEURAL_NETWORK__NEURAL_NETWORK_H

#include "vector.h"
using lib_calvin::vector;
class neural_network 
{
public:
	neural_network(size_t numInput, size_t numOutput, vector<size_t> hiddenLayers);
	void train(vector<std::pair<vector<float>, vector<float>>> trainData);

private:
	struct neuron {
		// output value = G (inner product of weights and input values + bias)
		// G(x) = 1 / (1 + e^-x)
		vector<float> weights_;
		float bias_; // bias >= 0
		float mutable output_;
	};

private:
	float calculateAvgCost(vector<std::pair<vector<float>, vector<float>>> trainData) const;
	float calculateCost(vector<float> input, vector<float> output) const; // sum of square 
	// Using sigmoid
	float getOutputOfNeuron(vector<float> input, neuron const &) const;
private:
	vector<vector<neuron>> layers_; // last element is output
};

#endif

