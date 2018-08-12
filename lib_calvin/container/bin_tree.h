#ifndef LIB_CALVIN__CONTAINER__BIN_TREE_H
#define LIB_CALVIN__CONTAINER__BIN_TREE_H 

#include "utility.h"
#include "container.h"
#include "stopwatch.h"

#define USE_HEAD_NODE

namespace lib_calvin_container
{
	enum class Direction {
		Left = 0,
		Right = 1
	};

	template <typename T>
	class BinTreeNode {
	public:
		BinTreeNode(T const &key);
		BinTreeNode(T &&key);

		~BinTreeNode();
		BinTreeNode<T> *getParent() const;
		BinTreeNode<T> *getLeftChild() const;
		BinTreeNode<T> *getRightChild() const;
		bool hasLeftChild() const;
		bool hasRightChild() const;
		bool hasChild(Direction) const;

		BinTreeNode<T> *getChild(Direction direction) const;
		void setParent(BinTreeNode<T> *newParent);
		void setLeftChild(BinTreeNode<T> *newLeft);
		void setRightChild(BinTreeNode<T> *newRight);
		void setChild(BinTreeNode<T> *newChild, Direction direction);
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
	private:
		BinTreeNode<T> *parent_;
		BinTreeNode<T> *children_[2];

		T key_; // cannot be const because of 'delete' routine uses setKey method
	};

	// K and ExtractKey added to implement map using this set implementation
	// ExtractKey: function object T -> K
	// Comp: function object (K, K) -> bool
	template <typename T, typename K = T, typename Comp = std::less<K>, typename ExtractKey = Identity<T>>
	class BinTree {
	public:
		typedef T value_type;
		typedef T * pointer;
		typedef T & reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

	private:
		// BinTree uses bidirectional iterator
		// This class (const iterator type) forms base to four kinds of real iterator
		class IteratorImpl
		{
		public:
			typedef T value_type;
			typedef T const * pointer;
			typedef T const & reference;
			typedef size_t size_type;
			typedef ptrdiff_t difference_type;
			typedef std::bidirectional_iterator_tag iterator_category;

			IteratorImpl();
			IteratorImpl(BinTreeNode<T> *node);
			// copy ctor	
			IteratorImpl(IteratorImpl const &rhs);
			IteratorImpl & operator=(IteratorImpl const &rhs);
			reference operator*() const;
			pointer operator->() const;
			IteratorImpl & operator++(); // prefix
			IteratorImpl & operator--();
			IteratorImpl const operator++(int); // suffix			
			IteratorImpl const operator--(int);
			bool operator==(IteratorImpl const &rhs) const;
			bool operator!=(IteratorImpl const &rhs) const;
		private:
			BinTreeNode<T> *node_;
		};
	public:
		typedef lib_calvin_container::Iterator<IteratorImpl> iterator;
		typedef lib_calvin_container::ConstIterator<IteratorImpl> const_iterator;
		typedef lib_calvin_container::ReverseIterator<IteratorImpl> reverse_iterator;
		typedef lib_calvin_container::ConstReverseIterator<IteratorImpl> const_reverse_iterator;

		BinTree<T, K, Comp, ExtractKey>();
		BinTree<T, K, Comp, ExtractKey>(BinTree<T, K, Comp, ExtractKey> const &);
		BinTree<T, K, Comp, ExtractKey>(BinTree<T, K, Comp, ExtractKey> &&);
		BinTree<T, K, Comp, ExtractKey> & operator= (BinTree<T, K, Comp, ExtractKey> const &);
		BinTree<T, K, Comp, ExtractKey> & operator= (BinTree<T, K, Comp, ExtractKey> &&);
		void swap(BinTree<T, K, Comp, ExtractKey> & rhs);
		// disallow instantiation, because log(n) is not guaranteed
		virtual ~BinTree<T, K, Comp, ExtractKey>();

