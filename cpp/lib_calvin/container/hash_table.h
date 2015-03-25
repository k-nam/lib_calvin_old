#ifndef LIB_CALVIN__CONTAINER__HASH_SET_IMPL_H
#define LIB_CALVIN__CONTAINER__HASH_SET_IMPL_H

#include "vector.h"
#include "boost/functional/hash.hpp"

namespace lib_calvin_container
{
double const REHASH_THRESHOLD = 1;
size_t const HASH_SET_SIZES[26] = { 53, 97, 193, 389, 769, 1543, 3079, 6151, 
															12289, 24593, 49157, 98317, 196613, 
															393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 
															100663319, 201326611, 402653189, 805306457, 1610612741 };

template <typename T, typename K = T, typename ExtractKey = std::identity<T>,
	typename HashFunc = boost::hash<K>> 
class HashTable 
{
public:
	typedef T value_type;
	typedef T * pointer;
	typedef T & reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
private:
	struct Node {
		Node(T const &elem, Node *next): elem_(elem), next_(next) { }
		T elem_;
		Node *next_; // empty bucket points to nullptr, and the last Node points to the fist Node
	};
private:
	class IteratorImpl {
	public:
		typedef T value_type;
		typedef T const * pointer;
		typedef T const & reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef std::bidirectional_iterator_tag iterator_category;
		reference operator*() const { return node_->elem_; }
		pointer operator->() const { return &(node_->elem_); }
		IteratorImpl();
		IteratorImpl(Node *table, size_t tableSize, Node *bucket, Node *Node);
		// copy ctor	
		IteratorImpl(IteratorImpl const &rhs);
		IteratorImpl & operator=(IteratorImpl const &rhs);
		IteratorImpl & operator++(); // prefix
		IteratorImpl & operator--();
		IteratorImpl const operator++(int) { IteratorImpl returnValue(*this);
			operator++(); return returnValue; }// suffix			
		IteratorImpl const operator--(int) { IteratorImpl returnValue(*this);
			operator--(); return returnValue; } // suffix	
		bool operator==(IteratorImpl const &rhs) const;
		bool operator!=(IteratorImpl const &rhs) const { return !(*this == rhs); }
	private:
		Node *table_;
		size_t tableSize_;
		Node *bucket_; // first Node of current bucket
		Node *node_; // actual Node pointed by this iterator; null for end iterator
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
	HashTable &operator=(HashTable const &hrs); // assignment
	~HashTable();
public:
	size_t size() const { return size_; }
	const_iterator find(K const &) const;
	iterator find(K const &);
	size_t count(K const &elem) const;
	std::pair<iterator, bool> insert(T const &elem);
	size_t erase(K const &elem);
	void clear();
	bool empty() const { return size_ == 0; }
private:
	Node *getBucket(K const &) const;
	void rehash();
private:
	void init();
	void initTable(size_t);
	void deleteTable(Node *, size_t);
private: // insert given elem or Node into the bucket without checking (only used in rehasing)
	void addToFirst(T const &elem);
	void addToFirst(Node *Node);
private:
	IteratorImpl getBeginIterator() const;
	IteratorImpl getEndIterator() const;
	IteratorImpl findIterator(K const &) const;
private:
	size_t size_;
	Node *table_;
	size_t tableSize_;
	size_t hashSetSizeIndex_;
};

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
bool operator==(HashTable<T, K, ExtractKey, HashFunc> const &lhs, 
								HashTable<T, K, ExtractKey, HashFunc> const &rhs) {
	return containerEqual(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
bool operator!=(HashTable<T, K, ExtractKey, HashFunc> const &lhs, 
								HashTable<T, K, ExtractKey, HashFunc> const &rhs) {
	return !(lhs == rhs);
}

/*
template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
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

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl::IteratorImpl(): 
	table_(nullptr), tableSize_(0), bucket_(nullptr), node_(nullptr) { }

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl::IteratorImpl(
	Node *table, size_t tableSize, Node *bucket, Node *Node):
	table_(table), tableSize_(tableSize), bucket_(bucket), node_(Node) { }

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl::IteratorImpl(IteratorImpl const &rhs): 
	table_(rhs.table_), tableSize_(rhs.tableSize_), bucket_(rhs.bucket_), node_(rhs.node_) { }

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
typename HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl &
HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl::operator=(IteratorImpl const &rhs) {
	table_ = rhs.table_;
	tableSize_ = rhs.tableSize_;
	bucket_ = rhs.bucket_;
	node_ = rhs.node_;
	return *this;
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
bool HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl::operator==(IteratorImpl const &rhs) const {
	return node_ == rhs.node_;
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
typename HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl & 
HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl::operator++() {
	if (node_->next_ == bucket_) { // it was pointing to the last Node in the chain
		bucket_++; // move to next chain
		while (true) {
			if (bucket_ - table_ >= static_cast<int>(tableSize_)) { // reached the end of table
				node_ = bucket_; // to make operator--() faster by reducing index comparison
				break;
			} else if (bucket_->next_ == nullptr) { // empty bucket
				bucket_++;
			} else {
				node_ = bucket_;
				break;
			}
		}
	} else { 
		node_ = node_->next_;
	}
	return *this;
}
template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
typename HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl & 
HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl::operator--() {
	if (node_ == bucket_) { // we cannot move backward inside this chain
		bucket_--;
		while (true) {
			if (bucket_ < table_) { // reached the first bucket; this iterator becomes invalid
				node_ = nullptr;
				break;
			} else if (bucket_->next_ == nullptr) { // empty bucket
				bucket_--;
			} else { // set Node to the tail of the chain
				for (node_ = bucket_; node_->next_ != bucket_; node_ = node_->next_) { }
				break;
			}
		}
	} else { 
		Node *previous = bucket_;
		while (previous->next_ != node_) {
			previous = previous->next_;
		}
		node_ = previous;
	}
	return *this;
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
HashTable<T, K, ExtractKey, HashFunc>::HashTable() {
	init();
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
HashTable<T, K, ExtractKey, HashFunc>::~HashTable() {
	deleteTable(table_, tableSize_);
}

// copy con
template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
HashTable<T, K, ExtractKey, HashFunc>::HashTable(HashTable const &rhs) {
	operator=(rhs);
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
HashTable<T, K, ExtractKey, HashFunc> &
HashTable<T, K, ExtractKey, HashFunc>::operator=(HashTable const &rhs) {
	if (this == &rhs) {
		return *this;
	}
	deleteTable(table_, tableSize_);

	size_ = rhs.size_;
	tableSize_ = rhs.tableSize_;
	hashSetSizeIndex_ = rhs.hashSetSizeIndex_;
	initTable(tableSize_ + 1);
	for (size_t i = 0; i < rhs.tableSize_; ++i) {
		Node *sourceBucket = &rhs.table_[i];
		Node *sourceNode = sourceBucket;
		Node *targetBucket = &table_[i];
		Node *targetNode = targetBucket;
		if (sourceBucket->next_ == nullptr) { // empty bucket
			continue;
		}
		while (true) {
			new (&targetNode->elem_) T(sourceNode->elem_);
			if (sourceNode->next_ == sourceBucket) {
				targetNode->next_ = targetBucket;
				break;
			} else {
				sourceNode = sourceNode->next_;
				Node *newTargetNode = (Node *)operator new(sizeof(Node));
				targetNode->next_ = newTargetNode;
				targetNode = newTargetNode;
			}
		}
	}
	return *this;
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
void HashTable<T, K, ExtractKey, HashFunc>::init() {
	size_ = 0;
	hashSetSizeIndex_ = 0;
	tableSize_ = HASH_SET_SIZES[hashSetSizeIndex_];
	initTable(tableSize_ + 1);
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
void HashTable<T, K, ExtractKey, HashFunc>::initTable(size_t size) {
	//std::cout << "inittable size: " << size << "\n";
	table_ = (Node *)operator new(sizeof(Node) * size);
	for (size_t i = 0; i < size; i++) {
		table_[i].next_ = nullptr;
	}
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
typename HashTable<T, K, ExtractKey, HashFunc>::Node * 
HashTable<T, K, ExtractKey, HashFunc>::getBucket(K const &key) const {
	size_t index = HashFunc()(key) % tableSize_;
	return &table_[index];
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
typename HashTable<T, K, ExtractKey, HashFunc>::const_iterator
HashTable<T, K, ExtractKey, HashFunc>::find(K const &key) const {
	return findIterator(key);
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
typename HashTable<T, K, ExtractKey, HashFunc>::iterator
HashTable<T, K, ExtractKey, HashFunc>::find(K const &key) {
	return findIterator(key);
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
size_t HashTable<T, K, ExtractKey, HashFunc>::count(K const &key) const {
	if (findIterator(key) == getEndIterator()) {
		return 0;
	} else {
		return 1;
	}
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
typename HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl 
HashTable<T, K, ExtractKey, HashFunc>::findIterator(K const &key) const {
	Node *bucket = getBucket(key);
	Node *thisNode = bucket;
	if (bucket->next_ == nullptr) { // not found
		return getEndIterator();
	}
	while (true) {
		if (ExtractKey()(thisNode->elem_) == key) {
			return IteratorImpl(table_, tableSize_, bucket, thisNode);
		} else {
			thisNode = thisNode->next_;
			if (thisNode == bucket) { // not found
				return getEndIterator();
			}
		}
	}
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
std::pair<typename HashTable<T, K, ExtractKey, HashFunc>::iterator, bool> 
HashTable<T, K, ExtractKey, HashFunc>::insert(T const &elem) {
	Node *bucket = getBucket(ExtractKey()(elem));
	Node *thisNode = bucket;
	int count = 0;
	if (bucket->next_ == nullptr) { // empty bucket, just insert
		new (&bucket->elem_) T(elem);
		bucket->next_ = bucket;
	} else {
		while (true) {
			if (ExtractKey()(thisNode->elem_) == ExtractKey()(elem)) {
				return std::pair<iterator, bool>(iterator(
					IteratorImpl(table_, tableSize_, bucket, thisNode)), false);
			} else {
				count++;
				thisNode = thisNode->next_;
				if (thisNode == bucket) {
					break;
				}
			}
		}
		Node *newNode = new Node(elem, bucket->next_);
		bucket->next_ = newNode;
	}
	size_++;
	if ((double)size_ / tableSize_ > REHASH_THRESHOLD) {
		rehash();
	}
	if (count > 9) {
		cout << "count was " << count << " size was " << size_ << " table size was " << tableSize_ << "\n";
		exit(0);
	}
	return std::pair<iterator, bool>(iterator(findIterator(ExtractKey()(elem))), true);
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
size_t
HashTable<T, K, ExtractKey, HashFunc>::erase(K const &key) {
	Node *bucket = getBucket(key);
	if (bucket->next_ == nullptr) { // empty bucket, no delete
		return 0;
	} else if (bucket->next_ == bucket) { // only one Node
		if (ExtractKey()(bucket->elem_) == key) { 
			bucket->elem_.~T();
			bucket->next_ = nullptr;
		} else { 
			return 0;
		}
	} else if (ExtractKey()(bucket->elem_) == key) { // multiple Node and delete first Node
		Node *nodeToDelete = bucket->next_;
		bucket->elem_ = std::move(nodeToDelete->elem_);
		bucket->next_ = bucket->next_->next_;
		delete nodeToDelete;
	} else {
		Node *thisNode = bucket;
		while (true) {
			if (ExtractKey()(thisNode->next_->elem_) == key) {
				Node *nodeToDelete = thisNode->next_;
				thisNode->next_ = thisNode->next_->next_;
				delete nodeToDelete;
				break;
			} else {
				thisNode = thisNode->next_;
				if (thisNode->next_ == bucket) {
					return 0;
				}
			}
		}
	}
	size_--;
	return 1;
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
void HashTable<T, K, ExtractKey, HashFunc>::rehash() {
	//std::cout << "rehashing\n";
	Node *oldTable = table_;
	size_t oldTableSize = tableSize_;
	hashSetSizeIndex_++;
	tableSize_ = HASH_SET_SIZES[hashSetSizeIndex_];
	initTable(tableSize_ + 1);
	for (size_t i = 0; i < oldTableSize; ++i) {
		Node *sourceBucket = &oldTable[i];
		if (sourceBucket->next_ == nullptr) {
			continue;
		}
		addToFirst(sourceBucket->elem_); // special case for bucket Node
		while (true) {
			if (sourceBucket->next_ == sourceBucket) {
				break;
			} else {
				Node *nodeToMove = sourceBucket->next_;
				sourceBucket->next_ = sourceBucket->next_->next_;
				addToFirst(nodeToMove);
			}
		}
	}
	deleteTable(oldTable, oldTableSize); 
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
void HashTable<T, K, ExtractKey, HashFunc>::addToFirst(T const &elem) {
	HashTable<T, K, ExtractKey, HashFunc>::Node *bucket = getBucket(ExtractKey()(elem));
	if (bucket->next_ == nullptr) {
		new (&bucket->elem_) T(elem);
		bucket->next_ = bucket;
	} else {
		Node *newNode = new Node(elem, bucket->next_);
		bucket->next_ = newNode;
	}
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
void HashTable<T, K, ExtractKey, HashFunc>::addToFirst(Node *Node) {
	HashTable<T, K, ExtractKey, HashFunc>::Node *bucket = getBucket(ExtractKey()(Node->elem_));
	if (bucket->next_ == nullptr) {
		new (&bucket->elem_) T(Node->elem_);
		bucket->next_ = bucket;
		delete Node;
	} else {
		Node->next_ = bucket->next_;
		bucket->next_ = Node;
	}
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
typename HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl
HashTable<T, K, ExtractKey, HashFunc>::getBeginIterator() const {
	Node *beginBucket = table_;
	while (beginBucket->next_ == nullptr && beginBucket - table_ < static_cast<int>(tableSize_)) {
		beginBucket++;
	}
	return IteratorImpl(table_, tableSize_, beginBucket, beginBucket);
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
typename HashTable<T, K, ExtractKey, HashFunc>::IteratorImpl
HashTable<T, K, ExtractKey, HashFunc>::getEndIterator() const {
	return IteratorImpl(table_, tableSize_, table_ + tableSize_, table_ + tableSize_);
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
void HashTable<T, K, ExtractKey, HashFunc>::clear() {
	deleteTable(table_, tableSize_);
	init();
}

template <typename T, typename K, typename typename ExtractKey, typename HashFunc> 
void HashTable<T, K, ExtractKey, HashFunc>::deleteTable(Node *table, size_t tableSize) {
	//std::cout << "Destruct buckets called on tableSize: " << tableSize << 
	//	" size: " << this->size_ << "\n" ; 
	for (size_t i = 0; i < tableSize; ++i) {
		Node *bucket = &table[i];
		if (bucket->next_ == nullptr) { // empty bucket
			continue;
		}
		bucket->elem_.~T();
		while (true) {
			if (bucket->next_ == bucket) {
				break;
			} else {
				Node *nodeToDelete = bucket->next_;
				bucket->next_ = bucket->next_->next_;
				delete nodeToDelete;
			}
		}
	}
	operator delete(table);
}

}

#endif



