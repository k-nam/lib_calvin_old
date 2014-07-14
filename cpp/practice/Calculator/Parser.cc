#include "Parser.h"
#include <iostream>

using namespace lib_calvin_desk_calculator;

double Parser::Parse() {
  return ParseExpression();
}

double Parser::ParseExpression() {
  std::cout << "parsing exp\n";
  double value1 = this->ParseTerm();
  Token nextToken = lexer_.SeeNextToken();
  if (nextToken.name_ == bin_op_plus) { // E -> T + E
    lexer_.GetNextToken();
    double value2 = this->ParseExpression();
    return value1 + value2;
  } else if (nextToken.name_ == bin_op_minus) { // E -> T - E
    lexer_.GetNextToken();
    double value2 = this->ParseExpression();
    return value1 - value2;
  } else { // E -> T
    return value1;
  }
}

double Parser::ParseTerm() {
  std::cout << "parsing term\n";
  double value1 = this->ParseFactor();
  Token nextToken = lexer_.SeeNextToken();
  if (nextToken.name_ == bin_op_multiply) { // T -> F * T
    lexer_.GetNextToken();
    double value2 = this->ParseTerm();
    return value1 * value2;
  } else if (nextToken.name_ == bin_op_divide) { // T -> F / T
    lexer_.GetNextToken();
    double value2 = this->ParseTerm();
    return value1 / value2;
  } else { // T -> F
    return value1;
  }
}

double Parser::ParseFactor() {
  std::cout << "parsing factor\n";
  Token nextToken = lexer_.SeeNextToken();  
  if (nextToken.name_ == paren_open) { // T -> ( E )
    lexer_.GetNextToken();
    double value = this->ParseExpression();
    if (lexer_.GetNextToken().name_ != paren_close) { // error
      std::cout << "Unmatched parenthesis!\n";
      return -1;
    } else {
		return value;
	}
  } else if (nextToken.name_ == number) { // T -> Num
    return lexer_.GetNextToken().value_;
  } else { // error
    std::cout << "error in parsing factor\n";
    return -1;
  }
}

  