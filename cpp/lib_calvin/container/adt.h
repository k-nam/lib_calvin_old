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
	using std::list;
	using std::pair;

	// D-ary heap for IntPq
	int const D = 80;

	/*************************** ADT Classes ***************************/

  // A dictionary for mapping objects to integers (0 ~ numOfObjects)
  // K: key
  // No support for erasing!
  // Maps keys to consecutive integers (starting from 0)
  template <typename K>
  class IntIndexer {
    public:
      IntIndexer();
			int size() const;
      // return negative if not exist
      int indexOf (K const &key) const;
      // true is inserted. always return valid index
      std::pair<int, bool> insert(K const &key); 
			void erase(int index);
      // undefined if index is invalid!
      K const & operator[] (int index) const; // get key from int
			K & operator[] (int index);
    private:
      int newIndex_;
      map<int, K> indexToKey_; // map int to key
      map<K, int> keyToIndex_; // map key to int
  };

  // P: priority should have < operator
  // Deals only with non-negative integers (as key)
  // This is a mean-heap: lowest priority is top element!!
  template <typename P>
  class IntPq {
    public:
      IntPq(int maxsize); // initial capacity
			IntPq(int maxsize, P const &); // initialize with given P
      void reserve(int maxsize);
      int capacity() { return maxsize_; }
      int size() const { return size_; }
      pair<int, P> const & peek() const; // returns the top element 
      pair<int, P> pop(); // pop top element (key, priority)
      // returns true if actioned performed (insert or decrease-priority)
      // retuns false if new prioritoy was not lower than before.
			template <typename P1>
				bool insert(int key, P1 &&priority);
			P const & getPriority(int key) const;
			bool hasKey(int key) const;
    private:
      int size_; // current num of elements (size of heap)
      int const maxsize_;
      int d_; // d-aray heap: important for performance
      vector<pair<int, P>> heap_; // pair of (key, priority)
      // -1 if not inserted yet, index if in heap
      mutable vector<int> indexArray_; // key(int) -> index(int)
      
      void swap_(int index1, int index2);
      // heap index starts from 0
      void percolateUp(int index);
      void percolateDown(int index);
  };
  
  // Careful: this is not normal Stack; only for DFS implementation!
  // Supports decreace-priority function (when push existing number, move it  
  // ..to the top of the Stack)
  // Dealing only with integers
  class IntStack {
    public:
      IntStack(int maxsize); // deals only with 0 ~ maxsize-1
      void reserve (int maxsize); // increase capacity
      int capacity() { return maxsize_; }
      int size() const { return size_; }
      // move key to the top
      void push(int key);
      int peek() const;
      int pop();
    private:
      int size_;
      int maxsize_;
      list<int> stack_; // Stack
      // pointer to elements in Stack
      vector<list<int>::iterator> intToListIterator_;
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
      DisjointSet(int size);
      bool isSameSet(int a, int b);
      int findSet(int a);
      void unite(int a, int b);
    private:
      vector<int> elems_;
      Stack<int> stack_;
  };
} // end namespace lib_calvin_adt




