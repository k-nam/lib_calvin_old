#include "Desk_Calculator.h"
#include <iostream>

using lib_calvin::Desk_Calculator;
using namespace lib_calvin_desk_calculator;

void Desk_Calculator::Calculate(std::string const &input) const {
  Lexer lexer(input);
  while(true) {
    struct Token curToken = lexer.GetNextToken();
    if(curToken.name_ == dollar || curToken.name_ == error)
      break;
  }
  lexer.reset();
  Parser parser(lexer);
  std::cout << "The result is: " << parser.Parse() << std::endl;
}
