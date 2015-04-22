#ifndef LIB_CALVIN__CONTAINER__LINKED_ARRAY
#define LIB_CALVIN__CONTAINER__LINKED_ARRAY

#include "container.h"
#include <iostream>

namespace lib_calvin_container
{

template <typename T>
class BlockedArray {
private:
static int const BLOCKED_ARRAY_BLOCK_SIZE = 1024;
static int const BLOCKED_ARRAY_BLOCK_INIT_CAPACITY = 4;
static int const BLOCKED_ARRAY_BLOCK_REALLOC_MULTIPLIER = 2;
	class Block {
	public:
		Block();
		Block(int capacity);
		~Block();
		bool isFull() const;
		void reserve(int);
		T & getElement(int index);
		T & getLastElement();
		T const & getElement(int index) const;
		T const & getLastElement() const;
		template <typename T1> void constructElement(int index, T1 &&elem);
		void destructElement(int index);
		template <typename T1> void assignElement(int index, T1 &&elem);
		template <typename T1> void insertElement(int index, T1 &&elem);
		template <typename T1> void insertLastElement(T1 &&elem);
		T *elements_;
		int size_;
		int capacity_;
		Block *next_; // end node's next is null
		Block *previous_; // head node's previous is null
	private:
		int targetCapacityFor(int capacity) const;
		void initWithCapacity(int capacity);
	};
public:
	typedef T value_type;
	typedef T * pointer;
	typedef T & reference;
	typedef T const & const_reference;
	typedef ptrdiff_t difference_type;

private:
	class IteratorImpl {
	public:
		typedef T value_type;
		typedef T * pointer;
		typedef T & reference;
		typedef T const & const_reference;
		typedef ptrdiff_t difference_type;
		typedef std::bidirectional_iterator_tag iterator_category;
		IteratorImpl();
		IteratorImpl(Block *block, int index);
		IteratorImpl(IteratorImpl const &rhs);
		IteratorImpl & operator=(IteratorImpl const &rhs);
		reference operator*() const;
		pointer operator->() const;
		IteratorImpl const operator+(ptrdiff_t offset) const;
		IteratorImpl const operator-(ptrdiff_t offset) const;
		IteratorImpl & operator+=(ptrdiff_t offset);
		IteratorImpl & operator-=(ptrdiff_t offset);
		IteratorImpl & operator++(); // prefix
		IteratorImpl & operator--();
		IteratorImpl const operator++(int);		
		IteratorImpl const operator--(int);
		difference_type operator-(IteratorImpl const &rhs) const;
		bool operator==(IteratorImpl const &rhs) const;
		bool operator!=(IteratorImpl const &rhs) const;
		bool operator<(IteratorImpl const &rhs) const;
		bool operator>(IteratorImpl const &rhs) const;
		bool operator<=(IteratorImpl const &rhs) const;
		bool operator>=(IteratorImpl const &rhs) const;
	public:
		Block *block_;
		int index_;
	private:
		bool isBegin() const;
		bool isEnd() const;
	};
public:
	typedef lib_calvin_container::Iterator<IteratorImpl> iterator;
	typedef lib_calvin_container::ConstIterator<IteratorImpl> const_iterator;
	typedef lib_calvin_container::ReverseIterator<IteratorImpl> reverse_iterator;
	typedef lib_calvin_container::ConstReverseIterator<IteratorImpl> const_reverse_iterator;

public:
	BlockedArray();
	BlockedArray(size_t, T const &elem = T());
	void init();
	~BlockedArray();
	BlockedArray(BlockedArray const &);
	BlockedArray(BlockedArray &&);
	BlockedArray & operator= (BlockedArray const &);
	BlockedArray & operator= (BlockedArray &&);
public:
	size_t size() const;
	bool empty() const;
	void resize(size_t, T const &elem = T());
	void reserve(size_t);
	reference operator[] (size_t index);
	const_reference operator[] (size_t index) const;
	template <typename T1> void push_back(T1 &&);
	void pop_back();
	reference front();
	reference back();
	const_reference front() const;
	const_reference back() const;
	// return the position of the first new elements
	template <typename T1> iterator insert(const_iterator const &pos, T1 &&elem);
	template <typename Iterator> 
		iterator insert(const_iterator const &pos, Iterator const &begin, Iterator const &end);
	void clear();
	void swap(BlockedArray &rhs);
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
	Block *getLastBlock();
	Block *copyBlock(Block const *original); // get a copy
	// split full block into half. returns number of elements remaining in original block
	int splitBlock(Block *); 
	// copy chain of blocks starting from given block and return the head of new chain
	void copyBlockChain(Block const *sourceFirst, Block *&resultFirst, Block *&resultLast);
	Block *pushBackNewBlock();
	void popLastBlock();
	void deleteAllBlocks();
private: // member variables	
	Block *head_;
	Block *end_;
	size_t size_;

}; // end BlockedArray

template <typename T>
bool operator== (BlockedArray<T> const &lhs, BlockedArray<T> const &rhs);

template <typename T>
bool operator!= (BlockedArray<T> const &lhs, BlockedArray<T> const &rhs);

template <typename T>
bool operator< (BlockedArray<T> const &lhs, BlockedArray<T> const &rhs);

template <typename T>
bool operator<= (BlockedArray<T> const &lhs, BlockedArray<T> const &rhs);

template <typename T>
bool operator> (BlockedArray<T> const &lhs, BlockedArray<T> const &rhs);

template <typename T>
bool operator>= (BlockedArray<T> const &lhs, BlockedArray<T> const &rhs);


/******************************* definitions *****************************/

template <typename T>
BlockedArray<T>::BlockedArray() {
	init();
}

template <typename T>
BlockedArray<T>::BlockedArray(size_t size, T const &elem) {
	init();
	resize(size, elem);
}

template <typename T>
void BlockedArray<T>::init() {
	size_ = 0;
	end_ = new Block();
	end_->previous_ = NULL;
	end_->next_ = NULL;
	head_ = end_;
}

template <typename T>
BlockedArray<T>::~BlockedArray() {
	clear();
	delete end_;
}

template <typename T>
BlockedArray<T>::BlockedArray(BlockedArray const &rhs): head_(NULL), end_(NULL), size_(rhs.size_) {
	copyBlockChain(rhs.head_, head_, end_);
}


template <typename T>
BlockedArray<T>::BlockedArray(BlockedArray &&rhs) {
	init();
	swap(rhs);
}

template <typename T>
BlockedArray<T> & BlockedArray<T>::operator=(BlockedArray const &rhs) {
	if (this == &rhs) {
		return *this;
	}
	size_ = rhs.size_;
	copyBlockChain(rhs.head_, head_, end_);
	return *this;
}

template <typename T>
BlockedArray<T> & BlockedArray<T>::operator=(BlockedArray &&rhs) {
	if (this == &rhs) {
		return *this;
	}
	swap(rhs);
	return *this;
}

template <typename T>
size_t BlockedArray<T>::size() const {
	return size_;
}

template <typename T>
bool BlockedArray<T>::empty() const {
	return size_ == 0;
}

template <typename T>
void BlockedArray<T>::resize(size_t newSize, T const &elem) {
	if (newSize > size_) {
		size_t numElementsToConstruct = newSize - size_;
		while (numElementsToConstruct > 0) {
			if (getLastBlock() == NULL || getLastBlock()->isFull()) { // need to allocate new block
				Block *newBlock = pushBackNewBlock();
			}
			int capacityToUse = BLOCKED_ARRAY_BLOCK_SIZE - getLastBlock()->size_;
			if (capacityToUse > numElementsToConstruct) {
				capacityToUse = static_cast<int>(numElementsToConstruct);
			}
			getLastBlock()->reserve(getLastBlock()->size_ + capacityToUse); // constructElement does not check capacity
			for (int i = getLastBlock()->size_; i < getLastBlock()->size_ + capacityToUse; ++i) {
				getLastBlock()->constructElement(i, elem);
			}
			getLastBlock()->size_ += capacityToUse;
			numElementsToConstruct -= capacityToUse;
		}
	} else if (newSize < size_) {
		size_t numElementsToDestruct = size_ - newSize;
		while (numElementsToDestruct > 0) {
			int sizeToDelete = getLastBlock()->size_;
			if (sizeToDelete > numElementsToDestruct) {
				sizeToDelete = static_cast<int>(numElementsToDestruct);
			}
			for (int i = getLastBlock()->size_ - 1; i >= getLastBlock()->size_ - sizeToDelete; i--) {
				getLastBlock()->destructElement(i);
			}
			getLastBlock()->size_ -= sizeToDelete;
			numElementsToDestruct -= sizeToDelete;
			if (getLastBlock()->size_ == 0) {
				popLastBlock();
			}
		}
	} else { // nothing to do
	}
	size_ = newSize;
}

template <typename T>
void BlockedArray<T>::reserve(size_t newSize) {

}

template <typename T>
typename BlockedArray<T>::reference BlockedArray<T>::operator[](size_t index) {
	return const_cast<typename BlockedArray<T>::reference>(
		static_cast<BlockedArray const *>(this)->operator[](index));
}

template <typename T>
typename BlockedArray<T>::const_reference BlockedArray<T>::operator[](size_t index) const {
	IteratorImpl iter(head_, 0);
	iter += index;
	return *iter;
}

template <typename T>
template <typename T1>
void BlockedArray<T>::push_back(T1 &&elem) {
	Block *lastBlock = this->getLastBlock();
	if (size_ > 0 && lastBlock->isFull() == false) {
		lastBlock->insertLastElement(std::forward<T1>(elem));
	} else {
		lastBlock = pushBackNewBlock();
		lastBlock->constructElement(0, std::forward<T1>(elem));
	}
	lastBlock->size_++;
	size_++;
	return;
}

template <typename T>
void BlockedArray<T>::pop_back() {
	Block *lastBlock = this->getLastBlock();
	lastBlock->getLastElement().~T();
	lastBlock->size_--;
	if (lastBlock->size_ == 0) {
		popLastBlock();
	} else {
	}
	size_--;
}

template <typename T>
typename BlockedArray<T>::reference BlockedArray<T>::front() {
	return head_->getElement(0);
}

template <typename T>
typename BlockedArray<T>::reference BlockedArray<T>::back() {
	return getLastBlock()->getLastElement();
}

template <typename T>
typename BlockedArray<T>::const_reference BlockedArray<T>::front() const {
	return head_->getElement(0);
}

template <typename T>
typename BlockedArray<T>::const_reference BlockedArray<T>::back() const {
	return getLastBlock()->getLastElement();
}

template <typename T>
template <typename T1>
typename BlockedArray<T>::iterator 
BlockedArray<T>::insert(const_iterator const &pos, T1 &&elem) {
	IteratorImpl iter = pos.getImpl();
	if (iter.block_ == end_) { // pos is end()
		push_back(std::forward<T1>(elem));
	} else {
		if (iter.block_->isFull()) {
			// break this full block into half
			int numRemainingElements = splitBlock(iter.block_);
			if (iter.index_ < numRemainingElements) { // iter is still valid
			} else { // iter should be modified
				iter.block_ = iter.block_->next_;
				iter.index_ -= numRemainingElements;
			}
		} else {
		}
		iter.block_->insertElement(iter.index_, std::forward<T1>(elem));
		iter.block_->size_++;
		size_++;
	}
	return iterator(iter);
}

template <typename T>
template <typename Iterator>
typename BlockedArray<T>::iterator 
BlockedArray<T>::insert(const_iterator const &pos, Iterator const &begin, Iterator const &end) {
}

template <typename T>
void BlockedArray<T>::clear() {
	Block *current = head_;
	while (true) {
		if (current == end_) {
			break;
		} else {
			Block *next = current->next_;
			//std::cout << "deleting block of size " << current->size_ << "\n";
			delete current;
			current = next;
		}
	}
	size_ = 0;
	end_->previous_ = NULL;
	head_ = end_;
}

template <typename T>
void BlockedArray<T>::swap(BlockedArray &rhs) {
	size_t rhsSize = rhs.size_;
	Block *rhsHead = rhs.head_;
	Block *rhsEnd = rhs.end_;
	rhs.size_ = size_;
	rhs.head_ = head_;
	rhs.end_ = end_;
	size_ = rhsSize;
	head_ = rhsHead;
	end_ = rhsEnd;
}

template <typename T>
typename BlockedArray<T>::const_iterator 
BlockedArray<T>::begin() const {
	return IteratorImpl(head_, 0);
}

template <typename T>
typename BlockedArray<T>::const_iterator 
BlockedArray<T>::end() const {
	return IteratorImpl(end_, 0);
}
template <typename T>
typename BlockedArray<T>::iterator 
BlockedArray<T>::begin() {
	return IteratorImpl(head_, 0);
}

template <typename T>
typename BlockedArray<T>::iterator 
BlockedArray<T>::end() {
	return IteratorImpl(end_, 0);
}

template <typename T>
typename BlockedArray<T>::const_reverse_iterator 
BlockedArray<T>::rbegin() const {
	return IteratorImpl(end_, 0);
}

template <typename T>
typename BlockedArray<T>::const_reverse_iterator 
BlockedArray<T>::rend() const {
	return IteratorImpl(head_, 0);
}

template <typename T>
typename BlockedArray<T>::reverse_iterator 
BlockedArray<T>::rbegin() {
	return IteratorImpl(end_, 0);
}

template <typename T>
typename BlockedArray<T>::reverse_iterator 
BlockedArray<T>::rend() {
	return IteratorImpl(head_, 0);
}

template <typename T>
typename BlockedArray<T>::Block *
BlockedArray<T>::getLastBlock() {
	return end_->previous_;
}

template <typename T>
typename BlockedArray<T>::Block *
BlockedArray<T>::copyBlock(Block const *original) {
	Block *block = new Block(original->capacity_);
	block->size_ = original->size_;
	copyConstruct(original->elements_, block->elements_, original->size_); 
	return block;
}

template <typename T>
int BlockedArray<T>::splitBlock(Block *block) {
	int numElementsToMove = block->size_ / 2;
	int numElementsToRemain = block->size_ - numElementsToMove;
	Block *newBlock = new Block(numElementsToMove);
	moveConstructAndDestruct(block->elements_ + numElementsToRemain, newBlock->elements_, numElementsToMove);
	block->size_ -= numElementsToMove;
	newBlock->size_ = numElementsToMove;
	// modify links
	newBlock->next_ = block->next_;
	block->next_->previous_ = newBlock;
	block->next_ = newBlock;
	newBlock->previous_ = block;
	return numElementsToRemain;
}

template <typename T>
void BlockedArray<T>::copyBlockChain(Block const *sourceHead, Block *&targetHead, Block *&targetEnd) {
	Block const *sourceBlock = sourceHead;
	Block *targetBlock = copyBlock(sourceBlock);
	targetBlock->previous_ = NULL;
	targetHead = targetBlock;
	while (true) {
		if (sourceBlock->next_ == NULL) { // this is end block
			targetBlock->next_ = NULL;
			targetEnd = targetBlock;
			break;
		}
		Block *targetNextBlock = copyBlock(sourceBlock->next_);
		targetBlock->next_ = targetNextBlock;
		targetNextBlock->previous_ = targetBlock;
		targetBlock = targetBlock->next_;
		sourceBlock = sourceBlock->next_;
	}
}

template <typename T>
typename BlockedArray<T>::Block * 
BlockedArray<T>::pushBackNewBlock() {
	Block *block = new Block();
	if (end_->previous_ == NULL) {
		head_ = block;
		block->previous_ = NULL;
	} else {
		block->previous_ = end_->previous_;
		block->previous_->next_ = block;
	}
	block->next_ = end_;
	end_->previous_ = block;
	return block;
}

template <typename T>
void BlockedArray<T>::popLastBlock() {
	Block *lastBlock = getLastBlock();
	if (lastBlock->previous_ == NULL) { // now goes empty 
		head_ = end_;
		end_->previous_ = NULL;
	} else {
		lastBlock->previous_->next_ = end_;
		end_->previous_ = lastBlock->previous_;
	}
	delete lastBlock;	
}

template <typename T>
void BlockedArray<T>::deleteAllBlocks() {

}

template <typename T>
bool operator== (BlockedArray<T> const &lhs, BlockedArray<T> const &rhs) {
	return containerEqual(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T>
bool operator!= (BlockedArray<T> const &lhs, BlockedArray<T> const &rhs) {
	return !(lhs == rhs);
}

template <typename T>
bool operator< (BlockedArray<T> const &lhs, BlockedArray<T> const &rhs) {
	return containerLess(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T>
bool operator<= (BlockedArray<T> const &lhs, BlockedArray<T> const &rhs) {
	return !(rhs < lhs);
}

template <typename T>
bool operator> (BlockedArray<T> const &lhs, BlockedArray<T> const &rhs) {
	return rhs < lhs;
}

template <typename T>
bool operator>= (BlockedArray<T> const &lhs, BlockedArray<T> const &rhs) {
	return !(lhs < rhs);
}

/**************************  Block methods  *************************/

template <typename T>
BlockedArray<T>::Block::Block() { 
	initWithCapacity(BLOCKED_ARRAY_BLOCK_INIT_CAPACITY);
}

template <typename T>
BlockedArray<T>::Block::Block(int capacity) { 
	initWithCapacity(capacity);
}

template <typename T>
BlockedArray<T>::Block::~Block() {
	for (int i = 0; i < size_; ++i) {
		elements_[i].~T();
	}
	operator delete(elements_);
}

template <typename T>
void BlockedArray<T>::Block::initWithCapacity(int capacity) {
	size_ = 0;
	capacity_ = targetCapacityFor(capacity);
	elements_ = static_cast<T *>(operator new(sizeof(T) * capacity_));
	next_ = NULL;
	previous_ = NULL;
}

template <typename T>
bool BlockedArray<T>::Block::isFull() const {
	return size_ == BLOCKED_ARRAY_BLOCK_SIZE;
}

template <typename T>
int BlockedArray<T>::Block::targetCapacityFor(int capacity) const {
	int targetCapacity = BLOCKED_ARRAY_BLOCK_INIT_CAPACITY;
	while (targetCapacity < capacity) {
		if (targetCapacity >= BLOCKED_ARRAY_BLOCK_SIZE) { // parameter was too large
			targetCapacity = BLOCKED_ARRAY_BLOCK_SIZE;
			break;
		} else {
			targetCapacity *= BLOCKED_ARRAY_BLOCK_REALLOC_MULTIPLIER;
		}
	}
	return targetCapacity;
}

template <typename T>
void BlockedArray<T>::Block::reserve(int capacity) {
	int targetCapacity = targetCapacityFor(capacity);
	T *oldArray = elements_;
	T *newArray = static_cast<T *>(operator new(sizeof(T) * targetCapacity));
	moveConstructAndDestruct(oldArray, newArray, size_);
	elements_ = newArray;
	capacity_ = targetCapacity;
	operator delete (oldArray);
}

template <typename T>
T & BlockedArray<T>::Block::getElement(int index) {
	return elements_[index];
}

template <typename T>
T & BlockedArray<T>::Block::getLastElement() {
	return elements_[size_ - 1];
}

template <typename T>
T const & BlockedArray<T>::Block::getElement(int index) const {
	return elements_[index];
}

template <typename T>
T const & BlockedArray<T>::Block::getLastElement() const {
	return elements_[size_ - 1];
}

template <typename T>
template <typename T1>
void BlockedArray<T>::Block::constructElement(int index, T1 &&elem) {
	new (elements_ + index) T(std::forward<T1>(elem));
}

template <typename T>
void BlockedArray<T>::Block::destructElement(int index) {
	elements_[index].~T();
}

template <typename T>
template <typename T1>
void BlockedArray<T>::Block::assignElement(int index, T1 &&elem) {
	elements_[index] = std::forward<T1>(elem);
}

template <typename T>
template <typename T1>
void BlockedArray<T>::Block::insertElement(int index, T1 &&elem) {
	if (size_ == capacity_) {
		reserve(size_ + 1);
	}
	insertSingleElement(elements_, size_, index, std::forward<T1>(elem));
}

template <typename T>
template <typename T1>
void BlockedArray<T>::Block::insertLastElement(T1 &&elem) {
	insertElement(size_, std::forward<T1>(elem));
}

/************************* Iterator methods *********************/

template <typename T>
BlockedArray<T>::IteratorImpl::IteratorImpl(): block_(NULL), index_(0) {
}

template <typename T>
BlockedArray<T>::IteratorImpl::IteratorImpl(Block *block, int index): block_(block), index_(index) { }

template <typename T>
BlockedArray<T>::IteratorImpl::IteratorImpl(IteratorImpl const &rhs): 
	block_(rhs.block_), index_(rhs.index_) { }

template <typename T>
typename BlockedArray<T>::IteratorImpl & 
BlockedArray<T>::IteratorImpl::operator=(IteratorImpl const &rhs) {
	block_ = rhs.block_;
	index_ = rhs.index_;
	return *this;
}

template <typename T>
typename BlockedArray<T>::reference 
BlockedArray<T>::IteratorImpl::operator*() const {
	return block_->getElement(index_);
}

template <typename T>
typename BlockedArray<T>::pointer 
BlockedArray<T>::IteratorImpl::operator->() const {
	return &block_->getElement(index_);
}

template <typename T>
typename BlockedArray<T>::IteratorImpl const 
BlockedArray<T>::IteratorImpl::operator+(ptrdiff_t offset) const {
	IteratorImpl copy(*this);
	copy += offset;
	return copy;
}

template <typename T>
typename BlockedArray<T>::IteratorImpl const 
BlockedArray<T>::IteratorImpl::operator-(ptrdiff_t offset) const {
	IteratorImpl copy(*this);
	copy -= offset;
	return copy;
}

template <typename T>
typename BlockedArray<T>::IteratorImpl & 
BlockedArray<T>::IteratorImpl::operator+=(ptrdiff_t offset) {
	// changing this iterator gradually to reach destination
	while (true) {
		ptrdiff_t targetIndex = index_ + offset;
		if (targetIndex < 0) {
			// move to first element in this block
			offset += index_; 
			index_ = 0;
			// jump to previous block
			block_ = block_->previous_; 
			offset += block_->size_; 
		} else if (targetIndex >= block_->size_ && block_->size_ > 0) { // special case for end block
			offset -= (block_->size_ - index_);
			block_ = block_->next_;
			index_ = 0;
		} else { // found in current block
			index_ = static_cast<int>(targetIndex);
			break;
		}
	}
	return *this;
}

template <typename T>
typename BlockedArray<T>::IteratorImpl & 
BlockedArray<T>::IteratorImpl::operator-=(ptrdiff_t offset) { 
	return operator+=(-offset);
}
template <typename T>
typename BlockedArray<T>::IteratorImpl & 
BlockedArray<T>::IteratorImpl::operator++() {
	index_++;
	if (index_ == block_->size_) {
		block_ = block_->next_;
		index_ = 0;
	}
	return *this;
}

template <typename T>
typename BlockedArray<T>::IteratorImpl & 
BlockedArray<T>::IteratorImpl::operator--() {
	index_--;
	if (index_ < 0) {
		block_ = block_->previous_;
		index_ = block_->size_ - 1;;
	}
	return *this;
}

template <typename T>
typename BlockedArray<T>::IteratorImpl const 
BlockedArray<T>::IteratorImpl::operator++(int) {
	IteratorImpl copy(*this);
	operator++();
	return *this;
}

template <typename T>
typename BlockedArray<T>::IteratorImpl const 
BlockedArray<T>::IteratorImpl::operator--(int) {
	IteratorImpl copy(*this);
	operator--();
	return *this;
}

template <typename T>
typename BlockedArray<T>::difference_type 
BlockedArray<T>::IteratorImpl::operator-(IteratorImpl const &rhs) const {
	// change gradually to rhs
	IteratorImpl forwardIter(*this); // this will move forward (next, next,...)
	IteratorImpl backwardIter(*this); // this will move backward (previous, previous,...)
	ptrdiff_t forwardDistance = 0; 
	ptrdiff_t backwardDistance = 0;
	// always apply same amount of opposite sign
	forwardDistance += (forwardIter.index_); 
	backwardDistance += (backwardIter.index_);
	// index is always 0 during searching
	forwardIter.index_ -= forwardIter.index_; // = 0
	backwardIter.index_ -= forwardIter.index_;	
	while (true) { 
		if (forwardIter.block_ == rhs.block_) {
			forwardDistance -= rhs.index_;
			forwardIter.index_ += rhs.index_;
			return forwardDistance;
		} else {
			// note that operator- should work fine on end() iterator, too
			if (forwardIter.isEnd()) { // not found, undefined
				break;
			} 
			// index is increased by the size of current block
			forwardDistance -= forwardIter.block_->size_;
			forwardIter.block_ = forwardIter.block_->next_;
		}
	}
	while (true) { 
		if (backwardIter.block_ == rhs.block_) {
			backwardDistance -= rhs.index_;
			backwardIter.index_ += rhs.index_;
			return backwardDistance;
		} else {
			if (backwardIter.isBegin()) { // not found, undefined
				break;
			}
			// index is decreased by the size of previous block
			backwardIter.block_ = backwardIter.block_->previous_;
			backwardDistance += backwardIter.block_->size_;
		}
	}
	return 0; // undefined
}

template <typename T>
bool BlockedArray<T>::IteratorImpl::operator==(IteratorImpl const &rhs) const {
	return block_ == rhs.block_ && index_ == rhs.index_;
}

template <typename T>
bool BlockedArray<T>::IteratorImpl::operator!=(IteratorImpl const &rhs) const {
	return !operator==(rhs);
}
template <typename T>
bool BlockedArray<T>::IteratorImpl::operator<(IteratorImpl const &rhs) const {
	if (block_ == rhs.block_ && index_ < rhs.index_) {
		return true;
	} 
	Block *block = block_->next_;
	while (true) {
		if (block == NULL) {
			return false;
		} 
		if (block == rhs.block_) {
			return true;
		}	else {
			block = block->next_;
		}
	}
}
template <typename T>
bool BlockedArray<T>::IteratorImpl::operator>(IteratorImpl const &rhs) const {
	return rhs.operator<(*this);
}
template <typename T>
bool BlockedArray<T>::IteratorImpl::operator<=(IteratorImpl const &rhs) const {
	return operator==(rhs) || operator<(rhs);
}
template <typename T>
bool BlockedArray<T>::IteratorImpl::operator>=(IteratorImpl const &rhs) const {
	return rhs.operator<=(*this);
}

template <typename T>
bool BlockedArray<T>::IteratorImpl::isBegin() const {
	return block_->previous_ == NULL;
}

template <typename T>
bool BlockedArray<T>::IteratorImpl::isEnd() const {
	return block_->next_ == NULL;
}

} // end namespace lib_calvin_container

#endif