#include "random.h"

lib_calvin::random_number_generator::random_number_generator(): engine(std::random_device()()) { }

lib_calvin::random_number_generator::random_number_generator(size_t seed): engine(seed) { }

size_t 
lib_calvin::random_number_generator::operator()() {
	return engine();
}

size_t 
lib_calvin::random_number_generator::operator()(size_t mod) {
	return engine() % mod;
}



