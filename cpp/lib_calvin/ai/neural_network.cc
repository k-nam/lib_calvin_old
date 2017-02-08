#include "neural_network.h"
#include <random>
#include <algorithm>
#include <iostream>

neural_network::neural_network(size_t numInput, size_t numOutput, vector<size_t> hiddenLayers) {
	layers_ = vector<vector<neuron>>(hiddenLayers.size() + 1);
	//std::cout << layers_.size() << "\n";
	for (size_t i = 0; i < layers_.size(); i++) {
		size_t numInputs = 0;
		if (i == 0) {
			numInputs = numInput;
		} else {
			numInputs = hiddenLayers[i - 1];
		}

		if (i < hiddenLayers.size()) {
			layers_[i] = vector<neuron>(hiddenLayers[i]);
		} else {
			layers_[i] = vector<neuron>(numOutput);
		}
		vector<neuron> &currentLayer = layers_[i];
		std::cout << currentLayer.size() <<"\n";
		std::random_device rd;
		std::mt19937 gen(rd());
		std::normal_distribution<> d(0, 1.0);

		for (size_t j = 0; j < currentLayer.size(); j++) {
			
			currentLayer[j].weights_ = vector<double>(numInputs, 0.0);
			for (size_t k = 0; k < numInputs; k++) {
				currentLayer[j].weights_[k] = d(gen);
				//std::cout << currentLayer[i].weights_[k] << "\n";
			}
			currentLayer[j].bias_ = d(gen);
			currentLayer[j].output_= 0.0;
			
		}
	}
}

void 
neural_network::train(vector<std::pair<vector<double>, vector<double>>> trainData) {
	double epsilon = 0.001;
	size_t subsetSize = 10;
	double markedCost = calculateAvgCost(trainData);
	vector<double *> points; // points to all weights and biases
	for (size_t i = 0; i < layers_.size(); i++) {
		for (size_t j = 0; j < layers_[i].size(); j++) {
			neuron &neuron = layers_[i][j];
			neuron.output_ = 0;
			for (size_t k = 0; k < neuron.weights_.size(); k++) {
				points.push_back(&neuron.weights_[k]);
			}
			points.push_back(&neuron.bias_);
		}
	}
	vector<double> gradient(points.size());
	std::random_device rd;
	std::mt19937 gen(rd());

	std::cout << "Training 1\n";
	while (true) {
		vector<std::pair<vector<double>, vector<double>>> subset;
		std::shuffle(trainData.begin(), trainData.end(), gen);
		for (size_t i = 0; i < subsetSize; i++) {
			subset.push_back(trainData[i]);
		}

		double currentAvgCost = calculateAvgCost(subset);
		if (currentAvgCost <= markedCost || true) {
			std::cout << "Training, current avg cost is: " << currentAvgCost << "\n";
		}
		for (size_t i = 0; i < points.size(); i++) {
			*points[i] += epsilon;
			gradient[i] = calculateAvgCost(subset) - currentAvgCost;
			*points[i] -= epsilon;
		}
		std::cout << "Training 2\n";
		// normalize
		double magnitude = 0;
		for (size_t i = 0; i < gradient.size(); i++) {
			magnitude += gradient[i] * gradient[i];
		}
		magnitude = static_cast<double>(std::pow(magnitude, 0.5));
		for (size_t i = 0; i < gradient.size(); i++) {
			gradient[i] /= magnitude;
		}
		std::cout << "Training 3\n";
		// move along the steepest downward slope
		for (size_t i = 0; i < points.size(); i++) {
			*points[i] -= gradient[i]; // minus, because we should go down
		}
	}
}

double 
neural_network::calculateAvgCost(vector<std::pair<vector<double>, vector<double>>> trainData) const {
	// get avg of square sum of deviation
	double totalCost = 0;
	for (size_t i = 0; i < trainData.size(); i++) {
		totalCost += calculateCost(trainData[i].first, trainData[i].second);
	}
	return totalCost / trainData.size();
}

double 
neural_network::calculateCost(vector<double> input, vector<double> output) const {
	if (output.size() != layers_[layers_.size() - 1].size()) {
		std::cout << "calculateCost: output size error\n";
		exit(0);
	}

	// recalculate all layers 
	for (size_t i = 0; i < layers_.size(); i++) {		
		for (size_t j = 0; j < layers_[i].size(); j++) {
			neuron const &neuron = layers_[i][j];
			neuron.output_ = 0;
			if (i == 0) {
				if (neuron.weights_.size() != input.size()) {
					std::cout << "calculateCost: input size error 1\n";
					exit(0);
				}
			} else {
				if (neuron.weights_.size() != layers_[i - 1].size()) {
					std::cout << "calculateCost: input size error 2\n";
					exit(0);
				}
			}
			for (size_t k = 0; k < neuron.weights_.size(); k++) {
				double currentInput = 0;
				if (i == 0) {
					currentInput = input[k];
				} else {
					currentInput = layers_[i - 1][k].output_;
				}
				neuron.output_ += neuron.weights_[k] * currentInput;				 
			}
			neuron.output_ += neuron.bias_;
			// apply sigmoid
			neuron.output_ = 1 / (1 + exp(-neuron.output_));
		}
	}

	// compare output neurons with given output
	double sumOfSquare = 0;
	for (size_t k = 0; k < output.size(); k++) {
		double deviation = output[k] - layers_[layers_.size() - 1][k].output_;
		sumOfSquare += deviation * deviation;
	}
	return sumOfSquare;
}



double
neural_network::getOutputOfNeuron(vector<double> input, neuron const &neuron) const {
	if (input.size() != neuron.weights_.size()) {
		std::cout << "getOutputOfNeuron: input size error\n";
		exit(0);
	}
	double output = 0;
	for (size_t i = 0; i < input.size(); i++) {
		output += input[i] * neuron.weights_[i];
	}
	output += neuron.bias_;
	return output;
}
