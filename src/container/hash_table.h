#ifndef LIB_CALVIN__CONTAINER__HASH_TABLE2_H
#define LIB_CALVIN__CONTAINER__HASH_TABLE2_H

#include "vector.h"
#include "boost/functional/hash.hpp"

namespace lib_calvin_container
{
double const REHASH_THRESHOLD = 1;
size_t const HASH_SET_SIZES[26] = {53, 97, 193, 389, 769, 1543, 3079, 6151,
								   12289, 24593, 49157, 98317, 196613,
								   393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653,
								   100663319, 201326611, 402653189, 805306457, 1610612741};

template <typename T, typename K = T, typename ExtractKey = Identity<T>,
		  typename HashFunc = boost::hash<K>>
class HashTable
{
public:
	typedef T value_type;
	typedef T *pointer;
	typedef T &reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

private:
	struct Node
	{
		Node(T const &elem, Node *next) : elem_(elem), next_(next) {}
		Node(T &&elem, Node *next) : elem_(std::forward<T>(elem)), next_(next) {}
		T elem_;
		Node *next_; // empty bucket points to nullptr, and the last Node points to the fist Node
	};

private:
	class IteratorImpl
	{
	public:
		typedef T value_type;
		typedef T const *pointer;
		typedef T const &reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef std::bidirectional_iterator_tag iterator_category;
		reference operator*() const { return node_->elem_; }
		pointer operator->() const { return &(node_->elem_); }
		IteratorImpl();
		IteratorImpl(Node **table, size_t tableSize, size_t index, Node *Node);
		// copy ctor
		IteratorImpl(IteratorImpl const &rhs);
		IteratorImpl &operator=(IteratorImpl const &rhs);
		IteratorImpl &operator++(); // prefix
		IteratorImpl &operator--();
		IteratorImpl const operator++(int)
		{
			IteratorImpl returnValue(*this);
			operator++();
			return returnValue;
		} // suffix
		IteratorImpl const operator--(int)
		{
			IteratorImpl returnValue(*this);
			operator--();
			return returnValue;
		} // suffix
		bool operator==(IteratorImpl const &rhs) const;
		bool operator!=(IteratorImpl const &rhs) const { return !(*this == rhs); }

