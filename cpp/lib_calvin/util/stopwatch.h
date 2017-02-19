#ifndef LIB_CALVIN__UTIL__STOPWATCH_H
#define LIB_CALVIN__UTIL__STOPWATCH_H

#ifdef _WIN32

#include <Windows.h>
#include <chrono>

namespace lib_calvin_stopwatch
{
double GetDoubleFrom(LARGE_INTEGER const *pLargetInteger);
}

namespace lib_calvin
{
using lib_calvin_stopwatch::GetDoubleFrom;
class stopwatch {
public:
	stopwatch() {
	}
	void start() {
		start_ = clock_.now();
	}
	void stop() {
		finish_ = clock_.now();
	}
	double read() { return std::chrono::duration<double>(finish_ - start_).count(); }
private:
	std::chrono::high_resolution_clock clock_;
	std::chrono::time_point<std::chrono::high_resolution_clock> start_;
	std::chrono::time_point<std::chrono::high_resolution_clock> finish_;
};
}

#else
#include <time.h>

namespace lib_calvin
{
class stopwatch {
public:
	stopwatch() {
	}
	void start() {
		start_ = clock();
	}
	void stop() {
		finish_ = clock();
	}
	double read() { return static_cast<float>(finish_ - start_) / CLOCKS_PER_SEC; }
private:
	clock_t start_;
	clock_t finish_;
};
}

#endif



#endif