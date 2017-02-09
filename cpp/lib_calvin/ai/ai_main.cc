#include "handwritten_digits_analyzer.h"
#include "matrix.h"

int main() {
	//using namespace lib_calvin_ai;
	lib_calvin_ai::handwritten_digits_analyzer a;
	a.trainWithFile("D:/dev/data/ai/handwriting/train-images.idx3-ubyte",
		"D:/dev/data/ai/handwriting/train-labels.idx1-ubyte");
}