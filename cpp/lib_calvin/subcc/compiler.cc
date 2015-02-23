#include "compiler.h"
#include "subcc_parser.h" 
#include "stopwatch.h"
#include "three_address.h"
#include "c_code_generator.h"

void subcc::compile(string sourceCode) {
	// Compilation steps are: 
	// Step 1. Parser: source code --> SyntaxTree(IR, with SymbolTable)  
	// Step 2. ThreeAdressCodeGenerator: SyntaxTree --> ThreeAddressCode(IR, with SymbolTable)
	// Step 3. CCodeGenerator: ThreeAddressCode --> C source code(stdout)

	/***** Step 1: *****/
	cout << "\n-------------- SubccParser building start ---------------\n";
  Parser myParser(sourceCode); // Build parser: it contains lexer 
  cout << "---------------------------------------------------------------\n\n";
  //myParser.test();
  stopwatch watch;
  watch.start();
	shared_ptr<SyntaxTree const> syntaxTree = myParser.getSyntaxTree();
  watch.stop();
  cout << "Type checking OK\n";
  cout << "Building syntax tree with static checking took " << watch.read() << 
    " sec\n";
	if (syntaxTree == NULL) {
		std::cout << "syntax tree was null\n";
		exit(0);
	}

	/***** Step 2: *****/
  ThreeAdressCodeGenerator threeAddrCodeGen(syntaxTree);
  shared_ptr<ThreeAdressCode const> threeAddrCode = threeAddrCodeGen.getCode();

	/***** Step 3: *****/
  CCodeGenerator cCodeGen(threeAddrCode);
	std::cout << "\n--------------- Below is generated subcc code ---------------\n";
	cCodeGen.printCCode();
  
  // Write C source code into a file
  //dup2(writeFd, STDOUT_FILENO);	

}