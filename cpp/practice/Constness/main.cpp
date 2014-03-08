#include "MyClass.h"


int *MyClass::constFunction() const {
	*pointer_ = 0;
	return pointer_;
}

int main() {
	MyClass class1;
	class1.constFunction();
}