subcc: main.cc Lexer.o Parser.o Type.o SymbolTable.o SyntaxTree.o parser.o ThreeAddr.o CCodeGenerator.o ../calvin/String.h ../calvin/String.cc ../calvin/Stopwatch.cc
	g++ -O2 -o subcc -Wall -I../calvin main.cc Lexer.o Parser.o Type.o SymbolTable.o \
		SyntaxTree.o parser.o ThreeAddr.o CCodeGenerator.o \
		../calvin/String.cc ../calvin/Stopwatch.cc 

parser.o: ../calvin/parser.h ../calvin/parser.cc
	g++ -O3 -c ../calvin/parser.cc

Lexer.o: Lexer.h Lexer.cc ../calvin/lexer.h
	g++ -O2 -c Lexer.cc -I../calvin

Parser.o: Parser.h Parser.cc ../calvin/parser.h
	g++ -O2 -c Parser.cc -I../calvin

Type.o: Type.h Type.cc
	g++ -O2 -c Type.cc -I../calvin

SymbolTable.o: SymbolTable.h SymbolTable.cc
	g++ -O2 -c SymbolTable.cc -I../calvin

SyntaxTree.o: SyntaxTree.h SyntaxTree.cc
	g++ -O2 -c SyntaxTree.cc -I../calvin

ThreeAddr.o: ThreeAddr.h ThreeAddr.cc
	g++ -O2 -c ThreeAddr.cc -I../calvin

CCodeGenerator.o: CCodeGenerator.h CCodeGenerator.cc
	g++ -O2 -c CCodeGenerator.cc -I../calvin

clean:
	rm -f *.o subcc result
	

