#include "adt_test.h"
#include "stopwatch.h"

using namespace lib_calvin_container;

void lib_calvin_container::adtTest() {
	size_t checkSum = 0;
	lib_calvin::stopwatch watch;

	watch.start();
	checkSum = pqTest<size_t, size_t, PqCorrect>();
	watch.stop();
	std::cout << "PqCorrect: " << watch.read() << ", " << checkSum << "\n";

	watch.start();
	checkSum = pqTest<size_t, size_t, Pq>();
	watch.stop();

	std::cout << "Pq: " << watch.read() << ", " << checkSum << "\n";

}
