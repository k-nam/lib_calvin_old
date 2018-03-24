#ifndef LIB_CALVIN__CONTAINER_RB_TREE_H
#define LIB_CALVIN__CONTAINER_RB_TREE_H

#include "bin_tree.h"

namespace lib_calvin_container
{

enum class RbColor { Black, Red };

template <typename T>
class RbTreeNode: public BinTreeNode<T> {
public:

	// Node is created with its color set to red
	RbTreeNode(T const &key);
	RbTreeNode(T &&key);
	RbTreeNode(T const &key, RbColor color);
	~RbTreeNode();
	RbColor getColor() const { return color_; }
	void setColor(RbColor newColor) { color_ = newColor; }	
	RbTreeNode<T> *getParent() const { 
		return static_cast<RbTreeNode<T> *>(BinTreeNode<T>::getParent()); }
	RbTreeNode<T> *getLeftChild() const { 
		return static_cast<RbTreeNode<T> *>(BinTreeNode<T>::getLeftChild()); }
	RbTreeNode<T> *getRightChild() const { 
		return static_cast<RbTreeNode<T> *>(BinTreeNode<T>::getRightChild()); }
	RbTreeNode<T> *getChild(Direction direction) const { 
		return static_cast<RbTreeNode<T> *>(BinTreeNode<T>::getChild(direction)); }
private:
	RbColor color_;
};

template <typename T, typename K = T, typename Comp = std::less<K>, typename ExtractKey = Identity<T>>
class RbTree: public BinTree<T, K, Comp, ExtractKey> {
public:
	RbTree<T, K, Comp, ExtractKey>(): BinTree<T, K, Comp, ExtractKey>() { }
	RbTree<T, K, Comp, ExtractKey>(RbTree<T, K, Comp, ExtractKey> const &rhs): 
		BinTree<T, K, Comp, ExtractKey>() {
			copyFrom(rhs);
			//std::cout << "copy con: RbTree\n";
	}
	RbTree<T, K, Comp, ExtractKey>(RbTree<T, K, Comp, ExtractKey> &&rhs): 
		BinTree<T, K, Comp, ExtractKey>() {
			//std::cout << "rb move ctor\n";
		swap(rhs);
	}
	RbTree &operator=(RbTree const &rhs) { return static_cast<RbTree &>(BinTree::operator=(rhs)); }
	RbTree &operator=(RbTree &&rhs) { return static_cast<RbTree &>(BinTree::operator=(std::forward<RbTree>(rhs))); }
	~RbTree() { }
	typedef BinTree<T, K, Comp, ExtractKey>::iterator iterator;
	std::pair<iterator, bool> insert(T const &);
	std::pair<iterator, bool> insert(T &&);
	template <typename InputIterator>
		void insert(InputIterator beg, InputIterator end);
	size_t erase(K const &);
	
private: 
	using BinTree<T, K, Comp, ExtractKey>::root_;
	using BinTree<T, K, Comp, ExtractKey>::empty;

	template <typename T1>
		std::pair<typename BinTree<T, K, Comp, ExtractKey>::iterator, bool> insert_(T1 &&elem);
	BinTreeNode<T> *makeNewNode(T const &elem) const override;
	BinTreeNode<T> *makeNewNode(T &&elem) const override;
	BinTreeNode<T> *makeNewNode(BinTreeNode<T> const *rhs) const override;
	RbTreeNode<T> *rotate(RbTreeNode<T> *node, Direction direction);
	RbTreeNode<T> *rbRotate(RbTreeNode<T> *node, Direction direction );

	void rbInsertFix(RbTreeNode<T> *);
	void rbDeleteFix(RbTreeNode<T> *);

