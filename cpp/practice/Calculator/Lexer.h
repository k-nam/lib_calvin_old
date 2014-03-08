#ifndef LEXER_H
#define LEXER_H

#include <vector>

using std::string;

namespace lib_calvin_desk_calculator
{
  enum Token_name {
    // unary operator is include in numbers
    // dollar is a special token to mark the end of input ('$')
    number, bin_op_plus, bin_op_minus, bin_op_multiply, bin_op_divide,
    paren_open, paren_close, dollar, error
  };

  struct Token {
    Token(enum Token_name name): name_(name), value_(0) { }
    Token(enum Token_name name, double value): name_(name), value_(value) { }
    enum Token_name name_;
    double value_;
  };

  // Lexer for desk calculator expression
  class Lexer {
  public:
    Lexer(string const &input): string_(input), index_(0), 
      isLexingNumber_(true) { }
    // Lex with the current index_ and return token (proceed)
    struct Token GetNextToken();
    // Restore index_ after showing next token
    struct Token SeeNextToken(); 
    // Initialize 
    void reset() { index_ = 0; isLexingNumber_ = true; } 

  private:
    void JumpBlanks(); // ignore blanks in the input
    double LexNumber(); // lex a number 

    string string_;
    int index_; // current lexing position
    bool isLexingNumber_; // true if a number is to come next
  };
} // end namespace lib_calvin_desk_calculator


#endif