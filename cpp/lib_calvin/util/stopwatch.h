#ifndef LIB_CALVIN__UTIL__STOPWATCH_H
#define LIB_CALVIN__UTIL__STOPWATCH_H

#ifdef _WIN32

#include <Windows.h>

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
      LARGE_INTEGER temp;
      QueryPerformanceFrequency(&temp);
      frequency_ = GetDoubleFrom(&temp);
    }
    void start() {
      LARGE_INTEGER temp;
      QueryPerformanceCounter(&temp);
      start_ = GetDoubleFrom(&temp);
    }
    void stop() {
      LARGE_INTEGER temp;
      QueryPerformanceCounter(&temp);
      finish_ = GetDoubleFrom(&temp);
    }
    double read() { return (finish_ - start_) / frequency_; }
  private:
    double start_;
    double finish_;
    double frequency_;
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
		double read() { return static_cast<float>(finish_ - start_) / CLOCKS_PER_SEC;  }
  private:
		clock_t start_;
		clock_t finish_;
  };
}

#endif



#endif