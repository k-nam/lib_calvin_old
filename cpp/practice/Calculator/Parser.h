#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"

namespace lib_calvin_desk_calculator
{
  // Using grammar, parse the expression
  // E -> T | T + E | T - E
  // T -> F | F * T | F / T
  // F -> Num | ( E )
  class Parser {
  public:
    Parser(Lexer lexer): lexer_(lexer) { }
    double Parse(); // return the value of the expression
  private:
    double ParseExpression();
    double ParseTerm();
    double ParseFactor();

    Lexer lexer_;
  };

}

#endif