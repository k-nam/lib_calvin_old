#ifdef BPLUS 
#define B_TREE_BASE BPlusTree
#else
#define B_TREE_BASE BTree
#endif

#include "container.h"

template <typename T, typename Comp = std::less<T>, typename K = T, typename ExtractKey = std::identity<T>>
class B_TREE_BASE 
{
private:
	// all internal nodes are full except end() of btree
	// all leaf nodes are full except leaf root and end() of bplus tree
	// only leaf root grows (by reserve())
	static int const B_TREE_FULL_NODE_CAPACITY = 127;
	static int const B_TREE_ROOT_INIT_CAPACITY = 4;
	static int const B_TREE_END_NODE_CAPACITY = 1;
	static int const B_TREE_NODE_REALLOC_MULTIPLIER = 2;
	class InternalNode;
	// btree node. number of links in a node: t ~ 2t, number of elements in a node:
	// t-1 ~ 2t-1. Therefore, maxNumKeys = 2t-1
	class Node 
	{
	private:
		friend B_TREE_BASE;
	public:
		void reserve(int capacity);
		int getCapacity() const;
		int getSize() const;
		void setSize(int size);
		InternalNode *getParent();
		void setParent(InternalNode *parent);
		bool isLeafNode() const; // used to record type information
		bool isFullNode() const;
	public:
		// 0 <= index < size_
		T &getElement(int index);
		T const &getElement(int index) const;
		T const &getFirstElement() const;
		T const &getLastElement() const;
		T *getElementArray();
		T const *getElementArray() const;
		void constructElement(int index, T const &elem);
		void constructElement(int index, T &&elem);
		void assignElement(int index, T const &elem);
		void assignElement(int index, T &&elem);
		// below insert and erase method does not change size_ value!
		template <typename T1> void insertElement(int index, T1 &&elem);
		// for addToNode method
		template <typename T1> void insertFirstElement(T1 &&elem); 
		template <typename T1> void insertLastElement(T1 &&elem);
		// for mergeNode method, tree::delete method
		T const eraseElement(int index);
		void eraseFirstElement();
		void eraseLastElement();
		// assuming appropriate preconditions. 
		void giveRightHalfTo(Node *target);
		void receiveRightHalfFrom(Node *source);
		void receiveRightHalfFromBPlusLeaf(Node *);
	public:
		// we should call below methods whenever size should be changed
		void increaseSizeByOne();
		void decreaseSizeByOne();
		static int const t = (B_TREE_FULL_NODE_CAPACITY + 1) / 2;
		bool isFull() const;
		bool isLeast() const;
#ifdef BPLUS
		Node *getNext();
		Node *getPrevious();
		void setNext(Node *next);
		void setPrevious(Node *previous);
#endif
	private:
		int getIndexInParent() const;
		void setIndexInParent(int index);
		int getTargetCapacity(int capacity) const;
	private:
		int size_; // current number keys in this node
		int capacity_;
		bool isLeafNode_;
		T *elements_;
		InternalNode *parent_;
		int indexInParent_;
#ifdef BPLUS
		Node *next_;
		Node *previous_;
#endif
	};

	class FullNode: public Node
	{
	private:
		friend B_TREE_BASE;
	private:
		T elements2_[B_TREE_FULL_NODE_CAPACITY];
	};

	class InternalNode: public Node
	{
	private:
		friend B_TREE_BASE;
	public:
		// 0 <= index <= size_
		Node *getChild(int index);
		Node const *getChild(int index) const;
		void setChild(int index, Node *child);
		Node *getFirstChild();
		Node *getLastChild();
		int findChild(Node const *child) const; // return the index of the child
		// insert right before index'th element (index == 0 means at first, 
		// index == size means at last)
		// insert and erase methods does not change size_!
		template <typename T1> void insertElementAndChild(int index, T1 &&elem, Node *child);
		void insertChild(int index, Node *child);
		// for addToNode method
		template <typename T1> void insertFirstElementAndChild(T1 &&elem, Node *child);
		template <typename T1> void insertLastElementAndChild(T1 &&elem, Node *child);
		void eraseFirstElementAndChild();
		void eraseLastElementAndChild();
		// for mergeNode method
		void eraseChild(int index);
		void eraseElementAndChild(int index);
	private:
		Node **children_;
	};

	class FullInternalNode: public InternalNode {
	private:
		friend B_TREE_BASE;
	private:
		T elements2_[B_TREE_FULL_NODE_CAPACITY];
		Node *children2_[B_TREE_FULL_NODE_CAPACITY + 1];
	};

public:
	typedef T value_type;
	typedef T * pointer;
	typedef T & reference;
	typedef int size_type;
	typedef ptrdiff_t difference_type;

private:
	// B_TREE_BASE uses bidirectional iterator
	// This class (const iterator type) forms base to four kinds of real iterator
	class IteratorImpl
  {
	public:
		typedef T value_type;
		typedef T const * pointer;
		typedef T const & reference;
		typedef int size_type;
		typedef ptrdiff_t difference_type;
		typedef std::bidirectional_iterator_tag iterator_category;
		reference operator*() const;
		pointer operator->() const;
	private:
		typedef B_TREE_BASE<T, Comp, K, ExtractKey> TreeType;
	public:
		IteratorImpl();
		IteratorImpl(TreeType const *tree, Node *node, int index = 0);
		IteratorImpl(IteratorImpl const &rhs);
		IteratorImpl & operator=(IteratorImpl const &rhs);
	public:
		IteratorImpl & operator++(); // prefix
		IteratorImpl & operator--();
		IteratorImpl const operator++(int); // suffix	
		IteratorImpl const operator--(int);
	public:
		bool operator==(IteratorImpl const &rhs) const;
		bool operator!=(IteratorImpl const &rhs) const;
	private:
		TreeType const * const tree_;
		Node *node_;
		int index_;
	};
	friend class IteratorImpl;
public:
	typedef lib_calvin_container::Iterator<IteratorImpl> iterator;
	typedef lib_calvin_container::ConstIterator<IteratorImpl> const_iterator;
	typedef lib_calvin_container::ReverseIterator<IteratorImpl> reverse_iterator;
	typedef lib_calvin_container::ConstReverseIterator<IteratorImpl> const_reverse_iterator;

public:
	B_TREE_BASE<T, Comp, K, ExtractKey>();
	// copy constructor
	B_TREE_BASE<T, Comp, K, ExtractKey>(B_TREE_BASE<T, Comp, K, ExtractKey> const &); 
	B_TREE_BASE<T, Comp, K, ExtractKey>(B_TREE_BASE<T, Comp, K, ExtractKey> &&); 
	// assignment
	B_TREE_BASE<T, Comp, K, ExtractKey> & operator= (B_TREE_BASE<T, Comp, K, ExtractKey> const &); 
	B_TREE_BASE<T, Comp, K, ExtractKey> & operator= (B_TREE_BASE<T, Comp, K, ExtractKey> &&); 
	void swap(B_TREE_BASE<T, Comp, K, ExtractKey> &rhs);
	~B_TREE_BASE<T, Comp, K, ExtractKey>();
public:
	size_t size() const;
	const_iterator find(K const &) const;
	iterator find(K const &);
	size_t count(K const &key) const;
	std::pair<iterator, bool> insert(T const &);
	std::pair<iterator, bool> insert(T &&);
	// Insert range [beg, end)
	template <typename InputIterator> void insert(InputIterator beg, InputIterator end);
	size_t erase(K const &key); 
	void clear();
	bool empty() const;
public:
	const_iterator begin() const;
	const_iterator end() const;
	iterator begin();
	iterator end(); 
	const_reverse_iterator rbegin() const;
	const_reverse_iterator rend() const;
	reverse_iterator rbegin();
	reverse_iterator rend();
private: 
	template <typename T1> std::pair<iterator, bool> insert_(T1 &&);
	Node *getFirstNode() const;
	Node *getLastNode() const;
	Node *getFirstOrLastNode(bool isFirst) const;
	IteratorImpl findIterator(K const &key) const;
	IteratorImpl makeIterator(Node *node, int index = 0) const;
	// insert certain elem into the right position in the node, 
	// and split the node if necessary
	template <typename T1>
		std::pair<iterator, bool> insertIntoNode(Node *node, T1 &&elem, int parentIndex);
	// delete certain elem from the the recursively
	int deleteFromNode(Node *node, K const &key, int parentIndex);
	// insert value into a leaf node
	template <typename T1> std::pair<iterator, bool> insertValue(Node *node, T1 &&elem);
	template <typename T1> void insertValue(Node *node, T1 &&elem, int indexToInsert);
	// delete value from a leaf node
	T const deleteValue(Node *node, int indexToDelete);
	// splitNode returns the pointer to newly created brother node
	Node *splitNode(Node *node, int parentIndex);
	// add one more element to a node from its brother
	// look into its right brother unless it is the rightmost children
	// returns whether the operation actually took place
	void addToLeafNodeFromLeft(Node *node, Node *brother, int parentIndex);
	void addToLeafNodeFromRight(Node *node, Node *brother, int parentIndex);
	void addToInternalNodeFromLeft(Node *node, Node *brother, int parentIndex);
	void addToInternalNodeFromRight(Node *node, Node *brother, int parentIndex);
	bool addToNode(Node *node, int parentIndex);
	// merge with its brother (other things are similar to above method)
	// returns the merged node
	Node *mergeNode(Node *node, int parentIndex); 
	Node *addToOrMerge(Node *node, int parentIndex);
	Node *addToOrMergeIfNeeded(Node *node, int parentIndex);
	// deleting successor
	T const deleteLeftMost(Node *node, int parentIndex); 
	// deleting predecessor
	T const deleteRightMost(Node *node, int parentIndex); 
	Node *makeNewLeafNode(int capacity);
	InternalNode *makeNewInternalNode(int capacity);
private:
	// Returns true when given element can be inserted (not already exists)
	std::pair<int, bool> getIndexToInsert(Node *node, T const &elem) const;
	std::pair<int, bool> getIndexToInsert2(Node *node, K const &key) const;
private:
	// subroutine for copy con & copy assign. Should be called only when in initial state.
	void copyFrom(B_TREE_BASE<T, Comp, K, ExtractKey> const &); 
	void bTreeNodeCopy(Node const *source, Node *&target);
	void bTreeNodeCopy(Node const *source, Node *&target, Node *&leftMode, Node *&rightMost);
	// target node is assumed empty
	void bTreeNodeElemCopy(Node const *source, Node *target);
	void bTreeNodeDelete(Node *node);
	void bTreeNodeDeleteElement(Node *node);
private:
	// for testing
	void addToNodeTest();
	void mergeNodeTest();
	void print() const;
	void printNode(Node *) const;
	void printTree(Node *) const;
private:
	size_t size_;
	Node *root_;
public:
	friend class BTreeTest<T, Comp, K, ExtractKey>;
};

