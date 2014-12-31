#ifndef LIB_CALVIN__LEXER__LEXER_H
#define LIB_CALVIN__LEXER__LEXER_H

/* 2008-04-01
 * Implement a model for finite state machine, motivated by their use in
 * ..lexical analyzers. 
 * Main subjects: Nfa, Dfa, Nfa to Dfa conversion, state minimization
 * Regular expreesion parser is needed but it needs to wait.
 * 2008-04-10: Done Thompson's construction
 * 2008-04-14: Succeeded lexical analysis. Spent about 8 hours today with two
 * ..bugs. 
 * First, forgetting that objects call destructors for member fields 
 * ..and member points when they go out of scope. This caused mysterious object
 * ..destruction(Sentence and ParseTreeNode object), when they were inserted into 
 * ..vector.Since inserting into a vector calles the copy constructor 
 * ..(not inserting the object itself!!), and temporary object gets out of scope 
 * ..after it is inserted, all nodes are destructed the moment the Sentence 
 * ..object is inserted into the vector!!!
 * Second, and more disgusting one was that I INCLUDED NULL VALUE in NotOf node 
 * ..construction. This is so foolish a mistake that I wanted to cry. In the 
 * ..first place, it was wrong to combine epsilon edges and normal edges in one
 * ..table. Bad design leads to worse implementation, and to worst bugs. 
 * 2008-05-16: Trying to rename the classes. Getting the names of things
 * ..seems to be one of the most important and difficult part of programming. 
 */

#include <iostream>
#include <memory>
#include <set>
#include <map>
#include <vector>
#include "boost/container/vector.hpp"
#include "boost/container/map.hpp"
#include "boost/container/set.hpp"
#include "vector.h"
#include "set.h"
#include "map.h"
#include "abstract_string.h"
#include "utility.h"
#include "array.h"

namespace lib_calvin_lexer {
using lib_calvin::vector;
using lib_calvin::set;
//using lib_calvin::set;
using lib_calvin::map;
using std::shared_ptr;
using std::cout;
using std::endl;
using std::cin;
using lib_calvin::abstract_string;


// Regular language: actual language (but again abstract class)
// Every language should have its own set of production rules and
// ..corresponding parse tree node types
template <typename Char = char>
class RegularExpression {
  public:
    virtual ~RegularExpression() { };
    // I wanted to make some good inheritance, but this is not that useful
    typedef Char CharType; // char type actually
    
    enum class RegExpProduction { 
      Cat, // Concatenation
      Or,  // Construct |
      Star, // Construct *
      Plus, // Construct +
      OneOrZero,   //  Construct ?
      Literal, // Construct " "
      OneOf, // Construct [ ]
      NotOf // Construct [^ ]
    };
    struct ParseTreeNode {
      ParseTreeNode(shared_ptr<ParseTreeNode>, shared_ptr<ParseTreeNode>);
      ParseTreeNode(shared_ptr<ParseTreeNode>);
      ParseTreeNode(abstract_string<Char> const &);
      ~ParseTreeNode();
      RegExpProduction production;
      abstract_string<Char> string_; // for leaf nodes
      //shared_ptr<ParseTreeNode> parent; 
      shared_ptr<ParseTreeNode> left; // used when there is only 1 child
      shared_ptr<ParseTreeNode> right;
    };
    typedef ParseTreeNode ParseTree;
  private:
    RegularExpression() { }; 
};

// A manipulator for NFAs for Thompson construction and subset construction
// Alphabet should be an alphabet class
template <typename Char = char>
class NfaLexerGenerator  {
  public:
    typedef Char CharType; // type of inputs
    typedef int Nfa; // each Nfa is index by integer(its initial state)
    typedef int State; // starts from 0; -1 indicates dead state
    typedef typename RegularExpression<Char>::ParseTreeNode ParseTreeNode;
    typedef typename RegularExpression<Char>::ParseTree ParseTree;
    struct Token { // dummy struct for storing a pair
      int key;
      int length;
    };

