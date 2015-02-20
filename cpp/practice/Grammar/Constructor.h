#include <iostream>
class Test {
public:
	Test(int a, int b) { std::cout << "direct ctor\n"; }
	Test(Test const &) { std::cout << "copy ctor\n"; }
};