		size_t size() const;
		const_iterator find(K const &) const;
		iterator find(K const &);
		size_t count(K const &) const;
		std::pair<iterator, bool> insert(T const &);
		std::pair<iterator, bool> insert(T &&);
		template <typename T1> std::pair<iterator, bool> insert_(T1 &&);
		// Insert range [beg, end)
		template <typename InputIterator> void insert(InputIterator beg, InputIterator end);
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
		bool is_null(BinTreeNode<T> const *node) const;
		// returns negative if key is less than return value, 0 if same, 1 if greater
		template <typename T1> std::pair<BinTreeNode<T> *, bool> insert_impl(T1 &&key);
		BinTreeNode<T> *erase_impl(K const &);
		void copyFrom(BinTree const &rhs); // should be called only when empty
	private:
		std::pair<BinTreeNode<T> *, int> findNode(K const &) const;
		BinTreeNode<T> *getFirstNode() const; // for begin()
		BinTreeNode<T> *deleteValueInNode(BinTreeNode<T> *); // erase this node
		BinTreeNode<T> *makeEndNode() const;
#ifdef USE_HEAD_NODE
		BinTreeNode<T> *makeHeadNode() const;
#endif
		// Copy nodes recursively. A copy of the tree pointed by 'srcNode' will 
		// be created, and 'targetNode' shall point to the root of copied tree.
		void binTreeNodeCopy(BinTreeNode<T> *srcNode, BinTreeNode<T> *&targetNode);
	protected:
		virtual BinTreeNode<T> *makeNewNode(T const &elem) const;
		virtual BinTreeNode<T> *makeNewNode(T &&elem) const;
		virtual BinTreeNode<T> *makeNewNode(BinTreeNode<T> const *rhs) const;
		void prepareNode(BinTreeNode<T> *) const;
	protected: // member variables
		size_t size_;
		BinTreeNode<T> *root_; // root_ is end_'s left child
		BinTreeNode<T> *end_; // iterator for end()
	};

	template <typename T, typename K, typename Comp, typename ExtractKey>
	bool operator==(BinTree<T, K, Comp, ExtractKey> const &lhs,
		BinTree<T, K, Comp, ExtractKey> const &rhs);

	template <typename T, typename K, typename Comp, typename ExtractKey>
	bool operator!=(BinTree<T, K, Comp, ExtractKey> const &lhs,
		BinTree<T, K, Comp, ExtractKey> const &rhs);

	template <typename T, typename K, typename Comp, typename ExtractKey>
	bool operator< (BinTree<T, K, Comp, ExtractKey> const &lhs,
		BinTree<T, K, Comp, ExtractKey> const &rhs);

	template <typename T, typename K, typename Comp, typename ExtractKey>
	bool operator<= (BinTree<T, K, Comp, ExtractKey> const &lhs,
		BinTree<T, K, Comp, ExtractKey> const &rhs);

	template <typename T, typename K, typename Comp, typename ExtractKey>
	bool operator> (BinTree<T, K, Comp, ExtractKey> const &lhs,
		BinTree<T, K, Comp, ExtractKey> const &rhs);

	template <typename T, typename K, typename Comp, typename ExtractKey>
	bool operator>= (BinTree<T, K, Comp, ExtractKey> const &lhs,
		BinTree<T, K, Comp, ExtractKey> const &rhs);

	// Return successor of node
	template <typename T>
	BinTreeNode<T> *binTreeSuccessor(BinTreeNode<T> const *node);

	// Return predecessor of node
	template <typename T>
	BinTreeNode<T> *binTreePredecessor(BinTreeNode<T> const *node);

	// Delete nodes recursively
	template <typename T>
	void binTreeNodeDelete(BinTreeNode<T> *node);

} // lib_calvin_container

namespace lib_calvin_container // open for definitions
{
	/*************************** Node definitions **************************/


	template <typename T>
	BinTreeNode<T>::BinTreeNode(T const &key) : parent_(nullptr), key_(key) {
		children_[0] = nullptr; 
		children_[1] = nullptr;
	}

	template <typename T>
	BinTreeNode<T>::BinTreeNode(T &&key) : parent_(nullptr), key_(std::forward<T>(key)) {
		children_[0] = nullptr; 
		children_[1] = nullptr;
	}

	template <typename T>
	BinTreeNode<T>::~BinTreeNode() { }

	template <typename T>
	BinTreeNode<T> *
		BinTreeNode<T>::getParent() const {
		return parent_;
	}

