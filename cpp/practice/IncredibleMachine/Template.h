#include <set>
#pragma once

class A {
public: 
	A() { }
	A(int a) { }
	virtual A &func() { return *this; }
};

class B: public A {
public:
	B(A a) { }
	A &func() { return *this; }
};

void f(A a) { }

namespace Library = std;

namespace space1 {
	class myClass {
		std::set<int> member1;
	};
}

namespace space3 {
	class myClass2 {
		std::set<int> member1;
	};
}


