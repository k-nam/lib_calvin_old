#ifndef LIB_CALVIN__CONTAINER__B_TREE_TEST_H
#define LIB_CALVIN__CONTAINER__B_TREE_TEST_H

#include "b_tree.h"
#include "b_plus_tree.h"
#include "container.h"

namespace lib_calvin_container
{
void bTreeTest();

template <typename T>
class BTreeTest
{
public:
	void bTreeNodeTest();
	void bTreeIteratorTest();
	void addToNodeTest();
	void mergeNodeTest();
	void eraseTest();
	void searchAlgoTest(); 
};

template <typename T>
void BTreeTest<T>::bTreeNodeTest() {
	BTree<T> tree;	
	tree.insert(1);
	tree.insert(3);
	tree.insert(5);
	tree.insert(7);
	tree.insert(9);
	tree.insert(3);		
	for (int i = 100; i > 0 ; --i) {
		tree.insert(i);
	}	
	tree.print();
	std::cout <<"\n";
}

template <typename T>
void BTreeTest<T>::bTreeIteratorTest() {
	BTree<T> tree;
	tree.insert(1);
	tree.insert(3);
	tree.insert(5);
	tree.insert(2);
	for (int i = 100; i > 0 ; --i) {
		tree.insert(i);
	}	
	typename BTree<T>::Node *node = tree.getFirstNode();
	tree.printNode(node);
	typename BTree<T>::iterator iter = tree.begin();
	for ( ; iter != tree.end(); ++iter) {
		std::cout << "this iter: " << *iter << "\n";
	}
	typename BTree<T>::reverse_iterator r_iter = tree.rbegin();
	for ( ; r_iter != tree.rend(); ++r_iter) {
		std::cout << "this iter: " << *r_iter << "\n";
	}
	std::cout <<"\n";
}

template <typename T>
void BTreeTest<T>::addToNodeTest() {
	BTree<T> tree;
	tree.insert(1);
	tree.insert(3);
	tree.insert(5);
	tree.insert(7);
	tree.print();
	tree.addToNode(tree.getFirstNode(), 0);
	tree.print();
	std::cout <<"\n";
}
template <typename T>
void BTreeTest<T>::mergeNodeTest() {
	BTree<T> tree;
	tree.insert(1);
	tree.insert(3);
	tree.insert(5);
	tree.insert(7);
	tree.insert(8);
	tree.insert(9);
	tree.print();
	tree.mergeNode(tree.getFirstNode(), 0);
	tree.print();
	std::cout <<"\n";
}
template <typename T>
void BTreeTest<T>::eraseTest() {
	BTree<T> tree;
	tree.insert(1);
	tree.insert(3);
	tree.insert(5);
	tree.insert(7);
	tree.insert(9);
	tree.print();
	tree.erase(1);
	tree.erase(7);
	tree.print();
	std::cout <<"\n";
}

template <typename T>
void BTreeTest<T>::searchAlgoTest() {
	BTree<int> tree;
	int testArray[10] = { 0, 1, 1, 2, 3, 4, 4, 5, 6, 7 };
	using std::cout;
	cout << "sequential: \n";
	cout << "0: " << sequentialSearch<int, int, std::less<int>, Identity<int>>(testArray, testArray + 10, 0) << "\n";
	cout << "1: " << sequentialSearch<int, int, std::less<int>, Identity<int>>(testArray, testArray + 10, 1) << "\n";
	cout << "2: " << sequentialSearch<int, int, std::less<int>, Identity<int>>(testArray, testArray + 10, 2) << "\n";
	cout << "4: " << sequentialSearch<int, int, std::less<int>, Identity<int>>(testArray, testArray + 10, 4) << "\n";
	cout << "8: " << sequentialSearch<int, int, std::less<int>, Identity<int>>(testArray, testArray + 10, 8) << "\n";
	cout << "binary: \n";
	cout << "0: " << binarySearch<int, int, std::less<int>, Identity<int>>(testArray, testArray + 10, 0) << "\n";
	cout << "1: " << binarySearch<int, int, std::less<int>, Identity<int>>(testArray, testArray + 10, 1) << "\n";
	cout << "2: " << binarySearch<int, int, std::less<int>, Identity<int>>(testArray, testArray + 10, 2) << "\n";
	cout << "4: " << binarySearch<int, int, std::less<int>, Identity<int>>(testArray, testArray + 10, 4) << "\n";
	cout << "8: " << binarySearch<int, int, std::less<int>, Identity<int>>(testArray, testArray + 10, 8) << "\n";
	std::cout <<"\n";
}

}

#endif

