#include "utility.h"
#include "container.h"

#ifdef USE_HEAD_NODE
#define BIN_TREE_BASE BinPlusTree
#else
#define BIN_TREE_BASE BinTree
#endif

namespace lib_calvin_container
{
// K and ExtractKey added to implement map using this set implementation
// ExtractKey: function object T -> K
// Comp: function object (K, K) -> bool
template <typename T, typename K = T, typename Comp = std::less<K>, typename ExtractKey = Identity<T>>
class BIN_TREE_BASE
{
public:
	enum class Direction
	{
		Left = 0,
		Right = 1
	};

	enum class RbColor
	{
		Black,
		Red
	};

	class Node
	{
	public:
		Node(T const &key);
		Node(T &&key);

		~Node();
		Node *getParent() const;
		Node *getLeftChild() const;
		Node *getRightChild() const;
		bool hasLeftChild() const;
		bool hasRightChild() const;
		bool hasChild(Direction) const;

		Node *getChild(Direction direction) const;
		void setParent(Node *newParent);
		void setLeftChild(Node *newLeft);
		void setRightChild(Node *newRight);
		void setChild(Node *newChild, Direction direction);
		Direction getPositionInParent() const;
		bool isLeftInParent() const;
		bool isRightInParent() const;
		T const &getKey() const;
		void setKey(T const &newKey);

	public:
#ifdef USE_HEAD_NODE
		bool is_null_;
#endif
		static Direction getOpposite(Direction direction);

	protected:
		RbColor color_;

	private:
		Node *parent_;
		Node *children_[2];

		T key_; // cannot be const because of 'delete' routine uses setKey method
	};

public:
	typedef T value_type;
	typedef T *pointer;
	typedef T &reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

private:
	// BIN_TREE_BASE uses bidirectional iterator
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

		IteratorImpl();
		IteratorImpl(Node *node);
		// copy ctor
		IteratorImpl(IteratorImpl const &rhs);
		IteratorImpl &operator=(IteratorImpl const &rhs);
		reference operator*() const;
		pointer operator->() const;
		IteratorImpl &operator++(); // prefix
		IteratorImpl &operator--();
		IteratorImpl const operator++(int); // suffix
		IteratorImpl const operator--(int);
		bool operator==(IteratorImpl const &rhs) const;
		bool operator!=(IteratorImpl const &rhs) const;

	private:
		Node *node_;
	};

public:
	typedef lib_calvin_container::Iterator<IteratorImpl> iterator;
	typedef lib_calvin_container::ConstIterator<IteratorImpl> const_iterator;
	typedef lib_calvin_container::ReverseIterator<IteratorImpl> reverse_iterator;
	typedef lib_calvin_container::ConstReverseIterator<IteratorImpl> const_reverse_iterator;

	BIN_TREE_BASE<T, K, Comp, ExtractKey>();
	BIN_TREE_BASE<T, K, Comp, ExtractKey>(BIN_TREE_BASE<T, K, Comp, ExtractKey> const &);
	BIN_TREE_BASE<T, K, Comp, ExtractKey>(BIN_TREE_BASE<T, K, Comp, ExtractKey> &&);
	BIN_TREE_BASE<T, K, Comp, ExtractKey> &operator=(BIN_TREE_BASE<T, K, Comp, ExtractKey> const &);
	BIN_TREE_BASE<T, K, Comp, ExtractKey> &operator=(BIN_TREE_BASE<T, K, Comp, ExtractKey> &&);
	void swap(BIN_TREE_BASE<T, K, Comp, ExtractKey> &rhs);
	// disallow instantiation, because log(n) is not guaranteed
	virtual ~BIN_TREE_BASE<T, K, Comp, ExtractKey>();

	size_t size() const;
	const_iterator find(K const &) const;
	iterator find(K const &);
	size_t count(K const &) const;
	std::pair<iterator, bool> insert(T const &);
	std::pair<iterator, bool> insert(T &&);
	template <typename T1>
	std::pair<iterator, bool> insert_(T1 &&);
	// Insert range [beg, end)
	template <typename InputIterator>
	void insert(InputIterator beg, InputIterator end);
	size_t erase(K const &);
	void clear();
	bool empty() const;

