#ifndef DESK_CALCULATOR_H
#define DESK_CALCULATOR_H

#include "Lexer.h"
#include "Parser.h"

namespace lib_calvin
{
  class Desk_Calculator {
  public:
    Desk_Calculator() { }
    void Calculate(std::string const &) const;
  private:

  };
} // end namespace lib_calvin_desk_calculator

#endif

