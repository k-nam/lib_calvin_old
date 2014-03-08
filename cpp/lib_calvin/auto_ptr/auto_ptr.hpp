#include <iostream>
#include <memory>

namespace lib_calvin
{
	/*
	template <typename Y>
	struct auto_ptr_ref {
		Y *yp;
		auto_ptr_ref(Y *rhs): yp(rhs) { 
			std::cout << "auto_ptr_ref::constructor\n";
		}
		
		auto_ptr_ref(auto_ptr_ref const &rhs): yp(rhs.yp) { 
			std::cout << "auto_ptr_ref::copy constructor\n";
		}
		~auto_ptr_ref() {
			std::cout << "auto_ptr_ref::destructor\n";
		}
	};*/

	template <typename T>
	class auto_ptr {
	private:
		T *ap;
		//auto_ptr(auto_ptr const &rhs);
	public:
		typedef T element_type;
		
		// Constructor
		explicit auto_ptr(T *ptr = 0) throw(): ap(ptr) { 
			std::cout << "auto_ptr::explicit constructor\n";
		}
		
		// Copy constructors
		auto_ptr(auto_ptr &rhs) throw(): ap(rhs.release()) { 
			std::cout << "auto_ptr::copy constructor\n";
		}
		template <typename Y>
		auto_ptr(auto_ptr<Y> &rhs) throw(): ap(rhs.release()) { 
			std::cout << "auto_ptr::template copy constructor\n";
		}
		
		// Assignments
		auto_ptr &operator= (auto_ptr &rhs) throw() {
			std::cout << "auto_ptr::assignment\n";
			reset(rhs.release());
			return *this;
		}
		template <typename Y>
		auto_ptr &operator= (auto_ptr &rhs) throw() {
			std::cout << "auto_ptr::template assignment\n";
			reset(rhs.release());
			return *this;
		}

		// Destructor
		~auto_ptr() throw() {
			std::cout << "auto_ptr::destructor\n";
			delete ap;
		}
		// value access
		T *get() const throw() { return ap; }
		T & operator*() const throw() { return *ap; }
		T * operator->() const throw() { return ap; }

		// Relase ownership
		T *release() throw() {
			std::cout << "auto_ptr::release\n";
			T *temp(ap);
			ap = 0;
			return temp;
		}
		
		// Reset value
		void reset(T *ptr = 0) throw() {
			std::cout << "auto_ptr::reset\n";
			if  (ap != ptr) {
				delete ap;
				ap = ptr;
			}
		}
	/*
		auto_ptr(auto_ptr_ref<T> rhs) throw(): ap(rhs.yp) { }
		auto_ptr & operator= (auto_ptr_ref<T> rhs) throw() {
			reset(rhs.yp);
			return *this;
		}
		template <typename Y>
		operator auto_ptr_ref<Y>() throw() {
			return auto_ptr_ref<Y>(release());
		}
		template <typename Y>
		operator auto_ptr<Y>() throw() {
			return auto_ptr<Y>(release());
		}*/
	};

	template <typename T>
	auto_ptr<T> identity(T t) {
		return auto_ptr<T>(new T(t));
	}
}

