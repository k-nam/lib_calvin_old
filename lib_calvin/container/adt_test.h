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
	
	size_t const numOps = 10000;
	size_t const numKeys = 1000;
	size_t const numPriorities = 100;

	// Giving seed to provide identical input sequence for each call
	lib_calvin::random_number_generator gen(0);

	size_t checkSum = 0;

	Impl<K, P> pq(numKeys);
	for (size_t i = 0; i < numOps; i++) {
		checkSum += pq.insert(gen() % numKeys, gen() % numPriorities);
		checkSum += pq.insert(gen() % numKeys, gen() % numPriorities);
		auto popped = pq.pop();
		checkSum += popped.first;
		checkSum += popped.second;
	}

	return checkSum;
}

}

#endif