#include "b_tree_test.h"
#include "rb_tree.h"
#include <iostream>
#include <set>


namespace lib_calvin_container
{
using std::cout;

void bTreeTest() {
	BTreeTest<int> test;
	test.bTreeNodeTest();
	test.bTreeIteratorTest();
	test.addToNodeTest();
	test.mergeNodeTest();
	test.eraseTest();
	test.searchAlgoTest();

	bTreeMemoryTest();
}

void bTreeMemoryTest() {	
#define CONTAINER_TYPE BTree
#define CONTAINER_TYPE2 BinTree
	std::cout << "B-tree memory test\n";
	for (int k = 0; k < 100; k++) {
		CONTAINER_TYPE<CONTAINER_TYPE2<double>> tree;
		for (int i = 0; i < 100; ++i) {
			CONTAINER_TYPE2<double> tree2;
			for (int j = 0; j < 100; j++) {
				tree2.insert(i + j);
			}
			tree.insert(tree2);
		}
		for (int i = 0; i < 100; ++i) {
			CONTAINER_TYPE2<double> tree2;
			for (int j = 0; j < 100; j++) {
				tree2.insert(i + j + 1);
			}
			tree.erase(tree2);
		}
		CONTAINER_TYPE<CONTAINER_TYPE2<double>> tree3(tree);
		CONTAINER_TYPE<CONTAINER_TYPE2<double>> tree4 = tree;
	}
}
#undef CONTAINER_NAME
} // end namespace lib_calvin_container