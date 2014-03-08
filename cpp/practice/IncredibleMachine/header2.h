#pragma once

#include <set>
#include <algorithm>
#include <cstdlib>
#include <math.h>
#include <cmath>
#include <utility>
#include <exception>
#include <iostream>
#include <string>
#include <cstdarg>


namespace space1 {
	
	template <typename T>
	class myClass2 {
	public:
		class mySub {
		public:
			friend bool operator==(mySub const &lhs, mySub const &rhs)
			{ return lhs.mem == rhs.mem; }
			int mem;
		};
	private:
		T mem;
	};
	
	/*
	template <typename T>
	bool operator== (typename myClass2<T>::mySub const &lhs, 
		typename myClass2<T>::mySub const &rhs)
	{ return lhs.mem == rhs.mem; }*/
}

