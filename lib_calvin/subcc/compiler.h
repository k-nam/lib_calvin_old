#ifndef LIB_CALVIN__SUBCC__COMPILER
#define LIB_CALVIN__SUBCC__COMPILER

#include "abstract_string.h"

namespace subcc 
{
	// Compiler and send output to stdout
	void compile(lib_calvin::string sourceCode, std::ostream &file);
}

#endif