#include "lexer.h"
#include <iostream>
#include <math.h>

using namespace lib_calvin_desk_calculator;

void Lexer::JumpBlanks() {
  while (index_ < string_.size() && string_[index_] == ' ')
    ++index_;
}

// Returns negative value if erroneous
double Lexer::LexNumber() {
  this->JumpBlanks();
  this->isLexingNumber_ = false; // next, we should lex an operator
  // Lex integer part
  int intValue = 0;
  int fractionValue = 0; // to be divided by 'power' later
  bool emptyIntegerPart = true;
  bool emptyFractionPart = true;
  while(index_ < string_.size() && string_[index_] >= '0'
    && string_[index_] <= '9') {
      emptyIntegerPart = false;
      intValue = intValue*10 + (string_[index_] - '0');
      ++index_;
  }
  if (index_ == string_.size()) {
    if (emptyIntegerPart)
      return -1;
    else 
      return intValue;
  }
  // Lex dot
  if (string_[index_] == '.')
    ++index_;
  else { // no fractional part
    if(emptyIntegerPart) // this is not a number!
      return -1;
    return intValue; 
  }
  // Lex fractional part
  double power = 0;
  while(index_ < string_.size() && string_[index_] >= '0' 
    && string_[index_] <= '9') {
      emptyFractionPart = false;
      fractionValue = fractionValue*10 + (string_[index_] - '0');
      ++index_;
      ++power;
  }
  if (emptyIntegerPart && emptyFractionPart) { // only '.'
    return -1;
  }
  double result = 
    intValue + static_cast<double>(fractionValue) / (pow(10.0, power));
  return result;
 
}


struct Token Lexer::GetNextToken() {
  this->JumpBlanks();
  if (index_ == string_.size()) { // reached the end
    return dollar;
  }
  switch (string_[index_]) {
    case '(':
      {
        ++index_;
        std::cout << "paren_open\n";
        return Token(paren_open);
      }
    case ')': 
      {
        ++index_;
        std::cout << "paren_close\n";
        return Token(paren_close);
      }
    case '*':
      {
        ++index_;
        this->isLexingNumber_ = true;
        std::cout << "multiply\n";
        return Token(bin_op_multiply);
      }
    case '/':
      {
        ++index_;
        this->isLexingNumber_ = true;
        std::cout << "divide\n";
        return Token(bin_op_divide);
      }
    case '+':
      // Be careful: this '+' might be unary operator
      {
        ++index_;
        if(isLexingNumber_) { // expecting a number
          // Scan a positive number
          double value = this->LexNumber();
          if (value >= 0) {
            std::cout << "number " << value << std::endl;
            return Token(number, value);
          } else
            return Token(error);
        } else { // expecting a binary operator
          this->isLexingNumber_ = true;
          std::cout << "bin_op_plus\n";
          return Token(bin_op_plus);
        }
      }
    case '-':
      {
        ++index_;
        if(isLexingNumber_) { // expecting a number
          // Scan a positive number
          double value = this->LexNumber();
          if (value >= 0) {
            std::cout << "number " << -value << std::endl;
            return Token(number, -value);
          } else
            return Token(error);
        } else { // expecting a binary operator
          this->isLexingNumber_ = true;
          std::cout << "bin_op_minus\n";
          return Token(bin_op_minus);
        }
      }
    default: 
      // Lex a number
      {
        if(isLexingNumber_) { // expecting a number
          // Scan a positive number
          double value = this->LexNumber();
          if (value >= 0) {
            std::cout << "number " << value << std::endl;
            return Token(number, value);
          } else
            return Token(error);
        } else { // expecting a binary operator
          std::cout << "error\n";
          return Token(error);
        }
      }
  }
}

struct Token Lexer::SeeNextToken() {
  int oldIndex = index_;
  bool oldIsLexingNumber = isLexingNumber_;
  Token nextToken = this->GetNextToken();
  index_ = oldIndex;
  isLexingNumber_ = oldIsLexingNumber;
  return nextToken;
}

