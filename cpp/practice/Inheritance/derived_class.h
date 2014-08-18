#include "base_class.h"

struct DerivedClass: public BaseClass {
	void hey() { std::cout << "hey in derived\n"; }
	virtual int func() { return value; }
	int value;
	int a;
};

struct DerivedClass2: public DerivedClass {
	void hey() { std::cout << "hey in derived2\n"; }
	using DerivedClass::func;
	int a;
};