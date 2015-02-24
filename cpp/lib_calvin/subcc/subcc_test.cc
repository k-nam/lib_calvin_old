#include <iostream>
#include <fstream>
#include <streambuf>
#include "subcc_test.h"
#include "compiler.h"
#include "symbol_table.h"
#include "type.h"
#include "syntax_tree.h"

void subcc::subccTest(int argc, char *argv[]) {
	using namespace subcc;
	using std::string;
	using std::cout;
	std::cout << "---------------- Beginning subcc test -----------------\n\n";
	char *inputFileName = "../subcc_run/test/a1.c";
	string sourceText;
  if (argc != 2) {
    cout << "usage: subcc <input-file>, will use default value:" << "\"" << inputFileName << "\"\n";
	} else {
		inputFileName = argv[1];
	}

	std::ifstream stream(inputFileName);
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
	//while(true) {
		compile(inputToCompiler);
	//}

	SymbolTable::countObjects();
	GlobalSymbolTable::countObjects();
	Type::countObjects();
	SyntaxTreeNode::countObjects();
	std::cout << "---------------- Subcc test finished -----------------\n\n\n";
}
