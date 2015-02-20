#include <iostream>

template <typename T>
class myclass {
public:
	myclass(int const &a):a_(a) { std::cout << "const &" << "\n"; }
	myclass(int &a):a_(a) { std::cout << "&" << "\n"; }
	myclass(int const &&a):a_(a) { std::cout << "const &&" << "\n"; }
	myclass(int &&a):a_(a) { std::cout << "&&" << "\n"; }
	template <typename T1>
	void foo(T1 &&input) { std::cout << "foo\n"; boo(std::forward<T1>(input)); }
	template <typename T1>
	void boo(T1 &&input) { std::cout << "boo\n"; goo(std::forward<T1>(input)); }
	void goo(int const &input) { std::cout << "goo lvalue\n"; }
	void goo(int &&input) { std::cout << "goo rvalue\n"; }
private:
	int a_;
};

template <typename T>
void f(T &&a) {
	myclass<int> object(std::forward<T>(a));
}
int const && g() {
	int j = 5;
	return j + 1; // dangling ref?
} 
int & h() {
	static int value = 0;
	return value; // dangling ref?
}