	template <typename T>
	BinTreeNode<T> *
		BinTreeNode<T>::getLeftChild() const {
		return children_[0];
	}

	template <typename T>
	BinTreeNode<T> *
		BinTreeNode<T>::getRightChild() const {
		return children_[1];
	}

	template <typename T>
	bool
		BinTreeNode<T>::hasLeftChild() const {
#ifdef USE_HEAD_NODE
		return !children_[0]->is_null_;
#else
		return children_[0];
#endif
	}

	template <typename T>
	bool
		BinTreeNode<T>::hasRightChild() const {
#ifdef USE_HEAD_NODE
		return !children_[1]->is_null_;
#else
		return children_[1];
#endif
	}

	template <typename T>
	bool
		BinTreeNode<T>::hasChild(Direction direction) const {
#ifdef USE_HEAD_NODE
		return !children_[static_cast<int>(direction)]->is_null_;
#else
		return children_[static_cast<int>(direction)];
#endif
	}

	template <typename T>
	BinTreeNode<T> *
		BinTreeNode<T>::getChild(Direction direction) const {
		return children_[static_cast<int>(direction)];
	}
	template <typename T>
	void BinTreeNode<T>::setParent(BinTreeNode<T> *parent) {
		parent_ = parent;
	}

	template <typename T>
	void BinTreeNode<T>::setLeftChild(BinTreeNode<T> *leftChild) {
		children_[0] = leftChild;
	}

	template <typename T>
	void BinTreeNode<T>::setRightChild(BinTreeNode<T> *rightChild) {
		children_[1] = rightChild;
	}

	template <typename T>
	void BinTreeNode<T>::setChild(BinTreeNode<T> *child, Direction direction) {
		children_[static_cast<int>(direction)] = child;
	}

	template <typename T>
	Direction
		BinTreeNode<T>::getPositionInParent() const {
		if (this == parent_->getLeftChild()) {
			return Direction::Left;
		} else {
			return Direction::Right;
		}
	}

	template <typename T>
	bool BinTreeNode<T>::isLeftInParent() const {
		return this == parent_->getLeftChild();
	}

	template <typename T>
	bool BinTreeNode<T>::isRightInParent() const {
		return this == parent_->getRightChild();
	}

	template <typename T>
	T const &
		BinTreeNode<T>::getKey() const {
		return key_;
	}

	template <typename T>
	void BinTreeNode<T>::setKey(T const &newKey) {
		key_ = newKey;
	}

	template <typename T>
	Direction
		BinTreeNode<T>::getOpposite(Direction direction) {
		return static_cast<Direction>(1 - static_cast<int>(direction));
	}

	/************************ Iterator definitions **********************/

	template <typename T, typename K, typename Comp, typename ExtractKey>
	BinTree<T, K, Comp, ExtractKey>::IteratorImpl::IteratorImpl() : node_(nullptr) { }

	template <typename T, typename K, typename Comp, typename ExtractKey>
	BinTree<T, K, Comp, ExtractKey>::IteratorImpl::IteratorImpl(BinTreeNode<T> *node) : node_(node) { }

