#include <iostream>
#include "derived_class.h"

void func(BaseClass *) {
	std::cout << "func calling with base class\n";
}
void func(DerivedClass *) {
	std::cout << "func calling with derived class\n";
}

int main() {
	BaseClass *base = NULL;
	DerivedClass *derived = new DerivedClass();
	DerivedClass *derived2 = new DerivedClass2();
	BaseClass *basePointerToDerived = derived;
	func(base);
	func(derived);
	func(derived2);
	func(basePointerToDerived);

	//base->callHey();
	derived->callHey();
	derived2->callHey();

	derived2->func();
	derived2 = static_cast<DerivedClass2 *>(operator new(sizeof(DerivedClass2)));
	//derived2->func(); // runtime error because virtual function func() requires valid RTTI
	//int a = derived->a;
}