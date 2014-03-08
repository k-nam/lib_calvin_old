
#include "stopwatch.h"
#include <math.h>

double
lib_calvin_stopwatch::GetDoubleFrom(LARGE_INTEGER const *pLargeInteger) {
	unsigned lowPart = pLargeInteger->LowPart;
	unsigned highPart = pLargeInteger->HighPart;
	double value = static_cast<double>(highPart) * pow(2.0, 32)  + lowPart;
	return value;
}