	template <typename T, typename K, typename Comp, typename ExtractKey>
	BinTree<T, K, Comp, ExtractKey>::IteratorImpl::IteratorImpl(IteratorImpl const &rhs) : node_(rhs.node_) { }


	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename BinTree<T, K, Comp, ExtractKey>::IteratorImpl &
		BinTree<T, K, Comp, ExtractKey>::IteratorImpl::operator=(IteratorImpl const &rhs) {
		node_ = rhs.node_;
		return *this;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename BinTree<T, K, Comp, ExtractKey>::IteratorImpl::reference
		BinTree<T, K, Comp, ExtractKey>::IteratorImpl::operator*() const {
		return node_->getKey();
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename BinTree<T, K, Comp, ExtractKey>::IteratorImpl::pointer
		BinTree<T, K, Comp, ExtractKey>::IteratorImpl::operator->() const {
		return &node_->getKey();
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename BinTree<T, K, Comp, ExtractKey>::IteratorImpl &
		BinTree<T, K, Comp, ExtractKey>::IteratorImpl::operator++() { // prefix
		node_ = binTreeSuccessor(node_);
		return *this;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename BinTree<T, K, Comp, ExtractKey>::IteratorImpl &
		BinTree<T, K, Comp, ExtractKey>::IteratorImpl::operator--() {
		node_ = binTreePredecessor(node_);
		return *this;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename BinTree<T, K, Comp, ExtractKey>::IteratorImpl const
		BinTree<T, K, Comp, ExtractKey>::IteratorImpl::operator++(int) { // suffix
		IteratorImpl original = *this;
		operator++();
		return original;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename BinTree<T, K, Comp, ExtractKey>::IteratorImpl const
		BinTree<T, K, Comp, ExtractKey>::IteratorImpl::operator--(int) {
		IteratorImpl original = *this;
		operator--();
		return original;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	bool
		BinTree<T, K, Comp, ExtractKey>::IteratorImpl::operator==(IteratorImpl const &rhs) const {
		return node_ == rhs.node_;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	bool
		BinTree<T, K, Comp, ExtractKey>::IteratorImpl::operator!=(IteratorImpl const &rhs) const {
		return node_ != rhs.node_;
	}

	/*************************** BinTree definitions **************************/

	template <typename T, typename K, typename Comp, typename ExtractKey>
	BinTree<T, K, Comp, ExtractKey>::BinTree() :
#ifdef USE_HEAD_NODE
		size_(0), root_(nullptr), end_(nullptr) { }
#else
		size_(0), root_(nullptr), end_(nullptr) { }
#endif

	template <typename T, typename K, typename Comp, typename ExtractKey>
	BinTree<T, K, Comp, ExtractKey>::BinTree(BinTree<T, K, Comp, ExtractKey> const &rhs) :
#ifdef USE_HEAD_NODE
		size_(0), root_(nullptr), end_(nullptr) {
#else
		size_(0), root_(nullptr), end_(nullptr) {
#endif
		copyFrom(rhs);
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	BinTree<T, K, Comp, ExtractKey>::BinTree(BinTree<T, K, Comp, ExtractKey> &&rhs) :
#ifdef USE_HEAD_NODE
		size_(0), root_(nullptr), end_(nullptr) {
#else
		size_(0), root_(nullptr), end_(nullptr) {
#endif
		swap(rhs);
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	BinTree<T, K, Comp, ExtractKey> &
		BinTree<T, K, Comp, ExtractKey>::operator=(BinTree<T, K, Comp, ExtractKey> const &rhs) {
		//std::cout << "bintree assign\n";
		if (&rhs != this) {
			clear();
			copyFrom(rhs);
		}
		return *this;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	void BinTree<T, K, Comp, ExtractKey>::copyFrom(BinTree<T, K, Comp, ExtractKey> const &rhs) {
		// this object should be empty at this point
		if (rhs.empty()) {
			return;
		}
		size_ = rhs.size_;
		end_ = makeEndNode();
		binTreeNodeCopy(rhs.root_, root_);		
		root_->setParent(end_);
		end_->setLeftChild(root_);
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	BinTree<T, K, Comp, ExtractKey> &
		BinTree<T, K, Comp, ExtractKey>::operator=(BinTree<T, K, Comp, ExtractKey> &&rhs) {
		//std::cout << "bintree move assign\n";
		if (&rhs != this) {
			swap(rhs);
		}
		return *this;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	void BinTree<T, K, Comp, ExtractKey>::swap(BinTree<T, K, Comp, ExtractKey> &rhs) {
		typedef BinTreeNode<T> * Node;
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
	BinTree<T, K, Comp, ExtractKey>::~BinTree() {
		// --- Misterious runtime error occurs unless we qualify the destructor ---
		// In fact there is no point in destructing the object in this case.
		//end_->BinTreeNode<T>::~BinTreeNode();		
		if (!empty()) {
			binTreeNodeDelete(root_);
		}
		operator delete (end_);
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	size_t
		BinTree<T, K, Comp, ExtractKey>::size() const {
		return size_;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename BinTree<T, K, Comp, ExtractKey>::const_iterator
		BinTree<T, K, Comp, ExtractKey>::find(K const &key) const {
		if (empty()) {
			return const_iterator(end_);
		}
		auto result = findNode(key);
		if (result.second == 0) { // found
			return const_iterator(result.first);
		} else {
			return const_iterator(end_);
		}
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename BinTree<T, K, Comp, ExtractKey>::iterator
		BinTree<T, K, Comp, ExtractKey>::find(K const &key) {
		if (empty()) {
			return iterator(end_);
		}
		auto result = findNode(key);
		if (result.second == 0) { // found
			return iterator(result.first);
		} else {
			return iterator(end_);
		}
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	size_t BinTree<T, K, Comp, ExtractKey>::count(K const &key) const {
		if (empty()) {
			return 0;
		}
		//K const &nearestKey = ExtractKey()(findNode(key)->getKey());
		std::pair<BinTreeNode<T> *, int> result = findNode(key);
		if (result.second == 0) {
			return 1;
		} else {
			return 0;
		}
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	std::pair<typename BinTree<T, K, Comp, ExtractKey>::iterator, bool>
		BinTree<T, K, Comp, ExtractKey>::insert(T const &elem) {
		return insert_(elem);
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	std::pair<typename BinTree<T, K, Comp, ExtractKey>::iterator, bool>
		BinTree<T, K, Comp, ExtractKey>::insert(T &&elem) {
		return insert_(std::move(elem));
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	template <typename InputIterator>
	void BinTree<T, K, Comp, ExtractKey>::insert(InputIterator beg, InputIterator end) {
		while (beg != end) {
			insert(*beg);
			++beg;
		}
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	size_t BinTree<T, K, Comp, ExtractKey>::erase(K const &key) {
		BinTreeNode<T> *deleted = erase_impl(key);
		if (deleted == nullptr) {
			return 0;
		} else {
			delete deleted;
			return 1;
		}
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	void BinTree<T, K, Comp, ExtractKey>::clear() {
		if (empty()) {
			return;
		}
		size_ = 0;
		binTreeNodeDelete<T>(root_);
		operator delete (end_);
		root_ = nullptr;
		end_ = nullptr;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	bool BinTree<T, K, Comp, ExtractKey>::empty() const {
		return size_ == 0;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename BinTree<T, K, Comp, ExtractKey>::const_iterator
		BinTree<T, K, Comp, ExtractKey>::begin() const {
		return const_iterator(getFirstNode());
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename BinTree<T, K, Comp, ExtractKey>::const_iterator
		BinTree<T, K, Comp, ExtractKey>::end() const {
		return const_iterator(end_);
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename BinTree<T, K, Comp, ExtractKey>::iterator
		BinTree<T, K, Comp, ExtractKey>::begin() {
		return iterator(getFirstNode());
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename BinTree<T, K, Comp, ExtractKey>::iterator
		BinTree<T, K, Comp, ExtractKey>::end() {
		return iterator(end_);
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename BinTree<T, K, Comp, ExtractKey>::const_reverse_iterator
		BinTree<T, K, Comp, ExtractKey>::rbegin() const {
		return const_reverse_iterator(end_);
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename BinTree<T, K, Comp, ExtractKey>::const_reverse_iterator
		BinTree<T, K, Comp, ExtractKey>::rend() const {
		return const_reverse_iterator(getFirstNode());
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename BinTree<T, K, Comp, ExtractKey>::reverse_iterator
		BinTree<T, K, Comp, ExtractKey>::rbegin() {
		return reverse_iterator(end_);
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename BinTree<T, K, Comp, ExtractKey>::reverse_iterator
		BinTree<T, K, Comp, ExtractKey>::rend() {
		return reverse_iterator(getFirstNode());
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	template <typename T1>
	std::pair<typename BinTree<T, K, Comp, ExtractKey>::iterator, bool>
		BinTree<T, K, Comp, ExtractKey>::insert_(T1 &&elem) {
		std::pair<BinTreeNode<T> *, bool> result = insert_impl(std::forward<T1>(elem));
		return std::pair<iterator, bool>(iterator(result.first), result.second);
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	bool 
		BinTree<T, K, Comp, ExtractKey>::is_null(BinTreeNode<T> const *node) const {
#ifdef USE_HEAD_NODE
		return node->is_null_;
#else
		return node == nullptr;
#endif
	}


	template <typename T, typename K, typename Comp, typename ExtractKey>
	template <typename T1>
	std::pair<BinTreeNode<T> *, bool>
		BinTree<T, K, Comp, ExtractKey>::insert_impl(T1 &&elem) {
		using std::pair;
		if (empty()) { // special case for empty tree
			end_ = makeEndNode();
			root_ = makeNewNode(std::forward<T1>(elem));			
			root_->setParent(end_);
			end_->setLeftChild(root_);
			size_++;
			return pair<BinTreeNode<T> *, bool>(root_, true);
		}
		K const &key = ExtractKey()(elem);
		std::pair<BinTreeNode<T> *, int> result = findNode(key);
		if (result.second != 0) { // need to insert
			BinTreeNode<T> *nodeToInsert = result.first;
			BinTreeNode<T> *newNode = makeNewNode(std::forward<T1>(elem));
			if (result.second > 0) {
				// successor cannot be null (there is end_)
				nodeToInsert->setRightChild(newNode); // only differenly part
			} else {
				// predecessor may be null for begin() node
				nodeToInsert->setLeftChild(newNode); // only differenly part
			}
			newNode->setParent(nodeToInsert);
			size_++;
			return pair<BinTreeNode<T> *, bool>(newNode, true);
		} else { // already exists
			return pair<BinTreeNode<T> *, bool>(result.first, false);
		}
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	BinTreeNode<T> *
		BinTree<T, K, Comp, ExtractKey>::erase_impl(K const &key) {
		if (empty()) {
			return nullptr;
		}
		std::pair<BinTreeNode<T> *, int> result = findNode(key); // node to erase                        
		if (result.second == 0) { // found key	
			auto nodeToDelete = deleteValueInNode(result.first);
			size_--;
			if (empty()) {
				operator delete (end_);
				end_ = nullptr;
			}
			return nodeToDelete;
		} else {
			return nullptr;
		}
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	BinTreeNode<T> *
		BinTree<T, K, Comp, ExtractKey>::getFirstNode() const {
		if (empty()) {
			return end_;
		}
		BinTreeNode<T> *temp = root_;
#ifdef USE_HEAD_NODE
		while (!temp->getLeftChild()->is_null_) {
#else
		while (temp->getLeftChild() != nullptr) {
#endif
			temp = temp->getLeftChild();
		}
		return temp;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	std::pair<BinTreeNode<T> *, int>
		BinTree<T, K, Comp, ExtractKey>::findNode(K const &key) const {
		if (empty()) {
			std::cout << "findNode called when empty tree\n";
			return std::pair<BinTreeNode<T> *, int>(nullptr, 0);
		}

#ifdef USE_HEAD_NODE
		BinTreeNode<T> *nullNode = end_;
#else
		BinTreeNode<T> *nullNode = nullptr;
#endif
		
		BinTreeNode<T> *curNode = root_;
		BinTreeNode<T> *nextNode = nullNode;
		bool isChoosingRightChild = true;
		BinTreeNode<T> *successor = nullNode;
		while (true) {
			K const &curNodeKey = ExtractKey()(curNode->getKey());
			if (Comp()(curNodeKey, key)) {
				isChoosingRightChild = true;
				nextNode = curNode->getRightChild();
				if (nextNode == nullNode) {
					break;
				} else {
					curNode = nextNode;
				}
			} else {
				isChoosingRightChild = false;
				successor = curNode;
				nextNode = curNode->getLeftChild();
				if (nextNode == nullNode) {
					break;
				} else {
					curNode = nextNode;
				}
			}
		}
		if (successor == nullNode || Comp()(key, ExtractKey()(successor->getKey()))) { // not exists, so insert
			if (isChoosingRightChild) {
				return std::make_pair(curNode, 1);
			} else {
				return std::make_pair(curNode, -1);
			}
		} else {
			return std::make_pair(successor, 0);
		}
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	BinTreeNode<T> *
		BinTree<T, K, Comp, ExtractKey>::deleteValueInNode(BinTreeNode<T> *node) {
		BinTreeNode<T> *nodeToDelete = node;
		BinTreeNode<T> *next = binTreeSuccessor(nodeToDelete);
#ifdef USE_HEAD_NODE
		BinTreeNode<T> *nullNode = end_;
#else
		BinTreeNode<T> *nullNode = nullptr;
#endif

		if (node->hasLeftChild() && node->hasRightChild()) { 
			// both children exist												 
			// successor of internal must be a leaf
			nodeToDelete = next;
			node->setKey(std::move(nodeToDelete->getKey()));
		}
		// manage iterating link
		if (nodeToDelete->hasLeftChild() != nodeToDelete->hasRightChild()) { // only one child exist
			BinTreeNode<T> *child = nodeToDelete->hasLeftChild() ?
				nodeToDelete->getLeftChild() : nodeToDelete->getRightChild();
			child->setParent(nodeToDelete->getParent());
			if (nodeToDelete->getParent() == end_) { // erasing the root
				root_ = child;
			}
			if (nodeToDelete->isLeftInParent()) {
				nodeToDelete->getParent()->setLeftChild(child);
			} else {
				nodeToDelete->getParent()->setRightChild(child);
			}
		} else { // This is a leaf node
			if (nodeToDelete->getParent() == end_) { // erasing the root
				root_ = nullptr;
			}
			if (nodeToDelete->isLeftInParent()) {
				nodeToDelete->getParent()->setLeftChild(nullNode);
			} else {
				nodeToDelete->getParent()->setRightChild(nullNode);
			}
		}
		return nodeToDelete;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	bool operator==(BinTree<T, K, Comp, ExtractKey> const &lhs,
		BinTree<T, K, Comp, ExtractKey> const &rhs) {
		if (lhs.size() != rhs.size()) {
			return false;
		}
		return containerEqual(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	bool operator!=(BinTree<T, K, Comp, ExtractKey> const &lhs,
		BinTree<T, K, Comp, ExtractKey> const &rhs) {
		return !(lhs == rhs);
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	bool operator< (BinTree<T, K, Comp, ExtractKey> const &lhs,
		BinTree<T, K, Comp, ExtractKey> const &rhs) {
		return containerLess(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	bool operator<= (BinTree<T, K, Comp, ExtractKey> const &lhs,
		BinTree<T, K, Comp, ExtractKey> const &rhs) {
		return !(lhs > rhs);
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	bool operator> (BinTree<T, K, Comp, ExtractKey> const &lhs,
		BinTree<T, K, Comp, ExtractKey> const &rhs) {
		return rhs < lhs;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	bool operator>= (BinTree<T, K, Comp, ExtractKey> const &lhs,
		BinTree<T, K, Comp, ExtractKey> const &rhs) {
		return !(lhs < rhs);
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	void
		BinTree<T, K, Comp, ExtractKey>::binTreeNodeCopy(BinTreeNode<T> *srcNode, BinTreeNode<T> *&targetNode)
	{
#ifdef USE_HEAD_NODE
		if (srcNode->is_null_) {
			targetNode = end_;
			return;
		}
		BinTreeNode<T> *leftChild = end_;
		BinTreeNode<T> *rightChild = end_;
#else
		if (srcNode == nullptr) {
			targetNode = nullptr;
			return;
		}
		BinTreeNode<T> *leftChild = nullptr;
		BinTreeNode<T> *rightChild = nullptr;
#endif
		binTreeNodeCopy(srcNode->getLeftChild(), leftChild);
		binTreeNodeCopy(srcNode->getRightChild(), rightChild);

		targetNode = makeNewNode(srcNode);
		targetNode->setLeftChild(leftChild);
		targetNode->setRightChild(rightChild);

#ifdef USE_HEAD_NODE
		if (!leftChild->is_null_) {
			leftChild->setParent(targetNode);
		}
		if (!rightChild->is_null_) {
			rightChild->setParent(targetNode);
		}
#else
		if (leftChild) {
			leftChild->setParent(targetNode);
		}
		if (rightChild) {
			rightChild->setParent(targetNode);
		}
#endif

	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	BinTreeNode<T> *
		BinTree<T, K, Comp, ExtractKey>::makeNewNode(T const &key) const {
		auto newNode = new BinTreeNode<T>(key);
#ifdef USE_HEAD_NODE
		prepareNode(newNode);
#endif	
		return newNode;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	BinTreeNode<T> *
		BinTree<T, K, Comp, ExtractKey>::makeNewNode(T &&key) const {
		auto newNode = new BinTreeNode<T>(std::forward<T>(key));
#ifdef USE_HEAD_NODE
		prepareNode(newNode);
#endif		
		return newNode;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	BinTreeNode<T> *
		BinTree<T, K, Comp, ExtractKey>::makeNewNode(BinTreeNode<T> const *rhs) const {
		auto newNode = new BinTreeNode<T>(rhs->getKey());
#ifdef USE_HEAD_NODE
		prepareNode(newNode);
#endif	
		return newNode;
	}

#ifdef USE_HEAD_NODE
	template <typename T, typename K, typename Comp, typename ExtractKey>
	void
		BinTree<T, K, Comp, ExtractKey>::prepareNode(BinTreeNode<T> *newNode) const {
		newNode->setParent(end_);
		newNode->setLeftChild(end_);
		newNode->setRightChild(end_);
		newNode->is_null_ = false;
		return;
	}
#endif	

	template <typename T, typename K, typename Comp, typename ExtractKey>
	BinTreeNode<T> *
		BinTree<T, K, Comp, ExtractKey>::makeEndNode() const {
		BinTreeNode<T> *endNode = static_cast<BinTreeNode<T> *>(operator new(sizeof(BinTreeNode<T>)));
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

	template <typename T>
	BinTreeNode<T> *
		binTreeSuccessor(BinTreeNode<T> const *node) {
#ifdef USE_HEAD_NODE
		if (!node->getRightChild()->is_null_) { // successor is in this subtree
			BinTreeNode<T> *temp = node->getRightChild();
			while (!temp->getLeftChild()->is_null_) {
				temp = temp->getLeftChild();
			}
			return temp;
		} else { // we have to go up
			while (true) {
				if (node->isLeftInParent()) {
					return node->getParent();
				} else {
					node = node->getParent();
				}
			}
		}
#else
		if (node->getRightChild() != nullptr) { // successor is in this subtree
			BinTreeNode<T> *temp = node->getRightChild();
			while (temp->getLeftChild() != nullptr) {
				temp = temp->getLeftChild();
			}
			return temp;
		} else { // we have to go up
			while (true) {
				if (node->isLeftInParent()) {
					return node->getParent();
				} else {
					node = node->getParent();
				}
			}
		}
#endif

	}

	template <typename T>
	BinTreeNode<T> *
		binTreePredecessor(BinTreeNode<T> const *node) {

#ifdef USE_HEAD_NODE

		if (!node->getLeftChild()->is_null_) { // predecessor is in this subtree
			BinTreeNode<T> *temp = node->getLeftChild();
			while (!temp->getRightChild()->is_null_) {
				temp = temp->getRightChild();
			}
			return temp;
		} else { // we have to go up
			while (true) {
				if (node->isRightInParent()) {
					return node->getParent();
				} else {
					node = node->getParent();
				}
			}
		}
#else

		if (node->getLeftChild() != nullptr) { // predecessor is in this subtree
			BinTreeNode<T> *temp = node->getLeftChild();
			while (temp->getRightChild() != nullptr) {
				temp = temp->getRightChild();
			}
			return temp;
		} else { // we have to go up
			while (true) {
				if (node->isRightInParent()) {
					return node->getParent();
				} else {
					node = node->getParent();
				}
			}
	}
#endif


	}

	template <typename T>
	void binTreeNodeDelete(BinTreeNode<T> *node) {
#ifdef USE_HEAD_NODE
		if (node->is_null_) {
			return;
		}
#else
		if (node == nullptr) {
			return;
		}
#endif

		binTreeNodeDelete(node->getLeftChild());
		binTreeNodeDelete(node->getRightChild());
		delete node;
	}

	} // close definition; lib_calvin_container

#endif


