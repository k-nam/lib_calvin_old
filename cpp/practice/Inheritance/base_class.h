struct BaseClass {
	virtual void hey() { std::cout << "hey in base\n"; }
	void callHey() { hey(); }
	virtual int func() = 0;
	int a;
};