	const_iterator begin() const;
	const_iterator end() const;
	iterator begin();
	iterator end();

	const_reverse_iterator rbegin() const;
	const_reverse_iterator rend() const;
	reverse_iterator rbegin();
	reverse_iterator rend();

protected:
	// returns negative if key is less than return value, 0 if same, 1 if greater
	template <typename T1>
	std::pair<Node *, bool> insert_impl(T1 &&key);
	Node *erase_impl(K const &);
	void copyFrom(BIN_TREE_BASE const &rhs); // should be called only when empty
private:
	std::pair<Node *, int> findNode(K const &) const;
	Node *getFirstNode() const;		 // for begin()
	Node *deleteValueInNode(Node *); // erase this node
	Node *makeEndNode() const;

	// Copy nodes recursively. A copy of the tree pointed by 'srcNode' will
	// be created, and 'targetNode' shall point to the root of copied tree.
	void binTreeNodeCopy(Node *srcNode, Node *&targetNode);

protected:
	virtual Node *makeNewNode(T const &elem) const;
	virtual Node *makeNewNode(T &&elem) const;
	virtual Node *makeNewNode(Node const *rhs) const;
	void prepareNode(Node *) const;

protected: // member variables
	size_t size_;
	Node *root_; // root_ is end_'s left child
	Node *end_;  // iterator for end()
};

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator==(BIN_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
				BIN_TREE_BASE<T, K, Comp, ExtractKey> const &rhs);

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator!=(BIN_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
				BIN_TREE_BASE<T, K, Comp, ExtractKey> const &rhs);

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator<(BIN_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
			   BIN_TREE_BASE<T, K, Comp, ExtractKey> const &rhs);

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator<=(BIN_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
				BIN_TREE_BASE<T, K, Comp, ExtractKey> const &rhs);

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator>(BIN_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
			   BIN_TREE_BASE<T, K, Comp, ExtractKey> const &rhs);

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator>=(BIN_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
				BIN_TREE_BASE<T, K, Comp, ExtractKey> const &rhs);

// Return successor of node
template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *binTreeSuccessor(
	typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node const *node);

// Return predecessor of node
template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *binTreePredecessor(
	typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node const *node);

// Delete nodes recursively
template <typename T, typename K, typename Comp, typename ExtractKey>
void binTreeNodeDelete(
	typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *node);

template <typename T, typename K, typename Comp, typename ExtractKey>
bool is_null(
	typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node const *node);
} // namespace lib_calvin_container

