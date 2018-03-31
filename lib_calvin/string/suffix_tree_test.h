#ifndef LIB_CALVIN__STRING__SUFFIX_TREE_TEST_H
#define LIB_CALVIN__STRING__SUFFIX_TREE_TEST_H

#include "suffix_tree.h"
#include "string_test.h"
#include "vector.h"

namespace lib_calvin_string
{
void suffixTreeTest();

template <typename Alphabet>
class SuffixTreeTest {
public:
	typedef lib_calvin::suffix_tree<char>::Node Node;
	typedef lib_calvin::suffix_tree<char>::NodeKey NodeKey;
	typedef lib_calvin::suffix_tree<char>::Link Link;
	typedef lib_calvin::suffix_tree<char>::NodeType NodeType;
	typedef lib_calvin::suffix_tree<char>::Point Point;
public:
	void suffixTreeTest();
private:
	void readSuffixTest();
	void followPathTest();
	void createBranchTest();
	void buildTest();
	void findPatternTest();
	void findPatternTest2();
	void findCommonSubstringTest();
	void findLongestCommonSubstringTest();
private:
	void prepareTree();
	void printMatchResult(lib_calvin::abstract_string<Alphabet> const &pattern, std::string rightAnswer,
		lib_calvin::vector<std::pair<size_t, size_t>> const &result) const;
private:

};
}

#endif