    NfaLexerGenerator();
    ~NfaLexerGenerator();
    void clear(); // clear all
    // Push a regular expression into this machine (not convert yet)
    // Ultimately, this function has to take just string argument and
    // ..parse that by itself.
		void addRegularExpression(int key, std::shared_ptr<ParseTree const> regex);
    void buildAll(); // convert REs to one Nfa
    State getInitialState() const { return initialState_; }
    
    void getEpsilonClosure(set<State> const &curStates, 
        set<State> &nextStates) const;
    void getMove(set<State> const &curStates, CharType input, 
        set<State> &nextStates) const;
    // Match input string, and return corresponding Sentence
    void getMatch(abstract_string<Char> const &, int index, Token &) const;
    int getMatch(set<State> const &) const;
  private:
    State makeNewState(); // simply return increasing sequence of int
    void setEpsilonTrans (State curState, State nextState);
    void setTransition (State curState, CharType input, State nextState);
    void getNextStates(State state, CharType input, set<State> &) const;
    // Convert given RE parse tree to Nfa and returns the initial state
		Nfa convert(std::shared_ptr<ParseTreeNode const>);
    // Used by Dfa in subset construction; tell which RE matches the 
    // ..current set of states. Be careful of the order of REs.
    // Return -1 if no match (key of a Sentence is non-negative)
    
    // A wrapper for a regex. Caused me a lot of trouble with mem. mgmt
    // ..and destructor matter.
    class Sentence {
      public:
        Sentence(Sentence const &rhs);
				Sentence(int inKey, std::shared_ptr<ParseTree const>);
        int getKey() const { return key_; }
				std::shared_ptr<ParseTree const> getParseTree() const { return pParseTree_; }
      protected:
        int key_; // it is client's duty to make all keys unique
				std::shared_ptr<ParseTree const> pParseTree_; // points the root node
    };

    int const charSize_;
    int size_; // num of total states
    State initialState_; // ultimate start state for string matching
    // Maps each Nfa to its final state
    // Also plays the role of keeping the list of current NFAs.
    map<Nfa, State> nfaToFinalState_;
    // Maps each RE to its final state
    map<int, State> keyToFinalState_;
    // Recognize the longest lexeme; and regex that appears first in the
    // ...array is prioritized. 
    vector<Sentence *> pSentences_;
    // All states are represented by integers.
    // Epsilon edges are represented by 0. (produces damn good bug)
    vector<vector<set<State>>> transitionTable_;
};

// Deterministic version of Nfa is Dfa: conversion straightforward from Nfa 
template <typename Char = char>
class DfaLexerGenerator {
  public:
    typedef Char CharType;
    typedef int NfaState;
    typedef int DfaState; // set of Nfa states
    typedef typename NfaLexerGenerator<Char>::Token Token;
  
    DfaLexerGenerator(): charSize_(static_cast<int>(lib_calvin_string::getSizeOfCharSet<Char>())) { }
    void clear();
    void convertFrom(NfaLexerGenerator<Char> const &);
    // Same function as its Nfa, but (hopefully) much faster!
    void getMatch(abstract_string<Char> const &, int index, Token &) const;
  private:
    void setTransition(DfaState curState, CharType input, DfaState nextState);
    void getNextState(DfaState curState, CharType input, 
        DfaState &nextState) const;
    DfaState makeNewState();

    int const charSize_;
    DfaState initialState_;
    vector<vector<int>> transitionTable_; // indexed by Dfa states
    map<set<NfaState>, DfaState> nfaStatesToDFAState_; // mapping
    map<DfaState, int> dfaStateToKey_; // maps Sentence key for each state
};
} // end namesapce lib_calvin_lexer

