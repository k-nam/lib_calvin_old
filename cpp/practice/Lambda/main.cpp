#include <iostream>
#include <string>

int main() {
	auto lambda = [] { std::cout << "this is my first lambda\n"; };
	lambda();

	[] { std::cout << "second lambda\n"; } ();
	
	auto printString = [] (const std::string &input) { std::cout << input << "\n"; };
	printString("test input string for argumented lambda");
}