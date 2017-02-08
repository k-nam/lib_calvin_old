#include "handwritten_digits_analyzer.h"

int main() {
	handwritten_digits_analyzer a;
	a.trainWithFile("D:/dev/data/ai/handwriting/train-images.idx3-ubyte",
		"D:/dev/data/ai/handwriting/train-labels.idx1-ubyte");
}