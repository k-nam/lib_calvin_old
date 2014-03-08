template <typename T>
class myBaseTemplateClass {
public:
	bool operator< (myBaseTemplateClass const &rhs) { return t_ < rhs.t_; }
	virtual bool myFunc() const { return t_ < 0; }
	class Iterator {
	public:  
		virtual T operator*() const { return 5; }
	};
	typedef Iterator iterator_type;
	virtual int getValue() { return 5; }
	virtual iterator_type* getIterator() const { return new Iterator(); }
private:
	T t_;
};

template <typename T>
class myTemplateClass1: public myBaseTemplateClass<T> 
{
public:
	class Iterator: public myBaseTemplateClass<T>::Iterator {
	public:
		T operator*() const { return 6; }
	};
	typedef Iterator iterator_type;
	virtual int getValue() { return 6; }
	iterator_type* getIterator() const { return new Iterator(); }
};

template <typename T>
void testFunction(myBaseTemplateClass<T> &templateObjecct) {
	//auto iter = templateObjecct.getIterator();
	std::cout << "value of iter of base is " << templateObjecct.getValue() << "\n";
}

template <typename T>
void testFunction2(myBaseTemplateClass<T> templateObjecct) {
	std::cout << "value of iter of base is " << templateObjecct.getValue() << "\n";
}

template <typename T>
myBaseTemplateClass<T> 
testFunction3() {
	std::cout << "test func 3\n";
	return myTemplateClass1<T>();
}