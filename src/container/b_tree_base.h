#ifdef BPLUS
#define B_TREE_BASE BPlusTree
#else
#define B_TREE_BASE BTree
#endif

#define TREE_SIZE

#include "container.h"

namespace lib_calvin_container
{
template <typename T, typename K = T, typename Comp = std::less<K>, typename ExtractKey = Identity<T>>
class B_TREE_BASE
{
private:
	static int64_t const NODE_BYTES = 300;
	// all leaf nodes are full except leaf root
	// only leaf root grows (by reserve())
	// B_TREE_FULL_NODE_CAPACITY must be odd
	static int64_t const B_TREE_FULL_NODE_CAPACITY =
		NODE_BYTES / sizeof(T) > 1 ? NODE_BYTES / sizeof(T) + (NODE_BYTES / sizeof(T)) % 2 + 1 : 3;
	static int64_t const B_TREE_ROOT_INIT_CAPACITY = 1;
	static int64_t const B_TREE_NODE_REALLOC_MULTIPLIER = 2;
	static bool const USE_BINARY_SEARCH = sizeof(T) > 16;
	class Node;

	// btree node. number of links in a node: t ~ 2t, number of elements in a node:
	// t-1 ~ 2t-1. Therefore, maxNumKeys = 2t-1
	class Node
	{
	private:
		friend B_TREE_BASE;

	protected:
	public:
		int64_t getCapacity() const;
		int64_t getSize() const;
#ifdef TREE_SIZE
		size_t getTreeSize() const;
		void setTreeSize(size_t);
		void refreshTreeSize();
		void increaseTreeSizeByOne();
		void decreaseTreeSizeByOne();
		void increaseTreeSizeByOne(size_t childIndex);
		void decreaseTreeSizeByOne(size_t childIndex);
#endif
		void setSize(int64_t size);
		Node *getParent();
		int64_t getIndexInParent() const;
		void setParent(Node *parent);
		bool isLeafNode() const; // used to record type information
	protected:
		void setIndexInParent(int64_t index);

	public:
		// 0 <= index < size_
		T &getElement(int64_t index);
		T const &getElement(int64_t index) const;
		T const &getFirstElement() const;
		T const &getLastElement() const;
		void constructElement(int64_t index, T const &elem);
		void constructElement(int64_t index, T &&elem);
		void assignElement(int64_t index, T const &elem);
		void assignElement(int64_t index, T &&elem);
		// below insert and erase method does not change size_ value!
		template <typename T1>
		void insertElement(int64_t index, T1 &&elem);
		// for addToNode method
		template <typename T1>
		void insertFirstElement(T1 &&elem);
		template <typename T1>
		void insertLastElement(T1 &&elem);
		// for mergeNode method, tree::delete method
		T const eraseElement(int64_t index);
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
		static int64_t const t = (B_TREE_FULL_NODE_CAPACITY + 1) / 2;
		bool isFull() const;
		bool isLeast() const;
#ifdef BPLUS
		Node *getNext();
		Node *getPrevious();
		void setNext(Node *next);
		void setPrevious(Node *previous);
#endif
	public:
		// 0 <= index <= size_
		Node *getChild(int64_t index);
		Node const *getChild(int64_t index) const;
		void setChild(int64_t index, Node *child);
		Node *getFirstChild();
		Node *getLastChild();
		// return the index of the child
		int64_t findChild(Node const *child) const;
		// insert right before index'th element (index == 0 means at first, index == size means at last)
		// insert and erase methods does not change size_!
		template <typename T1>
		void insertElementAndChild(int64_t index, T1 &&elem, Node *child);
		void insertChild(int64_t index, Node *child);
		// for addToNode method
		template <typename T1>
		void insertFirstElementAndChild(T1 &&elem, Node *child);
		template <typename T1>
		void insertLastElementAndChild(T1 &&elem, Node *child);
		void eraseFirstElementAndChild();
		void eraseLastElementAndChild();
		// for mergeNode method
		void eraseChild(int64_t index);
		void eraseElementAndChild(int64_t index);

	public:
		T *getElementArray();
		T const *getElementArray() const;

	private:
		void init();
		int64_t getTargetCapacity(int64_t capacity) const;

	private:
		struct BasicFields
		{
			int64_t size_; // current number keys in this node
			int64_t capacity_;
			bool isLeafNode_;
			Node *parent_;
			int64_t indexInParent_;
#ifdef BPLUS
			Node *next_;
			Node *previous_;
#endif
		};
		struct LeafFields : public BasicFields
		{
			char elements_[sizeof(T) * B_TREE_FULL_NODE_CAPACITY / sizeof(char)];
		};
		struct InternalFields : public LeafFields
		{
			Node *children_[B_TREE_FULL_NODE_CAPACITY + 1];
#ifdef TREE_SIZE
			size_t treeSize_;
			size_t treeSize2_[B_TREE_FULL_NODE_CAPACITY + 1];
#endif
		};
		InternalFields fields_;
	};

public:
	typedef T value_type;
	typedef T *pointer;
	typedef T &reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

public:
	// B_TREE_BASE uses bidirectional iterator
	// This class (const iterator type) forms base to four kinds of real iterator
	class IteratorImpl
	{
	public:
		typedef T value_type;
		typedef T const *pointer;
		typedef T const &reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef std::bidirectional_iterator_tag iterator_category;
		reference operator*() const;
		pointer operator->() const;

	private:
		typedef B_TREE_BASE<T, K, Comp, ExtractKey> TreeType;

	public:
		friend TreeType;

	public:
		IteratorImpl();
		IteratorImpl(TreeType const *tree, Node *node, int64_t index = 0);
		IteratorImpl(IteratorImpl const &rhs);
		IteratorImpl &operator=(IteratorImpl const &rhs);

	public:
		IteratorImpl &operator++(); // prefix
		IteratorImpl &operator--();
		IteratorImpl const operator++(int); // suffix
		IteratorImpl const operator--(int);

	public:
		bool operator==(IteratorImpl const &rhs) const;
		bool operator!=(IteratorImpl const &rhs) const;