template <typename T, typename Comp, typename K, typename ExtractKey>
bool operator==(B_TREE_BASE<T, Comp, K, ExtractKey> const &lhs, 
	B_TREE_BASE<T, Comp, K, ExtractKey> const &rhs);

template <typename T, typename Comp, typename K, typename ExtractKey>
bool operator!=(B_TREE_BASE<T, Comp, K, ExtractKey> const &lhs, 
	B_TREE_BASE<T, Comp, K, ExtractKey> const &rhs);

template <typename T, typename Comp, typename K, typename ExtractKey>
bool operator< (B_TREE_BASE<T, Comp, K, ExtractKey> const &lhs, 
	B_TREE_BASE<T, Comp, K, ExtractKey> const &rhs);

template <typename T, typename Comp, typename K, typename ExtractKey>
bool operator<= (B_TREE_BASE<T, Comp, K, ExtractKey> const &lhs, 
	B_TREE_BASE<T, Comp, K, ExtractKey> const &rhs);

template <typename T, typename Comp, typename K, typename ExtractKey>
bool operator> (B_TREE_BASE<T, Comp, K, ExtractKey> const &lhs, 
	B_TREE_BASE<T, Comp, K, ExtractKey> const &rhs);

template <typename T, typename Comp, typename K, typename ExtractKey>
bool operator>= (B_TREE_BASE<T, Comp, K, ExtractKey> const &lhs, 
	B_TREE_BASE<T, Comp, K, ExtractKey> const &rhs);

