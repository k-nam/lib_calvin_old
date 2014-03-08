#include <stdio.h>

class class_a {
public:
	class_a & operator=(class_a const &rhs){ printf("assignment called on a"); return *this;}
};
class class_b: public class_a { };

int main() {

	int c[3][3];
	int (*a)[3][3] = &c;
	int b[3][3][3];
	ptrdiff_t pointerSize = (char *)(a[0] + 1) - (char *)a[0];
	printf("pointer size = %d\n", pointerSize);	
	pointerSize = (char *)(b[0] + 1) - (char *)b[0];
	printf("pointer size = %d\n", pointerSize);	
	pointerSize = (char *)(a + 1) - (char *)a;
	printf("pointer size = %d\n", pointerSize);	
	pointerSize = (char *)(b + 1) - (char *)b;
	printf("pointer size = %d\n", pointerSize);

	class_b bb1;
	class_b bb2;
	class_b bb3;
	bb3 = (bb1 = bb2);

}

