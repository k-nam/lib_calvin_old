#include "syntax_tree.h"

namespace subcc
{
int syntaxTreeNodeCount = 0;

//------------------ SyntaxTree methods -------------------

SyntaxTree::~SyntaxTree() {
	delete root_; 
}

/************* SyntaxTreeNode methods *************/

SyntaxTreeNode::SyntaxTreeNode() { 
	++syntaxTreeNodeCount;
	//std::cout << "SyntaxTreeNode created\n";
}

SyntaxTreeNode::~SyntaxTreeNode() { 
	syntaxTreeNodeCount--;
	//std::cout << "SyntaxTreeNode destroyed: " << syntaxTreeNodeCount << "\n"; 
	if (this->isNonTerminal()) {
		//std::cout << "Nonterminal destroyed\n";
	} else {
		//std::cout << "Terminal destroyed\n";
	}
}

/************ NonTerminalNode methods *************/

NonTerminalNode::~NonTerminalNode() {
	
	//std::cout << "NonterminalNode deleted key: " << productionKey_ <<
		//	"size: " << children_.size() << "\n";
	int childToDelete = 0;
	// Catch "x += y" type expressions
	if (productionKey_ == 191 && getNumChildren() == 2 &&
				getChild(1)->isNonTerminal() == true) { // "x = NonTerminal" type
		NonTerminalNode const *rhsExprNode = 
			dynamic_cast<NonTerminalNode const *>(getChild(1));
		if (rhsExprNode->getProductionKey() >= 281 &&
				rhsExprNode->getProductionKey() <= 288 && // "x = a binOp b" type
				(getChild(0) == rhsExprNode->getChild(0))) { // "x = x binOp y" type
			// Skip first child in deleting to avoid double delete call
			childToDelete = 1;
			//std::cout << "----avoiding double delete!\n";
		}
	}
	for (; childToDelete < children_.size(); childToDelete++) {
		//std::cout << "Nonternalnode deleting children\n";
		delete children_[childToDelete];
	}
}

void NonTerminalNode::changeProductionKey(int newKey) {
  int &productionKey = const_cast<int &> (productionKey_);
  productionKey = newKey;
}

/*************** FunctionNode methods ****************/


/************** ParamListNode methods ***************/

void ParamListNode::pushParamType(shared_ptr<Type const> paramType) {
  typeVector_.push_back(paramType);
}

/****************** ExprNode methods ***************/

ExprNode::ExprNode(int key, shared_ptr<Type const> type, bool isConstant):
  TypedNode(key, type), isAssignable_(false), isConstant_(isConstant),
  rvalueIndex_(-1) { }

ExprNode::ExprNode(int key, shared_ptr<Type const> type, bool isAssignable, 
    bool isConstant):
  TypedNode(key, type), isAssignable_(isAssignable), 
  isConstant_(isConstant), rvalueIndex_(-1) { }

/****************** LvalueNode methods ***************/

LvalueNode::LvalueNode(int key, shared_ptr<Type const> type):
  ExprNode(key, type, true, false), 
  baseIndex_(-1), indexIndex_(-1), tempIndex_(-1)  { }

} // end namespace subcc