#include "neural_network.h"
#include "vector.h"

using namespace lib_calvin;

neural_network::neural_network(size_t numInput, size_t numOutput, vector<size_t> hiddenLayers) {
	layers_ = vector<vector<neuron>>(hiddenLayers.size() + 1);
	for (size_t i = 0; i < layers_.size(); i++) {
		size_t numInputs = 0;
		if (i == 0) {
			numInputs = numInput;
		} else {
			numInputs = hiddenLayers[i - 1];
		}

		if (i < hiddenLayers) {
			layers_[i] = vector<neuron>(hiddenLayers[i]);
		} else {
			layers_[i] = vector<neuron>(numOutput);
		}
		vector<neuron> &currentLayer = layers_[i];
		for (size_t j = 0; j < currentLayer.size(); j++) {
			currentLayer[i].weights_ = vector<float>(numInputs, 0.0);
			currentLayer[i].bias_ = 0.0;
			currentLayer[i].output_= 0.0;
		}
	}
}

void 
neural_network::train(vector<std::pair<vector<float>, vector<float>>> trainData) {
	float epsilon = 1.0;
	float markedCost = calculateAvgCost(trainData);
	vector<float *> points; // points to all weights and biases
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
	vector<float> gradient(points.size());
	while (true) {
		float currentCost = calculateAvgCost(trainData);
		if (currentCost < markedCost - 10) {
			std::cout << "Training, current cost is: " << currentCost << "\n";
		}
		for (size_t i = 0; i < points.size(); i++) {
			*points[i] += epsilon;
			gradient[i] = calculateAvgCost(trainData) - currentCost;
			*points[i] -= epsilon;
		}

		// normalize
		float magnitude = 0;
		for (size_t i = 0; i < gradient.size(); i++) {
			magnitude += gradient[i] * gradient[i];
		}
		magnitude = std::pow(magnitude, 0.5);
		for (size_t i = 0; i < gradient.size(); i++) {
			gradient[i] /= magnitude;
		}

		// move along the steepest downward slope
		for (size_t i = 0; i < points.size(); i++) {
			*points[i] -= gradient[i]; // minus, because we should go down
		}
	}
}

float 
neural_network::calculateAvgCost(vector<std::pair<vector<float>, vector<float>>> trainData) const {
	// get avg of square sum of deviation
	float totalCost = 0;
	for (size_t i = 0; i < trainData.size(); i++) {
		totalCost += calculateCost(trainData[i].first, trainData[i].second);
	}
	return totalCost / trainData.size();
}

float 
neural_network::calculateCost(vector<float> input, vector<float> output) const {
	if (input.size() != layers_[0].size()) {
		std::cout << "calculateCost: input size error\n";
		exit(0);
	}
	if (output.size() != layers_[layers_.size() - 1].size()) {
		std::cout << "calculateCost: output size error\n";
		exit(0);
	}

	// recalculate all layers 
	for (size_t i = 0; i < layers_.size(); i++) {		
		for (size_t j = 0; j < layers_[i].size(); j++) {
			neuron const &neuron = layers_[i][j];
			neuron.output_ = 0;
			for (size_t k = 0; k < neuron.weights_.size(); k++) {
				float currentInput = 0;
				if (i == 0) {
					currentInput = input[k];
				} else {
					currentInput = layers_[i - 1][k].output_;
				}
				neuron.output_ += neuron.weights_[k] * currentInput;				 
			}
			neuron.output_ += neuron.bias_;
		}
	}

	// compare output neurons with given output
	float sumOfSquare = 0;
	for (size_t k = 0; k < output.size(); k++) {
		float deviation = output[k] - layers_[layers_.size() - 1][k].output_;
		sumOfSquare += deviation * deviation;
	}
	return sumOfSquare;
}



float
neural_network::getOutputOfNeuron(vector<float> input, neuron const &neuron) const {
	if (input.size() != neuron.weights_.size()) {
		std::cout << "getOutputOfNeuron: input size error\n";
		exit(0);
	}
	float output = 0;
	for (size_t i = 0; i < input.size(); i++) {
		output += input[i] * neuron.weights_[i];
	}
	output += neuron.bias_;
	return output;
}