namespace lib_calvin_adt // open for definitions
{

/******************* IntIndexer definitions *******************/

template <typename K>
IntIndexer<K>::IntIndexer(): newIndex_(0) { }

template <typename K>
int IntIndexer<K>::size() const {
  return static_cast<int>(keyToIndex_.size());
}

template <typename K>
int IntIndexer<K>::indexOf(K const &key) const {
  map<K, int>::const_iterator iter = keyToIndex_.find(key);
  if (iter == keyToIndex_.end()) {
    return -1;
  }
  return (iter->second);
}

template <typename K>
std::pair<int, bool> IntIndexer<K>::insert(K const &key) {
  map<K, int>::iterator iter = keyToIndex_.find(key);
  if (iter == keyToIndex_.end()) {
    keyToIndex_.insert(std::pair<K, int>(key, newIndex_));
    indexToKey_.insert(std::pair<int, K>(newIndex_, key));
    return std::make_pair(newIndex_++, true);
  } else {
		return std::make_pair(iter->second, false);
	}
}

template <typename K>
void IntIndexer<K>::erase(int index) {
	K const &key = indexToKey_[index];
	keyToIndex_.erase(key);
	indexToKey_.erase(index);
}

template <typename K>
K const & 
IntIndexer<K>::operator[] (int index) const {
  return const_cast<map<int, K> &>(indexToKey_)[index];
}

template <typename K>
K & 
IntIndexer<K>::operator[] (int index) {
  return indexToKey_[index];
}

/******************* IntPq definitions *******************/

template <typename P>
IntPq<P>::IntPq(int maxsize): size_(0), maxsize_(maxsize), d_(D), 
  heap_(maxsize), indexArray_(maxsize, -1) { // LibrarySetAdaptor indices to -1 (null) 
}

template <typename P>
IntPq<P>::IntPq(int maxsize, P const &initValue): size_(0), maxsize_(maxsize), d_(D), 
  heap_(maxsize, std::make_pair(0, initValue)), indexArray_(maxsize, -1) { // LibrarySetAdaptor indices to -1 (null) 
}

template <typename P>
void IntPq<P>::reserve(int newMax) {
  if (newMax < maxsize_)
    return;
  heap_.resize(newMax);
  indexArray_.resize(newMax);
}

template <typename P>
pair<int, P> const &
IntPq<P>::peek() const {
  return heap_[0];
}

template <typename P>
pair<int, P>
IntPq<P>::pop() {
	//std::cout << "pq POP\n";
  swap_(0, size_ - 1);
  --size_;
  percolateDown(0);
	indexArray_[heap_[size_].first] = -1;
  return heap_[size_];
}

template <typename P>
template <typename P1>
bool IntPq<P>::insert(int key, P1 &&priority) {
  int index = indexArray_[key];
  if (index >= 0) { // key exists; decrease key operation
    if (heap_[index].second < priority) { // new priority is higher: wrong input
      return false;
    } else {
			// decrease priority. no-op for identical priority, but that is OK
      heap_[index].second = std::forward<P1>(priority); 
      percolateUp(index);
      return true;
    }
  } else { // new key: insert operation
    heap_[size_] = pair<int, P>(key, std::forward<P1>(priority));
    indexArray_[key] = size_;
    ++size_;
    percolateUp(size_ - 1);
    return true;
  }
}

template <typename P>
bool IntPq<P>::hasKey(int key) const {
	return indexArray_[key] >= 0;
}

template <typename P>
P const & 
IntPq<P>::getPriority(int key) const {
	int index = indexArray_[key];
	if (index < 0) {
		std::cout << "IntPq::getPriority error\n";
		exit(0);
	}
	return heap_[index].second;
}

template <typename P>
void IntPq<P>::swap_(int index1, int index2) {
  std::swap(heap_[index1], heap_[index2]);
  int key1 = heap_[index1].first;
  int key2 = heap_[index2].first;
  std::swap(indexArray_[key1], indexArray_[key2]);
}

template <typename P>
void IntPq<P>::percolateUp(int index) {
  while (index != 0) {
    int parentIndex = (index - 1) / d_;
    if (heap_[index].second < heap_[parentIndex].second) { // need to swap
      swap_ (parentIndex, index);
      index = parentIndex;
    } else {
      return;
    }
  }
}

template <typename P>
void IntPq<P>::percolateDown(int index) {
  while (true) {
    if (d_ * index + d_ < size_) { // all children are in valid range
      int minIndex  = d_ * index + 1; // lowest priority child's index
      int tempIndex   = d_ * index + 2; // second child
      int endIndex  = d_ * index + d_ + 1; // first illegal index
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
      int minIndex  = d_ * index + 1; // lowest priority child's index
      int tempIndex   = d_ * index + 1; 
      int endIndex  = size_; // end of heap
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

#undef D

#endif