namespace lib_calvin_lexer
{
/**** Global functions for convenient(?) regex building */

template <typename Char>
typename shared_ptr<typename RegularExpression<Char>::ParseTreeNode>
CAT(typename shared_ptr<typename RegularExpression<Char>::ParseTreeNode>, 
  typename shared_ptr<typename RegularExpression<Char>::ParseTreeNode>);

template <typename Char>
typename shared_ptr<typename RegularExpression<Char>::ParseTreeNode>
OR(typename shared_ptr<typename RegularExpression<Char>::ParseTreeNode>, 
  typename shared_ptr<typename RegularExpression<Char>::ParseTreeNode>);

template <typename Char>
typename shared_ptr<typename RegularExpression<Char>::ParseTreeNode>
STAR(typename shared_ptr<typename RegularExpression<Char>::ParseTreeNode>);

template <typename Char>
typename shared_ptr<typename RegularExpression<Char>::ParseTreeNode>
PLUS(typename shared_ptr<typename RegularExpression<Char>::ParseTreeNode>); 

template <typename Char>
typename shared_ptr<typename RegularExpression<Char>::ParseTreeNode>
ONEORZERO(typename shared_ptr<typename RegularExpression<Char>::ParseTreeNode>); 

template <typename Char>
typename shared_ptr<typename RegularExpression<Char>::ParseTreeNode>
LITERAL(abstract_string<Char> const &);

template <typename Char>
typename shared_ptr<typename RegularExpression<Char>::ParseTreeNode>
ONEOF(abstract_string<Char> const &);

template <typename Char>
typename shared_ptr<typename RegularExpression<Char>::ParseTreeNode>
NOTOF(abstract_string<Char> const &);

} // end namespace lib_calvin_lexer


/********************* RegularExpression<Char> definitions *****************/

template <typename Char>
lib_calvin_lexer::RegularExpression<Char>::ParseTreeNode::ParseTreeNode(
    shared_ptr<ParseTreeNode> inLeft, 
    shared_ptr<ParseTreeNode> inRight) {
  this->left = inLeft;
  this->right = inRight;
  //inLeft->parent = this;
  //inRight->parent = this;
}

template <typename Char>
lib_calvin_lexer::RegularExpression<Char>::ParseTreeNode::ParseTreeNode(shared_ptr<ParseTreeNode> inLeft) {
  this->left = inLeft;
  //inLeft->parent = this;
}

template <typename Char>
lib_calvin_lexer::RegularExpression<Char>::ParseTreeNode::ParseTreeNode(abstract_string<Char> const &instring) {
  this->string_ = instring;
}

// For automatic cleanup chain
template <typename Char>
lib_calvin_lexer::RegularExpression<Char>::ParseTreeNode::~ParseTreeNode() {
	/*
  switch (this->production) {
    case RegExpProduction::Cat:
      this->left->~ParseTreeNode();
      this->right->~ParseTreeNode();
      //cout << "Cat node destructed\n";
      return;
    case RegExpProduction::Or:
      this->left->~ParseTreeNode();
      this->right->~ParseTreeNode();
      //cout << "Or node destructed\n";
      return;
    case RegExpProduction::Star:
      this->left->~ParseTreeNode();
      //cout << "Star node destructed\n";
      return;
    case RegExpProduction::Plus:
      this->left->~ParseTreeNode();
      //cout << "Plus node destructed\n";
      return;
    case RegExpProduction::OneOrZero:
      this->left->~ParseTreeNode();
      //cout << "OneOrZero node destructed\n";
      return;
    case RegExpProduction::Literal:
      //cout << "Literal node destructed\n";
      return;
    case RegExpProduction::OneOf:
      //cout << "OneOf node destructed\n";
      return;
    case RegExpProduction::NotOf:
      //cout << "NotOf node destructed\n";
      return;
  }*/
}

/************* NfaLexerGenerator<Char>::Sentence definitions ***********/

template <typename Char>
lib_calvin_lexer::NfaLexerGenerator<Char>::Sentence::Sentence(
	int inKey, std::shared_ptr<ParseTree const> inTree):
  key_(inKey), pParseTree_(inTree) {
}

/************************ NfaLexerGenerator definitions ************************/

template <typename Char>
lib_calvin_lexer::NfaLexerGenerator<Char>::NfaLexerGenerator(): 
  charSize_(static_cast<int>(lib_calvin_string::getSizeOfCharSet<Char>())), size_(0) {
}

template <typename Char>
lib_calvin_lexer::NfaLexerGenerator<Char>::~NfaLexerGenerator() {
	for (int i = 0; i < static_cast<int>(pSentences_.size()); ++i) {
		delete pSentences_[i];
	}
}

