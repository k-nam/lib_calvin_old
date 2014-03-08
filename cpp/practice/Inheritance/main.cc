#include <iostream>
#include "derived_class.h"

void func(BaseClass *) {
	std::cout << "calling with base class\n";
}
void func(DerivedClass *) {
	std::cout << "calling with derived class\n";
}

int main() {
	/*
	BaseClass base;
	DerivedClass derived;
	BaseClass *derivedP = &derived;
	func(&base);
	func(derivedP);

	base.callHey();
	derived.callHey();
	*/

	DerivedClass2 *derived = new DerivedClass2();
	derived->func();
	DerivedClass2 *derived2 = static_cast<DerivedClass2 *>(operator new(sizeof(DerivedClass2)));
	derived2->func();
	int a = derived->a;
}