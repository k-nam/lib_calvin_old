#include "handwritten_digits_analyzer.h"

int main() {
	double learningRate = 10;

	lib_calvin_neural_network::handwritten_digits_analyzer analyzer({ 100, 30 }, learningRate);

	size_t numEpochs = 50;

	analyzer.trainWithBinaryFile(numEpochs, "../../data/mnist/train-images.idx3-ubyte",
		"../../data/mnist/train-labels.idx1-ubyte",
		"../../data/mnist/t10k-images.idx3-ubyte",
		"../../data/mnist/t10k-labels.idx1-ubyte");

	//analyzer.trainWithTextFile(numEpochs, "../../../data/mnist/mnist_train.csv",
		//"../../../data/mnist/mnist_test.csv");

}