namespace lib_calvin_container // open for definitions
{
/*************************** Node definitions **************************/

template <typename T, typename K, typename Comp, typename ExtractKey>
BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::Node(T const &key) : parent_(nullptr), key_(key)
{
	children_[0] = nullptr;
	children_[1] = nullptr;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::Node(T &&key) : parent_(nullptr), key_(std::forward<T>(key))
{
	children_[0] = nullptr;
	children_[1] = nullptr;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::~Node() {}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *
BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::getParent() const
{
	return parent_;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *
BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::getLeftChild() const
{
	return children_[0];
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *
BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::getRightChild() const
{
	return children_[1];
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::hasLeftChild() const
{
	return hasChild(Direction::Left);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::hasRightChild() const
{
	return hasChild(Direction::Right);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::hasChild(Direction direction) const
{

	return !is_null<T, K, Comp, ExtractKey>(children_[static_cast<int>(direction)]);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *
BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::getChild(Direction direction) const
{
	return children_[static_cast<int>(direction)];
}
template <typename T, typename K, typename Comp, typename ExtractKey>
void BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::setParent(BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *parent)
{
	parent_ = parent;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::setLeftChild(BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *leftChild)
{
	children_[0] = leftChild;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::setRightChild(BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *rightChild)
{
	children_[1] = rightChild;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::setChild(BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *child, Direction direction)
{
	children_[static_cast<int>(direction)] = child;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Direction
BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::getPositionInParent() const
{
	if (this == parent_->getLeftChild())
	{
		return Direction::Left;
	}
	else
	{
		return Direction::Right;
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::isLeftInParent() const
{
	return this == parent_->getLeftChild();
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::isRightInParent() const
{
	return this == parent_->getRightChild();
}

template <typename T, typename K, typename Comp, typename ExtractKey>
T const &
BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::getKey() const
{
	return key_;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::setKey(T const &newKey)
{
	key_ = newKey;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Direction
BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::getOpposite(Direction direction)
{
	return static_cast<Direction>(1 - static_cast<int>(direction));
}

/************************ Iterator definitions **********************/

template <typename T, typename K, typename Comp, typename ExtractKey>
BIN_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::IteratorImpl() : node_(nullptr) {}

template <typename T, typename K, typename Comp, typename ExtractKey>
BIN_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::IteratorImpl(BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *node) : node_(node) {}

template <typename T, typename K, typename Comp, typename ExtractKey>
BIN_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::IteratorImpl(IteratorImpl const &rhs) : node_(rhs.node_) {}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl &
BIN_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::operator=(IteratorImpl const &rhs)
{
	node_ = rhs.node_;
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::reference
	BIN_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::operator*() const
{
	return node_->getKey();
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::pointer
	BIN_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::operator->() const
{
	return &node_->getKey();
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl &
BIN_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::operator++()
{ // prefix
	node_ = binTreeSuccessor<T, K, Comp, ExtractKey>(node_);
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl &
BIN_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::operator--()
{
	node_ = binTreePredecessor<T, K, Comp, ExtractKey>(node_);
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl const
BIN_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::operator++(int)
{ // suffix
	IteratorImpl original = *this;
	operator++();
	return original;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl const
BIN_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::operator--(int)
{
	IteratorImpl original = *this;
	operator--();
	return original;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool BIN_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::operator==(IteratorImpl const &rhs) const
{
	return node_ == rhs.node_;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool BIN_TREE_BASE<T, K, Comp, ExtractKey>::IteratorImpl::operator!=(IteratorImpl const &rhs) const
{
	return node_ != rhs.node_;
}

/*************************** BIN_TREE_BASE definitions **************************/

template <typename T, typename K, typename Comp, typename ExtractKey>
BIN_TREE_BASE<T, K, Comp, ExtractKey>::BIN_TREE_BASE() : size_(0), root_(nullptr), end_(nullptr) {}

template <typename T, typename K, typename Comp, typename ExtractKey>
BIN_TREE_BASE<T, K, Comp, ExtractKey>::BIN_TREE_BASE(BIN_TREE_BASE<T, K, Comp, ExtractKey> const &rhs) : size_(0), root_(nullptr), end_(nullptr)
{

	copyFrom(rhs);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
BIN_TREE_BASE<T, K, Comp, ExtractKey>::BIN_TREE_BASE(BIN_TREE_BASE<T, K, Comp, ExtractKey> &&rhs) : size_(0), root_(nullptr), end_(nullptr)
{

	swap(rhs);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
BIN_TREE_BASE<T, K, Comp, ExtractKey> &
BIN_TREE_BASE<T, K, Comp, ExtractKey>::operator=(BIN_TREE_BASE<T, K, Comp, ExtractKey> const &rhs)
{
	//std::cout << "bintree assign\n";
	if (&rhs != this)
	{
		clear();
		copyFrom(rhs);
	}
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void BIN_TREE_BASE<T, K, Comp, ExtractKey>::copyFrom(BIN_TREE_BASE<T, K, Comp, ExtractKey> const &rhs)
{
	// this object should be empty at this point
	if (rhs.empty())
	{
		return;
	}
	size_ = rhs.size_;
	end_ = makeEndNode();
	binTreeNodeCopy(rhs.root_, root_);
	root_->setParent(end_);
	end_->setLeftChild(root_);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
BIN_TREE_BASE<T, K, Comp, ExtractKey> &
BIN_TREE_BASE<T, K, Comp, ExtractKey>::operator=(BIN_TREE_BASE<T, K, Comp, ExtractKey> &&rhs)
{
	//std::cout << "bintree move assign\n";
	if (&rhs != this)
	{
		swap(rhs);
	}
	return *this;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void BIN_TREE_BASE<T, K, Comp, ExtractKey>::swap(BIN_TREE_BASE<T, K, Comp, ExtractKey> &rhs)
{
	typedef BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *Node;
	Node tempRoot = root_;
	Node tempEnd = end_;
	size_t tempSize = size_;

	root_ = rhs.root_;
	end_ = rhs.end_;
	size_ = rhs.size_;

	rhs.root_ = tempRoot;
	rhs.end_ = tempEnd;
	rhs.size_ = tempSize;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
BIN_TREE_BASE<T, K, Comp, ExtractKey>::~BIN_TREE_BASE()
{
	// --- Misterious runtime error occurs unless we qualify the destructor ---
	// In fact there is no point in destructing the object in this case.
	//end_->BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::~Node();
	if (!empty())
	{
		binTreeNodeDelete<T, K, Comp, ExtractKey>(root_);
	}
	operator delete(end_);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
size_t
BIN_TREE_BASE<T, K, Comp, ExtractKey>::size() const
{
	return size_;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::const_iterator
BIN_TREE_BASE<T, K, Comp, ExtractKey>::find(K const &key) const
{
	if (empty())
	{
		return const_iterator(end_);
	}
	auto result = findNode(key);
	if (result.second == 0)
	{ // found
		return const_iterator(result.first);
	}
	else
	{
		return const_iterator(end_);
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::iterator
BIN_TREE_BASE<T, K, Comp, ExtractKey>::find(K const &key)
{
	if (empty())
	{
		return iterator(end_);
	}
	auto result = findNode(key);
	if (result.second == 0)
	{ // found
		return iterator(result.first);
	}
	else
	{
		return iterator(end_);
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
size_t BIN_TREE_BASE<T, K, Comp, ExtractKey>::count(K const &key) const
{
	if (empty())
	{
		return 0;
	}
	//K const &nearestKey = ExtractKey()(findNode(key)->getKey());
	std::pair<BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *, int> result = findNode(key);
	if (result.second == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
std::pair<typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::iterator, bool>
BIN_TREE_BASE<T, K, Comp, ExtractKey>::insert(T const &elem)
{
	return insert_(elem);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
std::pair<typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::iterator, bool>
BIN_TREE_BASE<T, K, Comp, ExtractKey>::insert(T &&elem)
{
	return insert_(std::move(elem));
}

template <typename T, typename K, typename Comp, typename ExtractKey>
template <typename InputIterator>
void BIN_TREE_BASE<T, K, Comp, ExtractKey>::insert(InputIterator beg, InputIterator end)
{
	while (beg != end)
	{
		insert(*beg);
		++beg;
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
size_t BIN_TREE_BASE<T, K, Comp, ExtractKey>::erase(K const &key)
{
	BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *deleted = erase_impl(key);
	if (deleted == nullptr)
	{
		return 0;
	}
	else
	{
		delete deleted;
		return 1;
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void BIN_TREE_BASE<T, K, Comp, ExtractKey>::clear()
{
	if (empty())
	{
		return;
	}
	size_ = 0;
	binTreeNodeDelete<T, K, Comp, ExtractKey>(root_);
	operator delete(end_);
	root_ = nullptr;
	end_ = nullptr;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool BIN_TREE_BASE<T, K, Comp, ExtractKey>::empty() const
{
	return size_ == 0;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::const_iterator
BIN_TREE_BASE<T, K, Comp, ExtractKey>::begin() const
{
	return const_iterator(getFirstNode());
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::const_iterator
BIN_TREE_BASE<T, K, Comp, ExtractKey>::end() const
{
	return const_iterator(end_);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::iterator
BIN_TREE_BASE<T, K, Comp, ExtractKey>::begin()
{
	return iterator(getFirstNode());
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::iterator
BIN_TREE_BASE<T, K, Comp, ExtractKey>::end()
{
	return iterator(end_);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::const_reverse_iterator
BIN_TREE_BASE<T, K, Comp, ExtractKey>::rbegin() const
{
	return const_reverse_iterator(end_);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::const_reverse_iterator
BIN_TREE_BASE<T, K, Comp, ExtractKey>::rend() const
{
	return const_reverse_iterator(getFirstNode());
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::reverse_iterator
BIN_TREE_BASE<T, K, Comp, ExtractKey>::rbegin()
{
	return reverse_iterator(end_);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::reverse_iterator
BIN_TREE_BASE<T, K, Comp, ExtractKey>::rend()
{
	return reverse_iterator(getFirstNode());
}

template <typename T, typename K, typename Comp, typename ExtractKey>
template <typename T1>
std::pair<typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::iterator, bool>
BIN_TREE_BASE<T, K, Comp, ExtractKey>::insert_(T1 &&elem)
{
	std::pair<BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *, bool> result = insert_impl(std::forward<T1>(elem));
	return std::pair<iterator, bool>(iterator(result.first), result.second);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
template <typename T1>
std::pair<typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *, bool>
BIN_TREE_BASE<T, K, Comp, ExtractKey>::insert_impl(T1 &&elem)
{
	using std::pair;
	if (empty())
	{ // special case for empty tree
		end_ = makeEndNode();
		root_ = makeNewNode(std::forward<T1>(elem));
		root_->setParent(end_);
		end_->setLeftChild(root_);
		size_++;
		return pair<BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *, bool>(root_, true);
	}
	K const &key = ExtractKey()(elem);
	std::pair<BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *, int> result = findNode(key);
	if (result.second != 0)
	{ // need to insert
		BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *nodeToInsert = result.first;
		BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *newNode = makeNewNode(std::forward<T1>(elem));
		if (result.second > 0)
		{
			// successor cannot be null (there is end_)
			nodeToInsert->setRightChild(newNode); // only differenly part
		}
		else
		{
			// predecessor may be null for begin() node
			nodeToInsert->setLeftChild(newNode); // only differenly part
		}
		newNode->setParent(nodeToInsert);
		size_++;
		return pair<BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *, bool>(newNode, true);
	}
	else
	{ // already exists
		return pair<BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *, bool>(result.first, false);
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *
BIN_TREE_BASE<T, K, Comp, ExtractKey>::erase_impl(K const &key)
{
	if (empty())
	{
		return nullptr;
	}
	std::pair<BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *, int> result = findNode(key); // node to erase
	if (result.second == 0)
	{ // found key
		auto nodeToDelete = deleteValueInNode(result.first);
		size_--;
		if (empty())
		{
			operator delete(end_);
			end_ = nullptr;
		}
		return nodeToDelete;
	}
	else
	{
		return nullptr;
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *
BIN_TREE_BASE<T, K, Comp, ExtractKey>::getFirstNode() const
{
	if (empty())
	{
		return end_;
	}
	BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *temp = root_;

	while (temp->hasLeftChild())
	{
		temp = temp->getLeftChild();
	}
	return temp;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
std::pair<typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *, int>
BIN_TREE_BASE<T, K, Comp, ExtractKey>::findNode(K const &key) const
{
	if (empty())
	{
		std::cout << "findNode called when empty tree\n";
		exit(0);
	}

#ifdef USE_HEAD_NODE
	BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *nullNode = end_;
#else
	BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *nullNode = nullptr;
#endif

	BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *curNode = root_;
	BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *nextNode = nullNode;
	bool isChoosingRightChild = true;
	BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *successor = nullNode;
	while (true)
	{
		K const &curNodeKey = ExtractKey()(curNode->getKey());
		if (Comp()(curNodeKey, key))
		{
			isChoosingRightChild = true;
			nextNode = curNode->getRightChild();
			if (is_null<T, K, Comp, ExtractKey>(nextNode))
			{
				break;
			}
			else
			{
				curNode = nextNode;
			}
		}
		else
		{
			isChoosingRightChild = false;
			successor = curNode;
			nextNode = curNode->getLeftChild();
			if (is_null<T, K, Comp, ExtractKey>(nextNode))
			{
				break;
			}
			else
			{
				curNode = nextNode;
			}
		}
	}
	if (is_null<T, K, Comp, ExtractKey>(successor) || Comp()(key, ExtractKey()(successor->getKey())))
	{ // not exists, so insert
		if (isChoosingRightChild)
		{
			return std::make_pair(curNode, 1);
		}
		else
		{
			return std::make_pair(curNode, -1);
		}
	}
	else
	{
		return std::make_pair(successor, 0);
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *
BIN_TREE_BASE<T, K, Comp, ExtractKey>::deleteValueInNode(BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *node)
{
	BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *nodeToDelete = node;
	BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *next = binTreeSuccessor<T, K, Comp, ExtractKey>(nodeToDelete);
#ifdef USE_HEAD_NODE
	BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *nullNode = end_;
#else
	BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *nullNode = nullptr;
#endif

	if (node->hasLeftChild() && node->hasRightChild())
	{
		// both children exist
		// successor of internal must be a leaf
		nodeToDelete = next;
		node->setKey(std::move(nodeToDelete->getKey()));
	}
	// manage iterating link
	if (nodeToDelete->hasLeftChild() != nodeToDelete->hasRightChild())
	{ // only one child exist
		BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *child = nodeToDelete->hasLeftChild() ? nodeToDelete->getLeftChild() : nodeToDelete->getRightChild();
		child->setParent(nodeToDelete->getParent());
		if (nodeToDelete->getParent() == end_)
		{ // erasing the root
			root_ = child;
		}
		if (nodeToDelete->isLeftInParent())
		{
			nodeToDelete->getParent()->setLeftChild(child);
		}
		else
		{
			nodeToDelete->getParent()->setRightChild(child);
		}
	}
	else
	{ // This is a leaf node
		if (nodeToDelete->getParent() == end_)
		{ // erasing the root
			root_ = nullptr;
		}
		if (nodeToDelete->isLeftInParent())
		{
			nodeToDelete->getParent()->setLeftChild(nullNode);
		}
		else
		{
			nodeToDelete->getParent()->setRightChild(nullNode);
		}
	}
	return nodeToDelete;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator==(BIN_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
				BIN_TREE_BASE<T, K, Comp, ExtractKey> const &rhs)
{
	if (lhs.size() != rhs.size())
	{
		return false;
	}
	return containerEqual(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator!=(BIN_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
				BIN_TREE_BASE<T, K, Comp, ExtractKey> const &rhs)
{
	return !(lhs == rhs);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator<(BIN_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
			   BIN_TREE_BASE<T, K, Comp, ExtractKey> const &rhs)
{
	return containerLess(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator<=(BIN_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
				BIN_TREE_BASE<T, K, Comp, ExtractKey> const &rhs)
{
	return !(lhs > rhs);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator>(BIN_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
			   BIN_TREE_BASE<T, K, Comp, ExtractKey> const &rhs)
{
	return rhs < lhs;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool operator>=(BIN_TREE_BASE<T, K, Comp, ExtractKey> const &lhs,
				BIN_TREE_BASE<T, K, Comp, ExtractKey> const &rhs)
{
	return !(lhs < rhs);
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void BIN_TREE_BASE<T, K, Comp, ExtractKey>::binTreeNodeCopy(BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *srcNode, BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *&targetNode)
{
#ifdef USE_HEAD_NODE
	BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *nullNode = end_;
#else
	BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *nullNode = nullptr;
#endif

	if (is_null<T, K, Comp, ExtractKey>(srcNode))
	{
		targetNode = nullNode;
		return;
	}
	BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *leftChild = nullNode;
	BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *rightChild = nullNode;

	binTreeNodeCopy(srcNode->getLeftChild(), leftChild);
	binTreeNodeCopy(srcNode->getRightChild(), rightChild);

	targetNode = makeNewNode(srcNode);
	targetNode->setLeftChild(leftChild);
	targetNode->setRightChild(rightChild);

	if (!is_null<T, K, Comp, ExtractKey>(leftChild))
	{
		leftChild->setParent(targetNode);
	}
	if (!is_null<T, K, Comp, ExtractKey>(rightChild))
	{
		rightChild->setParent(targetNode);
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *
BIN_TREE_BASE<T, K, Comp, ExtractKey>::makeNewNode(T const &key) const
{
	auto newNode = new BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node(key);
#ifdef USE_HEAD_NODE
	prepareNode(newNode);
#endif
	return newNode;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *
BIN_TREE_BASE<T, K, Comp, ExtractKey>::makeNewNode(T &&key) const
{
	auto newNode = new BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node(std::forward<T>(key));
#ifdef USE_HEAD_NODE
	prepareNode(newNode);
#endif
	return newNode;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *
BIN_TREE_BASE<T, K, Comp, ExtractKey>::makeNewNode(BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node const *rhs) const
{
	auto newNode = new BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node(rhs->getKey());
#ifdef USE_HEAD_NODE
	prepareNode(newNode);
#endif
	return newNode;
}

#ifdef USE_HEAD_NODE
template <typename T, typename K, typename Comp, typename ExtractKey>
void BIN_TREE_BASE<T, K, Comp, ExtractKey>::prepareNode(BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *newNode) const
{
	newNode->setParent(end_);
	newNode->setLeftChild(end_);
	newNode->setRightChild(end_);
	newNode->is_null_ = false;
	return;
}
#endif

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *
BIN_TREE_BASE<T, K, Comp, ExtractKey>::makeEndNode() const
{
	BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *endNode = static_cast<BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *>(operator new(sizeof(BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node)));
#ifdef USE_HEAD_NODE
	endNode->setParent(nullptr);
	endNode->setLeftChild(nullptr);
	endNode->setRightChild(nullptr);
	endNode->is_null_ = true;

#else
	endNode->setParent(nullptr);
	endNode->setLeftChild(nullptr);
	endNode->setRightChild(nullptr);
#endif

	return endNode;
}

/************************* functions definitions ********************/

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *
binTreeSuccessor(typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node const *node)
{

	if (node->hasRightChild())
	{ // successor is in this subtree
		typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *temp = node->getRightChild();
		while (temp->hasLeftChild())
		{
			temp = temp->getLeftChild();
		}
		return temp;
	}
	else
	{ // we have to go up
		while (true)
		{
			if (node->isLeftInParent())
			{
				return node->getParent();
			}
			else
			{
				node = node->getParent();
			}
		}
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *
binTreePredecessor(typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node const *node)
{

	if (node->hasLeftChild())
	{ // predecessor is in this subtree
		typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *temp = node->getLeftChild();
		while (temp->hasRightChild())
		{
			temp = temp->getRightChild();
		}
		return temp;
	}
	else
	{ // we have to go up
		while (true)
		{
			if (node->isRightInParent())
			{
				return node->getParent();
			}
			else
			{
				node = node->getParent();
			}
		}
	}
}

template <typename T, typename K, typename Comp, typename ExtractKey>
void binTreeNodeDelete(typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node *node)
{
	if (is_null<T, K, Comp, ExtractKey>(node))
	{
		return;
	}

	binTreeNodeDelete<T, K, Comp, ExtractKey>(node->getLeftChild());
	binTreeNodeDelete<T, K, Comp, ExtractKey>(node->getRightChild());
	delete node;
}

template <typename T, typename K, typename Comp, typename ExtractKey>
bool is_null(typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node const *node)
{
#ifdef USE_HEAD_NODE
	return node->is_null_;
#else
	return node == nullptr;
#endif
}

} // namespace lib_calvin_container

#undef BIN_TREE_BASE