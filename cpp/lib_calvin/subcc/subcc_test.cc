#include <iostream>
#include <fstream>
#include <streambuf>
#include "subcc_test.h"
#include "compiler.h"
   
void subcc::subccTest(int argc, char *argv[]) {
	using namespace subcc;
	using std::string;
	using std::cout;
	char *inputFileName = "../subcc_run/test/a1.c";
	string sourceText;
  if (argc != 2) {
    cout << "usage: subcc <input-file>, will use default value:" << "\"" << inputFileName << "\"\n";
	} else {
		inputFileName = argv[1];
	}

	std::ifstream t(inputFileName);
	t.seekg(0, std::ios::end);   
	sourceText.reserve(t.tellg());
	t.seekg(0, std::ios::beg);
	sourceText.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	cout << "input size is: " << sourceText.size() << "\n";
  string header("void printStr( char s[80] ); \n void printInt ( int a ); \n\
      void printNewLine(); \n");
  string inputToCompiler = header + sourceText;
	//while(true) {
		compile(inputToCompiler);
	//}
}
