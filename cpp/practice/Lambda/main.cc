#include <iostream>
#include <string>

int main() {
	int x = 0;
	int y = 1;
	auto lambda = [] { std::cout << "this is my first lambda\n"; };
	auto l2 = [x](int a[3]) { return x + a[1]; };
	// auto l3 = [x](int &a[3]) { return x + a[1]; }; // array of ref is not permitted
	auto l4 = [x](int const (&a)[3]) { return x + a[1]; };

	[] { std::cout << "second lambda\n"; } ();
	
	auto printString = [] (const std::string &input) { std::cout << input << "\n"; };
	printString("test input string for argumented lambda");
}