
//#include <vector>
#include <map>
#include <list>
#include <utility>
#include "adt.h"

using namespace lib_calvin_container;


/******************* IntStack definitions *******************/

IntStack::IntStack(size_t maxsize) : size_(0), maxsize_(maxsize), stack_(),
intToListIterator_(maxsize), isInserted_(maxsize, false) {

}

// key must be in valid range; otherwise, undefined behavior 
void IntStack::push(size_t key) {
	bool isInserted = isInserted_[key];
	if (isInserted) { // in stack, need operation 
		stack_.splice(stack_.begin(), stack_, intToListIterator_[key]);
		intToListIterator_[key] = stack_.begin();
		return;
	} else { // needs to be inserted
		stack_.push_front(key);
		intToListIterator_[key] = stack_.begin();
		isInserted_[key] = true;
		++size_;
		return;
	}
}

size_t IntStack::peek() const {
	return stack_.front();
}

size_t IntStack::pop() {
	size_t top = stack_.front();
	isInserted_[top] = false;
	stack_.pop_front();
	--size_;
	return top;
}

/************************ DisjointSet definitions ***************************/

DisjointSet::DisjointSet(size_t size) : elems_(size) {
	for (size_t i = 0; i < size; ++i) {
		elems_[i] = i;
	}
}

bool DisjointSet::isSameSet(size_t a, size_t b) {
	if (findSet(a) == findSet(b)) {
		return true;
	}
	return false;
}

size_t DisjointSet::findSet(size_t a) {
	// Following the link to the representative element
	// Doing path compression during the course
	while (a != elems_[a]) {
		stack_.push(a);
		a = elems_[a];
	}
	while (stack_.size() > 0) {
		elems_[stack_.pop()] = a;
	}
	stack_.init();
	return a;
}

void DisjointSet::unite(size_t a, size_t b) {
	a = findSet(a);
	b = findSet(b);
	elems_[a] = b;
}




