#ifndef LIB_CALVIN__SUBCC__SYMBOL_TABLE_H
#define LIB_CALVIN__SUBCC__SYMBOL_TABLE_H

#include "abstract_string.h"
#include "subcc_lexer.h" // to use my own string implementation
#include "type.h"
#include <iostream>
#include <stdio.h>
#include <memory>

using std::cout;
using std::shared_ptr;

// For compilers homework
namespace subcc {
extern int symtabCount;
extern int symbolCount;
using lib_calvin::string;
int const SYMBOL_VAR_MAX = 10; // below this are variables
int const SYMBOL_TEMP_MAX = 20; // belw this are temporaries
int const SYMBOL_CONST_MAX = 40; // below this are constants

enum SymbolKinds {
  SYMBOL_VAR_G              = 1, // global variable
  SYMBOL_VAR_L              = 2, // local variable
  SYMBOL_ELEM               = 5, // member of a struct
  SYMBOL_PARAMETER          = 6, // formal parameter of a function
  
  SYMBOL_TEMPORARY          = 11, // compiler-generated temporaries
  
  // All constants are stored in global scope
  SYMBOL_CONSTANT_CHAR      = 31,
  SYMBOL_CONSTANT_SHORT     = 32,
  SYMBOL_CONSTANT_INT       = 33,
  SYMBOL_CONSTANT_FLOAT     = 34,
  SYMBOL_CONSTANT_DOUBLE    = 35,
  SYMBOL_CONSTANT_STRING    = 39,
  
  SYMBOL_TYPENAME           = 61, // typedef name
  SYMBOL_STRUCT             = 71, // struct name
  SYMBOL_FUNC               = 81, // fuction name

	SYMBOL_ERROR							= 99
};


class Symbol {
public:
  Symbol(enum SymbolKinds symbolKind, string const &lexeme, shared_ptr<Type const> type);
	virtual ~Symbol();
  string const &getLexeme() const { return lexeme_; }
  shared_ptr<Type const> getType() const { return type_; }
  enum SymbolKinds getSymbolKind() const { return symbolKind_; }
  // Returns whether the symbol occupies space on stack or not
  // Type names (func, struct, typedef) and constants are not variables
  bool isVariable() const;
  bool isTemporary() const;
  bool isConstant() const;
  void setOffset(int offset) { offset_ = offset; }
  int getOffset() const { return offset_; }
	//virtual void print() { std::cout << "base symbol\n"; }
public:
	static void countObjects();
protected:
  enum SymbolKinds symbolKind_;
  string lexeme_; // also becomes constant value for string constants
private:
  shared_ptr<Type const> type_; // type of the object the symbol refers
  int offset_; // offset in the stack frame 
private:
	static intmax_t objectCount_;
};

class Constant: public Symbol {
public:
  Constant(char charValue);
  Constant(short shortValue);
  Constant(int intValue);
  Constant(string const &lexeme); // string constant
	~Constant();
  char getCharValue() const { return constant_.charValue_; }
  short getShortValue() const { return constant_.shortValue_; }
  int getIntValue() const { return constant_.intValue_; }
	//virtual void print() { std::cout << "constant symbol\n"; }
private:
  union { // only for constant
    char charValue_;
    short shortValue_;
    int intValue_;
  } constant_;
};

class Temporary: public Symbol {
public:
  Temporary(string const &lexeme, shared_ptr<Type const> type): 
    Symbol(SYMBOL_TEMPORARY, lexeme, type) { }
	~Temporary();
	//virtual void print() { std::cout << "temmp symbol\n"; }
};

class GlobalSymbolTable;

// Node of the symbol table tree
// Revised to support offsets of each symbols, using type width information.
class SymbolTable {
public:
  // Using pointer to distinguish with copy constructor!
  // Global scope table: parent = *this
  // Inheriting parent's offset (not for type declarations like struct or
  // ..function declarations!)
  SymbolTable(GlobalSymbolTable const &globalTable, SymbolTable *parent, 
      bool isEnvironment);
  // Special constructor for global scope table
  SymbolTable(GlobalSymbolTable const &globalTable);
	~SymbolTable();    
	// Returns index if declared, -1 if not
  int findIndex(string const &) const;
  // Look only into this symbol table(not ancestors)
  shared_ptr<Type const> getTypeOf(string const &field) const;
  // Assuming this symbol has not been declared
  void insert(shared_ptr<Symbol>, int index);
  int getOffset() const { return offset_; }
  // For type declarations, offset should be 0
  void setOffsetToZero() { offset_ = 0; } 
  // Used for record type
  //int getTotalWidth() const { return totalWidth_; }
  SymbolTable *getParent() { return parent_; }
  SymbolTable const &getYoungestChild() { return *children_.back(); }
  SymbolTable const &makeChild(bool isEnvironment); // make another child
  bool isEnvironment() const { return isEnvironment_; }
public:
	static void countObjects();
private:
  // Integer value is the index to the global table
  map<string, int> mapping_;
  GlobalSymbolTable const &globalTable_;
  SymbolTable *parent_;
  vector<SymbolTable const *> children_;
  int offset_; // current offset (in the stack frame of current function) 
  bool const isEnvironment_; // true if this is an env of record declaration
private:
	static intmax_t objectCount_;
};

class GlobalSymbolTable {
public:
  GlobalSymbolTable();
	~GlobalSymbolTable();
  void pushTable(); // entering into nested scope: '{': DO NOT use for env
  void pushEnvironment(); // entering into a record declaration
  void pushFunction(); // entering into a function definition(before params)
  bool popTable(); // escape from nested scope: '}'
  // Find out whether we are in global scope or not
  bool isInGlobal() { return root_ == curTable_; }
  int findIndex(string const &lexeme) const; // returns negative when faulty
  shared_ptr<Symbol const> lookUp(int index) const;
  // Prepared for record type declarations: returns finished symbol table
  SymbolTable const &getEnvironment();
  // Whether we are in record declaration or not
  bool isInEnvironment() const { return curTable_->isEnvironment(); }
  // Returns the index of the new entry; -1 when faulty
  // Inserting is concerned with only the topmost table for ordinary symbols
  // Constant symbols are always inserted to global scope, and it allows 
  // ..multiple insertion with same lexeme.
  // If the symbol is a record, it should be inserted to closest
  // 'non-environment' table! (do not support nested type declaration)
  int insert(shared_ptr<Symbol>);
  // Reset the offset of current symbol table (used in functions definition)
  void setOffsetToZero() const { curTable_->setOffsetToZero(); }
  int getOffset() const { return curTable_->getOffset(); }
  // Return true if new size is the maximum, false otherwise
  bool updateMaxStackSize(int newSize);
  int getMaxStackSize() const { return maxStackSize_; }
public:
	static void countObjects();
private:
  int setEntry(shared_ptr<Symbol const>); // make new entry and return its index
  SymbolTable *root_;
  SymbolTable *curTable_; // currently operating table
  // Global symbol table
  vector<shared_ptr<Symbol const>> array_;
  int maxStackSize_; // maximum stack size of the current function definition
private:
	static intmax_t objectCount_;
};
} // end namespace




#endif
