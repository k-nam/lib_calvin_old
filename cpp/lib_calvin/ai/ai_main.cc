#include "handwritten_digits_analyzer.h"
#include "json/json.hpp"

int main() {
	//using namespace lib_calvin_ai;
	lib_calvin_ai::handwritten_digits_analyzer a;
	//a.trainWithBinaryFile("D:/dev/data/ai/handwriting/train-images.idx3-ubyte",
		//"D:/dev/data/ai/handwriting/train-labels.idx1-ubyte");
	a.trainWithTextFile("D:/dev/data/ai/handwriting/mnist_train.csv",
		"D:/dev/data/ai/handwriting/mnist_test.csv");
}