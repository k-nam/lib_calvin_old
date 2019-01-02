#ifndef LIB_CALVIN__CONTAINER__ADT_TEST_H
#define LIB_CALVIN__CONTAINER__ADT_TEST_H

#include "adt.h"
#include "random.h"

namespace lib_calvin_container
{

void adtTest();

template <typename K, typename P,
	template<typename _K, typename _P> class Impl>
size_t pqTest();


template <typename K, typename P, 
	template<typename _K, typename _P> class Impl>
size_t
pqTest() {
	
	size_t const numOps = 100000;
	size_t const numKeys = 10000;
	size_t const relaxPerPop = 100;

	// Giving seed to provide identical input sequence for each call
	lib_calvin::random_number_generator gen(0);

	size_t checkSum = 0;

	Impl<K, P> pq(numKeys);
	for (size_t i = 0; i < numOps; i++) {
		for (size_t j = 0; j < relaxPerPop; j++) {
			// Note that identical priority values will cause ambiguity
			checkSum += pq.insert(gen() % numKeys, gen());
		}
		auto popped = pq.pop();
		checkSum += popped.first;
		checkSum += popped.second;
	}
	return checkSum;
}

}

#endif