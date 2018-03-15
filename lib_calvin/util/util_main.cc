// util.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <memory>

namespace lib_calvin_util
{
class myClass {
public:
	myClass() { std::cout << "ctor\n"; }
	~myClass() { std::cout << "dtor\n"; }
	void nonConstMethod() { std::cout << "non-const method\n"; }
	void constMethod() const { std::cout << "const method\n"; }
};
}

int main(int argc, char* argv[])
{
	using std::shared_ptr;
	using lib_calvin_util::myClass;
	shared_ptr<myClass> a(new myClass());
	shared_ptr<myClass> const b(new myClass());
	shared_ptr<myClass> c(a);

	a->nonConstMethod();
	a->constMethod();
	//b->nonConstMethod();
	b->constMethod();
	c->constMethod();

	return 0;
}