template <typename Char>
void lib_calvin_lexer::NfaLexerGenerator<Char>::clear() {
  nfaToFinalState_.clear();
  keyToFinalState_.clear();
  pSentences_.clear();
  transitionTable_.clear();
  size_ = 0;
}

template <typename Char>
void lib_calvin_lexer::NfaLexerGenerator<Char>::addRegularExpression(
	int inKey, std::shared_ptr<ParseTree const> inNode) {
 
  Sentence *newSentence = new Sentence(inKey, inNode);
  pSentences_.push_back(newSentence);
}

template <typename Char>
void lib_calvin_lexer::NfaLexerGenerator<Char>::buildAll() {

  typename vector<Sentence *>::const_iterator iter;
  Nfa ultimateStart = makeNewState(); // make ultimate Nfa!
  for (iter = pSentences_.begin(); iter != pSentences_.end(); ++iter) {
    Nfa nfa = convert((*iter)->getParseTree());
    State finalState = nfaToFinalState_[nfa];
    keyToFinalState_.insert(std::pair<int, State>
        ((*iter)->getKey(), finalState));
    setEpsilonTrans(ultimateStart, nfa); // link all NFAs
  }
  initialState_ = ultimateStart;
}

// 2008-04-10: Wrestled with a bug in this function.
// ..what happens if curStates and nextStates are the same object?
// And actually that was the way I used this function!
// Need to consider 'aliasing' throughout my code again.
template <typename Char>
void lib_calvin_lexer::NfaLexerGenerator<Char>::getEpsilonClosure(set<State> const &curStates, 
    set<State> &nextStates) const {
  // epsilon edge is represented by null_char (hidden character)
  int null = static_cast<int>(lib_calvin_string::getNullChar<Char>());
  vector<bool> isMarked(size_, false); // marked: grey OR black vertex
  vector<State> stack;
  typename set<State>::const_iterator iter, iter2;
  for (iter = curStates.begin(); iter != curStates.end(); ++iter) {
    stack.push_back(*iter);
    isMarked[*iter] = true; // mark also grey vertices
  }
  // Using a simplified version of BFS here
  while (!stack.empty()) {
    State top = stack[stack.size() - 1];
    stack.pop_back();
    // push all unvisited adjacent States by epsilon edges
    for (iter2 = transitionTable_[top][null].begin(); 
        iter2 != transitionTable_[top][null].end(); ++iter2) {
      if (isMarked[*iter2] == false) { // if white vertex
        stack.push_back(*iter2);
        isMarked[*iter2] = true;
      }
    }
  }
  nextStates.clear();
  for (int i = 0; i < size_; ++i) {
    if (isMarked[i])
      nextStates.insert(i);
  }
}

template <typename Char>
void lib_calvin_lexer::NfaLexerGenerator<Char>::getMove (
    set<State> const &curStates, CharType input, set<State> &nextStates) const {
  set<State> result; // check for aliasing
  /** This is not a closure !! **/
  set<State>::const_iterator iter, iter2;
  int inChar = static_cast<int>(input);
  for (iter = curStates.begin(); iter != curStates.end(); ++iter) {
    set<State> const &next = transitionTable_[*iter][inChar];
    for (iter2 = next.begin(); iter2 != next.end(); ++iter2) {
      result.insert(*iter2);
    }
  }
  nextStates = result;
}

template <typename Char>
void lib_calvin_lexer::NfaLexerGenerator<Char>::getMatch(abstract_string<Char> const &instring, 
    int startIndex, Token &outToken) const {
  // Choice priority: 1. longest lexeme  2. first added regex
  set<State> curStates;
  int matchedKey = -1; // records the latest matched key
  int matchedLen = 0; // length of the matched lexeme
  curStates.insert(initialState_); // ultimate initial state of Nfa
  getEpsilonClosure(curStates, curStates); // be careful with aliasing
  int stringLen = instring.size();
  for (int i = startIndex; i < stringLen; ++i) {
    getMove(curStates, instring[i], curStates);
    if (curStates.empty()) // fell into dead state
      break;
    getEpsilonClosure(curStates, curStates);
    int temp = getMatch(curStates);
    if (temp != -1) { // matched a regex 
      matchedKey = temp;
      matchedLen = i - startIndex + 1;
    }
  }
  outToken.key = matchedKey; // -1 indicates no match
  outToken.length = matchedLen;
}

