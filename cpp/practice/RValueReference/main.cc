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
	std::cout << "Starting test1\n";
	int i = 0;
	int const j = 0;
	int && rValueRef = 1 + 2;
	std::cout << "Should be &&: ";
	myclass<int> object1(std::forward<int &&>(rValueRef));
	std::cout << "Should be &: ";
	myclass<int> object2(rValueRef);
	std::cout << "Should be const &&: ";
	myclass<int> object3(g());
	std::cout << "Should be &: ";
	myclass<int> object4(h());
	std::cout << "Should be const &: ";
	f(j);
	std::cout << "Should be foo, boo, goo, lvalue: ";
	object1.foo(i);
	std::cout << "Should be foo, boo, goo, rvalue: ";
	object1.foo(3);
	std::cout << "\n";
}

void test2() {
	std::cout << "Starting test2\n";
	std::cout << "result for increase1\n";
	simple result = increase(increase(increase(simple(5))));
	std::cout << "result for increase2\n";
	simple result2 = increase2(increase2(increase2(simple(5))));
}

int main(int argc, char *argv[]) {
	test1();
	test2();
}
