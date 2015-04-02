#ifndef LIB_CALVIN__CONTAINER__ADT_H
#define LIB_CALVIN__CONTAINER__ADT_H

/*
 * 2008-02-28
 * 2008-05-03: Reviewed this file, and found that it would be nicer if I 
 * ..implement these ADTs in template-template of basic container types.
 * ..It is amazing that the level of abstraction we can achieve with C++ is
 * ..almost as high as that we can do with functional languages.
 * 2008-05-03: Rejected above change, because it hampers simplicity. I 
 * ..could just override vector, map, set, etc if I want to use different 
 * ..implementation. 
 */

#include "vector.h"
#include "map.h"
#include <list>
#include <utility>

namespace lib_calvin_adt 
{
	using lib_calvin::vector;
	using lib_calvin::map;
	using lib_calvin::hash_map;
	using std::list;
	using std::pair;

	// HEAP_D-ary heap for IntPq
	size_t const HEAP_D = 80;
	size_t const NULL_INDEX = SIZE_MAX;

	/*************************** ADT Classes ***************************/

  // A dictionary for mapping objects to integers (0 ~ numOfObjects)
  // K: key
  // No support for erasing!
  // Maps keys to consecutive integers (starting from 0)
  template <typename K>
  class IntIndexer {
    public:
      IntIndexer();
			size_t size() const;
      // true if found
      std::pair<size_t, bool> indexOf (K const &key) const;
      // true if inserted. always return valid index
      std::pair<size_t, bool> insert(K const &key); 
			void erase(size_t index);
      // undefined if index is invalid!
      K const & operator[] (size_t index) const; // get key from int
			K & operator[] (size_t index);
    private:
      size_t newIndex_;
      //hash_map<size_t, K> indexToKey_;
			//hash_map<K, size_t> keyToIndex_;
			map<size_t, K> indexToKey_; // map indx to key
      map<K, size_t> keyToIndex_; // map key to index
  };

  // P: priority should have < operator
  // Deals only with non-negative integers (as key)
  // This is a mean-heap: lowest priority is top element!!
  template <typename P>
  class IntPq {
    public:
      IntPq(size_t maxsize); // initial capacity
			IntPq(size_t maxsize, P const &); // initialize with given P
      void reserve(size_t maxsize);
      size_t capacity() { return maxsize_; }
      size_t size() const { return size_; }
      pair<size_t, P> const & peek() const; // returns the top element 
      pair<size_t, P> pop(); // pop top element (key, priority)
      // returns true if actioned performed (insert or decrease-priority)
      // retuns false if new prioritoy was not lower than before.
			template <typename P1>
				bool insert(size_t key, P1 &&priority);
			P const & getPriority(size_t key) const;
			bool hasKey(size_t key) const;
    private:
      size_t size_; // current num of elements (size of heap)
      size_t const maxsize_;
      size_t d_; // d-aray heap: important for performance
      vector<pair<size_t, P>> heap_; // pair of (key, priority)
      // NULL_INDEX if not inserted yet, index if in heap
      mutable vector<size_t> indexArray_; // key(int) -> index(int)
      
      void swap_(size_t index1, size_t index2);
      // heap index starts from 0
      void percolateUp(size_t index);
      void percolateDown(size_t index);
  };
  
  // Careful: this is not normal Stack; only for DFS implementation!
  // Supports decreace-priority function (when push existing number, move it  
  // ..to the top of the Stack)
  // Dealing only with integers
  class IntStack {
    public:
      IntStack(size_t maxsize); // deals only with 0 ~ maxsize-1
      void reserve (size_t maxsize); // increase capacity
      size_t capacity() { return maxsize_; }
      size_t size() const { return size_; }
      // move key to the top
      void push(size_t key);
      size_t peek() const;
      size_t pop();
    private:
      size_t size_;
      size_t maxsize_;
      list<size_t> stack_; // Stack
      // pointer to elements in Stack
      vector<list<size_t>::iterator> intToListIterator_;
      // stores elements' status
      vector<bool> isInserted_; 
  };

  // General priority queue: using IntPq and IntIndexer
  template <typename K, typename P>
  class PQ {   
  };
  
  // General Stack (no priority, no decrease-priority)
  // Using IntStack and IntIndexer
  template <typename T>
  class Stack {
    public:
      Stack();
      size_t size() const { return stack_.size(); }
      void push (T const &inObject);
      T peek() const;
      T pop();
      void init() { stack_.clear(); }
    private:
      vector<T> stack_;
  };

	// Plays with integers from 0 to size-1
  class DisjointSet {
    public:
      DisjointSet(size_t size);
      bool isSameSet(size_t a, size_t b);
      size_t findSet(size_t a);
      void unite(size_t a, size_t b);
    private:
      vector<size_t> elems_;
      Stack<size_t> stack_;
  };
} // end namespace lib_calvin_adt