template <typename Char>
typename lib_calvin_lexer::NfaLexerGenerator<Char>::State 
lib_calvin_lexer::NfaLexerGenerator<Char>::makeNewState() {
  transitionTable_.resize(size_ + 1);  
  transitionTable_[size_] = vector<set<State>>(charSize_);
	//transitionTable_.push_back(vector<set<State>>(charSize_);
  return(size_++);
}

template <typename Char>
void lib_calvin_lexer::NfaLexerGenerator<Char>::setEpsilonTrans (
    State curState, State nextState) {
  int null = static_cast<int>(lib_calvin_string::getNullChar<Char>());
  transitionTable_[curState][null].insert(nextState);
}

template <typename Char>
void lib_calvin_lexer::NfaLexerGenerator<Char>::setTransition(State curState, CharType input, 
    State nextState) {
  if (input < 0) {
    cout << "setTransition: input is negative: "<< static_cast<int>(input) << "\n";
    exit(0);
  }
  if (transitionTable_[curState][static_cast<int>(input)].count(input) != 0) {
    cout << "setTransition: multiple edge error\n";
    exit(0);
  }
  transitionTable_[curState][static_cast<int>(input)].insert(nextState);
}

template <typename Char>
void lib_calvin_lexer::NfaLexerGenerator<Char>::getNextStates(State curState, CharType input, 
    set<State> &nextStates) const {
  nextStates = transitionTable_[curState][static_cast<int>(input)];
}

