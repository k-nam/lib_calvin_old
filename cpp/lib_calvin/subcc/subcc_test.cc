#include "subcc_test.h"
#include <iostream>
#include "fcntl.h" // for open, read
//#include "unistd.h" // for constants
#include <Windows.h>

#include "compiler.h"
   

void subcc::subccTest(int argc, char *argv[]) {
	using namespace subcc;
	using lib_calvin::string;
	using std::cout;
	// 10MB will be enough for a source file
	int const MAX_FILE_SIZE = 10000000;
  //cout << "size of subcc parser is " << sizeof (Parser) << endl;
  //cout << "size of subcc lexer is " << sizeof (SubccLexer) << endl;
	std::cout << "\n---------- Beginning subcc test -----------\n";
  char *buffer = new char[MAX_FILE_SIZE]; // store entire file 
  int fileSize = 0;
	char *inputFileName = NULL;
  if (argc != 2) {
		inputFileName = "../subcc_run/test/a1.c";
    cout << "usage: subcc <input-file>, will use default value:" << "\"" << inputFileName << "\"\n";

	} else {
		inputFileName = argv[1];
	}
	HANDLE inputFile = CreateFile((LPCTSTR)
		inputFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 
											FILE_ATTRIBUTE_NORMAL, NULL);
	if (inputFile == INVALID_HANDLE_VALUE) {
		cout << "open file fail: " << inputFileName << "\n";
		exit(0);
	}
	DWORD numBytesRead = 0;
	BOOL read = ReadFile(inputFile, buffer, MAX_FILE_SIZE, &numBytesRead, NULL);
	cout << "reading size: " << numBytesRead << "\n";

  string sourceFile(buffer, numBytesRead); // entire input
	cout << "input size is: " << sourceFile.size() << "\n";
  string header("void printStr( char s[80] ); \n void printInt ( int a ); \n\
      void printNewLine(); \n");
  string input = header + sourceFile;
	//while(true) {
		compile(input);
	//}
}
