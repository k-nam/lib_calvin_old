#include "handwritten_digits_analyzer.h"

int main() {
	//using namespace lib_calvin_neural_network;
	lib_calvin_neural_network::handwritten_digits_analyzer analyzer;

	analyzer.trainWithBinaryFile("../../../data/mnist/train-images.idx3-ubyte",
		"../../../data/mnist/train-labels.idx1-ubyte",
		"../../../data/mnist/t10k-images.idx3-ubyte",
		"../../../data/mnist/t10k-labels.idx1-ubyte");
	//analyzer.trainWithTextFile("../../../data/mnist/mnist_train.csv",
		//"../../../data/mnist/mnist_test.csv");

}