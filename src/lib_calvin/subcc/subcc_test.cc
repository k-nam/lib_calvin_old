#include <iostream>
#include <fstream>
#include <streambuf>
#include "subcc_test.h"
#include "subcc_compiler.h"
#include "subcc_symbol_table.h"
#include "subcc_type.h"
#include "subcc_syntax_tree.h"

void subcc::subccTest() {
	using namespace subcc;
	using std::string;
	using std::cout;
	std::cout << "---------------- Beginning subcc test -----------------\n\n";
	// Adding "../subcc/" to make this path compatible with other projects
	std::string inputFileName = "../subcc/subc_code/a2.c";
	//char *inputFileName = "../subcc/subc_code/test.c";

	string sourceText;

	std::ifstream stream(inputFileName);
	if (!stream) {
		cout << "subccTest error: cannot open input file\n";
		exit(0);
	}
	stream.seekg(std::ios::end);
	sourceText.reserve(static_cast<size_t>(stream.tellg()));
	stream.seekg(std::ios::beg);
	sourceText.assign((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
	stream.close();

	cout << "input size is: " << sourceText.size() << "\n";
	string header("void printStr( char s[80] ); \n void printInt ( int a ); \n\
      void printNewLine(); \n");
	string inputToCompiler = header + sourceText;
	cout << "\n";

	/* Uncomment below line to save output to a file (which can be run in subcc_run project) */
	//std::ofstream file("../subcc/output/assembly.cc");
	std::ostream &file = std::cout;

	//while(true) { // loop to check memory leak
	compile(inputToCompiler, file);
	//}

	SymbolTable::countObjects();
	GlobalSymbolTable::countObjects();
	Type::countObjects();
	SyntaxTreeNode::countObjects();
	std::cout << "---------------- Subcc test finished -----------------\n\n\n";
}