	private:
		TreeType const *const tree_;
		Node *node_;
		int64_t index_;
	};
	friend IteratorImpl;

public:
	typedef lib_calvin_container::Iterator<IteratorImpl> iterator;
	typedef lib_calvin_container::ConstIterator<IteratorImpl> const_iterator;
	typedef lib_calvin_container::ReverseIterator<IteratorImpl> reverse_iterator;
	typedef lib_calvin_container::ConstReverseIterator<IteratorImpl> const_reverse_iterator;

public:
	B_TREE_BASE<T, K, Comp, ExtractKey>();
	// copy constructor
	B_TREE_BASE<T, K, Comp, ExtractKey>(B_TREE_BASE<T, K, Comp, ExtractKey> const &);
	B_TREE_BASE<T, K, Comp, ExtractKey>(B_TREE_BASE<T, K, Comp, ExtractKey> &&);
	// assignment
	B_TREE_BASE<T, K, Comp, ExtractKey> &operator=(B_TREE_BASE<T, K, Comp, ExtractKey> const &);
	B_TREE_BASE<T, K, Comp, ExtractKey> &operator=(B_TREE_BASE<T, K, Comp, ExtractKey> &&);
	void swap(B_TREE_BASE<T, K, Comp, ExtractKey> &rhs);
	~B_TREE_BASE<T, K, Comp, ExtractKey>();

public:
	size_t size() const;
	const_iterator find(K const &) const;
	iterator find(K const &);
	// semi-random access methods
	const_iterator at(size_t) const;
	iterator at(size_t);
	size_t index_of(const_iterator) const; // inverse of at()
	size_t count(K const &key) const;
	std::pair<iterator, bool> insert(T const &);
	std::pair<iterator, bool> insert(T &&);
	// Insert range [beg, end)
	template <typename InputIterator>
	void insert(InputIterator beg, InputIterator end);
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
	template <typename T1>
	std::pair<iterator, bool> insert_(T1 &&);
	Node *getFirstNode() const;
	Node *getLastNode() const;
	Node *getFirstOrLastNode(bool isFirst) const;
	IteratorImpl findIterator(K const &key) const;
	std::pair<IteratorImpl, bool> lowerBoundIterator(K const &key) const;
	IteratorImpl findIteratorAt(size_t index) const;
	IteratorImpl makeIterator(Node *node, int64_t index = 0) const;
	// insert certain elem into the right position in the node,
	// and split the node if necessary
	template <typename T1>
	std::pair<iterator, bool> insertIntoNode(Node *node, T1 &&elem, int64_t parentIndex);
	// delete certain elem from the the recursively
	int64_t deleteFromNode(Node *node, K const &key, int64_t parentIndex);
	// insert value into a leaf node
	template <typename T1>
	std::pair<iterator, bool> insertValue(Node *node, T1 &&elem);
	template <typename T1>
	void insertValue(Node *node, T1 &&elem, int64_t indexToInsert);
	// delete value from a leaf node
	T const deleteValue(Node *node, int64_t indexToDelete);
	// splitNode returns the pointer to newly created brother node
	Node *splitNode(Node *node, int64_t parentIndex);
	// add one more element to a node from its brother
	// look into its right brother unless it is the rightmost children
	// returns whether the operation actually took place
	void addToLeafNodeFromLeft(Node *node, Node *brother, int64_t parentIndex);
	void addToLeafNodeFromRight(Node *node, Node *brother, int64_t parentIndex);
	void addToInternalNodeFromLeft(Node *node, Node *brother, int64_t parentIndex);
	void addToInternalNodeFromRight(Node *node, Node *brother, int64_t parentIndex);
	bool addToNode(Node *node, int64_t parentIndex);
	// merge with its brother (other things are similar to above method)
	// returns the merged node
	Node *mergeNode(Node *node, int64_t parentIndex);
	Node *addToOrMerge(Node *node, int64_t parentIndex);
	Node *addToOrMergeIfNeeded(Node *node, int64_t parentIndex);
	// deleting successor
	T const deleteLeftMost(Node *node, int64_t parentIndex);
	// deleting predecessor
	T const deleteRightMost(Node *node, int64_t parentIndex);
	Node *makeNewLeafNode(size_t capacity);
	Node *makeNewInternalNode();
	void initNode(Node *, size_t capacity) const;

private:
	// Returns true when given element can be inserted (not already exists)
	std::pair<int64_t, bool> getIndexToInsert(Node *node, T const &elem) const;
	std::pair<int64_t, bool> getIndexToInsert2(Node *node, K const &key) const;

private:
	// subroutine for copy con & copy assign. Should be called only when in initial state.
	void copyFrom(B_TREE_BASE<T, K, Comp, ExtractKey> const &);
	void bTreeNodeCopy(Node const *source, Node *&target);
	void bTreeNodeCopy(Node const *source, Node *&target, Node *&leftMode, Node *&rightMost);
	// target node is assumed empty
	void bTreeNodeContentCopy(Node const *source, Node *target);
	void deleteTree(Node *node);
	void deleteNode(Node *node);

private:
	// for testing
	void addToNodeTest();
	void mergeNodeTest();
	void print() const;
	void printNode(Node *) const;
	void printTree(Node *) const;

private:
	Node *root_;

public:
	friend BTreeTest<T>;

public:
	static void countNodes();

private:
	static size_t nodeCount_;
};

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator==(B_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
				B_TREE_BASE<T, K, Comp, ExtractKey> const &rhs);

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator!=(B_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
				B_TREE_BASE<T, K, Comp, ExtractKey> const &rhs);

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator<(B_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
			   B_TREE_BASE<T, K, Comp, ExtractKey> const &rhs);

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator<=(B_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
				B_TREE_BASE<T, K, Comp, ExtractKey> const &rhs);

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator>(B_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
			   B_TREE_BASE<T, K, Comp, ExtractKey> const &rhs);

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator>=(B_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
				B_TREE_BASE<T, K, Comp, ExtractKey> const &rhs);

//--------------------------  node implementations -------------------------//

template <typename T, typename K, typename Comp, typename ExtractKey>
int64_t B_TREE_BASE<T, K, Comp, ExtractKey>::Node::getCapacity() const
{
	return fields_.capacity_;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
int64_t B_TREE_BASE<T, K, Comp, ExtractKey>::Node::getSize() const
{
	return fields_.size_;
}

#ifdef TREE_SIZE
template <typename T, typename K, typename Comp, typename ExtractKey>
size_t B_TREE_BASE<T, K, Comp, ExtractKey>::Node::getTreeSize() const
{
	if (isLeafNode())
	{
		return fields_.size_;
	}
	else
	{
		return fields_.treeSize_;
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::setTreeSize(size_t treeSize)
{
	fields_.treeSize_ = treeSize;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::refreshTreeSize()
{
	if (isLeafNode())
	{
		return;
	}
	else
	{
		//std::cout << "refreshTreeSize called\n";
		size_t treeSize = 0;
#ifndef BPLUS
		treeSize += getSize(); // num elements in the node itself
#endif
		for (int64_t i = 0; i <= getSize(); i++)
		{
			Node *child = getChild(i);
			treeSize += child->getTreeSize();
			fields_.treeSize2_[i] = child->getTreeSize();
			//std::cout << "child tree size at: " << i << " was: " << child->getTreeSize();
		}
		fields_.treeSize_ = treeSize;
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::increaseTreeSizeByOne()
{
	if (getParent())
	{
		getParent()->increaseTreeSizeByOne(fields_.indexInParent_);
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::decreaseTreeSizeByOne()
{
	if (getParent())
	{
		getParent()->decreaseTreeSizeByOne(fields_.indexInParent_);
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::increaseTreeSizeByOne(size_t childIndex)
{
	fields_.treeSize_++;
	fields_.treeSize2_[childIndex]++;
	if (getParent())
	{
		getParent()->increaseTreeSizeByOne(fields_.indexInParent_);
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::decreaseTreeSizeByOne(size_t childIndex)
{
	fields_.treeSize_--;
	fields_.treeSize2_[childIndex]--;
	if (getParent())
	{
		getParent()->decreaseTreeSizeByOne(fields_.indexInParent_);
	}
}
#endif

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::setSize(int64_t size)
{
	fields_.size_ = size;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::Node *
B_TREE_BASE<T, K, Comp, ExtractKey>::Node::getParent()
{
	return fields_.parent_;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::setParent(Node *parent)
{
	fields_.parent_ = parent;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
int64_t B_TREE_BASE<T, K, Comp, ExtractKey>::Node::getIndexInParent() const
{
	return fields_.indexInParent_;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::setIndexInParent(int64_t index)
{
	fields_.indexInParent_ = index;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool B_TREE_BASE<T, K, Comp, ExtractKey>::Node::isLeafNode() const
{
	return fields_.isLeafNode_;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
T &B_TREE_BASE<T, K, Comp, ExtractKey>::Node::getElement(int64_t index)
{
	return getElementArray()[index];
}

template <typename T, typename K, typename Comp, typename ExtractKey>
T const &B_TREE_BASE<T, K, Comp, ExtractKey>::Node::getElement(int64_t index) const
{
	return getElementArray()[index];
}

template <typename T, typename K, typename Comp, typename ExtractKey>
T const &B_TREE_BASE<T, K, Comp, ExtractKey>::Node::getFirstElement() const
{
	return getElementArray()[0];
}

template <typename T, typename K, typename Comp, typename ExtractKey>
T const &B_TREE_BASE<T, K, Comp, ExtractKey>::Node::getLastElement() const
{
	return getElementArray()[getSize() - 1];
}

template <typename T, typename K, typename Comp, typename ExtractKey>
T *B_TREE_BASE<T, K, Comp, ExtractKey>::Node::getElementArray()
{
	return const_cast<T *>(static_cast<Node const *>(this)->getElementArray());
}

template <typename T, typename K, typename Comp, typename ExtractKey>
T const *B_TREE_BASE<T, K, Comp, ExtractKey>::Node::getElementArray() const
{
	return reinterpret_cast<T const *>(fields_.elements_);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::constructElement(int64_t index, T const &elem)
{
	new (getElementArray() + index) T(elem);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::constructElement(int64_t index, T &&elem)
{
	new (getElementArray() + index) T(std::forward<T>(elem));
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::assignElement(int64_t index, T const &elem)
{
	getElementArray()[index] = elem;
}
template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::assignElement(int64_t index, T &&elem)
{
	getElementArray()[index] = std::forward<T>(elem);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::increaseSizeByOne()
{
	fields_.size_++;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::decreaseSizeByOne()
{
	fields_.size_--;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
template <typename T1>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::insertElement(int64_t index, T1 &&elem)
{
	if (this->isFull())
	{
		std::cout << "inserting into full node\n";
		exit(0);
	}
	if (getSize() == getCapacity())
	{
		std::cout << "inserting into full node2\n";
		exit(0);
	}
	insertSingleElement(getElementArray(), getSize(), index, std::forward<T1>(elem));
}

template <typename T, typename K, typename Comp, typename ExtractKey>
template <typename T1>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::insertFirstElement(T1 &&elem)
{
	insertElement(0, std::forward<T1>(elem));
}

template <typename T, typename K, typename Comp, typename ExtractKey>
template <typename T1>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::insertLastElement(T1 &&elem)
{
	insertElement(getSize(), std::forward<T1>(elem));
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::eraseFirstElement()
{
	eraseElement(0);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::eraseLastElement()
{
	eraseElement(getSize() - 1);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::giveRightHalfTo(Node *target)
{
	moveConstructAndDestruct(getElementArray() + t, target->getElementArray(), t - 1);
	setSize(t);
	target->setSize(t - 1);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::receiveRightHalfFrom(Node *source)
{
	moveConstructAndDestruct(source->getElementArray(), getElementArray() + t, t - 1);
	setSize(B_TREE_FULL_NODE_CAPACITY);
	source->setSize(0);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::receiveRightHalfFromBPlusLeaf(Node *source)
{
	moveConstructAndDestruct(source->getElementArray(), getElementArray() + t - 1, t - 1);
	setSize(B_TREE_FULL_NODE_CAPACITY - 1);
	source->setSize(0);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
T const
B_TREE_BASE<T, K, Comp, ExtractKey>::Node::eraseElement(int64_t index)
{
	T const elem(std::move(getElementArray()[index]));
	eraseSingleElement(getElementArray(), getSize(), index);
	return elem;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool B_TREE_BASE<T, K, Comp, ExtractKey>::Node::isFull() const
{
	return fields_.size_ == B_TREE_FULL_NODE_CAPACITY;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool B_TREE_BASE<T, K, Comp, ExtractKey>::Node::isLeast() const
{
	return fields_.size_ == t - 1;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
int64_t B_TREE_BASE<T, K, Comp, ExtractKey>::Node::getTargetCapacity(int64_t capacity) const
{
	int64_t targetCapacity = getCapacity();
	while (true)
	{
		if (capacity <= targetCapacity)
		{
			break;
		}
		else
		{
			targetCapacity *= B_TREE_NODE_REALLOC_MULTIPLIER;
			if (targetCapacity >= B_TREE_FULL_NODE_CAPACITY)
			{
				targetCapacity = B_TREE_FULL_NODE_CAPACITY;
				break;
			}
		}
	}
	return targetCapacity;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
size_t
	B_TREE_BASE<T, K, Comp, ExtractKey>::nodeCount_ = 0;

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::countNodes()
{
	std::cout << "BTree node current count is: \"" << nodeCount_ << "\": ";
	if (nodeCount_ == 0)
	{
		std::cout << "O.K\n\n";
	}
	else
	{
		std::cout << "memory leak!!!\n";
		exit(0);
	}
}

#ifdef BPLUS
template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::Node *
B_TREE_BASE<T, K, Comp, ExtractKey>::Node::getNext()
{
	return fields_.next_;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::Node *
B_TREE_BASE<T, K, Comp, ExtractKey>::Node::getPrevious()
{
	return fields_.previous_;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::setNext(Node *next)
{
	fields_.next_ = next;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::setPrevious(Node *previous)
{
	fields_.previous_ = previous;
}
#endif

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::Node *
B_TREE_BASE<T, K, Comp, ExtractKey>::Node::getChild(int64_t index)
{
	return fields_.children_[index];
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::Node const *
B_TREE_BASE<T, K, Comp, ExtractKey>::Node::getChild(int64_t index) const
{
	return fields_.children_[index];
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::setChild(int64_t index, Node *child)
{
	fields_.children_[index] = child;
	child->setIndexInParent(index);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::Node *
B_TREE_BASE<T, K, Comp, ExtractKey>::Node::getFirstChild()
{
	return getChild(0);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::Node *
B_TREE_BASE<T, K, Comp, ExtractKey>::Node::getLastChild()
{
	return getChild(getSize());
}

template <typename T, typename K, typename Comp, typename ExtractKey>
int64_t B_TREE_BASE<T, K, Comp, ExtractKey>::Node::findChild(Node const *child) const
{
	return child->getIndexInParent();
}

template <typename T, typename K, typename Comp, typename ExtractKey>
template <typename T1>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::insertElementAndChild(
	int64_t index, T1 &&elem, Node *child)
{
	// insert elem right before index'th elem
	// and insert child right before index + 1'th child
	insertElement(index, std::forward<T1>(elem));
	insertChild(index + 1, child);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::insertChild(int64_t index, Node *child)
{
	for (int64_t i = getSize(); i >= index; --i)
	{
		//children_[i + 1] = children_[i];
		setChild(i + 1, getChild(i));
	}
	//children_[index] = child;
	setChild(index, child);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
template <typename T1>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::insertFirstElementAndChild(
	T1 &&elem, Node *child)
{
	insertChild(0, child);
	insertFirstElement(std::forward<T1>(elem));
}

template <typename T, typename K, typename Comp, typename ExtractKey>
template <typename T1>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::insertLastElementAndChild(
	T1 &&elem, Node *child)
{
	insertChild(getSize() + 1, child);
	insertLastElement(std::forward<T1>(elem));
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::eraseFirstElementAndChild()
{
	eraseChild(0);
	eraseFirstElement();
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::eraseLastElementAndChild()
{
	eraseChild(getSize());
	eraseLastElement();
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::eraseChild(int64_t index)
{
	for (int64_t i = index; i < getSize(); ++i)
	{
		//children_[i] = children_[i + 1];
		setChild(i, getChild(i + 1));
	}
	fields_.children_[getSize()] = NULL;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::Node::eraseElementAndChild(int64_t index)
{
	eraseChild(index + 1);
	eraseElement(index);
}

//----------------------- iterator implementation ------------------//

template <typename T, typename K, typename Comp, typename ExtractKey>
B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::IteratorImpl() : tree_(NULL), node_(NULL), index_(0) {}

template <typename T, typename K, typename Comp, typename ExtractKey>
B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::IteratorImpl(
	B_TREE_BASE<T, K, Comp, ExtractKey> const *tree, Node *node, int64_t index) : tree_(tree), node_(node), index_(index) {}

template <typename T, typename K, typename Comp, typename ExtractKey>
B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::IteratorImpl(IteratorImpl const &rhs) : tree_(rhs.tree_), node_(rhs.node_), index_(rhs.index_) {}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl &
B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::operator=(IteratorImpl const &rhs)
{
	const_cast<TreeType const *>(tree_) = rhs.tree_;
	node_ = rhs.node_;
	index_ = rhs.index_;
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::reference
	B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::operator*() const
{
	return node_->getElement(index_);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::pointer
	B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::operator->() const
{
	return &node_->getElement(index_);
}

//------------------------------------------------------- MACRO BEGIN -----//
#define ITERATOR_MOVE_ROUTINE(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) \
	if (node_->isLeafNode() == true)                                    \
	{ /* leaf node */                                                   \
		if (index_ ARG0)                                                \
		{ /* not last(first) element */                                 \
			index_ ARG1;                                                \
		}                                                               \
		else                                                            \
		{ /* last element of a leaf */                                  \
			while (true)                                                \
			{                                                           \
				if (node_->getParent())                                 \
				{ /* not root */                                        \
					if (node_->getIndexInParent() ARG2)                 \
					{ /* yes, return this */                            \
						index_ = node_->getIndexInParent() ARG3;        \
						node_ = node_->getParent();                     \
						break;                                          \
					}                                                   \
					else                                                \
					{ /* go upward again */                             \
						node_ = node_->getParent();                     \
					}                                                   \
				}                                                       \
				else                                                    \
				{ /* cannot go upward, return end() */                  \
					node_ = NULL;                                       \
					index_ = 0;                                         \
					break;                                              \
				}                                                       \
			}                                                           \
		}                                                               \
	}                                                                   \
	else                                                                \
	{ /* internal node */                                               \
		Node *temp =                                                    \
			static_cast<Node *>(node_)->getChild(index_ ARG4);          \
		while (temp->isLeafNode() == false)                             \
		{                                                               \
			temp = static_cast<Node *>(temp)->getChild(ARG5);           \
		}                                                               \
		node_ = temp;                                                   \
		index_ = ARG6;                                                  \
	}

#define ITERATOR_INCREASE \
	ITERATOR_MOVE_ROUTINE(< node_->getSize() - 1, ++, < node_->getParent()->getSize(), , +1, 0, 0)

#define ITERATOR_DECREASE \
	ITERATOR_MOVE_ROUTINE(> 0, --, > 0, -1, , temp->getSize(), temp->getSize() - 1)

//------------------------------------------------------- MACRO END -----//

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl &
B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::operator++()
{
#ifdef BPLUS
	index_++;
	if (index_ < node_->getSize())
	{
	}
	else
	{
		node_ = node_->getNext();
		index_ = 0;
	}
#else
	ITERATOR_INCREASE
#endif
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl &
B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::operator--()
{
	if (!node_)
	{ // currently end (null)
		node_ = tree_->getLastNode();
		index_ = node_->getSize() - 1;
		return *this;
	}
#ifdef BPLUS
	index_--;
	if (index_ < 0)
	{
		node_ = node_->getPrevious();
		index_ = node_->getSize() - 1;
	}
#else
	ITERATOR_DECREASE
#endif
	return *this;
}

#undef ITERATOR_MOVE_ROUTINE
#undef ITERATOR_INCREASE
#undef ITERATOR_DECREASE

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl const
B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::operator++(int)
{
	IteratorImpl returnValue(*this);
	operator++();
	return returnValue;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl const
B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::operator--(int)
{
	IteratorImpl returnValue(*this);
	operator--();
	return returnValue;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::operator==(IteratorImpl const &rhs) const
{
	return tree_ == rhs.tree_ && node_ == rhs.node_ && index_ == rhs.index_;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::operator!=(IteratorImpl const &rhs) const
{
	return !(*this == rhs);
}

//--------------------------- tree implementation ------------------//

template <typename T, typename K, typename Comp, typename ExtractKey>
B_TREE_BASE<T, K, Comp, ExtractKey>::B_TREE_BASE() : root_(NULL)
{
	//std::cout << "btree default ctor\n";
}

template <typename T, typename K, typename Comp, typename ExtractKey>
B_TREE_BASE<T, K, Comp, ExtractKey>::B_TREE_BASE(B_TREE_BASE<T, K, Comp, ExtractKey> const &rhs) : root_(NULL)
{
	//std::cout << "btree copy ctor\n";
	copyFrom(rhs);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
B_TREE_BASE<T, K, Comp, ExtractKey>::B_TREE_BASE(B_TREE_BASE<T, K, Comp, ExtractKey> &&rhs) : root_(NULL)
{
	//std::cout << "btree move ctor\n";
	swap(rhs);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
B_TREE_BASE<T, K, Comp, ExtractKey> &
B_TREE_BASE<T, K, Comp, ExtractKey>::operator=(B_TREE_BASE<T, K, Comp, ExtractKey> const &rhs)
{
	//std::cout << "btree copy assignment\n";
	if (this == &rhs)
	{
		return *this;
	}
	clear();
	copyFrom(rhs);
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
B_TREE_BASE<T, K, Comp, ExtractKey> &
B_TREE_BASE<T, K, Comp, ExtractKey>::operator=(B_TREE_BASE<T, K, Comp, ExtractKey> &&rhs)
{
	//std::cout << "btree move assignment\n";
	if (this == &rhs)
	{
		return *this;
	}
	swap(rhs);
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::swap(B_TREE_BASE<T, K, Comp, ExtractKey> &rhs)
{
	Node *tempRoot = root_;
	root_ = rhs.root_;
	rhs.root_ = tempRoot;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
B_TREE_BASE<T, K, Comp, ExtractKey>::~B_TREE_BASE()
{
	deleteTree(root_);
	//countNodes();
}

template <typename T, typename K, typename Comp, typename ExtractKey>
size_t B_TREE_BASE<T, K, Comp, ExtractKey>::size() const
{
	if (root_ == nullptr)
	{
		return 0;
	}
	else
	{
		return root_->getTreeSize();
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
size_t B_TREE_BASE<T, K, Comp, ExtractKey>::count(K const &key) const
{
	if (find(key) == end())
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::const_iterator
B_TREE_BASE<T, K, Comp, ExtractKey>::find(K const &key) const
{
	return findIterator(key);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::iterator
B_TREE_BASE<T, K, Comp, ExtractKey>::find(K const &key)
{
	return findIterator(key);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::const_iterator
B_TREE_BASE<T, K, Comp, ExtractKey>::at(size_t index) const
{
	return findIteratorAt(index);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::iterator
B_TREE_BASE<T, K, Comp, ExtractKey>::at(size_t index)
{
	return findIteratorAt(index);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
size_t
B_TREE_BASE<T, K, Comp, ExtractKey>::index_of(const_iterator iterator) const
{
	IteratorImpl const &impl = iterator.getImpl();
	Node *curNode = impl.node_;
	size_t curIndex = static_cast<size_t>(impl.index_);
	size_t index = 0;
#ifndef BPLUS
	bool startedInInternalNode;
	if (curNode->isLeafNode())
	{
		startedInInternalNode = false;
	}
	else
	{
		curIndex++;
		startedInInternalNode = true;
	}
#endif
	while (true)
	{
		if (curNode->isLeafNode())
		{
			index += curIndex;
		}
		else
		{
#ifndef BPLUS
			index += curIndex; // for elements in internal node
#endif
				// add tree sizes of brothers in left of curNode
			for (size_t i = 0; i < curIndex; i++)
			{
				index += curNode->fields_.treeSize2_[i];
			}
		}
		if (curNode->getParent() == nullptr)
		{
			break;
		}
		else
		{
			curIndex = curNode->getIndexInParent();
			curNode = curNode->getParent();
		}
	}
#ifdef BPLUS
	return index;
#else
	if (startedInInternalNode)
	{
		return index - 1;
	}
	else
	{
		return index;
	}
#endif
}

template <typename T, typename K, typename Comp, typename ExtractKey>
std::pair<typename B_TREE_BASE<T, K, Comp, ExtractKey>::iterator, bool>
B_TREE_BASE<T, K, Comp, ExtractKey>::insert(T const &elem)
{
	return insert_(elem);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
std::pair<typename B_TREE_BASE<T, K, Comp, ExtractKey>::iterator, bool>
B_TREE_BASE<T, K, Comp, ExtractKey>::insert(T &&elem)
{
	return insert_(std::forward<T>(elem));
}

template <typename T, typename K, typename Comp, typename ExtractKey>
template <typename InputIterator>
void B_TREE_BASE<T, K, Comp, ExtractKey>::insert(InputIterator beg, InputIterator end)
{
	while (beg != end)
	{
		insert(*beg++);
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
size_t
B_TREE_BASE<T, K, Comp, ExtractKey>::erase(K const &key)
{
	if (empty())
	{
		return 0;
	}
	else
	{
		int64_t count = deleteFromNode(root_, key, 0);
		return count;
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::clear()
{

	deleteTree(root_);
	root_ = NULL;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool B_TREE_BASE<T, K, Comp, ExtractKey>::empty() const
{
	return root_ == nullptr;
}
template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::Node *
B_TREE_BASE<T, K, Comp, ExtractKey>::getFirstNode() const
{
	return getFirstOrLastNode(true);
}
template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::Node *
B_TREE_BASE<T, K, Comp, ExtractKey>::getLastNode() const
{
	return getFirstOrLastNode(false);
}
template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::Node *
B_TREE_BASE<T, K, Comp, ExtractKey>::getFirstOrLastNode(bool isFirst) const
{
	if (empty())
	{
		return nullptr;
	}
	Node *node = root_;
	while (node->isLeafNode() == false)
	{
		if (isFirst)
		{
			node = static_cast<Node *>(node)->getFirstChild();
		}
		else
		{
			node = static_cast<Node *>(node)->getLastChild();
		}
	}
	return node;
}
template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::const_iterator
B_TREE_BASE<T, K, Comp, ExtractKey>::begin() const
{
	return makeIterator(this->getFirstNode());
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::const_iterator
B_TREE_BASE<T, K, Comp, ExtractKey>::end() const
{
	return makeIterator(NULL);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::iterator
B_TREE_BASE<T, K, Comp, ExtractKey>::begin()
{
	return makeIterator(this->getFirstNode());
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::iterator
B_TREE_BASE<T, K, Comp, ExtractKey>::end()
{
	return makeIterator(NULL);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::const_reverse_iterator
B_TREE_BASE<T, K, Comp, ExtractKey>::rbegin() const
{
	return makeIterator(NULL);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::const_reverse_iterator
B_TREE_BASE<T, K, Comp, ExtractKey>::rend() const
{
	return makeIterator(this->getFirstNode());
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::reverse_iterator
B_TREE_BASE<T, K, Comp, ExtractKey>::rbegin()
{
	return makeIterator(NULL);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::reverse_iterator
B_TREE_BASE<T, K, Comp, ExtractKey>::rend()
{
	return makeIterator(this->getFirstNode());
}

template <typename T, typename K, typename Comp, typename ExtractKey>
template <typename T1>
std::pair<typename B_TREE_BASE<T, K, Comp, ExtractKey>::iterator, bool>
B_TREE_BASE<T, K, Comp, ExtractKey>::insert_(T1 &&elem)
{
	if (empty())
	{ // empty
		Node *newRoot = makeNewLeafNode(B_TREE_ROOT_INIT_CAPACITY);
		root_ = newRoot;
		newRoot->setSize(1);
		newRoot->constructElement(0, std::forward<T1>(elem));
		return std::pair<iterator, bool>(makeIterator(root_, 0), true);
	}
	if (root_ != NULL && root_->getSize() == 0)
	{
		std::cout << "insert error 1\n";
		exit(0);
	}
	// Non-empty
	if (root_->isLeafNode() && root_->getSize() == root_->getCapacity() &&
		!root_->isFull())
	{
		// Need to reallocate root
		size_t newCapacity = root_->getTargetCapacity(root_->getSize() + 1);
		Node *newRoot = makeNewLeafNode(newCapacity);
		moveConstructAndDestruct(root_->getElementArray(), newRoot->getElementArray(), root_->getSize());
		operator delete(root_);
		root_ = newRoot;
	}

	//std::cout << "insert root size: " << root_->getSize() << "\ntree size is: " << size_ << "\n";
	return insertIntoNode(root_, std::forward<T1>(elem), 0);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl
B_TREE_BASE<T, K, Comp, ExtractKey>::findIterator(K const &key) const
{
	std::pair<IteratorImpl, bool> result = lowerBoundIterator(key);
	if (result.second == false)
	{ // element was not present
		return makeIterator(nullptr);
	}
	else
	{
		return result.first;
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
std::pair<typename B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl, bool>
B_TREE_BASE<T, K, Comp, ExtractKey>::lowerBoundIterator(K const &key) const
{
	if (empty())
	{
		return std::make_pair(makeIterator(nullptr), false);
	}
	Node *thisNode = root_;
	while (true)
	{
		std::pair<int64_t, bool> result = getIndexToInsert2(thisNode, key);
#ifdef BPLUS
		if (thisNode->isLeafNode())
		{
			if (result.second == false)
			{ // found
				return std::make_pair(makeIterator(thisNode, result.first), true);
			}
			else
			{
				return std::make_pair(makeIterator(thisNode, result.first), false);
			}
		}
		else
		{ // internal nodes are not data elements
			thisNode = static_cast<Node *>(thisNode)->getChild(result.first);
		}
#else
		if (result.second == false)
		{ /* we found the element */
			return std::make_pair(makeIterator(thisNode, result.first), true);
		}
		else
		{ /* elements doesn't exist in this node */
			if (thisNode->isLeafNode() == true)
			{
				return std::make_pair(makeIterator(thisNode, result.first), false);
			}
			else
			{
				thisNode = static_cast<Node *>(thisNode)->getChild(result.first);
			}
		}
#endif
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl
B_TREE_BASE<T, K, Comp, ExtractKey>::findIteratorAt(size_t index) const
{
	if (empty())
	{ // empty
		return makeIterator(nullptr);
	}
	Node *currentNode = root_;
	while (true)
	{
#ifdef TREE_SIZE
		if (index >= currentNode->getTreeSize())
		{
			std::cout << "elementAt2 index error\n";
			exit(0);
		}
#endif
		if (currentNode->isLeafNode())
		{
			return makeIterator(currentNode, static_cast<int64_t>(index));
		}
		else
		{
			Node *currentInternalNode = static_cast<Node *>(currentNode);
			for (int64_t i = 0; i <= currentInternalNode->getSize(); i++)
			{
				if (index < currentInternalNode->fields_.treeSize2_[i])
				{
					currentNode = currentInternalNode->getChild(i);
					break;
				}
				else
				{
					index -= currentInternalNode->fields_.treeSize2_[i]; // pass through i'th subtreee
#ifndef BPLUS
					if (index == 0)
					{
						return makeIterator(currentNode, i);
					}
					else
					{
						index--; // pass through i'th element
					}
#endif
				}
			}
		}
	}
	std::cout << "elementAt error\n";
	exit(0);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl
B_TREE_BASE<T, K, Comp, ExtractKey>::makeIterator(Node *node, int64_t index) const
{
	return IteratorImpl(this, node, index);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
template <typename T1>
std::pair<typename B_TREE_BASE<T, K, Comp, ExtractKey>::iterator, bool>
B_TREE_BASE<T, K, Comp, ExtractKey>::insertIntoNode(Node *node, T1 &&elem, int64_t parentIndex)
{
	//std::cout << "insertInto node size: " << node->getSize() << "\n";
	//std::cout << "called insertIntoNode\n";
	std::pair<int64_t, bool> result = getIndexToInsert(node, elem);
	int64_t indexToInsert = result.first;
	if (result.second == false)
	{ // element found
		if (node->isLeafNode())
		{
			return std::pair<iterator, bool>(makeIterator(node, result.first), false);
		}
		else
		{ // bplus tree does not keep elem in internal nodes
#ifndef BPLUS
			return std::pair<iterator, bool>(makeIterator(node, result.first), false);
#endif
		}
	}
	if (node->isLeafNode() == false)
	{ // this is internal node
		Node *childNodeToInsert = static_cast<Node *>(node)->getChild(indexToInsert);
		if (node->isFull())
		{
			// If we are going down with the right part of splitted node
			if (Comp()(ExtractKey()(node->getElement(node->t - 1)), ExtractKey()(elem)))
			{
				// Integrity check
				if ((indexToInsert >= node->t) == false)
				{
					std::cout << "insertIntoNode integrity error\n";
					exit(0);
				}
				indexToInsert -= node->t;
			}
			//std::cout << "split node for internal calling\n";
			splitNode(node, parentIndex);
		}
		return insertIntoNode(childNodeToInsert, std::forward<T1>(elem), indexToInsert);
	}
	else
	{ // this is a leaf
		Node *leafNodeToInsert = node;
		if (node->isFull())
		{
			//std::cout << "split node for leaf calling\n";
			Node *newBrotherNode = splitNode(node, parentIndex);
			// if new element is bigger than the center,
			//  we insert into the newly created brother
			if (indexToInsert >= node->t)
			{
				leafNodeToInsert = newBrotherNode; // insert into brother instead
				indexToInsert -= node->t;
			}
		}
		insertValue(leafNodeToInsert, std::forward<T1>(elem), indexToInsert);
		return std::pair<iterator, bool>(makeIterator(leafNodeToInsert, indexToInsert), true);
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
int64_t
B_TREE_BASE<T, K, Comp, ExtractKey>::deleteFromNode(Node *node, K const &key, int64_t parentIndex)
{
	if (node != root_)
	{
		node = addToOrMergeIfNeeded(node, parentIndex);
		if (node == NULL)
		{
			std::cout << "node=null\n";
			exit(0);
		}
		//printNode(node);
	}
	int64_t returnValue = 0;
	std::pair<int64_t, bool> result = getIndexToInsert2(node, key);
	if (node->isLeafNode() == false)
	{ // this is internal
		Node *thisNode = static_cast<Node *>(node);
		Node *childNodeToDeleteFrom = thisNode->getChild(result.first);
#ifdef BPLUS
		return deleteFromNode(childNodeToDeleteFrom, key, result.first);
#else
		if (result.second == true)
		{ // not exist in this node
			//std::cout << "deleting: key not found in internal node\n";
			return (deleteFromNode(childNodeToDeleteFrom, key, result.first));
		}
		else
		{ // key found in this node
			//std::cout << "deleting: key found in internal node\n";
			if (thisNode->getChild(result.first)->isLeast() == false)
			{ // left side
				// find the predecessor of the node and delete it
				T predecessor =
					deleteRightMost(thisNode->getChild(result.first), result.first);
				node->assignElement(result.first, std::move(predecessor));
				returnValue = 1;
			}
			else if (thisNode->getChild(result.first + 1)->isLeast() == false)
			{ // right
				// find the successor of the node and delete it
				T successor =
					deleteLeftMost(thisNode->getChild(result.first + 1), result.first + 1);
				node->assignElement(result.first, std::move(successor));
				returnValue = 1;
			}
			else
			{ // both children are least; so merge them and recusive call to it
				Node *mergedNode =
					mergeNode(thisNode->getChild(result.first), result.first);
				return (deleteFromNode(mergedNode, key, result.first));
			}
		}
#endif
	}
	else
	{ // leaf node
		if (result.second == true)
		{
			// not exist in this tree at all
			//std::cout << "deleting: key not found in leaf node\n";
			returnValue = 0;
		}
		else
		{ // exists
			//std::cout << "deleting: key found in leaf node\n";
			deleteValue(node, result.first);
			if (node == root_ && node->getSize() == 0)
			{ // now empty
				deleteNode(root_);
				root_ = NULL;
			}
			returnValue = 1;
		}
	}
	return returnValue;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
template <typename T1>
std::pair<typename B_TREE_BASE<T, K, Comp, ExtractKey>::iterator, bool>
B_TREE_BASE<T, K, Comp, ExtractKey>::insertValue(Node *node, T1 &&elem)
{
	//std::cout << "called insertValue1\n";
	if (node->isLeafNode() == false)
	{
		std::cout << "insertValue error: is not a leaf node\n";
		exit(0);
	}
	if (node->isFull() == true)
	{
		std::cout << "insertValue error: node is full\n";
		exit(0);
	}
	std::pair<int64_t, bool> result = getIndexToInsert(node, elem);
	if (result.second == false)
	{
		return false;
	}
	else
	{
		insertValue(node, std::forward<T1>(elem), result.first);
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
template <typename T1>
void B_TREE_BASE<T, K, Comp, ExtractKey>::insertValue(
	Node *node, T1 &&elem, int64_t indexToInsert)
{
	node->insertElement(indexToInsert, std::forward<T1>(elem));
	node->increaseSizeByOne();
#ifdef TREE_SIZE
	node->increaseTreeSizeByOne();
#endif
}

template <typename T, typename K, typename Comp, typename ExtractKey>
T const
B_TREE_BASE<T, K, Comp, ExtractKey>::deleteValue(Node *node, int64_t indexToDelete)
{
	T const deletedElement = (node->eraseElement(indexToDelete));
	node->decreaseSizeByOne();
#ifdef TREE_SIZE
	node->decreaseTreeSizeByOne();
#endif
	return deletedElement;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::Node *
B_TREE_BASE<T, K, Comp, ExtractKey>::splitNode(Node *node, int64_t parentIndex)
{
	//std::cout << "splitnode called with: " << parentIndex << "\n";
	int64_t t = node->t;
	// possible elems in a node: t-1 ~ 2t-1
	// possible number of children in a node: t ~ 2t
	// 2t-1 = maxNumKeys, t = (maxNumKeys-1)/2+1
	// elems: 0 ...  t-2 <t-1> t ... 2t-2
	// links:0 1    t-2  t-1  t ... 2t-2 2t-1
	if (node->isFull() == false)
	{
		std::cout << "splitNode error: number of elems is not 2t - 1\n";
		exit(0);
	}
	if (node != root_ && node->getParent()->isFull())
	{
		std::cout << "splitNode error: parent is full\n";
		exit(0);
	}
	if (node == root_)
	{
		Node *newRoot = makeNewInternalNode();
		node->setParent(newRoot);
		newRoot->setChild(0, node);
		root_ = newRoot; // change the root of this B-tree
	}
	Node *newBrotherNode = NULL;
	if (node->isLeafNode() == false)
	{ // node is internal
		newBrotherNode = makeNewInternalNode();
		//static_cast<Node *>(newBrotherNode)->setTreeSize(0);
		// copy links and change parents
		for (int64_t i = 0; i < t; ++i)
		{
			static_cast<Node *>(newBrotherNode)->setChild(i, static_cast<Node *>(node)->getChild(i + t));
			static_cast<Node *>(newBrotherNode)->getChild(i)->setParent(static_cast<Node *>(newBrotherNode));
		}
	}
	else
	{ // node is leaf
		newBrotherNode = makeNewLeafNode(B_TREE_FULL_NODE_CAPACITY);
	}
	// copy elements and delete original ones
	node->giveRightHalfTo(newBrotherNode);
	newBrotherNode->setParent(node->getParent());
	Node *parent = static_cast<Node *>(node->getParent());
	if (parent->isFull())
	{
		std::cout << "splitNode error: parent is full\n";
		exit(0);
	}

#ifdef BPLUS
	if (node->isLeafNode() == false)
	{
		parent->insertElementAndChild(parentIndex, std::move(node->getElement(t - 1)), newBrotherNode);
		node->getElement(t - 1).~T();
		node->setSize(t - 1);
	}
	else
	{ // leaf
		parent->insertElementAndChild(parentIndex, node->getLastElement(), newBrotherNode);
		newBrotherNode->setNext(node->getNext());
		newBrotherNode->setPrevious(node);
		if (node->getNext())
		{
			node->getNext()->setPrevious(newBrotherNode);
		}
		node->setNext(newBrotherNode);
	}
#else
	// push the middle element into the parent node
	parent->insertElementAndChild(parentIndex, std::move(node->getElement(t - 1)), newBrotherNode);
	node->getElement(t - 1).~T();
	node->setSize(t - 1);
#endif
	parent->increaseSizeByOne();
#ifdef TREE_SIZE
	node->refreshTreeSize();
	newBrotherNode->refreshTreeSize();
	node->getParent()->refreshTreeSize();
#endif

	return newBrotherNode;
}

#ifdef BPLUS
#define ADD_TO_NODE_ROUTINE_LEAF(ARG2, ARG4, ARG6, ARG7, ARG8)    \
	node->ARG7(brotherNode->ARG4());                              \
	node->increaseSizeByOne();                                    \
	brotherNode->ARG6();                                          \
	brotherNode->decreaseSizeByOne();                             \
	/* set the key of parent as the rightmost key of left node */ \
	node->getParent()->assignElement(parentIndex ARG2, ARG8->getLastElement());
#else
#define ADD_TO_NODE_ROUTINE_LEAF(ARG2, ARG4, ARG6, ARG7, ARG8)                          \
	node->ARG7(node->getParent()->getElement(parentIndex ARG2));                        \
	node->increaseSizeByOne();                                                          \
	node->getParent()->assignElement(parentIndex ARG2, std::move(brotherNode->ARG4())); \
	brotherNode->ARG6();                                                                \
	brotherNode->decreaseSizeByOne();
#endif

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::addToLeafNodeFromLeft(Node *node, Node *brotherNode, int64_t parentIndex)
{
	ADD_TO_NODE_ROUTINE_LEAF(-1, getLastElement, eraseLastElement, insertFirstElement, brotherNode)
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::addToLeafNodeFromRight(Node *node, Node *brotherNode, int64_t parentIndex)
{
	ADD_TO_NODE_ROUTINE_LEAF(, getFirstElement, eraseFirstElement, insertLastElement, node)
}
#undef ADD_TO_NODE_ROUTINE_LEAF

#define ADD_TO_NODE_ROUTINE_INTERNAL(ARG1, ARG2, ARG3, ARG4, ARG5)                                  \
	static_cast<Node *>(node)->ARG1(                                                                \
		node->getParent()->getElement(parentIndex ARG2), static_cast<Node *>(brotherNode)->ARG3()); \
	static_cast<Node *>(brotherNode)->ARG3()->setParent(static_cast<Node *>(node));                 \
	node->increaseSizeByOne();                                                                      \
	node->getParent()->assignElement(parentIndex ARG2, std::move(brotherNode->ARG4()));             \
	static_cast<Node *>(brotherNode)->ARG5();                                                       \
	brotherNode->decreaseSizeByOne();

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::addToInternalNodeFromLeft(Node *node, Node *brotherNode, int64_t parentIndex)
{
	ADD_TO_NODE_ROUTINE_INTERNAL(insertFirstElementAndChild, -1, getLastChild, getLastElement, eraseLastElementAndChild)
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::addToInternalNodeFromRight(Node *node, Node *brotherNode, int64_t parentIndex)
{
	ADD_TO_NODE_ROUTINE_INTERNAL(insertLastElementAndChild, , getFirstChild, getFirstElement, eraseFirstElementAndChild)
}
#undef ADD_TO_NODE_ROUTINE_INTERNAL

template <typename T, typename K, typename Comp, typename ExtractKey>
bool B_TREE_BASE<T, K, Comp, ExtractKey>::addToNode(Node *node, int64_t parentIndex)
{
	if (empty())
	{ // should not have been called
		std::cout << "addToNode called on the root node\n";
		return false;
	}
	if (node->getSize() != node->t - 1)
	{
		std::cout << "addToNode called on not t-1 node\n";
		return false;
	}
	// getRightBrother returns null if this is the rightmost children
	Node *brotherNode = NULL;
	if (parentIndex < node->getParent()->getSize())
	{
		brotherNode = static_cast<Node *>(node->getParent())->getChild(parentIndex + 1);
		if (brotherNode->isLeast())
		{
			return false;
		}
		if (node->isLeafNode())
		{
			addToLeafNodeFromRight(node, brotherNode, parentIndex);
		}
		else
		{
			addToInternalNodeFromRight(node, brotherNode, parentIndex);
		}
	}
	else
	{ /* this is the rightmost child; look into its left brother */
		brotherNode = static_cast<Node *>(node->getParent())->getChild(parentIndex - 1);
		if (brotherNode->isLeast())
		{
			return false;
		}
		if (node->isLeafNode())
		{
			addToLeafNodeFromLeft(node, brotherNode, parentIndex);
		}
		else
		{
			addToInternalNodeFromLeft(node, brotherNode, parentIndex);
		}
	}

#ifdef TREE_SIZE
	node->refreshTreeSize();
	brotherNode->refreshTreeSize();
	node->getParent()->refreshTreeSize();
#endif
	return true;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::Node *
B_TREE_BASE<T, K, Comp, ExtractKey>::mergeNode(Node *node, int64_t parentIndex)
{
	if (node == root_)
	{ // should not have been called
		std::cout << "mergeNode called on the root node\n";
		return NULL;
	}
	if (node->getSize() != node->t - 1)
	{
		std::cout << "mergeNode called on not t-1 node\n";
		return NULL;
	}
	Node *brotherNode = NULL;
	if (parentIndex < node->getParent()->getSize())
	{
		brotherNode = static_cast<Node *>(node->getParent())->getChild(parentIndex + 1);
	}
	else
	{ // using a trick: exchange node and brotherNode for symmetricity
		brotherNode = static_cast<Node *>(node->getParent())->getChild(parentIndex - 1);
		Node *temp = node;
		node = brotherNode;
		brotherNode = temp;
		parentIndex--; // due to left-right exchange
	}

	if (node->isLeafNode() == false)
	{ // copy children and change parent
		node->receiveRightHalfFrom(brotherNode);
		for (int64_t i = 0; i < node->t; ++i)
		{
			static_cast<Node *>(node)->setChild(i + node->t,
												static_cast<Node *>(brotherNode)->getChild(i));
			static_cast<Node *>(brotherNode)->getChild(i)->setParent(static_cast<Node *>(node));
		}
		node->constructElement(node->t - 1, std::move(node->getParent()->getElement(parentIndex)));
	}
	else
	{ // leaf
#ifdef BPLUS
		node->receiveRightHalfFromBPlusLeaf(brotherNode);
		node->setNext(brotherNode->getNext());
		if (brotherNode->getNext())
		{
			brotherNode->getNext()->setPrevious(node);
		}
#else
		node->receiveRightHalfFrom(brotherNode);
		node->constructElement(node->t - 1, std::move(node->getParent()->getElement(parentIndex)));
#endif
	}
	deleteNode(brotherNode);
	// erase one element and one child from the parent
	static_cast<Node *>(node->getParent())->eraseElementAndChild(parentIndex);
	node->getParent()->decreaseSizeByOne();
	// special case for root
	if (node->getParent() == root_ && root_->getSize() == 0)
	{
		//std::cout << "freeing root\n";
		deleteNode(root_);
		root_ = node;
		root_->setParent(NULL);
	}
#ifdef TREE_SIZE
	node->refreshTreeSize();
	if (node->getParent())
	{
		node->getParent()->refreshTreeSize();
	}
#endif
	return node;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::Node *
B_TREE_BASE<T, K, Comp, ExtractKey>::addToOrMerge(Node *node, int64_t parentIndex)
{
	bool addToExcuted = addToNode(node, parentIndex);
	if (addToExcuted == false)
	{
		return mergeNode(node, parentIndex);
	}
	else
	{
		return node;
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::Node *
B_TREE_BASE<T, K, Comp, ExtractKey>::addToOrMergeIfNeeded(Node *node, int64_t parentIndex)
{
	if (node->isLeast())
	{
		return addToOrMerge(node, parentIndex);
	}
	else
	{
		return node;
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
T const
B_TREE_BASE<T, K, Comp, ExtractKey>::
	deleteLeftMost(Node *node, int64_t parentIndex)
{
	node = addToOrMergeIfNeeded(node, parentIndex);
	if (node->isLeafNode() == true)
	{
		return this->deleteValue(node, 0);
	}
	else
	{
		return deleteLeftMost(
			static_cast<Node *>(node)->getFirstChild(), 0);
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
T const
B_TREE_BASE<T, K, Comp, ExtractKey>::deleteRightMost(
	Node *node, int64_t parentIndex)
{
	node = addToOrMergeIfNeeded(node, parentIndex);
	if (node->isLeafNode() == true)
	{
		return this->deleteValue(node, node->getSize() - 1);
	}
	else
	{
		return deleteRightMost(
			static_cast<Node *>(node)->getLastChild(), node->getSize());
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::Node *
B_TREE_BASE<T, K, Comp, ExtractKey>::makeNewLeafNode(size_t capacity)
{
	//std::cout << "size of elem is " << sizeof(T) << "\n";
	//std::cout << "size of btree is " << sizeof(B_TREE_BASE<T, K, Comp, ExtractKey>) << "\n";
	//std::cout << "size of node is " << sizeof(B_TREE_BASE<T, K, Comp, ExtractKey>::Node) << "\n";
	//std::cout << "size of fullnode is " << sizeof(B_TREE_BASE<T, K, Comp, ExtractKey>::FullNode) << "\n";
	//std::cout << "size of internal node is " << sizeof(B_TREE_BASE<T, K, Comp, ExtractKey>::Node) << "\n";
	//std::cout << "size of full internal node is " <<
	//	sizeof(B_TREE_BASE<T, K, Comp, ExtractKey>::FullInternalNode) << "\n";
	capacity = B_TREE_FULL_NODE_CAPACITY;
	size_t basicSize = sizeof(typename Node::BasicFields);
	size_t elemsSize = sizeof(T) * capacity;

	Node *newObject = reinterpret_cast<Node *>(operator new(basicSize + elemsSize));
	initNode(newObject, capacity);
	return newObject;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename B_TREE_BASE<T, K, Comp, ExtractKey>::Node *
B_TREE_BASE<T, K, Comp, ExtractKey>::makeNewInternalNode()
{
	Node *newObject = reinterpret_cast<Node *>(operator new(sizeof(Node)));
	initNode(newObject, B_TREE_FULL_NODE_CAPACITY);
	newObject->fields_.isLeafNode_ = false;
	return newObject;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::initNode(Node *node, size_t capacity) const
{
	node->fields_.size_ = 0;
	node->fields_.capacity_ = capacity;
	node->fields_.parent_ = nullptr;
	node->fields_.isLeafNode_ = true;
#ifdef BPLUS
	node->setNext(NULL);
	node->setPrevious(NULL);
#endif
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::print() const
{
	printTree(root_);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::printNode(Node *node) const
{
	for (int64_t i = 0; i < node->getSize(); ++i)
	{
		std::cout << node->getElement(i) << " ";
	}
	std::cout << "\n";
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::printTree(Node *node) const
{
	if (node->isLeafNode() == false)
	{
		for (int64_t i = 0; i < node->getSize(); ++i)
		{
			printTree(static_cast<Node *>(node)->getChild(i));
			std::cout << node->getElement(i);
			std::cout << "\n";
		}
		printTree(static_cast<Node *>(node)->getLastChild());
	}
	else
	{
		printNode(node);
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
std::pair<int64_t, bool>
B_TREE_BASE<T, K, Comp, ExtractKey>::getIndexToInsert(Node *node, T const &elem) const
{
	K const &key = ExtractKey()(elem);
	return getIndexToInsert2(node, key);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
std::pair<int64_t, bool>
B_TREE_BASE<T, K, Comp, ExtractKey>::getIndexToInsert2(Node *node, K const &key) const
{
	T *begin = node->getElementArray();
	T *end = node->getElementArray() + node->getSize();
	auto result = lib_calvin_container::getIndexToInsert<T, K, Comp, ExtractKey>(begin, end, key, USE_BINARY_SEARCH);
	return std::pair<int64_t, bool>(static_cast<int64_t>(result.first), result.second);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::copyFrom(B_TREE_BASE const &rhs)
{
	// this object is empty at the point of calling this function

	if (rhs.empty())
	{ // rhs is empty
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

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::bTreeNodeCopy(Node const *source, Node *&target)
{
	Node *dummyLeftMost;
	Node *dummyRightMost;
	bTreeNodeCopy(source, target, dummyLeftMost, dummyRightMost);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::bTreeNodeCopy(
	Node const *source, Node *&target, Node *&leftMost, Node *&rightMost)
{
	if (source == NULL)
	{
		target = NULL;
		return;
	}
	bool isLeafNode = source->isLeafNode();
	if (isLeafNode == false)
	{ // this is internal
		Node *realTarget = makeNewInternalNode();
		bTreeNodeContentCopy(source, realTarget);
		Node const *realSource = static_cast<Node const *>(source);
#ifdef BPLUS
		Node *previousRightMostOfChild = NULL;
#endif
		for (int64_t i = 0; i <= realSource->getSize(); ++i)
		{
			Node *target;
			Node *leftMostOfChild;
			Node *rightMostOfChild;
			bTreeNodeCopy(realSource->getChild(i), target, leftMostOfChild, rightMostOfChild);
			realTarget->setChild(i, target);
			realTarget->getChild(i)->setParent(realTarget);
#ifdef BPLUS
			if (i == 0)
			{
				leftMost = leftMostOfChild;
				previousRightMostOfChild = rightMostOfChild;
			}
			else
			{
				previousRightMostOfChild->setNext(leftMostOfChild);
				leftMostOfChild->setPrevious(previousRightMostOfChild);
				if (i == realSource->getSize())
				{
					rightMost = rightMostOfChild;
				}
				else
				{
					previousRightMostOfChild = rightMostOfChild;
				}
			}
#endif
		}
		target = realTarget;
	}
	else
	{ // leaf node
		target = makeNewLeafNode(source->getCapacity());
		bTreeNodeContentCopy(source, target);
#ifdef BPLUS
		leftMost = target;
		rightMost = target;
#endif
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::bTreeNodeContentCopy(Node const *source, Node *target)
{
	target->setSize(source->getSize());
	copyConstruct(source->getElementArray(), target->getElementArray(), source->getSize());
	if (!source->isLeafNode())
	{
		Node const *src = static_cast<Node const *>(source);
		Node *tar = static_cast<Node *>(target);
		tar->fields_.treeSize_ = src->fields_.treeSize_;
		for (int64_t i = 0; i <= src->getSize(); i++)
		{
			tar->fields_.treeSize2_[i] = src->fields_.treeSize2_[i];
		}
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::deleteTree(Node *node)
{
	if (node == NULL)
	{
		return;
	}
	bool isLeafNode = node->isLeafNode();
	if (isLeafNode == false)
	{ // this is internal
		for (int64_t i = 0; i <= node->getSize(); ++i)
		{
			deleteTree(static_cast<Node *>(node)->getChild(i));
		}
	}
	deleteNode(node);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void B_TREE_BASE<T, K, Comp, ExtractKey>::deleteNode(Node *node)
{
	for (int64_t i = 0; i < node->getSize(); ++i)
	{
		node->getElement(i).~T();
	}
	operator delete(node);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator==(B_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
				B_TREE_BASE<T, K, Comp, ExtractKey> const &rhs)
{
	if (lhs.size() != rhs.size())
	{
		return false;
	}
	return containerEqual(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator!=(B_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
				B_TREE_BASE<T, K, Comp, ExtractKey> const &rhs)
{
	return !(lhs == rhs);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator<(B_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
			   B_TREE_BASE<T, K, Comp, ExtractKey> const &rhs)
{
	return containerLess(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator<=(B_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
				B_TREE_BASE<T, K, Comp, ExtractKey> const &rhs)
{
	return !(lhs > rhs);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator>(B_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
			   B_TREE_BASE<T, K, Comp, ExtractKey> const &rhs)
{
	return rhs < lhs;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator>=(B_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
				B_TREE_BASE<T, K, Comp, ExtractKey> const &rhs)
{
	return !(lhs < rhs);
}
} // namespace lib_calvin_container

#undef B_TREE_BASE