namespace lib_calvin_adt // open for definitions
{

/******************* IntIndexer definitions *******************/

template <typename K>
IntIndexer<K>::IntIndexer(): newIndex_(0) { }

template <typename K>
size_t IntIndexer<K>::size() const {
  return static_cast<int>(keyToIndex_.size());
}

template <typename K>
std::pair<size_t, bool> IntIndexer<K>::indexOf(K const &key) const {
  auto iter = keyToIndex_.find(key);
  if (iter == keyToIndex_.end()) {
    return std::make_pair(0, false);
  }
  return std::make_pair(iter->second, true);
}

template <typename K>
std::pair<size_t, bool> IntIndexer<K>::insert(K const &key) {
  auto iter = keyToIndex_.find(key);
  if (iter == keyToIndex_.end()) {
    keyToIndex_.insert(std::pair<K, size_t>(key, newIndex_));
    indexToKey_.insert(std::pair<size_t, K>(newIndex_, key));
    return std::make_pair(newIndex_++, true);
  } else {
		return std::make_pair(iter->second, false);
	}
}

template <typename K>
void IntIndexer<K>::erase(size_t index) {
	K const &key = indexToKey_[index];
	keyToIndex_.erase(key);
	indexToKey_.erase(index);
}

template <typename K>
K const & 
IntIndexer<K>::operator[] (size_t index) const {
  //return const_cast<hash_map<size_t, K> &>(indexToKey_)[index];
	return const_cast<map<size_t, K> &>(indexToKey_)[index];
}

template <typename K>
K & 
IntIndexer<K>::operator[] (size_t index) {
  return indexToKey_[index];
}

/******************* IntPq definitions *******************/

template <typename P>
IntPq<P>::IntPq(size_t maxsize): size_(0), maxsize_(maxsize), d_(HEAP_D), 
  heap_(maxsize), indexArray_(maxsize, NULL_INDEX) { // LibrarySetAdaptor indices to NULL_INDEX
}

template <typename P>
IntPq<P>::IntPq(size_t maxsize, P const &initValue): size_(0), maxsize_(maxsize), d_(HEAP_D), 
  heap_(maxsize, std::make_pair(0, initValue)), indexArray_(maxsize, NULL_INDEX) {  
}

template <typename P>
void IntPq<P>::reserve(size_t newMax) {
  if (newMax < maxsize_) {
    return;
	}
  heap_.resize(newMax);
  indexArray_.resize(newMax);
}

template <typename P>
pair<size_t, P> const &
IntPq<P>::peek() const {
  return heap_[0];
}

template <typename P>
pair<size_t, P>
IntPq<P>::pop() {
	//std::cout << "pq POP\n";
  swap_(0, size_ - 1);
  --size_;
  percolateDown(0);
	indexArray_[heap_[size_].first] = NULL_INDEX;
  return heap_[size_];
}

template <typename P>
template <typename P1>
bool IntPq<P>::insert(size_t key, P1 &&priority) {
  size_t index = indexArray_[key];
  if (index != NULL_INDEX) { // key exists; decrease key operation
    if (heap_[index].second < priority) { // new priority is higher: wrong input
      return false;
    } else {
			// decrease priority. no-op for identical priority, but that is OK
      heap_[index].second = std::forward<P1>(priority); 
      percolateUp(index);
      return true;
    }
  } else { // new key: insert operation
    heap_[size_] = pair<size_t, P>(key, std::forward<P1>(priority));
    indexArray_[key] = size_;
    ++size_;
    percolateUp(size_ - 1);
    return true;
  }
}

template <typename P>
bool IntPq<P>::hasKey(size_t key) const {
	return indexArray_[key] != NULL_INDEX;
}

template <typename P>
P const & 
IntPq<P>::getPriority(size_t key) const {
	size_t index = indexArray_[key];
	if (index == NULL_INDEX) {
		std::cout << "IntPq::getPriority error\n";
		exit(0);
	}
	return heap_[index].second;
}

template <typename P>
void IntPq<P>::swap_(size_t index1, size_t index2) {
  std::swap(heap_[index1], heap_[index2]);
  size_t key1 = heap_[index1].first;
  size_t key2 = heap_[index2].first;
  std::swap(indexArray_[key1], indexArray_[key2]);
}

template <typename P>
void IntPq<P>::percolateUp(size_t index) {
  while (index != 0) {
    size_t parentIndex = (index - 1) / d_;
    if (heap_[index].second < heap_[parentIndex].second) { // need to swap
      swap_ (parentIndex, index);
      index = parentIndex;
    } else {
      return;
    }
  }
}

template <typename P>
void IntPq<P>::percolateDown(size_t index) {
  while (true) {
    if (d_ * index + d_ < size_) { // all children are in valid range
      size_t minIndex  = d_ * index + 1; // lowest priority child's index
      size_t tempIndex   = d_ * index + 2; // second child
      size_t endIndex  = d_ * index + d_ + 1; // first illegal index
      for ( ; tempIndex < endIndex; ++tempIndex) {
         if (heap_[tempIndex].second < heap_[minIndex].second) { 
          minIndex = tempIndex;
         }
      }
      if (heap_[minIndex].second < heap_[index].second) {
        // swap and recursive call
        swap_ (index, minIndex);
        index = minIndex;
        continue;
      } else {
        return;      
      }
    } else if (d_ * index + 1 >= size_) { // no children are in valid range
      return;
    } else { // some children are in valid range
      size_t minIndex  = d_ * index + 1; // lowest priority child's index
      size_t tempIndex   = d_ * index + 1; 
      size_t endIndex  = size_; // end of heap
      for ( ; tempIndex < endIndex; ++tempIndex) {
         if (heap_[tempIndex].second < heap_[minIndex].second) { 
          minIndex = tempIndex;
         }
      }
      if (heap_[minIndex].second < heap_[index].second) {
        swap_ (index, minIndex);
        return;
      } else {
        return;
      }
    }
  }
}

/************************** Stack definitions **************************/

template <typename T>
Stack<T>::Stack(): stack_(0) {
}

template <typename T>
void Stack<T>::push(T const &inObject) {
  stack_.push_back(inObject);
}

template <typename T>
T Stack<T>::peek() const {
  return stack_[stack_.size() - 1];
}

template <typename T>
T Stack<T>::pop() {
  T temp = stack_[stack_.size() - 1];
  stack_.pop_back();
  return temp;
}
} // end namespace lib_calvin_adt definitions

#endif