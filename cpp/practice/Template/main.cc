#include "my_template.h"
#include <iostream>

struct my { };

int main(int agrc, char *argv[]) {
	myBaseTemplateClass<int> a;
	auto iter = a.getIterator();
	std::cout << "value of iter of base is " << **iter << "\n";

	myTemplateClass1<int> b;
	auto iter2 = b.getIterator();
	std::cout << "value of iter of class1 is " << **iter2 << "\n";

	testFunction(a);
	testFunction(b);
	testFunction2(a);
	testFunction2(myTemplateClass1<int>());
	testFunction3<int>();
	//std::cout << "size of base class value type is " << sizeof(myBaseTemplateClass<int>::ValueType) << "\n";
	//std::cout << "size of derived class value type is " << sizeof(myTemplateClass1<int>::ValueType) << "\n";
	//bool a = myClass1 < myClass2;
	//myClass1.myFunc();

}
