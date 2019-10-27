#include <stdio.h>
#include "subcc_interpreter.h"

/* stack 8KB */
 char subcc_interpreter::STACK[ 8096];
 int  subcc_interpreter::SP = 0;
 int  subcc_interpreter::FP = 0;

/* stack structure for a function call */
/*
 *   +------------------------+
 *   | Actual Parameters      |
 *   +------------------------+
 *   | Returned values        |
 *   +------------------------+ <-- FP
 *   | Saved machine status   |
 *   +------------------------+ <-- SP
 */

void subcc_interpreter::printInt()
{
	// --------------------------------------------------------------------
	// declarations of formal parameters
	int a;
	// declarations of symbolic local variables
	// declarations of symbolic tempolary variables
	// --------------------------------------------------------------------

	// --------------------------------------------------------------------
	// prolog begin
	// save machine status
	STORE( FP, STACK, SP - 4, 4 );
	FP = SP;
	SP = SP - 4;
	// retrieve formal parameters
	LOAD( a, STACK, FP, 4 );
	// set initial values of symbolic variables.
	// prolog end
	// --------------------------------------------------------------------

	// --------------------------------------------------------------------
	// native function call.
	printf( "%d", a );
	// --------------------------------------------------------------------

	// --------------------------------------------------------------------
	// epilog begin
	SP = FP;
	LOAD( FP, STACK, FP - 4, 4 );
	RETURN;
	// epilog end 
	// --------------------------------------------------------------------
}

void subcc_interpreter::printStr()
{
	// --------------------------------------------------------------------
	// declarations of formal parameters
	char s[80];
	// declarations of symbolic local variables
	// declarations of symbolic tempolary variables
	// --------------------------------------------------------------------

	// --------------------------------------------------------------------
	// prolog begin
	// save machine status
	STORE( FP, STACK, SP - 4, 4 );
	FP = SP;
	SP = SP - 4;
	// retrieve formal parameters
	LOAD( s, STACK, FP, 80 );
	// set initial values of symbolic variables.
	// prolog end
	// --------------------------------------------------------------------

	// --------------------------------------------------------------------
	// native function call.
	printf( "%s", s );
	// --------------------------------------------------------------------

	// --------------------------------------------------------------------
	// epilog begin
	SP = FP;
	LOAD( FP, STACK, FP - 4, 4 );
	RETURN;
	// epilog end 
	// --------------------------------------------------------------------
}

void subcc_interpreter::printNewLine()
{
	// --------------------------------------------------------------------
	// declarations of formal parameters
	// declarations of symbolic local variables
	// declarations of symbolic tempolary variables
	// --------------------------------------------------------------------

	// --------------------------------------------------------------------
	// prolog begin
	// save machine status
	STORE( FP, STACK, SP - 4, 4 );
	FP = SP;
	SP = SP - 4;
	// retrieve formal parameters
	// set initial values of symbolic variables.
	// prolog end
	// --------------------------------------------------------------------

	// --------------------------------------------------------------------
	// native function call.
	printf( "\n" );
	// --------------------------------------------------------------------

	// --------------------------------------------------------------------
	// epilog begin
	SP = FP;
	LOAD( FP, STACK, FP - 4, 4 );
	RETURN;
	// epilog end 
	// --------------------------------------------------------------------
}