template <typename Char>
typename lib_calvin_lexer::NfaLexerGenerator<Char>::Nfa
lib_calvin_lexer::NfaLexerGenerator<Char>::convert(std::shared_ptr<ParseTreeNode const> node) {
  Nfa leftStart, rightStart, newStart;
  State leftFinal, rightFinal, newFinal;
  int len;
  // Almost the same with textbook method, I did not merge two states in the
  // ..CAT construction. Just adding an epsilon edge is more sensible. 
  switch (node->production) {
    case RegularExpression<Char>::RegExpProduction::Cat: 
      {
        //cout << "Converting CAT\n";
        leftStart = convert(node->left);
        leftFinal = nfaToFinalState_[leftStart];
        rightStart = convert(node->right);
        rightFinal = nfaToFinalState_[rightStart];
        setEpsilonTrans(leftFinal, rightStart);
        nfaToFinalState_.erase(leftStart);
        nfaToFinalState_.erase(rightStart);
        nfaToFinalState_[leftStart] = rightFinal;
        return leftStart;
      }
    case RegularExpression<Char>::RegExpProduction::Or:
      {
        //cout << "Converting OR\n";
        leftStart = convert(node->left);
        leftFinal = nfaToFinalState_[leftStart];
        rightStart = convert(node->right);
        rightFinal = nfaToFinalState_[rightStart];
        newStart = makeNewState();
        newFinal = makeNewState();
        setEpsilonTrans(newStart, leftStart);
        setEpsilonTrans(newStart, rightStart);
        setEpsilonTrans(leftFinal, newFinal);
        setEpsilonTrans(rightFinal, newFinal);
        nfaToFinalState_.erase(leftStart);
        nfaToFinalState_.erase(rightStart);
        nfaToFinalState_[newStart] = newFinal;
        return newStart;
      }
    case RegularExpression<Char>::RegExpProduction::Star:
      {
        //cout << "Converting STAR\n";
        leftStart = convert(node->left);
        leftFinal = nfaToFinalState_[leftStart];
        newStart = makeNewState();
        newFinal = makeNewState();
        setEpsilonTrans(newStart, leftStart);
        setEpsilonTrans(newStart, newFinal);
        setEpsilonTrans(leftFinal, newFinal);
        setEpsilonTrans(leftFinal, leftStart);
        nfaToFinalState_.erase(leftStart);
        nfaToFinalState_[newStart] = newFinal;
        return newStart;
      }
    // The same as CAT(A, A*)
    case RegularExpression<Char>::RegExpProduction::Plus:
      {
        //cout << "Converting PLUS\n";
        leftStart = convert(node->left);
        leftFinal = nfaToFinalState_[leftStart];
        // make another copy
        rightStart = convert(node->left);
        rightFinal = nfaToFinalState_[rightStart];
        newStart = makeNewState();
        Nfa newMiddle = makeNewState(); // start of A*
        newFinal = makeNewState();
        setEpsilonTrans(newStart, leftStart);
        setEpsilonTrans(leftFinal, newMiddle);
        setEpsilonTrans(newMiddle, rightStart);
        setEpsilonTrans(newMiddle, newFinal);
        setEpsilonTrans(rightFinal, newFinal);
        setEpsilonTrans(rightFinal, rightStart);
        nfaToFinalState_.erase(leftStart);
        nfaToFinalState_.erase(rightStart);
        nfaToFinalState_[newStart] = newFinal;
        return newStart;
      }
    case RegularExpression<Char>::RegExpProduction::OneOrZero:
      {
        //cout << "Converting ONEORZERO\n";
        leftStart = convert(node->left);
        leftFinal = nfaToFinalState_[leftStart];
        newStart = makeNewState();
        newFinal = makeNewState();
        setEpsilonTrans(newStart, leftStart);
        setEpsilonTrans(newStart, newFinal);
        setEpsilonTrans(leftFinal, newFinal);
        nfaToFinalState_.erase(leftStart);
        nfaToFinalState_[newStart] = newFinal;
        return newStart;
      }
    case RegularExpression<Char>::RegExpProduction::Literal: // consecutive CAT
      {
        //cout << "Converting LITERAL\n";
        len = node->string_.size();
        vector<State> temp(len + 1);
        temp[0] = makeNewState();
        for (int i = 0; i < len; ++i) {
          temp[i + 1] = makeNewState();
          setTransition(temp[i], node->string_[i], temp[i + 1]);
        }
        nfaToFinalState_[temp[0]] = temp[len];
        return temp[0];
      }
    case RegularExpression<Char>::RegExpProduction::OneOf:
      {
        //cout << "Converting ONEOF\n";
        newStart = makeNewState();
        newFinal = makeNewState();
        len = node->string_.size();
        for (int i = 0; i < len; ++i) {
          setTransition(newStart, node->string_[i], newFinal);
        }
        nfaToFinalState_[newStart] = newFinal;
        return newStart;
      }
    case RegularExpression<Char>::RegExpProduction::NotOf:
      {
        int null = static_cast<int>(lib_calvin_string::getNullChar<Char>());
        //cout << "Converting NOTOF. charsize is: " << charSize_ << endl;
        newStart = makeNewState();
        newFinal = makeNewState();
        len = node->string_.size();
        // I started with i = 0, adding an epsilong edge !!! oTL
        for (int i = 0; i < charSize_; ++i) { 
          if (i != null) // don't touch null character
            setTransition(newStart, i, newFinal);
        }
        for (int i = 0; i < len; ++i) { // delete edges
          transitionTable_[newStart][node->string_[i]].clear(); 
        }
        nfaToFinalState_[newStart] = newFinal;
        return newStart;
      }
    default: // can not happen
      cout << "Nfa convert function: detected non-existent RegularExpression\
        production.\n";
      exit(0);
  }
}

// Returns -1 if no match
template <typename Char>
int lib_calvin_lexer::NfaLexerGenerator<Char>::getMatch(set<State> const &curStates) const {
  // Scan starts from the highest priority sentences
  for (unsigned i = 0; i < pSentences_.size(); ++i) {
    int key = pSentences_[i]->getKey();
    State finalState = (keyToFinalState_.find(key))->second;
    if (curStates.count(finalState) > 0)
      return key;
  }
  return -1;
}

/************************ DfaLexerGenerator definitnios *********************/

template <typename Char>
void lib_calvin_lexer::DfaLexerGenerator<Char>::clear() {
  transitionTable_.clear();
  nfaStatesToDFAState_.clear(); 
  dfaStateToKey_.clear();
}

