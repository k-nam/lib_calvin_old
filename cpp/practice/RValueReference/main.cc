#include "myclass.h"


class simple {
public:
	simple(int value): value_(value) { 
		std::cout << "simple default con\n";
	}
	simple(simple const &rhs): value_(rhs.value_) {
		std::cout << "simple copy con\n";
	}
	simple(simple &&rhs): value_(rhs.value_) {
		std::cout << "simple move con\n";
	}
	void increment() { value_++; }
private:
	int value_;
};

// we should return r-value reference to preserve r-value semantic of argument

simple &&increase(simple &&original) {
	original.increment();
	return std::move(original);
}
// if we return object, each function call will cause copy con
simple increase2(simple &&original) {
	original.increment();
	return original;
}

void test1() {
	int i = 0;
	int const j = 0;
	int && rValueRef = 1 + 2;
	myclass<int> object1(static_cast<int &&>(rValueRef));
	myclass<int> object2(rValueRef);
	myclass<int> object3(g());
	myclass<int> object4(h());
	f(j);
	object1.foo(i);
	object1.foo(3);
}

void test2() {
	std::cout << "result for increase1\n";
	simple result = increase(increase(increase(simple(5))));
	std::cout << "result for increase2\n";
	simple result2 = increase2(increase2(increase2(simple(5))));
}

int main(int argc, char *argv[]) {
	//test1();
	test2();
}
