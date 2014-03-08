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
	std::cout << "B-tree memory test\n";
	for (int k = 0; k < 100; k++) {
		RbTree<RbTree<double>> tree;
		for (int i = 0; i < 100; ++i) {
			RbTree<double> tree2;
			for (int j = 0; j < 100; j++) {
				tree2.insert(i + j);
			}
			tree.insert(tree2);
		}
		for (int i = 0; i < 100; ++i) {
			RbTree<double> tree2;
			for (int j = 0; j < 100; j++) {
				tree2.insert(i + j + 1);
			}
			tree.erase(tree2);
		}
		RbTree<RbTree<double>> tree3(tree);
		RbTree<RbTree<double>> tree4 = tree;
	}
}

} // end namespace lib_calvin_container