	private:
		Node **table_;
		size_t tableSize_;
		size_t index_; // index in hash table
		Node *node_;   // actual Node pointed by this iterator
	};

public:
	typedef lib_calvin_container::Iterator<IteratorImpl> iterator;
	typedef lib_calvin_container::ConstIterator<IteratorImpl> const_iterator;
	typedef lib_calvin_container::ReverseIterator<IteratorImpl> reverse_iterator;
	typedef lib_calvin_container::ConstReverseIterator<IteratorImpl> const_reverse_iterator;

public:
	const_iterator begin() const { return getBeginIterator(); }
	const_iterator end() const { return getEndIterator(); }
	iterator begin() { return getBeginIterator(); }
	iterator end() { return getEndIterator(); }
	const_reverse_iterator rbegin() const { return getEndIterator(); }
	const_reverse_iterator rend() const { return getBeginIterator(); }
	reverse_iterator rbegin() { return getEndIterator(); }
	reverse_iterator rend() { return getBeginIterator(); }

public:
	HashTable();
	HashTable(HashTable const &rhs); // copy ctor
	HashTable(HashTable &&rhs);
	HashTable &operator=(HashTable const &hrs); // assignment
	HashTable &operator=(HashTable &&hrs);
	void swap(HashTable &rhs);
	~HashTable();

public:
	size_t size() const { return size_; }
	const_iterator find(K const &) const;
	iterator find(K const &);
	size_t count(K const &elem) const;
	template <typename T1>
	std::pair<iterator, bool> insert(T1 &&elem);
	size_t erase(K const &elem);
	void clear();
	bool empty() const { return size_ == 0; }

private:
	Node *getBucket(K const &) const;
	size_t getIndex(K const &) const;
	void rehash();

private:
	void init();
	void initTable(size_t);
	void deleteTable(Node **, size_t);

private: // insert given elem or Node into the bucket without checking (only used in rehasing)
	void addToFirst(T &elem);
	void addToFirst(Node *Node);

private:
	IteratorImpl getBeginIterator() const;
	IteratorImpl getEndIterator() const;
	IteratorImpl findIterator(K const &) const;

private:
	size_t size_;
	Node **table_;
	size_t tableSize_;
	size_t hashSetSizeIndex_;
};

template <typename T, typename K, typename ExtractKey, typename HashFunc>
bool operator==(HashTable<T, K, ExtractKey, HashFunc> const &lhs,
				HashTable<T, K, ExtractKey, HashFunc> const &rhs)
{
	return containerEqual(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
bool operator!=(HashTable<T, K, ExtractKey, HashFunc> const &lhs,
				HashTable<T, K, ExtractKey, HashFunc> const &rhs)
{
	return !(lhs == rhs);
}

/*
template <typename T, typename K, typename ExtractKey, typename HashFunc> 
size_t getBucketLength(HashTable<T, K, ExtractKey, HashFunc>::Node *bucket) { 
	if (bucket->next_ == nullptr) {
		return 0;
	}
	typename HashTable<T>::Node *thisNode = bucket;
	int length = 0;
	while (true) {
		length++;
		thisNode = thisNode->next_;
		if (thisNode == bucket) {
			break;
		}
	}
	return length;
}*/

template <typename T, typename K, typename ExtractKey, typename HashFunc>
HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl::IteratorImpl() : table_(nullptr), tableSize_(0), index_(0), node_(nullptr) {}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl::IteratorImpl(
	Node **table, size_t tableSize, size_t index, Node *Node) : table_(table), tableSize_(tableSize), index_(index), node_(Node) {}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl::IteratorImpl(IteratorImpl const &rhs) : table_(rhs.table_), tableSize_(rhs.tableSize_), index_(rhs.index_), node_(rhs.node_) {}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
typename HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl &
HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl::operator=(IteratorImpl const &rhs)
{
	table_ = rhs.table_;
	tableSize_ = rhs.tableSize_;
	index_ = rhs.index_;
	node_ = rhs.node_;
	return *this;
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
bool HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl::operator==(IteratorImpl const &rhs) const
{
	return node_ == rhs.node_;
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
typename HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl &
HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl::operator++()
{
	if (node_->next_ == nullptr) // it was pointing to the last Node in the chain
	{
		index_++; // move to next chain
		while (true)
		{
			if (index_ == tableSize_) // reached the end of table, should return end()
			{
				// note that actual table size is tableSize_ + 1 (additional 1 slot is for iterator optimizing)
				node_ = nullptr;
				break;
			}
			else if (table_[index_] == nullptr) // empty bucket
			{
				index_++;
			}
			else
			{
				node_ = table_[index_];
				break;
			}
		}
	}
	else
	{
		node_ = node_->next_;
	}
	return *this;
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
typename HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl &
HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl::operator--()
{
	if (table_[index_] == node_) // we cannot move backward inside this chain
	{
		index_--;
		while (true)
		{
			if (index_ < 0) // got past the first bucket; this iterator becomes invalid
			{
				node_ = nullptr;
				break;
			}
			else if (table_[index_] == nullptr) // empty bucket
			{
				index_--;
			}
			else
			{
				// set Node to the tail of the chain
				for (node_ = table_[index_]; node_->next_ != nullptr; node_ = node_->next_)
				{
				}
				break;
			}
		}
	}
	else
	{
		Node *previous = table_[index_];
		while (previous->next_ != node_)
		{
			previous = previous->next_;
		}
		node_ = previous;
	}
	return *this;
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
HashTable<T, K, ExtractKey, HashFunc>::HashTable()
{
	init();
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
HashTable<T, K, ExtractKey, HashFunc>::~HashTable()
{
	deleteTable(table_, tableSize_);
}

// copy con
template <typename T, typename K, typename ExtractKey, typename HashFunc>
HashTable<T, K, ExtractKey, HashFunc>::HashTable(HashTable const &rhs)
{
	init();
	operator=(rhs);
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
HashTable<T, K, ExtractKey, HashFunc>::HashTable(HashTable &&rhs)
{
	init();
	swap(rhs);
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
HashTable<T, K, ExtractKey, HashFunc> &
HashTable<T, K, ExtractKey, HashFunc>::operator=(HashTable const &rhs)
{
	if (this == &rhs)
	{
		return *this;
	}
	if (rhs.empty())
	{
		clear();
		return *this;
	}
	deleteTable(table_, tableSize_);

	size_ = rhs.size_;
	tableSize_ = rhs.tableSize_;
	hashSetSizeIndex_ = rhs.hashSetSizeIndex_;
	initTable(tableSize_ + 1);
	for (size_t i = 0; i < rhs.tableSize_; ++i)
	{
		Node *sourceNode = rhs.table_[i];
		while (true)
		{
			if (sourceNode == nullptr)
			{
				break;
			}
			else
			{
				Node *copy = new Node(sourceNode->elem_, nullptr);
				Node *firstNode = table_[i];
				table_[i] = copy;
				copy->next_ = firstNode;
				sourceNode = sourceNode->next_;
			}
		}
	}
	return *this;
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
HashTable<T, K, ExtractKey, HashFunc> &
HashTable<T, K, ExtractKey, HashFunc>::operator=(HashTable &&rhs)
{
	if (this != &rhs)
	{
		swap(rhs);
	}
	return *this;
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
void HashTable<T, K, ExtractKey, HashFunc>::swap(HashTable &rhs)
{
	std::swap(table_, rhs.table_);
	std::swap(size_, rhs.size_);
	std::swap(tableSize_, rhs.tableSize_);
	std::swap(hashSetSizeIndex_, rhs.hashSetSizeIndex_);
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
void HashTable<T, K, ExtractKey, HashFunc>::init()
{
	size_ = 0;
	hashSetSizeIndex_ = 0;
	tableSize_ = 0;
	table_ = nullptr;
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
void HashTable<T, K, ExtractKey, HashFunc>::initTable(size_t size)
{
	//std::cout << "inittable size: " << size << "\n";
	table_ = new Node *[size];
	for (size_t i = 0; i < size; i++)
	{
		table_[i] = nullptr;
	}
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
typename HashTable<T, K, ExtractKey, HashFunc>::Node *
HashTable<T, K, ExtractKey, HashFunc>::getBucket(K const &key) const
{
	size_t index = HashFunc()(key) % tableSize_;
	return table_[index];
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
size_t
HashTable<T, K, ExtractKey, HashFunc>::getIndex(K const &key) const
{
	return HashFunc()(key) % tableSize_;
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
typename HashTable<T, K, ExtractKey, HashFunc>::const_iterator
HashTable<T, K, ExtractKey, HashFunc>::find(K const &key) const
{
	return findIterator(key);
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
typename HashTable<T, K, ExtractKey, HashFunc>::iterator
HashTable<T, K, ExtractKey, HashFunc>::find(K const &key)
{
	return findIterator(key);
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
size_t HashTable<T, K, ExtractKey, HashFunc>::count(K const &key) const
{
	if (findIterator(key) == getEndIterator())
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
typename HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl
HashTable<T, K, ExtractKey, HashFunc>::findIterator(K const &key) const
{
	if (empty())
	{
		return getEndIterator();
	}
	size_t index = getIndex(key);
	Node *thisNode = table_[index];
	while (true)
	{
		if (thisNode == nullptr)
		{
			return getEndIterator();
		}
		if (ExtractKey()(thisNode->elem_) == key)
		{
			return IteratorImpl(table_, tableSize_, index, thisNode);
		}
		else
		{
			thisNode = thisNode->next_;
		}
	}
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
template <typename T1>
std::pair<typename HashTable<T, K, ExtractKey, HashFunc>::iterator, bool>
HashTable<T, K, ExtractKey, HashFunc>::insert(T1 &&elem)
{
	//cout << "insert\n";
	if (empty())
	{
		tableSize_ = HASH_SET_SIZES[hashSetSizeIndex_];
		initTable(tableSize_ + 1);
	}
	size_t index = getIndex(ExtractKey()(elem));
	Node *thisNode = table_[index];
	Node *newNode;
	int count = 0;
	while (true)
	{
		if (thisNode == nullptr)
		{ // not found; insert
			Node *firstNode = table_[index];
			newNode = new Node(T(std::forward<T1>(elem)), firstNode);
			table_[index] = newNode;
			break;
		}
		else if (ExtractKey()(thisNode->elem_) == ExtractKey()(elem))
		{
			return std::pair<iterator, bool>(iterator(IteratorImpl(table_, tableSize_, index, thisNode)), false);
		}
		else
		{
			count++;
			thisNode = thisNode->next_;
		}
	}
	if ((double)size_ / tableSize_ > REHASH_THRESHOLD)
	{
		rehash();
	}
	if (count > 9)
	{
		//cout << "count was " << count << " size was " << size_ << " table size was " << tableSize_ << "\n";
		//exit(0);
	}
	size_++;
	return std::pair<iterator, bool>(iterator(IteratorImpl(table_, tableSize_, index, newNode)), true);
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
size_t
HashTable<T, K, ExtractKey, HashFunc>::erase(K const &key)
{
	if (empty())
	{
		return 0;
	}
	Node *previous = nullptr;
	size_t index = getIndex(key);
	Node *thisNode = table_[index];
	while (true)
	{
		if (thisNode == nullptr)
		{
			return 0;
		}
		if (ExtractKey()(thisNode->elem_) == key)
		{
			if (previous != nullptr)
			{
				previous->next_ = thisNode->next_;
			}
			else // first node in chain
			{
				table_[index] = thisNode->next_;
			}
			delete thisNode;
			break;
		}
		else
		{
			previous = thisNode;
			thisNode = thisNode->next_;
		}
	}
	size_--;
	if (empty())
	{
		clear();
	}
	return 1;
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
void HashTable<T, K, ExtractKey, HashFunc>::rehash()
{
	//std::cout << "rehashing\n";
	Node **oldTable = table_;
	size_t oldTableSize = tableSize_;
	hashSetSizeIndex_++;
	if (hashSetSizeIndex_ >= 26)
	{
		std::cout << "HashTable rehash too big\n";
		exit(0);
	}
	tableSize_ = HASH_SET_SIZES[hashSetSizeIndex_];
	initTable(tableSize_ + 1);
	for (size_t i = 0; i < oldTableSize; ++i)
	{
		Node *sourceBucket = oldTable[i];
		while (true)
		{
			if (sourceBucket == nullptr)
			{
				break;
			}
			Node *nodeToMove = sourceBucket;
			sourceBucket = sourceBucket->next_;
			addToFirst(nodeToMove);
		}
		oldTable[i] = nullptr;
	}
	deleteTable(oldTable, oldTableSize);
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
void HashTable<T, K, ExtractKey, HashFunc>::addToFirst(Node *node)
{
	//std::cout << "addToFirst\n";
	size_t index = getIndex(ExtractKey()(node->elem_));
	Node *firstNode = table_[index];
	table_[index] = node;
	node->next_ = firstNode;
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
typename HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl
HashTable<T, K, ExtractKey, HashFunc>::getBeginIterator() const
{
	if (empty())
	{
		return getEndIterator();
	}
	size_t index = 0;
	while (true)
	{
		if (table_[index] == nullptr && index < tableSize_)
		{
			index++;
		}
		else
		{
			break;
		}
	}
	return IteratorImpl(table_, tableSize_, index, table_[index]);
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
typename HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl
HashTable<T, K, ExtractKey, HashFunc>::getEndIterator() const
{
	return IteratorImpl(table_, tableSize_, tableSize_, nullptr);
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
void HashTable<T, K, ExtractKey, HashFunc>::clear()
{
	deleteTable(table_, tableSize_);
	init();
}

template <typename T, typename K, typename ExtractKey, typename HashFunc>
void HashTable<T, K, ExtractKey, HashFunc>::deleteTable(Node **table, size_t tableSize)
{
	//std::cout << "Destruct buckets called on tableSize: " << tableSize <<
	//	" size: " << this->size_ << "\n" ;
	for (size_t i = 0; i < tableSize; ++i)
	{
		Node *node = table[i];
		while (true)
		{
			if (node == nullptr)
			{
				break;
			}
			else
			{
				Node *nodeToDelete = node;
				node = node->next_;
				delete nodeToDelete;
			}
		}
	}
	delete table;
}

} // namespace lib_calvin_container

#endif
