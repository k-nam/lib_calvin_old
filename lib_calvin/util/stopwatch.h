#ifndef LIB_CALVIN__UTIL__STOPWATCH_H
#define LIB_CALVIN__UTIL__STOPWATCH_H


#include <chrono>

namespace lib_calvin
{
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


#endif