	// For debugging
	size_t RbTreeCheck(RbTreeNode<T> const *) const;
};

//-------------------------- RbTreeNode public methods --------------------------//

template <typename T>
RbTreeNode<T>::RbTreeNode(T const &key): BinTreeNode<T>(key), color_(RbColor::Red) { }

template <typename T>
RbTreeNode<T>::RbTreeNode(T &&key): BinTreeNode<T>(std::forward<T>(key)), color_(RbColor::Red) { }

template <typename T>
RbTreeNode<T>::RbTreeNode(T const &key, RbColor color): BinTreeNode<T>(key), color_(color) { }

template <typename T>
RbTreeNode<T>::~RbTreeNode() { 
	// std::cout << "rbnode dtored\n";
}


//-------------------------- RbTree public methods --------------------------//

template <typename T, typename K, typename Comp, typename ExtractKey>
std::pair<typename RbTree<T, K, Comp, ExtractKey>::iterator, bool> 
RbTree<T, K, Comp, ExtractKey>::insert(T const &elem) {
	return insert_(elem);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
std::pair<typename RbTree<T, K, Comp, ExtractKey>::iterator, bool> 
RbTree<T, K, Comp, ExtractKey>::insert(T &&elem) {
	return insert_(std::forward<T>(elem));
}

template <typename T, typename K, typename Comp, typename ExtractKey>
template <typename T1>
std::pair<typename BinTree<T, K, Comp, ExtractKey>::iterator, bool> 
RbTree<T, K, Comp, ExtractKey>::insert_(T1 &&elem) {
	//std::cout << "rb insert called\n";
	std::pair<BinTreeNode<T> *, bool> result = 
		BinTree<T, K, Comp, ExtractKey>::insert_impl(std::forward<T1>(elem));
	BinTreeNode<T> *insertedNode = result.first;
	bool isInserted = result.second;
	// RbTree maintenance
	if (isInserted == true) {
		rbInsertFix(static_cast<RbTreeNode<T> *>(insertedNode));
	}
	//RbTreeCheck(static_cast<RbTreeNode<T> *>(root_));
	return std::pair<iterator, bool>(iterator(insertedNode), isInserted);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
size_t RbTree<T, K, Comp, ExtractKey>::erase(K const &elem) {
	RbTreeNode<T> *deleted = static_cast<RbTreeNode<T> *>(
		BinTree<T, K, Comp, ExtractKey>::erase_impl(elem));
	if (deleted == NULL) { // not deleted anything
		return 0;
	} 
	if (deleted->getColor() == RbColor::Red) { // deleted a red node: OK
		delete deleted;
		return 1;
	} 
	if (empty() == true) { // nothing is left
		delete deleted;
		return 1;
	}
	// deleted node acts as the NIL node (which is black) 
	if (deleted->getLeftChild() == NULL && deleted->getRightChild() == NULL) { 
		rbDeleteFix(deleted);
	} else if (deleted->getLeftChild() != NULL) {
		rbDeleteFix(deleted->getLeftChild());
	} else {
		rbDeleteFix(deleted->getRightChild());
	}
	//RbTreeCheck(static_cast<RbTreeNode<T> *>(root_));
	delete deleted;
	return 1;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
template <typename InputIterator>
void RbTree<T, K, Comp, ExtractKey>::insert(InputIterator beg, InputIterator end)
{
	while (beg != end) {
		this->insert(*beg);
		++beg;
	}
}

//--------------------------- RbTree private methods --------------------------//

template <typename T, typename K, typename Comp, typename ExtractKey>
BinTreeNode<T> *
RbTree<T, K, Comp, ExtractKey>::makeNewNode(T const &key) const { 
	return new RbTreeNode<T>(key); 
}

template <typename T, typename K, typename Comp, typename ExtractKey>
BinTreeNode<T> *
RbTree<T, K, Comp, ExtractKey>::makeNewNode(T &&key) const { 
	return new RbTreeNode<T>(std::move(key)); 
}

template <typename T, typename K, typename Comp, typename ExtractKey>
BinTreeNode<T> *
RbTree<T, K, Comp, ExtractKey>::makeNewNode(BinTreeNode<T> const *rhs) const {
	return new RbTreeNode<T>(rhs->getKey(), 
		static_cast<RbTreeNode<T> const *>(rhs)->getColor());
}

template <typename T, typename K, typename Comp, typename ExtractKey>
RbTreeNode<T> * 
RbTree<T, K, Comp, ExtractKey>::rotate(RbTreeNode<T> *node, Direction direction) {
	Direction oppositeDirection = BinTreeNode<T>::getOpposite(direction);
	RbTreeNode<T> *child = node->getChild(oppositeDirection);
	if (child == NULL) {
		std::cout << "rotate error: child null\n";
		exit(0);
	}
	/* save link */
	RbTreeNode<T> *grandChild = child->getChild(direction);
	/* change root */
	if (node->isLeftInParent()) {
		node->getParent()->setLeftChild(child);
	} else {
		node->getParent()->setRightChild(child);
	}
	child->setParent(node->getParent());
	/* modify local tree */
	node->setParent(child); 
	child->setChild(node, direction); 
	node->setChild(grandChild, oppositeDirection); 
	if (grandChild != NULL) { 
		grandChild->setParent(node); 
	} 
	/* resetting global root */
	if (root_ == node) { 
		root_ = child; 
	} 
	return child;	
}

template <typename T, typename K, typename Comp, typename ExtractKey>
RbTreeNode<T> * 
RbTree<T, K, Comp, ExtractKey>::rbRotate(RbTreeNode<T> *node, Direction direction) {
	Direction oppositeDirection = BinTreeNode<T>::getOpposite(direction);
	if (node->getColor() == RbColor::Black && node->getChild(oppositeDirection) != NULL &&
		node->getChild(oppositeDirection)->getColor() == RbColor::Red &&
		(node->getChild(direction) == NULL || 
						node->getChild(direction)->getColor() == RbColor::Black)) { // OK...
	} else { // precondition not satisfied!
		std::cout << "rbRotateLeft pre error\n";
		exit(0);
	}
	node->setColor(RbColor::Red);
	node->getChild(oppositeDirection)->setColor(RbColor::Black);
	return rotate(node, direction);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void RbTree<T, K, Comp, ExtractKey>::rbInsertFix(RbTreeNode<T> *node) {
	// We are to make 'node' as black
	if (node->getColor() != RbColor::Red) { // job should have been finished before
		std::cout << "rb insert fix error: black node\n";
		exit(0);
	}
	if (node == root_) { // red node should be colored black; job done
		node->setColor(RbColor::Black);
		//std::cout << "setting root black\n";
		return;
	}
	if (node->getParent()->getColor() == RbColor::Black) { // if parent is black, job done
		return;
	}
	RbTreeNode<T> *parentNode = node->getParent();
	RbTreeNode<T> *grandPaNode = parentNode->getParent();
	RbTreeNode<T> *uncleNode = NULL;
	if (grandPaNode == NULL) {
		std::cout << "grandPa null\n";
		exit(0);
	}
	Direction direction = Direction::Left;
	Direction oppositeDirection = Direction::Right;
	if (parentNode == grandPaNode->getLeftChild()) {
	} else {
		direction = Direction::Right;
		oppositeDirection = Direction::Left;
	}
	uncleNode = grandPaNode->getChild(oppositeDirection);
	if (uncleNode != NULL && 
				uncleNode->getColor() == RbColor::Red) { /* both parent and uncle are red */ 
		parentNode->setColor(RbColor::Black); 
		uncleNode->setColor(RbColor::Black); 
		grandPaNode->setColor(RbColor::Red); 
		rbInsertFix(grandPaNode); /* recursive call */
		return; 
	} 
	/* make suitable for red-black rotation */ 
	if (node == parentNode->getChild(oppositeDirection)) {
		node = parentNode; 
		rotate(node, direction); 
		parentNode = node->getParent(); 
	} 
	/* red-black rotation! */
	/*parentNode->setColor(RbColor::Black); */
	/*grandPaNode->setColor(RbColor::Red); */
	rbRotate(grandPaNode, oppositeDirection); 
	return;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void RbTree<T, K, Comp, ExtractKey>::rbDeleteFix(RbTreeNode<T> *doubleBlack) {
	//std::cout << "rbDeleteFix start\n";
	if (doubleBlack == root_ || doubleBlack->getColor() == RbColor::Red) {
		doubleBlack->setColor(RbColor::Black);
		return;
	}
	RbTreeNode<T> *parentNode = doubleBlack->getParent(), *brotherNode = NULL;
	// Symmetric code
	Direction direction = Direction::Left;
	Direction oppositeDirection = Direction::Right;
	if (doubleBlack == parentNode->getLeftChild()) { // leftChild
	} else { 
		direction = Direction::Right;
		oppositeDirection = Direction::Left;
	}
	brotherNode = parentNode->getChild(oppositeDirection); 
	if (brotherNode == NULL) { /* NULL is considered black */ 
		rbDeleteFix(parentNode); 
		return; 
	} else if (brotherNode->getColor() == RbColor::Red) { /* unwanted situation -> change */ 
		rbRotate(parentNode, direction); 
		rbDeleteFix(doubleBlack); /* redo with the same node */ 
		return; 
	} else { /* brother is black*/ 
		/* If no red nephew, go upward */ 
		if ((brotherNode->getLeftChild() == NULL || 
				brotherNode->getLeftChild()->getColor() == RbColor::Black) && 
				(brotherNode->getRightChild() == NULL || 
				brotherNode->getRightChild()->getColor() == RbColor::Black)) { 
			brotherNode->setColor(RbColor::Red); 
			rbDeleteFix(parentNode); 
			return; 
		} else if (brotherNode->getChild(oppositeDirection) != NULL && 
				brotherNode->getChild(oppositeDirection)->getColor() == RbColor::Red) { /* can end! */ 
			brotherNode->getChild(oppositeDirection)->setColor(RbColor::Black); 
			brotherNode->setColor(parentNode->getColor()); 
			parentNode->setColor(RbColor::Black); 
			rotate(parentNode, direction); 
			return; 
		} else { /* brother has red left child */ 
			rbRotate(brotherNode, oppositeDirection); 
			rbDeleteFix(doubleBlack); 
			return; 
		} 
	}
}

// returns black height
template <typename T, typename K, typename Comp, typename ExtractKey>
size_t RbTree<T, K, Comp, ExtractKey>::RbTreeCheck(
	RbTreeNode<T> const *node) const {
	if (node == NULL) {
		return 0;
	}
	// color check
	if (node->getColor() == RbColor::Red) {
		if (node->getLeftChild() != NULL && 
			node->getLeftChild()->getColor() == RbColor::Red) {
				std::cout << "rb check: color error!\n";
				exit(0);
		}
		if (node->getRightChild() != NULL && 
			node->getRightChild()->getColor() == RbColor::Red) {
				std::cout << "rb check: color error!\n";
				exit(0);
		}
	}
	size_t leftBlackHeight = RbTreeCheck(node->getLeftChild());
	size_t rightBlackHeight = RbTreeCheck(node->getRightChild());
	if (leftBlackHeight != rightBlackHeight) {
		std::cout << "rb check: black height error!\n";
		exit(0);
	}
	if (node->getColor() == RbColor::Red) {
		return leftBlackHeight;
	} else {
		return leftBlackHeight + 1; 
	}
}

} // lib_calvin_container

#endif

