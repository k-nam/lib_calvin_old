struct Person {
	char name[10];
	int age;
};


int fibo(int n) {
	if (n == 1 || n == 2) {
		return 1;
	} else {
		return fibo(n - 2) + fibo(n - 1);
	}
}


int main() {
	char string1[80] = "Fibonacci(10) is";
	printStr(string1);
	printInt(fibo(10));
	return 0;
}

