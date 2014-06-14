#include <memory>
#include <iostream>

class A { };
class B: public A { };

int main() {
	using std::shared_ptr;
	//std::cout << "main\n";
	shared_ptr<A> a1(new A());
	shared_ptr<A> a2(a1);
	shared_ptr<B> b1(new B());
	shared_ptr<B> b2 = b1;
	// shared_ptr<B> b2(b1.get()); // causes runtime error!
}