template <typename Char>
void lib_calvin_lexer::DfaLexerGenerator<Char>::convertFrom (
    NfaLexerGenerator<Char> const &nfa) {

  // Subset construction
  DfaState curDFAState, nextDFAState;
  vector<set<NfaState>> stack;
  set<NfaState> curStates, nextStates;
  int key;
  // Get initial state set
  curStates.insert(nfa.getInitialState());
  nfa.getEpsilonClosure(curStates, nextStates);
  // Store and push initial state set
  nextDFAState = makeNewState();
  initialState_ = nextDFAState; // set initial state of this entire Dfa
  nfaStatesToDFAState_[nextStates] = nextDFAState;
  key = nfa.getMatch(nextStates);
  dfaStateToKey_[nextDFAState] = key;
  stack.push_back(nextStates);
  while (!stack.empty()) {
    curStates = stack[stack.size() - 1];
    stack.pop_back();
    curDFAState = nfaStatesToDFAState_[curStates]; // source vertex
    for (int i = 1; i < charSize_; ++i) { // check every outward edge
      nfa.getMove(curStates, i, nextStates);
      nfa.getEpsilonClosure(nextStates, nextStates);
      // As transitionTable_ is initialized to -1 (dead state), no action
      if (nextStates.empty()) 
        continue; 
      if (nfaStatesToDFAState_.count(nextStates) == 0) { // newly found
        // Store and push new state set
        nextDFAState = makeNewState(); // make correponding Dfa state
        nfaStatesToDFAState_[nextStates] = nextDFAState;
        key = nfa.getMatch(nextStates);
        dfaStateToKey_[nextDFAState] = key;
        stack.push_back(nextStates);
      } else { // already existing
        nextDFAState = nfaStatesToDFAState_[nextStates];
				//std::cout << "size1 is " << nextStates.size() << " size2 is " << nfaStatesToDFAState_.size() << "\n";
      }
      // Make transition edge
      setTransition(curDFAState, i, nextDFAState);
    }
  }
  cout << "Total Dfa states num is : " << transitionTable_.size() << endl;
}

template <typename Char>
void lib_calvin_lexer::DfaLexerGenerator<Char>::getMatch (abstract_string<Char> const &instring, 
    int startIndex, Token &outToken) const {
  
  DfaState curState = initialState_; // start at initial state
  int matchedKey = -1; // records the latest matched key
  int matchedLen = 1; // length of the matched lexeme
  int stringLen = instring.size();
  for (int i = startIndex; i < stringLen; ++i) {
    getNextState(curState, instring[i], curState);
    if (curState == -1) { // fell into dead state
      break;
    }
    else { // moved into another state
      if (dfaStateToKey_.count(curState) == 0) {
        cout << "Dfa getMatch error\n";
				exit(0);
			}
      matchedKey = dfaStateToKey_.find(curState)->second;
      matchedLen = i - startIndex + 1;
    }
  }
  outToken.key = matchedKey; // -1 indicates no match
  outToken.length = matchedLen;
}

template <typename Char>
void lib_calvin_lexer::DfaLexerGenerator<Char>::setTransition(DfaState curState, CharType input, 
    DfaState nextState){
  //cout << "Dfa: Seting edge " << curState << " " << input << " " << 
    //nextState << endl;
  transitionTable_[curState][static_cast<int>(input)] = nextState;
}

template <typename Char>
void lib_calvin_lexer::DfaLexerGenerator<Char>::getNextState(DfaState curState, CharType input,
    DfaState &nextState) const {
  nextState = transitionTable_[curState][static_cast<int>(input)];
}

template <typename Char>
typename lib_calvin_lexer::DfaLexerGenerator<Char>::DfaState 
lib_calvin_lexer::DfaLexerGenerator<Char>::makeNewState() {
  // Initialize values to -1, which means dead state
  transitionTable_.push_back(vector<int>(charSize_, -1));
  return static_cast<int>(transitionTable_.size()) - 1;
}

/************************ Global functions ***********************/

