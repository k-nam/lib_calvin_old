#ifndef LIB_CALVIN__UTIL__RANDOM_H
#define LIB_CALVIN__UTIL__RANDOM_H
#include <random>

namespace lib_calvin
{
  class random_number_generator {
  public:
		random_number_generator();
		random_number_generator(size_t seed);
		size_t operator()();
		size_t operator()(size_t mod);
  private:
		std::mt19937_64 engine;
  };
}

#endif