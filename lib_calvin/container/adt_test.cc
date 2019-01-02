#include "adt_test.h"

using namespace lib_calvin_container;

void lib_calvin_container::adtTest() {
	size_t checkSum = pqTest<int, int, PqCorrect>();
	std::cout << "PqCorrect: " << checkSum << "\n";
}
