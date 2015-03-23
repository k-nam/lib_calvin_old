#ifndef LIB_CALVIN__STRING__SUFFIX_TREE_TEST_H
#define LIB_CALVIN__STRING__SUFFIX_TREE_TEST_H

#include "suffix_tree.h"
#include "string_test.h"

namespace lib_calvin_string
{
void suffixTreeTest();

template <typename Alphabet>
class SuffixTreeTest {
public:
	typedef lib_calvin::suffix_tree<char>::Node Node;
	typedef lib_calvin::suffix_tree<char>::Link Link;
	typedef lib_calvin::suffix_tree<char>::NodeType NodeType;
	typedef lib_calvin::suffix_tree<char>::Point Point;
public:
	void suffixTreeTest();
private:
	void readSuffixTest();
	void followPathTest();
private:
	void prepareTree();
private:

};
}

#endif