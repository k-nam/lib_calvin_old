#include "boost/shared_ptr.hpp"
#include <iostream>

class A { };
class B: public A { };

int main() {
	using boost::shared_ptr;
	//std::cout << "main\n";
	shared_ptr<A> a1(new A());
	shared_ptr<A> a2(a1);
	shared_ptr<B> b1(new B());
	shared_ptr<B> a3(b1.get());
}