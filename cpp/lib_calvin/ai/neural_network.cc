#include "neural_network.h"
#include <random>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <fstream>
#include "json/json.hpp"

namespace lib_calvin_ai
{

neural_network::neural_network(size_t numInput, size_t numOutput, vector<size_t> hiddenLayers) {
	layers_ = vector<layer>(hiddenLayers.size() + 1);
	//std::cout << layers_.size() << "\n";
	for (size_t i = 0; i < layers_.size(); i++) {
		size_t thisLayerNumInput = 0;
		size_t thisLayerNumOutput = 0;
		bool isOutputLayer = false;
		if (i == 0) {
			thisLayerNumInput = numInput;
		} else {
			thisLayerNumInput = hiddenLayers[i - 1];
		}

		if (i < hiddenLayers.size()) {
			thisLayerNumOutput = hiddenLayers[i];
		} else {
			thisLayerNumOutput = numOutput;
		}

		if (i == hiddenLayers.size()) {
			isOutputLayer = true;
		}

		layers_[i] = layer(thisLayerNumInput, thisLayerNumOutput, isOutputLayer);
	}
}

void 
neural_network::train(vector<std::pair<vector<double>, vector<double>>> trainData,
					vector<std::pair<vector<double>, vector<double>>> testData) {
	size_t subsetSize = 10;	
	size_t num_epoch = 10000;
	std::random_device rd;
	std::mt19937 gen(rd());
	double maxAccuracy = 0.0; // in percent

	randomize();
	for (size_t i = 0; i < num_epoch; i++) {
		double epochTotalCost = 0.0;
		double subsetAvgCost = 0.0;
		std::shuffle(trainData.begin(), trainData.end(), gen);
		auto subsetBegin = trainData.begin();
		auto subsetEnd = trainData.end();
		while (true) {
			if (trainData.end() - subsetBegin > static_cast<ptrdiff_t>(subsetSize)) {
				subsetEnd = subsetBegin + subsetSize;
			} else if (trainData.end() - subsetBegin > 0) {
				subsetEnd = trainData.end();
			} else {
				break;
			}
			vector<std::pair<vector<double>, vector<double>>> subset(subsetBegin, subsetEnd);
			// Calling learnFromData() will also update all z and output values in the network.
			subsetAvgCost = learnFromData(subset);
			epochTotalCost += subsetAvgCost * subsetSize;

			subsetBegin = subsetEnd;
		}
		double currAccuracy = test(testData) * 100;
		std::cout << "Epoch: " << i << " Current sigma: " << pow(epochTotalCost / trainData.size(), 0.5) << 
			 ". Subset sigma: " << pow(subsetAvgCost, 0.5) << ". Ratio: " << currAccuracy << "\n";
		if (currAccuracy > maxAccuracy) { // save as JSON 
			saveJson();
		}
	}
}

double 
neural_network::test(vector<std::pair<vector<double>, vector<double>>> testData) const {	
	size_t numCorrectAnswer = 0;
	for (auto pair : testData) {
		auto input = pair.first;
		auto output = pair.second;
		auto networkOutput = runNetwork(input);
		bool isCorrect = true;
		double max = 0;
		size_t maxIndex = 0;
		size_t correctOutput = 0;

		for (size_t i = 0; i < networkOutput.size(); i++) {
			if (networkOutput[i] > max) {
				max = networkOutput[i];
				maxIndex = i;
			}
			if (output[i] == 1.0) {
				correctOutput = i;
			}
			if (abs(output[i] - networkOutput[i]) > 0.5) {
				//std::cout << output[i] << "," << networkOutput[i] << "\n";
				isCorrect = false;
			}
		}
		if (maxIndex == correctOutput) {
			numCorrectAnswer++;
		}
		if (isCorrect) {
			if (maxIndex != correctOutput) {
				std::cout << output[maxIndex] << "," << networkOutput[maxIndex] << "\n";
			}
			//numCorrectAnswer++;
		}
	}
	return static_cast<double>(numCorrectAnswer) / testData.size();
}

double 
neural_network::learnFromData(vector<std::pair<vector<double>, vector<double>>> const &trainData) {
	// get avg of square sum of deviation
	double totalCost = 0;
	double learningRate = 1;
	for (size_t i = 0; i < trainData.size(); i++) {
		totalCost += learnFromData(trainData[i].first, trainData[i].second);
	}
	double multiplier = learningRate / trainData.size();
	for (layer &currentLayer : layers_) {
		currentLayer.adjustVariablesWithMultiplier(multiplier);
		currentLayer.clearGradient();
	}
	//std::cout << "learnFromData with size: " << trainData.size() << "avg cost: "<< totalCost / trainData.size() <<"\n";
	return totalCost / trainData.size();
}

vector<double>
neural_network::runNetwork(vector<double> const &input) const {
	vector<double> currentOutputVector = input;
	for (layer const &currentLayer : layers_) {
		currentOutputVector = currentLayer.runNetwork(currentOutputVector);
	}
	return currentOutputVector;
}

double
neural_network::learnFromData(vector<double> const &input, vector<double> const &output) {
	// First run the network
	//std::cout << "input: " << input[0] << "\n";
	vector<double> networkOutput = runNetwork(input);
	if (networkOutput.size() != output.size()) {
		std::cout << "runNetwork error\n";
		exit(0);	
	}

	// Backpropagate to calculate and accumulate gradient
	for (int i = static_cast<int>(layers_.size()) - 1; i >= 0; i--) {
		vector<double> previousLayerOutput;
		vector<double> nextLayerMultiplier;
		if (i == 0) {
			previousLayerOutput = input;
		} else {
			previousLayerOutput = layers_[i - 1].getOutput();
		}
		if (i == layers_.size() - 1) {
			nextLayerMultiplier = output;
			//std::cout << "1\n";
		} else {
			nextLayerMultiplier = layers_[i + 1].getLayerMultiplier();
			//std::cout << "2\n";
		}
		layers_[i].addGradient(previousLayerOutput, nextLayerMultiplier);
		//std::cout << "z: " << layers_[i].z_[0] << " " << layers_[i].outputs_[0] << "\n";
	}

	double sum = 0;
	for (size_t i = 0; i < networkOutput.size(); i++) {
		double deviation = networkOutput[i] - output[i];
		sum += deviation * deviation;
	}
	return sum / output.size();
}

void
neural_network::randomize() {
	std::cout << "Randomizing network\n";
	for (layer &thisLayer : layers_) {
		thisLayer.randomize();
	}
}


void
neural_network::saveJson() const {
	// 
	using json = nlohmann::json;
	json layersJson = json::array();
	for (auto const &layer : layers_) {
		json thisLayerJson;
		json weightsJson(convertMatrixToVetor(layer.weights_));
		json biasesJson(layer.biases_);
		thisLayerJson["weights"] = weightsJson;
		thisLayerJson["biases"] = biasesJson;
		layersJson.push_back(thisLayerJson);
	}
	std::ofstream jsonFile("handwriting_network.js");
	jsonFile << "let neuralNetwork = " << layersJson.dump() << ";";
	jsonFile.close();
}

vector<vector<double>>
neural_network::convertMatrixToVetor(matrix<double> const &matrix) const {
	vector<vector<double>> result;
	for (size_t i = 0; i < matrix.height(); i++) {
		vector<double> row;
		for (size_t j = 0; j < matrix.width(); j++) {
			row.push_back(matrix[i][j]);
		}
		result.push_back(row);
	}
	return result;
}

neural_network::layer::layer() :
	weights_(1, 1), biases_(1),
	accumulated_gradient_weights_(1, 1), accumulated_gradient_biases_(1),
	z_(1), errors_(1),
	isOutputLayer_(false) { }

neural_network::layer::layer(size_t numInput, size_t numOutput, bool isOutputLayer) :
	weights_(numOutput, numInput), biases_(numOutput),
	accumulated_gradient_weights_(numOutput, numInput), accumulated_gradient_biases_(numOutput),
	z_(numOutput), outputs_(numOutput), errors_(numOutput),
	isOutputLayer_(isOutputLayer) { }

neural_network::layer &
neural_network::layer::operator=(layer const &rhs) {
	weights_.reset(rhs.weights_);
	biases_ = rhs.biases_;
	accumulated_gradient_weights_.reset(rhs.accumulated_gradient_weights_);
	accumulated_gradient_biases_ = rhs.accumulated_gradient_biases_;
	z_ = rhs.z_;
	outputs_ = rhs.outputs_;
	errors_ = rhs.errors_;
	isOutputLayer_ = rhs.isOutputLayer_;
	return *this;
}


size_t 
neural_network::layer::getNumInput() const {
	return weights_.width();
}

size_t 
neural_network::layer::getNumOutput() const {
	return weights_.height();
}

vector<double>
neural_network::layer::getOutput() const {
	return outputs_;
}

void
neural_network::layer::randomize() {
	std::random_device rd;
	std::mt19937 gen(rd());
	double d = 1 / pow(getNumInput(), 0.5);
	std::normal_distribution<> dist(0, d);
	std::normal_distribution<> dist2(0, 1.0);
	for (size_t i = 0; i < weights_.height(); i++) {
		for (size_t j = 0; j < weights_.width(); j++) {
			weights_(i, j) = dist(gen);
			//weights_(i, j) = 0;
		}
	}
	for (double &bias : biases_) {
		bias = dist2(gen);
		//bias = 0;
	}
}

void 
neural_network::layer::clearGradient() const {
	for (size_t i = 0; i < accumulated_gradient_weights_.height(); i++) {
		for (size_t j = 0; j < accumulated_gradient_weights_.width(); j++) {
			accumulated_gradient_weights_(i, j) = 0.0;
		}
	}
	for (size_t i = 0; i < accumulated_gradient_biases_.size(); i++) {
		accumulated_gradient_biases_[i] = 0.0;
	}
}

vector<double>
neural_network::layer::runNetwork(vector<double> const &previousLayerOutput) const {
	for (double &z : z_) {
		z = 0.0;
	}
	for (size_t i = 0; i < weights_.height(); i++) {
		for (size_t j = 0; j < weights_.width(); j++) {
			z_[i] += weights_(i, j) * previousLayerOutput[j];
			//std::cout << previousLayerOutput[j] << " " << weights_(i, j) << "\n";
		}
	}
	for (size_t i = 0; i < biases_.size(); i++) {
		z_[i] += biases_[i];
	}
	for (size_t i = 0; i < z_.size(); i++) {
		outputs_[i] = getSigmoid(z_[i]);
		//std::cout << "output: " << outputs_[i];
		//if (outputs_[i] != outputs_[i]) {
		//	std::cout << "runNetwork nan\n";
		//	exit(0);
		//}
	}
	return outputs_;
}

void 
neural_network::layer::addGradient(vector<double> const &previousLayerOutput, 
		vector<double> const &nextLayerMultiplier) const {
	if (isOutputLayer_) {
		calculateErrorsForOutputLayer(nextLayerMultiplier);
	} else {
		calculateErrorsForHiddenLayer(nextLayerMultiplier);
	}
	//std::cout << previousLayerOutput.size() << ", " << nextLayerMultiplier.size() << ", " <<
	//	accumulated_gradient_weights_.height() << ", " << accumulated_gradient_weights_.width() << "\n";

	for (size_t i = 0; i < accumulated_gradient_weights_.height(); i++) {
		for (size_t j = 0; j < accumulated_gradient_weights_.width(); j++) {
			accumulated_gradient_weights_(i, j) += previousLayerOutput[j] * errors_[i];
		}
	}
	for (size_t i = 0; i < accumulated_gradient_biases_.size(); i++) {
		accumulated_gradient_biases_[i] += errors_[i];
	}
}


void 
neural_network::layer::adjustVariablesWithMultiplier(double multipler) {
	for (size_t i = 0; i < weights_.height(); i++) {
		for (size_t j = 0; j < weights_.width(); j++) {
			weights_(i, j) -= accumulated_gradient_weights_(i, j) * multipler;
			//std::cout << accumulated_gradient_weights_(i, j) << ", ";
			//if (weights_(i, j) != weights_(i, j)) {
			//	std::cout << "adjustVariablesWithMultiplier nan\n";
			//	exit(0);
			//}
		}
	}
	for (size_t i = 0; i < biases_.size(); i++) {
		biases_[i] -= accumulated_gradient_biases_[i] * multipler;
		//std::cout << accumulated_gradient_biases_[i] << ", ";
	}
}

vector<double> 
neural_network::layer::getLayerMultiplier() const {
	matrix<double> error_vector(1, errors_.size());
	for (size_t i = 0; i < errors_.size(); i++) {
		error_vector(0, i) = errors_[i];
	}
	auto temp = error_vector * weights_;
	vector<double> result(getNumInput()); // weights_.height() == numInput
	for (size_t i = 0; i < result.size(); i++) {
		result[i] = temp(0, i);
		//if (result[i] != result[i]) {
		//	std::cout << "getLayerMultiplier nan\n";
		//	exit(0);
		//}
	}
	return result;
}

void 
neural_network::layer::calculateErrorsForHiddenLayer(vector<double> const &nextLayerMultiplier) const {
	if (getNumOutput() != nextLayerMultiplier.size()) {
		std::cout << "calculateErrorsForHiddenLayer dimension error\n";
		exit(0);
	}
	for (size_t i = 0; i < errors_.size(); i++) {
		errors_[i] = nextLayerMultiplier[i] * getDerivativeOfSigmoid(z_[i]);
		//std::cout << "error: " << errors_[i] << ", " << z_[i] << "\n";
		//if (errors_[i] != errors_[i]) {
		//	std::cout << "calculateErrorsForHiddenLayer nan\n" << nextLayerMultiplier[i] << ", " <<
		//		getDerivativeOfSigmoid(z_[i]);
		//	exit(0);
		//}
	}
}

void
neural_network::layer::calculateErrorsForOutputLayer(vector<double> const &outputData) const {
	if (getNumOutput() != outputData.size()) {
		std::cout << "calculateErrorsForOutputLayer dimension error\n";
		exit(0);
	}
	for (size_t i = 0; i < errors_.size(); i++) {
		errors_[i] = (outputs_[i] - outputData[i]) * getDerivativeOfSigmoid(z_[i]) / outputData.size();
		//std::cout << "error: " << errors_[i] << "\n";
		//if (errors_[i] != errors_[i]) {
		//	std::cout << "calculateErrorsForOutputLayer nan\n" << getSigmoid(z_[i]) - outputData[i] << ", " <<
		//	getDerivativeOfSigmoid(z_[i]) << ", " << z_[i];
		//	exit(0);
		//}
	}
}

double
neural_network::layer::getSigmoid(double x) const {
	/*
	return 1 / (1 + exp(-x));
	*/	
	return tanh(x);
	/*
	return x > 0 ? x : 0;
	*/
}

double 
neural_network::layer::getDerivativeOfSigmoid(double x) const {
	/*
	double temp = exp(-x);
	return temp / (1 + temp) / (1 + temp);
	*/	
	double a = tanh(x);
	return (1 - a * a);	
	/*
	return x > 0 ? 1 : 0;
	*/
}

}