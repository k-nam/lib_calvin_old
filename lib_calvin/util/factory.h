#ifndef LIB_CALVIN__UTIL__FACTORY_H
#define LIB_CALVIN__UTIL__FACTORY_H 

#include "thread.h"
#include <vector>

namespace lib_calvin
{
// Interface to factory only for adding work (cannot call work())
template <typename Argument>
class FactoryLoader
{
public:
	FactoryLoader() : lock_(lib_calvin::create_lock()), workQueue_(),
		isClosed_(false) { }
	virtual ~FactoryLoader() { }
	void add(Argument const &); // add additional work to do
	void close() { isClosed_ = true; } // close the factory
protected:
	lock_type lock_;
	std::vector<Argument> workQueue_;
	bool isClosed_;
private:
};

// Wrapping class for factory pattern with multithreading support
// Simply calls Operation(Argument) for arguments in the given vector
template <typename Argument, typename Operation>
class Factory : public FactoryLoader<Argument>
{
public:
	using FactoryLoader<Argument>::lock_;
	using FactoryLoader<Argument>::workQueue_;
	using FactoryLoader<Argument>::isClosed_;

	Factory(Operation const &op) : FactoryLoader<Argument>(), index_(0), op_(op) { }
	// work() does not return until all work is done AND the factory is closed
	void work(); // includes critical section	
private:
	unsigned index_; // indicates current position
	Operation op_; // function object
};

// Thread function for factory pattern
// lpParam should be a pointer to Factory<Argument, Operation>
template <typename Argument, typename Operation>
void *factoryThreadFunction(void *lpParam);

}

template <typename Argument>
void lib_calvin::FactoryLoader<Argument>::add(Argument const &newWork)
{
	acquire_lock(lock_);
	if (isClosed_) { // can not add work if closed
		release_lock(lock_);
		return;
	} else {
		workQueue_.push_back(newWork);
		release_lock(lock_);
	}
}

template <typename Argument, typename Operation>
void lib_calvin::Factory<Argument, Operation>::work() {
	using namespace lib_calvin;


	// data mambers should be accessed only in the critical section
	while (true) {
		acquire_lock(lock_);
		if (index_ >= workQueue_.size()) { // all work done
			if (isClosed_) { // return only when the factory is closed
				release_lock(lock_);
				return;
			} else {
				release_lock(lock_);
				thread_yield(); // don't consume CPU time
								//Sleep(1);
			}
		} else {
			unsigned index = index_;
			++index_;
			release_lock(lock_);
			op_(workQueue_[index]);
		}
	}
}

template <typename Argument, typename Operation>
void *
lib_calvin::factoryThreadFunction(void *param) {
	Factory<Argument, Operation> *factory = (Factory<Argument, Operation> *)param;
	factory->work();
	return NULL;
}

#endif