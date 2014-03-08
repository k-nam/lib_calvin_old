#ifndef LIB_CALVIN__UTIL__STOPWATCH_H
#define LIB_CALVIN__UTIL__STOPWATCH_H

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

#endif