template <typename Char>
std::shared_ptr<typename lib_calvin_lexer::RegularExpression<Char>::ParseTreeNode>
lib_calvin_lexer::CAT(shared_ptr<typename lib_calvin_lexer::RegularExpression<Char>::ParseTreeNode> left, 
    shared_ptr<typename lib_calvin_lexer::RegularExpression<Char>::ParseTreeNode> right) {
  
  typedef typename RegularExpression<Char>::ParseTreeNode ParseTreeNode;
  shared_ptr<ParseTreeNode> pParseNode(new ParseTreeNode(left, right));
  pParseNode->production = RegularExpression<Char>::RegExpProduction::Cat;
  return pParseNode;
}

template <typename Char>
std::shared_ptr<typename lib_calvin_lexer::RegularExpression<Char>::ParseTreeNode>
lib_calvin_lexer::OR(shared_ptr<typename RegularExpression<Char>::ParseTreeNode> left, 
      shared_ptr<typename RegularExpression<Char>::ParseTreeNode> right) {

  typedef typename RegularExpression<Char>::ParseTreeNode ParseTreeNode;
  shared_ptr<ParseTreeNode> pParseNode(new ParseTreeNode(left, right));
  pParseNode->production = RegularExpression<Char>::RegExpProduction::Or;
  return pParseNode;
}

template <typename Char>
std::shared_ptr<typename lib_calvin_lexer::RegularExpression<Char>::ParseTreeNode>
lib_calvin_lexer::STAR(shared_ptr<typename RegularExpression<Char>::ParseTreeNode> left) {

  typedef typename RegularExpression<Char>::ParseTreeNode ParseTreeNode;
  shared_ptr<ParseTreeNode> pParseNode(new ParseTreeNode(left));
  pParseNode->production = RegularExpression<Char>::RegExpProduction::Star;
  return pParseNode;
}

template <typename Char>
std::shared_ptr<typename lib_calvin_lexer::RegularExpression<Char>::ParseTreeNode>
lib_calvin_lexer::PLUS(shared_ptr<typename RegularExpression<Char>::ParseTreeNode> left) {

  typedef typename RegularExpression<Char>::ParseTreeNode ParseTreeNode;
  shared_ptr<ParseTreeNode> pParseNode(new ParseTreeNode(left));
  pParseNode->production = RegularExpression<Char>::RegExpProduction::Plus;
  return pParseNode;
}

template <typename Char>
std::shared_ptr<typename lib_calvin_lexer::RegularExpression<Char>::ParseTreeNode>
lib_calvin_lexer::ONEORZERO(shared_ptr<typename RegularExpression<Char>::ParseTreeNode> left) {

  typedef typename RegularExpression<Char>::ParseTreeNode ParseTreeNode;
  shared_ptr<ParseTreeNode> pParseNode(new ParseTreeNode(left));
  pParseNode->production = RegularExpression<Char>::RegExpProduction::OneOrZero;
  return pParseNode;
}

template <typename Char>
std::shared_ptr<typename lib_calvin_lexer::RegularExpression<Char>::ParseTreeNode>
lib_calvin_lexer::LITERAL(abstract_string<Char> const &instring) {
  
  typedef typename RegularExpression<Char>::ParseTreeNode ParseTreeNode;
  shared_ptr<ParseTreeNode> pParseNode(new ParseTreeNode(instring));
  pParseNode->production = RegularExpression<Char>::RegExpProduction::Literal;
  return pParseNode;
}

template <typename Char>
std::shared_ptr<typename lib_calvin_lexer::RegularExpression<Char>::ParseTreeNode>
lib_calvin_lexer::ONEOF(abstract_string<Char> const &instring) {

  typedef typename RegularExpression<Char>::ParseTreeNode ParseTreeNode;
  shared_ptr<ParseTreeNode> pParseNode(new ParseTreeNode(instring));
  pParseNode->production = RegularExpression<Char>::RegExpProduction::OneOf;
  return pParseNode;
}

template <typename Char>
std::shared_ptr<typename lib_calvin_lexer::RegularExpression<Char>::ParseTreeNode>
lib_calvin_lexer::NOTOF(abstract_string<Char> const &instring) {

  typedef typename RegularExpression<Char>::ParseTreeNode ParseTreeNode;
  shared_ptr<ParseTreeNode> pParseNode(new ParseTreeNode(instring));
  pParseNode->production = RegularExpression<Char>::RegExpProduction::NotOf;
  return pParseNode;
}

#endif
