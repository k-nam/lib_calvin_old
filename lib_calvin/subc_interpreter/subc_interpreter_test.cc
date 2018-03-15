#include <iostream>
#include "subc_interpreter.h"
#include "subc_interpreter_test.h"

void subc_interpreter::subcInterpreterTest() {
	std::cout << "---------- Beginning subcc_interpreter test -----------\n\n";
	subc_interpreter::test();
	std::cout << "---------- subcc_interpreter test finished -----------\n\n\n";

}

void subc_interpreter::test()
{
	/* initialize stack pointer (SP) */
	SP = STACK_SIZE;
	FP = STACK_SIZE;
	memset(STACK, 0, STACK_SIZE);

	SP = SP - 4;
	_main();
}