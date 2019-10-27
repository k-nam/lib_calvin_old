#include "string_test.h"

int main(int argc, char* argv[])
{
	using namespace lib_calvin;
	lib_calvin_string::stringTest();
	lib_calvin::c_string a = "good";
	std::cout << a;
	return 0;
}

