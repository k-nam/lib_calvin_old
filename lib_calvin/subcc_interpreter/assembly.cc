#include <stdio.h> 
#include "subcc_interpreter.h"

namespace subcc_interpreter
{
char DATA[1024];
int GPR0;
int GPR1;
int GPR2;
int const ZERO = 0;
char M0[256];

void fibo() {

	STORE ( FP, STACK, SP - 4, 4);
	SP = SP - 4;
	FP = SP;
	SP = SP - 32;
	LOAD( GPR0 , STACK , FP + (4) + ZERO , 4 );
	 GPR1  = 1;
	 GPR2  =  GPR0 == GPR1 ;
	STORE( GPR2 , STACK , FP + (-4) + ZERO , 4 );
	LOAD( GPR0 , STACK , FP + (-4) + ZERO , 4 );
	IF ( GPR0 , L2 );
	GOTO (L5);
L5:
	LOAD( GPR0 , STACK , FP + (4) + ZERO , 4 );
	 GPR1  = 2;
	 GPR2  =  GPR0 == GPR1 ;
	STORE( GPR2 , STACK , FP + (-8) + ZERO , 4 );
	LOAD( GPR0 , STACK , FP + (-8) + ZERO , 4 );
	IF ( GPR0 , L2 );
	GOTO (L4);
L2:
	*((int *)M0) = 1;
	SP = FP;
	LOAD ( FP, STACK, SP, 4);
	SP = SP + 4;
	RETURN;
L3:
	GOTO (L1);
L4:
	LOAD( GPR0 , STACK , FP + (4) + ZERO , 4 );
	 GPR1  = 2;
	 GPR2  =  GPR0 - GPR1 ;
	STORE( GPR2 , STACK , FP + (-16) + ZERO , 4 );
	LOAD( M0 , STACK , FP + (-16) + ZERO , 4 );
	STORE ( M0, STACK, SP - 4 ,4 );
	SP = SP - 4;
	CALL( fibo);
	STORE( M0 , STACK , FP + (-20) + ZERO , 4 );
	SP = SP + 4 ;
	LOAD( GPR0 , STACK , FP + (4) + ZERO , 4 );
	 GPR1  = 1;
	 GPR2  =  GPR0 - GPR1 ;
	STORE( GPR2 , STACK , FP + (-24) + ZERO , 4 );
	LOAD( M0 , STACK , FP + (-24) + ZERO , 4 );
	STORE ( M0, STACK, SP - 4 ,4 );
	SP = SP - 4;
	CALL( fibo);
	STORE( M0 , STACK , FP + (-28) + ZERO , 4 );
	SP = SP + 4 ;
	LOAD( GPR0 , STACK , FP + (-20) + ZERO , 4 );
	LOAD( GPR1 , STACK , FP + (-28) + ZERO , 4 );
	 GPR2  =  GPR0 + GPR1 ;
	STORE( GPR2 , STACK , FP + (-32) + ZERO , 4 );
	LOAD( M0 , STACK , FP + (-32) + ZERO , 4 );
	SP = FP;
	LOAD ( FP, STACK, SP, 4);
	SP = SP + 4;
	RETURN;
L1:
	SP = FP;
	LOAD ( FP, STACK, SP, 4);
	SP = SP + 4;
	RETURN;
} // end of( fibo )

void _main() {

	STORE ( FP, STACK, SP - 4, 4);
	SP = SP - 4;
	FP = SP;
	SP = SP - 92;
	LOAD( M0 , STACK , FP + (-80) + ZERO , 80 );
	STORE ( M0, STACK, SP - 80 ,80 );
	SP = SP - 80;
	CALL( printStr);
	SP = SP + 80 ;
L7:
	*((int *)M0) = 10;
	STORE ( M0, STACK, SP - 4 ,4 );
	SP = SP - 4;
	CALL( fibo);
	STORE( M0 , STACK , FP + (-88) + ZERO , 4 );
	SP = SP + 4 ;
	LOAD( M0 , STACK , FP + (-88) + ZERO , 4 );
	STORE ( M0, STACK, SP - 4 ,4 );
	SP = SP - 4;
	CALL( printInt);
	SP = SP + 4 ;
L8:
	*((int *)M0) = 0;
	SP = FP;
	LOAD ( FP, STACK, SP, 4);
	SP = SP + 4;
	RETURN;
L6:
	SP = FP;
	LOAD ( FP, STACK, SP, 4);
	SP = SP + 4;
	RETURN;
} // end of( main )


}