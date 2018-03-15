#include <iostream>
#include <fstream>
#include <streambuf>

#include "compiler.h"
#include "subcc_parser.h" 
#include "stopwatch.h"
#include "three_address.h"
#include "c_code_generator.h"

void subcc::compile(string sourceCode, std::ostream &file) {
	// Compilation steps are: 
	// Step 1. Parser: source code --> SyntaxTree(IR, with SymbolTable)  
	// Step 2. ThreeAdressCodeGenerator: SyntaxTree --> ThreeAddressCode(IR, with SymbolTable)
	// Step 3. CCodeGenerator: ThreeAddressCode --> C source code(stdout)

	/***** Step 1: *****/
	cout << "-------------- SubccParser building start ---------------\n";
	Parser myParser(sourceCode); // Build parser: it contains lexer 
	cout << "-------------- SubccParser building finished ---------------\n\n";
	stopwatch watch;
	watch.start();
	shared_ptr<SyntaxTree const> syntaxTree = myParser.getSyntaxTree();
	watch.stop();
	cout << "Type checking OK\n";
	cout << "Building syntax tree with static checking took " << watch.read() << " sec\n";
	if (syntaxTree == NULL) {
		std::cout << "syntax tree was null\n";
		exit(0);
	}
	cout << "\n";

	/***** Step 2: *****/
	ThreeAdressCodeGenerator threeAddrCodeGen(syntaxTree);
	shared_ptr<ThreeAdressCode const> threeAddrCode = threeAddrCodeGen.getCode();

	/***** Step 3: *****/
	CCodeGenerator cCodeGen(threeAddrCode);
	cout << "--------------- Below is generated subcc code ---------------\n";

	// Redirect cout to a file
	std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(file.rdbuf()); //redirect std::cout to out.txt!

	cCodeGen.printCCode();

	// Reset to standard output again
	std::cout.rdbuf(coutbuf);

	cout << "--------------- End of generated subcc code ---------------\n\n";
}