//--------------------------  node implementations -------------------------//

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::Node::reserve(int capacity) {
	if (capacity_ >= capacity) {
		return;
	}
	if (isLeafNode() == false) {
		std::cout << "reserve error\n";
		exit(0);
	}
	capacity_ = getTargetCapacity(capacity);
	T *newArray = static_cast<T *>(operator new(sizeof(T) * capacity_));
	moveConstructAndDestruct(elements_, newArray, size_);
	operator delete(elements_);
	elements_ = newArray;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
int B_TREE_BASE<T, Comp, K, ExtractKey>::Node::getCapacity() const {
	return capacity_; 
}


template <typename T, typename Comp, typename K, typename ExtractKey>
int B_TREE_BASE<T, Comp, K, ExtractKey>::Node::getSize() const {
	return size_;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::Node::setSize(int size) {
	size_ = size;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::InternalNode * 
B_TREE_BASE<T, Comp, K, ExtractKey>::Node::getParent() {
	return parent_;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::Node::setParent(InternalNode *parent) {
	parent_ = parent;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
int B_TREE_BASE<T, Comp, K, ExtractKey>::Node::getIndexInParent() const {
	return indexInParent_;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::Node::setIndexInParent(int index) {
	indexInParent_ = index;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
bool B_TREE_BASE<T, Comp, K, ExtractKey>::Node::isLeafNode() const { 
	return isLeafNode_; 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
T & B_TREE_BASE<T, Comp, K, ExtractKey>::Node::getElement(int index) {
	return getElementArray()[index];
}

template <typename T, typename Comp, typename K, typename ExtractKey>
T const & B_TREE_BASE<T, Comp, K, ExtractKey>::Node::getElement(int index) const {
	return getElementArray()[index];
}

template <typename T, typename Comp, typename K, typename ExtractKey>
T const & B_TREE_BASE<T, Comp, K, ExtractKey>::Node::getFirstElement() const {
	return getElementArray()[0];
}

template <typename T, typename Comp, typename K, typename ExtractKey>
T const & B_TREE_BASE<T, Comp, K, ExtractKey>::Node::getLastElement() const {
	return getElementArray()[size_ - 1];
}

template <typename T, typename Comp, typename K, typename ExtractKey>
T * B_TREE_BASE<T, Comp, K, ExtractKey>::Node::getElementArray() {
	return const_cast<T *>(static_cast<Node const *>(this)->getElementArray());
}

template <typename T, typename Comp, typename K, typename ExtractKey>
T const * B_TREE_BASE<T, Comp, K, ExtractKey>::Node::getElementArray() const {
	return elements_;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::Node::constructElement(int index, T const &elem) { 
	new (getElementArray() + index) T(elem); 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::Node::constructElement(int index, T &&elem) { 
	new (getElementArray() + index) T(std::forward<T>(elem)); 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::Node::assignElement(int index, T const &elem) { 
	getElementArray()[index] = elem; 
}
template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::Node::assignElement(int index, T &&elem) { 
	getElementArray()[index] = std::forward<T>(elem);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::Node::increaseSizeByOne() { 
	size_++; 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::Node::decreaseSizeByOne() { 
	size_--; 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
template <typename T1>
void B_TREE_BASE<T, Comp, K, ExtractKey>::Node::insertElement(int index, T1 &&elem) {
	if (this->isFull()) {
		std::cout << "inserting into full node\n";
		exit(0);
	}
	if (size_ == capacity_) {
		reserve(size_ + 1);
	}
	insertSingleElement(getElementArray(), size_, index, std::forward<T1>(elem));
}

template <typename T, typename Comp, typename K, typename ExtractKey>
template <typename T1>
void B_TREE_BASE<T, Comp, K, ExtractKey>::Node::insertFirstElement(T1 &&elem) {
	insertElement(0, std::forward<T1>(elem));
}

template <typename T, typename Comp, typename K, typename ExtractKey>
template <typename T1>
void B_TREE_BASE<T, Comp, K, ExtractKey>::Node::insertLastElement(T1 &&elem) {
	insertElement(size_, std::forward<T1>(elem));
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::Node::eraseFirstElement() {
	eraseElement(0);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::Node::eraseLastElement() {
	eraseElement(size_ - 1);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::Node::giveRightHalfTo(Node *target) {

	moveConstructAndDestruct(getElementArray() + t, target->getElementArray(), t - 1);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::Node::receiveRightHalfFrom(Node *source) {
	moveConstructAndDestruct(source->getElementArray(), getElementArray() + t, t - 1);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::Node::receiveRightHalfFromBPlusLeaf(Node *source) {
	moveConstructAndDestruct(source->getElementArray(), getElementArray() + t - 1, t - 1);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
T const 
B_TREE_BASE<T, Comp, K, ExtractKey>::Node::eraseElement(int index) {
	T const elem(std::move(getElementArray()[index]));
	eraseSingleElement(getElementArray(), size_, index);
	return elem;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
bool B_TREE_BASE<T, Comp, K, ExtractKey>::Node::isFull() const { 
	return size_ == B_TREE_FULL_NODE_CAPACITY; 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
bool B_TREE_BASE<T, Comp, K, ExtractKey>::Node::isLeast() const { 
	return size_ == t - 1; 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
int B_TREE_BASE<T, Comp, K, ExtractKey>::Node::getTargetCapacity(int capacity) const {
	int targetCapacity = capacity_;
	while (true) {
		if (capacity <= targetCapacity) {
			break;
		} else {
			targetCapacity *= B_TREE_NODE_REALLOC_MULTIPLIER;
			if (targetCapacity >= B_TREE_FULL_NODE_CAPACITY) {
				targetCapacity = B_TREE_FULL_NODE_CAPACITY;
				break;
			}
		}
	}
	return targetCapacity;
}

#ifdef BPLUS
template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::Node *  
B_TREE_BASE<T, Comp, K, ExtractKey>::Node::getNext() { 
	return next_; 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::Node *  
B_TREE_BASE<T, Comp, K, ExtractKey>::Node::getPrevious() { 
	return previous_; 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::Node::setNext(Node *next) { 
	next_ = next; 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::Node::setPrevious(Node *previous) { 
	previous_ = previous; 
}
#endif

//--------------------------  internal node implementations -------------------------//

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::Node * 
B_TREE_BASE<T, Comp, K, ExtractKey>::InternalNode::getChild(int index) { 
	return children_[index]; 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::Node const *  
B_TREE_BASE<T, Comp, K, ExtractKey>::InternalNode::getChild(int index) const { 
	return children_[index]; 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::InternalNode::setChild(int index, Node *child) { 
	children_[index] = child; 
	child->setIndexInParent(index); 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::Node *  
B_TREE_BASE<T, Comp, K, ExtractKey>::InternalNode::getFirstChild() { 
	return getChild(0); 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::Node *  
B_TREE_BASE<T, Comp, K, ExtractKey>::InternalNode::getLastChild() { 
	return getChild(size_); 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
int B_TREE_BASE<T, Comp, K, ExtractKey>::InternalNode::findChild(Node const *child) const {
	return child->getIndexInParent();
}

template <typename T, typename Comp, typename K, typename ExtractKey>
template <typename T1>
void B_TREE_BASE<T, Comp, K, ExtractKey>::InternalNode::insertElementAndChild(
		int index, T1 &&elem, Node *child) {
	// insert elem right before index'th elem 
	// and insert child right before index + 1'th child
	insertElement(index, std::forward<T1>(elem));
	insertChild(index + 1, child);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::InternalNode::insertChild(int index, Node *child) {
	for (int i = size_; i >= index; --i) {
		//children_[i + 1] = children_[i];
		setChild(i + 1, getChild(i));
	}
	//children_[index] = child;
	setChild(index, child);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
template <typename T1>
void B_TREE_BASE<T, Comp, K, ExtractKey>::InternalNode::insertFirstElementAndChild(
		T1 &&elem, Node *child) { 
	insertChild(0, child); 
	insertFirstElement(std::forward<T1>(elem));	
}

template <typename T, typename Comp, typename K, typename ExtractKey>
template <typename T1>
void B_TREE_BASE<T, Comp, K, ExtractKey>::InternalNode::insertLastElementAndChild(
		T1 &&elem, Node *child) {
	insertChild(getSize() + 1, child);
	insertLastElement(std::forward<T1>(elem));	
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::InternalNode::eraseFirstElementAndChild() {
	eraseChild(0);
	eraseFirstElement();	
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::InternalNode::eraseLastElementAndChild() {
	eraseChild(size_);
	eraseLastElement();	
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::InternalNode::eraseChild(int index) {
	for (int i = index; i < size_; ++i) { 
		//children_[i] = children_[i + 1];
		setChild(i, getChild(i + 1));
	}
	children_[size_] = NULL;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::InternalNode::eraseElementAndChild(int index) {
	eraseChild(index + 1);
	eraseElement(index);	
}

//----------------------- iterator implementation ------------------//

template <typename T, typename Comp, typename K, typename ExtractKey>
B_TREE_BASE<T, Comp, K, ExtractKey>::IteratorImpl::IteratorImpl():
	tree_(NULL), node_(NULL), index_(0) { }

template <typename T, typename Comp, typename K, typename ExtractKey>
B_TREE_BASE<T, Comp, K, ExtractKey>::IteratorImpl::IteratorImpl(
	B_TREE_BASE<T, Comp, K, ExtractKey> const *tree, Node *node, int index):
		tree_(tree), node_(node), index_(index) { }

template <typename T, typename Comp, typename K, typename ExtractKey>
B_TREE_BASE<T, Comp, K, ExtractKey>::IteratorImpl::IteratorImpl(IteratorImpl const &rhs): 
	tree_(rhs.tree_), node_(rhs.node_), index_(rhs.index_) { }		
		
template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::IteratorImpl &
B_TREE_BASE<T, Comp, K, ExtractKey>::IteratorImpl::operator=(IteratorImpl const &rhs) { 
	const_cast<TreeType const *>(tree_) = rhs.tree_;
	node_ = rhs.node_; 
	index_ = rhs.index_; 
	return *this; 
}	

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::IteratorImpl::reference
B_TREE_BASE<T, Comp, K, ExtractKey>::IteratorImpl::operator*() const { 
	return node_->getElement(index_); 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::IteratorImpl::pointer
B_TREE_BASE<T, Comp, K, ExtractKey>::IteratorImpl::operator->() const { 
	return &node_->getElement(index_); 
}

//------------------------------------------------------- MACRO BEGIN -----//
#define ITERATOR_MOVE_ROUTINE(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) \
	if (node_->isLeafNode() == true) { /* leaf node */ \
		if (index_ ARG0) { /* not last(first) element */\
			index_ ARG1; \
		} else { /* last element of a leaf */ \
			while (true) { \
				if (node_->getParent()) { /* not root */ \
					if (node_->indexInParent_ ARG2) { /* yes, return this */ \
						index_ = node_->indexInParent_ ARG3; \
						node_ = node_->getParent(); \
						break; \
					} else { /* go upward again */ \
						node_ = node_->getParent(); \
					}	\
				} else { /* cannot go upward, return end() */ \
					node_ = NULL; \
					index_ = 0; \
					break; \
				} \
			} \
		} \
	} else { /* internal node */ \
		Node *temp = \
			static_cast<InternalNode *>(node_)->getChild(index_ ARG4); \
		while (temp->isLeafNode() == false) { \
			temp = static_cast<InternalNode *>(temp)->getChild(ARG5); \
		} \
		node_ = temp; \
		index_ = ARG6;	\
	} 

//------------------------------------------------------- MACRO END -----//

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::IteratorImpl &
B_TREE_BASE<T, Comp, K, ExtractKey>::IteratorImpl::operator++() {
#ifdef BPLUS
	index_++;
	if (index_ < node_->getSize()) {
	} else {
		node_ = node_->getNext();
		index_ = 0;
	}
#else
	ITERATOR_MOVE_ROUTINE(<node_->getSize()-1, ++, <node_->getParent()->getSize(), , +1, 0, 0) 
#endif
	//if (node_->getParent() == NULL) {
	//	int a = 0;
	//}
	return *this;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::IteratorImpl &
B_TREE_BASE<T, Comp, K, ExtractKey>::IteratorImpl::operator--() {
	if (!node_) { // currently end (null)
		node_ = tree_->getLastNode();
		index_ = node_->getSize() - 1;
		return *this;
	}
#ifdef BPLUS
	index_--;
	if (index_ < 0) {
		node_ = node_->getPrevious();
		index_ = node_->getSize() - 1;
	}
#else
	ITERATOR_MOVE_ROUTINE(>0, --, >0, -1, , temp->getSize(), temp->getSize()-1) 
#endif
	return *this;
}
#undef ITERATOR_MOVE_ROUTINE


template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::IteratorImpl const 
B_TREE_BASE<T, Comp, K, ExtractKey>::IteratorImpl::operator++(int) {
	IteratorImpl returnValue(*this);
	operator++(); 
	return returnValue;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::IteratorImpl const 
B_TREE_BASE<T, Comp, K, ExtractKey>::IteratorImpl::operator--(int) {
	IteratorImpl returnValue(*this);
	operator--(); 
	return returnValue;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
bool B_TREE_BASE<T, Comp, K, ExtractKey>::IteratorImpl::operator==(IteratorImpl const &rhs) const {
	return tree_ == rhs.tree_ && node_ == rhs.node_ && index_ == rhs.index_;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
bool B_TREE_BASE<T, Comp, K, ExtractKey>::IteratorImpl::operator!=(IteratorImpl const &rhs) const {
	return !(*this == rhs);
}

//--------------------------- tree implementation ------------------//
 
template <typename T, typename Comp, typename K, typename ExtractKey>
B_TREE_BASE<T, Comp, K, ExtractKey>::B_TREE_BASE():size_(0), root_(NULL) {
	//std::cout << "btree default ctor\n";
}

template <typename T, typename Comp, typename K, typename ExtractKey>
B_TREE_BASE<T, Comp, K, ExtractKey>::B_TREE_BASE(B_TREE_BASE<T, Comp, K, ExtractKey> const &rhs):
size_(0), root_(NULL) {
	//std::cout << "btree copy ctor\n";
	copyFrom(rhs);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
B_TREE_BASE<T, Comp, K, ExtractKey>::B_TREE_BASE(B_TREE_BASE<T, Comp, K, ExtractKey> &&rhs):
size_(0), root_(NULL) {
	//std::cout << "btree move ctor\n";
	swap(rhs);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
B_TREE_BASE<T, Comp, K, ExtractKey> &
B_TREE_BASE<T, Comp, K, ExtractKey>::operator=(B_TREE_BASE<T, Comp, K, ExtractKey> const &rhs) {
	//std::cout << "btree copy assignment\n";
	if (this == &rhs) {
		return *this;
	}
	clear();
	copyFrom(rhs);
	return *this;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
B_TREE_BASE<T, Comp, K, ExtractKey> &
B_TREE_BASE<T, Comp, K, ExtractKey>::operator=(B_TREE_BASE<T, Comp, K, ExtractKey> &&rhs) {
	//std::cout << "btree move assignment\n";
	if (this == &rhs) {
		return *this;
	}
	swap(rhs);
	return *this;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::swap(B_TREE_BASE<T, Comp, K, ExtractKey> &rhs) {
	size_t tempSize = size_;
	Node *tempRoot = root_;
	size_ = rhs.size_;
	root_ = rhs.root_;
	rhs.size_ = tempSize;
	rhs.root_ = tempRoot;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
B_TREE_BASE<T, Comp, K, ExtractKey>::~B_TREE_BASE() {
	bTreeNodeDelete(root_);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
size_t B_TREE_BASE<T, Comp, K, ExtractKey>::size() const { 
	return size_; 
}
template <typename T, typename Comp, typename K, typename ExtractKey>
size_t B_TREE_BASE<T, Comp, K, ExtractKey>::count(K const &key) const { 
	if (find(key) == end()) {
		return 0; 
	} else {
		return 1; 
	}
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::const_iterator
B_TREE_BASE<T, Comp, K, ExtractKey>::find(K const &key) const {
	return findIterator(key);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::iterator
B_TREE_BASE<T, Comp, K, ExtractKey>::find(K const &key) {
	return findIterator(key);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
std::pair<typename B_TREE_BASE<T, Comp, K, ExtractKey>::iterator, bool>
B_TREE_BASE<T, Comp, K, ExtractKey>::insert(T const &elem) {
	return insert_(elem);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
std::pair<typename B_TREE_BASE<T, Comp, K, ExtractKey>::iterator, bool>
B_TREE_BASE<T, Comp, K, ExtractKey>::insert(T &&elem) {
	return insert_(std::forward<T>(elem));
}

template <typename T, typename Comp, typename K, typename ExtractKey>
template <typename InputIterator>
void B_TREE_BASE<T, Comp, K, ExtractKey>::insert(InputIterator beg, InputIterator end) { 
	while(beg != end) { 
		insert(*beg++); 
	} 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
size_t 
B_TREE_BASE<T, Comp, K, ExtractKey>::erase(K const &key) {
	if (empty()) { 
		return 0; 
	} else { 
		int count = deleteFromNode(root_, key, 0);		
		return count;
	}
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::clear() {
	size_ = 0;
	bTreeNodeDelete(root_);
	root_ = NULL;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
bool B_TREE_BASE<T, Comp, K, ExtractKey>::empty() const {
	return size_ == 0; 
}
template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::Node *
B_TREE_BASE<T, Comp, K, ExtractKey>::getFirstNode() const {
	return getFirstOrLastNode(true);
}
template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::Node *
B_TREE_BASE<T, Comp, K, ExtractKey>::getLastNode() const {
	return getFirstOrLastNode(false);

}
template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::Node *
B_TREE_BASE<T, Comp, K, ExtractKey>::getFirstOrLastNode(bool isFirst) const {
	if (empty()) {
		return NULL;
	}
	Node *node = root_;
	while (node->isLeafNode() == false) {
		if (isFirst) {
			node = static_cast<InternalNode *>(node)->getFirstChild();
		} else {
			node = static_cast<InternalNode *>(node)->getLastChild();
		}
	}
	return node;
}
template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::const_iterator 
B_TREE_BASE<T, Comp, K, ExtractKey>::begin() const { 
	return makeIterator(this->getFirstNode()); 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::const_iterator 
B_TREE_BASE<T, Comp, K, ExtractKey>::end() const { 
	return makeIterator(NULL); 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::iterator 
B_TREE_BASE<T, Comp, K, ExtractKey>::begin() { 
	return makeIterator(this->getFirstNode()); 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::iterator 
B_TREE_BASE<T, Comp, K, ExtractKey>::end() { 
	return makeIterator(NULL);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::const_reverse_iterator 
B_TREE_BASE<T, Comp, K, ExtractKey>::rbegin() const { 
	return makeIterator(NULL); 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::const_reverse_iterator 
B_TREE_BASE<T, Comp, K, ExtractKey>::rend() const { 
	return makeIterator(this->getFirstNode()); 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::reverse_iterator 
B_TREE_BASE<T, Comp, K, ExtractKey>::rbegin() { 
	return makeIterator(NULL);  
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::reverse_iterator 
B_TREE_BASE<T, Comp, K, ExtractKey>::rend() { 
	return makeIterator(this->getFirstNode()); 
}

template <typename T, typename Comp, typename K, typename ExtractKey>
template <typename T1>
std::pair<typename B_TREE_BASE<T, Comp, K, ExtractKey>::iterator, bool>
B_TREE_BASE<T, Comp, K, ExtractKey>::insert_(T1 &&elem) {
	if (empty()) { // empty 
		Node *newRoot = makeNewLeafNode(B_TREE_ROOT_INIT_CAPACITY);
		root_ = newRoot;
		newRoot->setSize(1);
		newRoot->constructElement(0, std::forward<T1>(elem));
		size_ = 1;
		return std::pair<iterator, bool>(makeIterator(root_, 0), true);
	}
	if (root_ != NULL && root_->getSize() == 0) {
			std::cout << "insert error 1\n";
			exit(0);
	}
	// Non-empty 
	//std::cout << "insert root size: " << root_->getSize() << "\ntree size is: " << size_ << "\n";
	return insertIntoNode(root_, std::forward<T1>(elem), 0);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::IteratorImpl
B_TREE_BASE<T, Comp, K, ExtractKey>::findIterator(K const &key) const {
	if (empty()) { 
		return makeIterator(NULL); 
	} 
	Node *thisNode = root_; 
	while (true) { 
		std::pair<int, bool> result = getIndexToInsert2(thisNode, key); 
#ifdef BPLUS
		if (thisNode->isLeafNode()) {
			if (result.second == false) { // found
				return makeIterator(thisNode, result.first); 
			} else {
				return makeIterator(NULL);
			}
		} else { // internal nodes are not data elements
			thisNode = static_cast<InternalNode *>(thisNode)->getChild(result.first); 
		}
#else
		if (result.second == false) { /* we found the element */
			return makeIterator(thisNode, result.first); 
		} else { /* elements doesn't exist in this node */ 
			if (thisNode->isLeafNode() == true) { 
				return makeIterator(NULL); 
			} else { 
				thisNode = static_cast<InternalNode *>(thisNode)->getChild(result.first); 
			} 
		} 
#endif
	}	
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::IteratorImpl
B_TREE_BASE<T, Comp, K, ExtractKey>::makeIterator(Node *node, int index) const {
	return IteratorImpl(this, node, index);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
template <typename T1>
std::pair<typename B_TREE_BASE<T, Comp, K, ExtractKey>::iterator, bool>
B_TREE_BASE<T, Comp, K, ExtractKey>::insertIntoNode(Node *node, T1 &&elem, int parentIndex) {
	//std::cout << "insertInto node size: " << node->getSize() << "\n";
	//std::cout << "called insertIntoNode\n";
	std::pair<int, bool> result = getIndexToInsert(node, elem);
	int indexToInsert = result.first;
	if (result.second == false) { // element found
		if (node->isLeafNode()) {
			return std::pair<iterator, bool>(makeIterator(node, result.first), false);
		} else { // bplus tree does not keep elem in internal nodes
#ifndef BPLUS
			return std::pair<iterator, bool>(makeIterator(node, result.first), false);
#endif			
		}
	}
	if (node->isLeafNode() == false) { // this is internal node
		Node *childNodeToInsert = static_cast<InternalNode *>(node)->getChild(indexToInsert);
		if (node->isFull()) {
			// If we are going down with the right part of splitted node
			if (Comp()(ExtractKey()(node->getElement(node->t - 1)), ExtractKey()(elem))) {
				// Integrity check
				if ((indexToInsert >= node->t) == false) {
					std::cout << "insertIntoNode integrity error\n";
					exit(0);
				}
				indexToInsert -= node->t;
			}			
			//std::cout << "split node for internal calling\n";
			splitNode(node, parentIndex);			 
		}
		return insertIntoNode(childNodeToInsert, std::forward<T1>(elem), indexToInsert);
	} else { // this is a leaf
		Node *leafNodeToInsert = node;	
		if (node->isFull()) {
			//std::cout << "split node for leaf calling\n";
			Node *newBrotherNode = splitNode(node, parentIndex);
			// if new element is bigger than the center, 
			//  we insert into the newly created brother
			if (indexToInsert >= node->t) {
				leafNodeToInsert = newBrotherNode; // insert into brother instead
				indexToInsert -= node->t;
			}		
		}
		insertValue(leafNodeToInsert, std::forward<T1>(elem), indexToInsert);
		return std::pair<iterator, bool>(makeIterator(leafNodeToInsert, indexToInsert), true);
	}
}

template <typename T, typename Comp, typename K, typename ExtractKey>
int
B_TREE_BASE<T, Comp, K, ExtractKey>::deleteFromNode(Node *node, K const &key, int parentIndex) {
	if (node != root_) {
		 node = addToOrMergeIfNeeded(node, parentIndex);
		 if (node == NULL) {
			 std::cout << "node=null\n";
			 exit(0);
		 }
		 //printNode(node);
	}
	int returnValue = 0;
	std::pair<int, bool> result = getIndexToInsert2(node, key);
	if (node->isLeafNode() == false) { // this is internal
		InternalNode *thisNode = static_cast<InternalNode *>(node);
		Node *childNodeToDeleteFrom = thisNode->getChild(result.first);
#ifdef BPLUS
		return deleteFromNode(childNodeToDeleteFrom, key, result.first);
#else
		if (result.second == true) { // not exist in this node
			//std::cout << "deleting: key not found in internal node\n";
			return (deleteFromNode(childNodeToDeleteFrom, key, result.first));		
		} else { // key found in this node
			//std::cout << "deleting: key found in internal node\n";
			if (thisNode->getChild(result.first)->isLeast() == false) { // left side
				// find the predecessor of the node and delete it
				T predecessor = 
					deleteRightMost(thisNode->getChild(result.first), result.first);
				node->assignElement(result.first, std::move(predecessor));
				returnValue = 1;
			} else if (thisNode->getChild(result.first + 1)->isLeast() == false) { // right
				// find the successor of the node and delete it
				T successor = 
					deleteLeftMost(thisNode->getChild(result.first + 1), result.first + 1);
				node->assignElement(result.first, std::move(successor));
				returnValue = 1;
			} else { // both children are least; so merge them and recusive call to it
				Node *mergedNode = 
					mergeNode(thisNode->getChild(result.first), result.first);
				return (deleteFromNode(mergedNode, key, result.first));
			}
		}
#endif
	} else { // leaf node		
		if (result.second == true) { // not exist in this tree at all
			//std::cout << "deleting: key not found in leaf node\n";
			returnValue = 0;
		} else { // exists
			//std::cout << "deleting: key found in leaf node\n";
			deleteValue(node, result.first);
			if (empty()) { // now empty
				operator delete(root_);
				root_ = NULL;				
			}
			returnValue = 1;
		}
	}
	if (returnValue > 0) {
		size_--;
	}
	return returnValue;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
template <typename T1>
std::pair<typename B_TREE_BASE<T, Comp, K, ExtractKey>::iterator, bool>
B_TREE_BASE<T, Comp, K, ExtractKey>::insertValue(Node *node, T1 &&elem) {
	//std::cout << "called insertValue1\n";
	if (node->isLeafNode() == false) {
		std::cout << "insertValue error: is not a leaf node\n";
		exit(0);
	}
	if (node->isFull() == true) {
		std::cout << "insertValue error: node is full\n";
		exit(0);
	}
	std::pair<int, bool> result = getIndexToInsert(node, elem);
	if (result.second == false) {
		return false;
	} else {
		insertValue(node, std::forward<T1>(elem), result.first);
	}
}

template <typename T, typename Comp, typename K, typename ExtractKey>
template <typename T1>
void
B_TREE_BASE<T, Comp, K, ExtractKey>::insertValue(
		Node *node, T1 &&elem, int indexToInsert) {
	node->insertElement(indexToInsert, std::forward<T1>(elem));	
	node->increaseSizeByOne();
	size_++;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
T const
B_TREE_BASE<T, Comp, K, ExtractKey>::deleteValue(Node *node, int indexToDelete) {
  T const deletedElement = (node->eraseElement(indexToDelete));	
	node->decreaseSizeByOne();
	return deletedElement;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::Node *
B_TREE_BASE<T, Comp, K, ExtractKey>::splitNode(Node *node, int parentIndex) {
	//std::cout << "splitnode called with: " << parentIndex << "\n";
	int t = node->t;
	// possible elems in a node: t-1 ~ 2t-1
	// possible number of children in a node: t ~ 2t
	// 2t-1 = maxNumKeys, t = (maxNumKeys-1)/2+1
	// elems: 0 ...  t-2 <t-1> t ... 2t-2
	// links:0 1    t-2  t-1  t ... 2t-2 2t-1
	if (node->isFull() == false) {
		std::cout << "splitNode error: number of elems is not 2t - 1\n";
		exit(0);
	}
	if (node!= root_ && node->getParent()->isFull()) {
		std::cout << "splitNode error: parent is full\n";
		exit(0);
	}
	if (node == root_) { 
		InternalNode *newRoot = makeNewInternalNode(B_TREE_FULL_NODE_CAPACITY);
		node->setParent(newRoot);
		newRoot->setChild(0, node);
		root_ = newRoot; // change the root of this B-tree
	}
	Node *newBrotherNode = NULL;
	if (node->isLeafNode() == false) { // node is internal
		newBrotherNode = makeNewInternalNode(B_TREE_FULL_NODE_CAPACITY);
		// copy links and change parents
		for (int i = 0; i < t; ++i) {
			static_cast<InternalNode *>(newBrotherNode)->
				setChild(i, static_cast<InternalNode *>(node)->getChild(i + t));
			static_cast<InternalNode *>(newBrotherNode)->
				getChild(i)->setParent(static_cast<InternalNode *>(newBrotherNode));
		}
	} else { // node is leaf
		newBrotherNode = makeNewLeafNode(B_TREE_FULL_NODE_CAPACITY);
	}
	// copy elements and delete original ones
	node->giveRightHalfTo(newBrotherNode);
	newBrotherNode->setSize(t - 1);
	newBrotherNode->setParent(node->getParent());
	InternalNode *parent = static_cast<InternalNode *>(node->getParent());
	if (parent->isFull()) {
		std::cout << "splitNode error: parent is full\n";
		exit(0);
	}

#ifdef BPLUS
	if (node->isLeafNode() == false) {
		node->setSize(t - 1);
		parent->insertElementAndChild(parentIndex, std::move(node->getElement(t - 1)), newBrotherNode);
		node->getElement(t - 1).~T();
	} else { // leaf
		node->setSize(t);
		parent->insertElementAndChild(parentIndex, node->getLastElement(), newBrotherNode);
		newBrotherNode->setNext(node->getNext());
		newBrotherNode->setPrevious(node);
		if (node->getNext()) {
			node->getNext()->setPrevious(newBrotherNode);
		}
		node->setNext(newBrotherNode);
	}
#else
	// push the middle element into the parent node
	node->setSize(t - 1);
	parent->insertElementAndChild(parentIndex, std::move(node->getElement(t - 1)), newBrotherNode);
	node->getElement(t - 1).~T();
#endif

	parent->increaseSizeByOne();
	return newBrotherNode;
}

#ifdef BPLUS
#define ADD_TO_NODE_ROUTINE_LEAF(ARG2, ARG4, ARG6, ARG7, ARG8) \
	node->ARG7(brotherNode->ARG4()); \
	node->increaseSizeByOne(); \
	brotherNode->ARG6(); \
	brotherNode->decreaseSizeByOne(); \
	/* set the key of parent as the rightmost key of left node */ \
	node->getParent()->assignElement(parentIndex ARG2, ARG8->getLastElement());
#else
#define ADD_TO_NODE_ROUTINE_LEAF(ARG2, ARG4, ARG6, ARG7, ARG8) \
	node->ARG7(node->getParent()->getElement(parentIndex ARG2)); \
	node->increaseSizeByOne(); \
	node->getParent()->assignElement(parentIndex ARG2, std::move(brotherNode->ARG4())); \
	brotherNode->ARG6(); \
	brotherNode->decreaseSizeByOne();
#endif

template <typename T, typename Comp, typename K, typename ExtractKey>
void
B_TREE_BASE<T, Comp, K, ExtractKey>::addToLeafNodeFromLeft(Node *node, Node *brotherNode, int parentIndex) {
	ADD_TO_NODE_ROUTINE_LEAF(-1, getLastElement, eraseLastElement, insertFirstElement, brotherNode)
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void
B_TREE_BASE<T, Comp, K, ExtractKey>::addToLeafNodeFromRight(Node *node, Node *brotherNode, int parentIndex) {
	ADD_TO_NODE_ROUTINE_LEAF( , getFirstElement, eraseFirstElement, insertLastElement, node)
}
#undef ADD_TO_NODE_ROUTINE_LEAF

#define ADD_TO_NODE_ROUTINE_INTERNAL(ARG1, ARG2, ARG3, ARG4, ARG5) \
	static_cast<InternalNode *>(node)->ARG1(\
	node->getParent()->getElement(parentIndex ARG2), static_cast<InternalNode *>(brotherNode)->ARG3()); \
	static_cast<InternalNode *>(brotherNode)->ARG3()->setParent(static_cast<InternalNode *>(node)); \
	node->increaseSizeByOne(); \
	node->getParent()->assignElement(parentIndex ARG2, std::move(brotherNode->ARG4())); \
	static_cast<InternalNode *>(brotherNode)->ARG5(); \
	brotherNode->decreaseSizeByOne();

template <typename T, typename Comp, typename K, typename ExtractKey>
void
B_TREE_BASE<T, Comp, K, ExtractKey>::addToInternalNodeFromLeft(Node *node, Node *brotherNode, int parentIndex) {
	ADD_TO_NODE_ROUTINE_INTERNAL(insertFirstElementAndChild, -1, getLastChild, getLastElement, eraseLastElementAndChild)
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void
B_TREE_BASE<T, Comp, K, ExtractKey>::addToInternalNodeFromRight(Node *node, Node *brotherNode, int parentIndex) {
	ADD_TO_NODE_ROUTINE_INTERNAL(insertLastElementAndChild, , getFirstChild, getFirstElement, eraseFirstElementAndChild)
}
#undef ADD_TO_NODE_ROUTINE_INTERNAL

template <typename T, typename Comp, typename K, typename ExtractKey>
bool
B_TREE_BASE<T, Comp, K, ExtractKey>::addToNode(Node *node, int parentIndex) {
	if (root_ == node) { // should not have been called
		std::cout << "addToNode called on the root node\n";
		return false;
	}
	if (node->getSize() != node->t - 1) {
		std::cout << "addToNode called on not t-1 node\n";
		return false;
	}	
	// getRightBrother returns null if this is the rightmost children
	Node *brotherNode = NULL;
	if (parentIndex < node->getParent()->getSize()) { 
		brotherNode = static_cast<InternalNode *>(node->getParent())-> 
			getChild(parentIndex + 1); 
		if (brotherNode->isLeast()) {
			return false;
		}
		if (node->isLeafNode()) {
			addToLeafNodeFromRight(node, brotherNode, parentIndex);
		} else {
			addToInternalNodeFromRight(node, brotherNode, parentIndex);
		}
	} else { /* this is the rightmost child; look into its left brother */ 
		brotherNode = static_cast<InternalNode *>(node->getParent())-> 
			getChild(parentIndex - 1); 
		if (brotherNode->isLeast()) {
			return false;
		}
		if (node->isLeafNode()) {
			addToLeafNodeFromLeft(node, brotherNode, parentIndex);
		} else {
			addToInternalNodeFromLeft(node, brotherNode, parentIndex);
		}
	} 	
	return true;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::Node *
B_TREE_BASE<T, Comp, K, ExtractKey>::mergeNode(Node *node, int parentIndex) {	
	if (node == root_) { // should not have been called
		std::cout << "mergeNode called on the root node\n";
		return NULL;
	}  
	if (node->getSize() != node->t - 1) {
		std::cout << "mergeNode called on not t-1 node\n";
		return NULL;
	}	
	Node *brotherNode = NULL;
	if (parentIndex < node->getParent()->getSize()) { 
		brotherNode = static_cast<InternalNode *>(node->getParent())-> 
			getChild(parentIndex + 1); 
	} else { // using a trick: exchange node and brotherNode for symmetricity
		brotherNode = static_cast<InternalNode *>(node->getParent())-> 
			getChild(parentIndex - 1); 
		Node *temp = node;
		node = brotherNode;
		brotherNode = temp;
		parentIndex--; // due to left-right exchange
	}	
	node->reserve(B_TREE_FULL_NODE_CAPACITY);
	if (node->isLeafNode() == false) { // copy children and change parent
		node->receiveRightHalfFrom(brotherNode);
		node->setSize(B_TREE_FULL_NODE_CAPACITY);
		for (int i = 0; i < node->t; ++i) {
			static_cast<InternalNode *>(node)->setChild(i + node->t, 
				static_cast<InternalNode *>(brotherNode)->getChild(i));
			static_cast<InternalNode *>(brotherNode)->getChild(i)->setParent(static_cast<InternalNode *>(node));
		}
		node->constructElement(node->t - 1, std::move(node->getParent()->getElement(parentIndex)));
	} else { // leaf
#ifdef BPLUS
		node->receiveRightHalfFromBPlusLeaf(brotherNode); 
		node->setSize(B_TREE_FULL_NODE_CAPACITY - 1);
		node->setNext(brotherNode->getNext());
		if (brotherNode->getNext()) {
			brotherNode->getNext()->setPrevious(node);
		}
#else 
		node->receiveRightHalfFrom(brotherNode);
		node->setSize(B_TREE_FULL_NODE_CAPACITY);
		node->constructElement(node->t - 1, std::move(node->getParent()->getElement(parentIndex)));
#endif
	}

	operator delete(brotherNode);
	// erase one element and one child from the parent
	static_cast<InternalNode *>(node->getParent())->eraseElementAndChild(parentIndex);
	node->getParent()->decreaseSizeByOne();
	// special case for root
	if (node->getParent() == root_ && root_->getSize() == 0) {
		//std::cout << "freeing root\n";		
		operator delete(root_);
		root_ = node;
		root_->setParent(NULL);
	}
	return node;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::Node *
B_TREE_BASE<T, Comp, K, ExtractKey>::addToOrMerge(Node *node, int parentIndex) {
	bool addToExcuted = addToNode(node, parentIndex);
	if (addToExcuted == false) {
		return mergeNode(node, parentIndex);
	} else {
		return node;
	}
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::Node *
B_TREE_BASE<T, Comp, K, ExtractKey>::addToOrMergeIfNeeded(Node *node, int parentIndex) {
	if (node->isLeast()) {
		return addToOrMerge(node, parentIndex);
	} else {		
		return node;
	}
}

template <typename T, typename Comp, typename K, typename ExtractKey>
T const
B_TREE_BASE<T, Comp, K, ExtractKey>::
deleteLeftMost(Node *node, int parentIndex) {
	node = addToOrMergeIfNeeded(node, parentIndex);
	if (node->isLeafNode() == true) {
		return this->deleteValue(node, 0);
	} else {
		return deleteLeftMost(
			static_cast<InternalNode *>(node)->getFirstChild(), 0);
	}
}

template <typename T, typename Comp, typename K, typename ExtractKey>
T const
B_TREE_BASE<T, Comp, K, ExtractKey>::deleteRightMost(
		Node *node, int parentIndex) {
	node = addToOrMergeIfNeeded(node, parentIndex);
	if (node->isLeafNode() == true) {
		return this->deleteValue(node, node->getSize() - 1);
	} else {
		return deleteRightMost(
			static_cast<InternalNode *>(node)->getLastChild(), node->getSize());
	}
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::Node *
B_TREE_BASE<T, Comp, K, ExtractKey>::makeNewLeafNode(int capacity) {
	//std::cout << "size of elem is " << sizeof(T) << "\n";
	//std::cout << "size of btree is " << sizeof(B_TREE_BASE<T, Comp, K, ExtractKey>) << "\n";
	//std::cout << "size of node is " << sizeof(B_TREE_BASE<T, Comp, K, ExtractKey>::Node) << "\n";
	//std::cout << "size of fullnode is " << sizeof(B_TREE_BASE<T, Comp, K, ExtractKey>::FullNode) << "\n";
	//std::cout << "size of internal node is " << sizeof(B_TREE_BASE<T, Comp, K, ExtractKey>::InternalNode) << "\n";
	//std::cout << "size of full internal node is " << sizeof(B_TREE_BASE<T, Comp, K, ExtractKey>::FullInternalNode) << "\n";
	Node *newNode = NULL;
	if (capacity == B_TREE_FULL_NODE_CAPACITY) {
		FullNode *fullNode = static_cast<FullNode *>(operator new(sizeof(FullNode)));
		fullNode->elements_ = fullNode->elements2_;
		newNode = fullNode;
	} else {
		if (capacity < B_TREE_END_NODE_CAPACITY) {
			std::cout << "makeNewLeafNode error\n";
			exit(0);
		}
		newNode = static_cast<Node *>(operator new(sizeof(Node)));
		if (capacity > B_TREE_END_NODE_CAPACITY) {
			newNode->elements_ = static_cast<T *>(operator new(sizeof(T) * capacity));
		} else if (capacity == B_TREE_END_NODE_CAPACITY) {	
			newNode->elements_ = NULL;
		}
	}
	newNode->isLeafNode_ = true;
	newNode->setSize(0);
	newNode->setParent(NULL);
	newNode->setIndexInParent(-1);
	newNode->capacity_ = capacity;
#ifdef BPLUS
	newNode->setNext(NULL);
	newNode->setPrevious(NULL);
#endif
	return newNode;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
typename B_TREE_BASE<T, Comp, K, ExtractKey>::InternalNode *
B_TREE_BASE<T, Comp, K, ExtractKey>::makeNewInternalNode(int capacity) {
	InternalNode *newNode = NULL;
	if (capacity == B_TREE_FULL_NODE_CAPACITY) {
		FullInternalNode *fullNode = static_cast<FullInternalNode *>(operator new(sizeof(FullInternalNode)));
		fullNode->elements_ = fullNode->elements2_;
		fullNode->children_ = fullNode->children2_;
		newNode = fullNode;
	} else if (capacity == B_TREE_END_NODE_CAPACITY) {
		newNode = static_cast<InternalNode *>(operator new(sizeof(InternalNode)));
		newNode->elements_ = NULL;
		newNode->children_ = static_cast<Node **>(operator new(sizeof(Node *) * (capacity + 1)));
	} else {
		std::cout << "makeNewInternalNode error\n";
		exit(0);
	}
	newNode->isLeafNode_ = false;
	newNode->setSize(0);
	newNode->setParent(NULL);
	newNode->setIndexInParent(-1);
	newNode->capacity_ = capacity;
#ifdef BPLUS
	newNode->setNext(NULL);
	newNode->setPrevious(NULL);
#endif
	return newNode;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void
B_TREE_BASE<T, Comp, K, ExtractKey>::print() const {
	printTree(root_);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void
B_TREE_BASE<T, Comp, K, ExtractKey>::printNode(Node *node) const {
	for (int i = 0; i < node->getSize(); ++i) {
		std::cout << node->getElement(i) << " ";
	}
	std::cout << "\n";
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void
B_TREE_BASE<T, Comp, K, ExtractKey>::printTree(Node *node) const {
	if (node->isLeafNode() == false) {
		for (int i = 0; i < node->getSize(); ++i) {
			printTree(static_cast<InternalNode *>(node)->getChild(i));
			std::cout << node->getElement(i);
			std::cout << "\n";
		}
		printTree(static_cast<InternalNode *>(node)->getLastChild());
	} else {
		printNode(node);
	}
}

template <typename T, typename Comp, typename K, typename ExtractKey>
std::pair<int, bool> 
B_TREE_BASE<T, Comp, K, ExtractKey>::getIndexToInsert(Node *node, T const &elem) const {
	K const &key = ExtractKey()(elem);
	return getIndexToInsert2(node, key);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
std::pair<int, bool> 
B_TREE_BASE<T, Comp, K, ExtractKey>::getIndexToInsert2(Node *node, K const &key) const {
	T *begin = node->getElementArray();
	T *end = node->getElementArray() + node->getSize();
	auto result = lib_calvin_container::getIndexToInsert<T, Comp, K, ExtractKey>(begin, end, key, false);
	return std::pair<int, bool>(static_cast<int>(result.first), result.second);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::copyFrom(B_TREE_BASE const &rhs) {
	// this object is empty at the point of calling this function
	size_ = rhs.size_;
	if (rhs.empty()) { // rhs is empty
		return;
	}
#ifdef BPLUS
	Node *leftMost;
	Node *rightMost;
	bTreeNodeCopy(rhs.root_, root_, leftMost, rightMost);
#else
	bTreeNodeCopy(rhs.root_, root_);
#endif
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::bTreeNodeCopy(Node const *source, Node *&target) {
	Node *dummyLeftMost;
	Node *dummyRightMost;
	bTreeNodeCopy(source, target, dummyLeftMost, dummyRightMost);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::bTreeNodeCopy(
		Node const *source, Node *&target, Node *&leftMost, Node *&rightMost) {	
	if (source == NULL) {
		target = NULL;
		return;
	}
	bool isLeafNode = source->isLeafNode();
	if (isLeafNode == false) { // this is internal	
		InternalNode *realTarget = makeNewInternalNode(source->getCapacity());
		bTreeNodeElemCopy(source, realTarget);
		InternalNode const *realSource = static_cast<InternalNode const *>(source);
#ifdef BPLUS
		Node *previousRightMostOfChild = NULL;
#endif
		for (int i = 0; i <= realSource->getSize(); ++i) {
			Node *target;
			Node *leftMostOfChild;
			Node *rightMostOfChild;
			bTreeNodeCopy(realSource->getChild(i), target, leftMostOfChild, rightMostOfChild);
			realTarget->setChild(i, target);
			realTarget->getChild(i)->setParent(realTarget);
#ifdef BPLUS
			if (i == 0) { 
				leftMost = leftMostOfChild;
				previousRightMostOfChild = rightMostOfChild;
			} else {
				previousRightMostOfChild->setNext(leftMostOfChild);
				leftMostOfChild->setPrevious(previousRightMostOfChild);
				if (i == realSource->getSize()) {
					rightMost = rightMostOfChild;
				} else {
					previousRightMostOfChild = rightMostOfChild;
				}
			}
#endif
		}
		target = realTarget;
	} else { // leaf node		
		target = makeNewLeafNode(source->getCapacity());
		bTreeNodeElemCopy(source, target);
#ifdef BPLUS
		leftMost = target;
		rightMost = target;
#endif
	}
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::bTreeNodeElemCopy(Node const *source, Node *target) {
	target->setSize(source->getSize());
	copyConstruct(source->getElementArray(), target->getElementArray(), source->getSize());
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::bTreeNodeDelete(Node *node) {
	if (node == NULL) {
		return;
	}
	bool isLeafNode = node->isLeafNode();
	if (isLeafNode == false) { // this is internal
		for (int i = 0; i <= node->getSize(); ++i) {
			bTreeNodeDelete(static_cast<InternalNode *>(node)->getChild(i));
		}
	} 
	bTreeNodeDeleteElement(node);
	operator delete(node);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
void B_TREE_BASE<T, Comp, K, ExtractKey>::bTreeNodeDeleteElement(Node *node) {
	for (int i = 0; i <  node->getSize(); ++i) {
		node->getElement(i).~T();
	}
}

template <typename T, typename Comp, typename K, typename ExtractKey>
bool operator==(B_TREE_BASE<T, Comp, K, ExtractKey> const &lhs, 
		B_TREE_BASE<T, Comp, K, ExtractKey> const &rhs) {
	return containerEqual(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename Comp, typename K, typename ExtractKey>
bool operator!=(B_TREE_BASE<T, Comp, K, ExtractKey> const &lhs, 
		B_TREE_BASE<T, Comp, K, ExtractKey> const &rhs) {
	return !(lhs == rhs);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
bool operator< (B_TREE_BASE<T, Comp, K, ExtractKey> const &lhs, 
		B_TREE_BASE<T, Comp, K, ExtractKey> const &rhs) {
	return containerLess(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename Comp, typename K, typename ExtractKey>
bool operator<= (B_TREE_BASE<T, Comp, K, ExtractKey> const &lhs, 
		B_TREE_BASE<T, Comp, K, ExtractKey> const &rhs) {
	return !(lhs > rhs);
}

template <typename T, typename Comp, typename K, typename ExtractKey>
bool operator> (B_TREE_BASE<T, Comp, K, ExtractKey> const &lhs, 
		B_TREE_BASE<T, Comp, K, ExtractKey> const &rhs) {
	return rhs < lhs;
}

template <typename T, typename Comp, typename K, typename ExtractKey>
bool operator>= (B_TREE_BASE<T, Comp, K, ExtractKey> const &lhs, 
		B_TREE_BASE<T, Comp, K, ExtractKey> const &rhs) {
	return !(lhs < rhs);
}

#undef B_TREE_BASE

