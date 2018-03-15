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
}

#undef CONTAINER_NAME
} // end namespace lib_calvin_container