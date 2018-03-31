#include "suffix_tree_test.h"
#include "suffix_tree.h"
#include "vector.h"

using namespace lib_calvin;

void lib_calvin_string::suffixTreeTest() {
	SuffixTreeTest<char> a;
	a.suffixTreeTest();
}

template <typename Alphabet>
void lib_calvin_string::SuffixTreeTest<Alphabet>::suffixTreeTest() {
	std::cout << "------------suffixTreeTest start ----------------\n\n";
	lib_calvin::suffix_tree<char> a("aa");
	//readSuffixTest();
	//createBranchTest();
	//buildTest();
	//findPatternTest();
	findPatternTest2();
	findCommonSubstringTest();
	findLongestCommonSubstringTest();
	//followPathTest();
	std::cout << "------------suffixTreeTest finished ----------------\n\n";
}

template <typename Alphabet>
void lib_calvin_string::SuffixTreeTest<Alphabet>::prepareTree() {

}

template <typename Alphabet>
void lib_calvin_string::SuffixTreeTest<Alphabet>::readSuffixTest() {
	abstract_string<> text("s012s01234");
	lib_calvin::suffix_tree<Alphabet> tree(text);
	tree.phase_ = text.length();
	decltype(tree.graph_) &graph = tree.graph_;
	// root: 500, internal: 100, leaf: 0, 1
	NodeKey rootKey(0, 500);
	NodeKey internalKey(0, 100);
	NodeKey FirstLeafKey(0, 0);
	NodeKey SecondLeafKey(0, 1);
	NodeKey nullKey = tree.getNullKey();
	graph.insert_vertex(Node(rootKey, NodeType::Root, nullKey, nullKey));
	graph.insert_vertex(Node(internalKey, NodeType::Internal, rootKey, nullKey));
	graph.insert_vertex(Node(FirstLeafKey, NodeType::Leaf, internalKey, nullKey));
	graph.insert_vertex(Node(SecondLeafKey, NodeType::Leaf, internalKey, nullKey));
	graph.insert_edge(rootKey, internalKey, Link(0, 's', 0, 4)); // "s012"
	graph.insert_edge(internalKey, FirstLeafKey, Link(0, 's', 4, 0)); // "-s01234"
	graph.insert_edge(internalKey, SecondLeafKey, Link(0, '3', 8, 0)); // "-34"

	Point s1	= Point(internalKey, FirstLeafKey, 10);
	Point s2	= Point(internalKey, FirstLeafKey, 9);
	Point s3	= Point(internalKey, FirstLeafKey, 8);
	Point s4	= Point(internalKey, FirstLeafKey, 7);
	Point s5	= Point(internalKey, FirstLeafKey, 6);
	Point s6	= Point(internalKey, FirstLeafKey, 5);
	Point s7	= Point(internalKey, SecondLeafKey, 10);
	Point s8	= Point(internalKey, SecondLeafKey, 9);
	Point s9	= Point(rootKey, internalKey, 4);
	Point s10 = Point(rootKey, internalKey, 3);
	Point s11 = Point(rootKey, internalKey, 2);
	Point s12 = Point(rootKey, internalKey, 1);

	tree.readToPoint(s1).println();
	tree.readToPoint(s2).println();
	tree.readToPoint(s3).println();
	tree.readToPoint(s4).println();
	tree.readToPoint(s5).println();
	tree.readToPoint(s6).println();
	tree.readToPoint(s7).println();
	tree.readToPoint(s8).println();
	tree.readToPoint(s9).println();
	tree.readToPoint(s10).println();
	tree.readToPoint(s11).println();
	tree.readToPoint(s12).println();
	//std::cout << "\n";
	
	Point t1 = tree.followPathDown(0, rootKey, 0, 10);
	Point t2 = tree.followPathDown(0, rootKey, 0, 9);
	Point t3 = tree.followPathDown(0, rootKey, 0, 8);
	Point t4 = tree.followPathDown(0, rootKey, 0, 7);
	Point t5 = tree.followPathDown(0, rootKey, 0, 6);
	Point t6 = tree.followPathDown(0, rootKey, 0, 5);
	Point t7 = tree.followPathDown(0, rootKey, 0, 4);
	Point t8 = tree.followPathDown(0, rootKey, 0, 3);
	Point t9 = tree.followPathDown(0, rootKey, 0, 2);
	Point t10 = tree.followPathDown(0, rootKey, 0, 1);

	tree.readToPoint(t1).println();
	tree.readToPoint(t2).println();
	tree.readToPoint(t3).println();
	tree.readToPoint(t4).println();
	tree.readToPoint(t5).println();
	tree.readToPoint(t6).println();
	tree.readToPoint(t7).println();
	tree.readToPoint(t8).println();
	tree.readToPoint(t9).println();
	tree.readToPoint(t10).println();
	std::cout << "\n\n";
}

template <typename Alphabet>
void lib_calvin_string::SuffixTreeTest<Alphabet>::followPathTest() {}

template <typename Alphabet>
void lib_calvin_string::SuffixTreeTest<Alphabet>::createBranchTest() {
	abstract_string<> text("aab");
	lib_calvin::suffix_tree<Alphabet> tree(text);
	tree.textId_ = 0;
	tree.phase_ = text.length(); // final phase
	tree.extension_ = 1; // inserting "ab"
	decltype(tree.graph_) &graph = tree.graph_;

	// root: 500, leaf: 0
	NodeKey rootKey = tree.getRootKey();
	NodeKey leafKey(0, 0);
	NodeKey nullKey = tree.getNullKey();

	graph.insert_vertex(Node(rootKey, NodeType::Root, nullKey, nullKey));
	graph.insert_vertex(Node(leafKey, NodeType::Leaf, rootKey, nullKey));
	graph.insert_edge(rootKey, leafKey, Link(0, 'a', 0, 0)); // "a-"
	// go to "a" and append 'b'
	tree.createBranch(tree.followPathDown(0, rootKey, 1, 2), 'b');
	std::cout << "\n";
	Point ab = tree.followPathDown(0, rootKey, 1, 3);
	tree.readToPoint(ab).println();
	std::cout << "\n\n";
}

template <typename Alphabet>
void lib_calvin_string::SuffixTreeTest<Alphabet>::buildTest() {
	abstract_string<> text("aabcabcdee");
	lib_calvin::suffix_tree<Alphabet> tree(text);
	tree.build();
	tree.printAllSuffix();
}

template <typename Alphabet>
void lib_calvin_string::SuffixTreeTest<Alphabet>::findPatternTest() {
	abstract_string<> text("abbaabbaa");
	abstract_string<> pattern1("baa");
	abstract_string<> pattern2("ab");
	abstract_string<> pattern3("bb");
	abstract_string<> pattern4("aab");
	lib_calvin::suffix_tree<Alphabet> tree(text);
	tree.build();
	tree.printAllSuffix();
	auto result1 = tree.find_pattern(pattern1);
	auto result2 = tree.find_pattern(pattern2);
	auto result3 = tree.find_pattern(pattern3);
	auto result4 = tree.find_pattern(pattern4);
	printMatchResult(pattern1, "2, 6", result1);
	printMatchResult(pattern2, "0, 4", result2);
	printMatchResult(pattern3, "1, 5", result3);
	printMatchResult(pattern4, "3", result4);
}

template <typename Alphabet>
void lib_calvin_string::SuffixTreeTest<Alphabet>::findPatternTest2() {
	std::cout << "------------findPatternTest2 start ----------------\n";
	abstract_string<> text1("abbaabbaa");
	abstract_string<> text2("baabcbbaab");
	abstract_string<> text3("babaabb");
	lib_calvin::vector<decltype(text1)> texts;
	texts.push_back(text1);
	texts.push_back(text2);
	texts.push_back(text3);
	abstract_string<> pattern1("baa");
	abstract_string<> pattern2("ab");
	abstract_string<> pattern3("bb");
	abstract_string<> pattern4("aab");
	lib_calvin::suffix_tree<Alphabet> tree(texts);
	tree.build();
	tree.printAllSuffix();
	auto result1 = tree.find_pattern(pattern1);
	auto result2 = tree.find_pattern(pattern2);
	auto result3 = tree.find_pattern(pattern3);
	auto result4 = tree.find_pattern(pattern4);
	printMatchResult(pattern1, "(0,2),(0,6),(1,0),(1,6),(2,2)", result1);
	printMatchResult(pattern2, "(0,0),(0,4),(1,2),(1,8),(2,1),(2,4)", result2);
	printMatchResult(pattern3, "(0,1),(0,5),(1,5),(2,5)", result3);
	printMatchResult(pattern4, "(0,3),(1,1),(1,7),(2,3)", result4);
	std::cout << "------------findPatternTest2 finished ----------------\n\n";
}

template <typename Alphabet>
void lib_calvin_string::SuffixTreeTest<Alphabet>::findCommonSubstringTest() {
	std::cout << "------------findCommonSubstringTest start ----------------\n";
	abstract_string<> text1("abbacabbaa");
	abstract_string<> text2("baabcabbacaab");
	abstract_string<> text3("babcabbabb");
	lib_calvin::vector<decltype(text1)> texts;
	texts.push_back(text1);
	texts.push_back(text2);
	texts.push_back(text3);
	lib_calvin::suffix_tree<Alphabet> tree(texts);
	tree.build();
	tree.printAllCommonSubstring();
	std::cout << "------------findCommonSubstringTest finished ----------------\n\n";
}

template <typename Alphabet>
void lib_calvin_string::SuffixTreeTest<Alphabet>::findLongestCommonSubstringTest() {
	std::cout << "------------findLongestCommonSubstringTest start ----------------\n";
	abstract_string<> text1("abbacabbaa");
	abstract_string<> text2("baabcabbacaab");
	abstract_string<> text3("babcabbabb");
	lib_calvin::vector<decltype(text1)> texts;
	texts.push_back(text1);
	texts.push_back(text2);
	texts.push_back(text3);
	lib_calvin::suffix_tree<Alphabet> tree(texts);
	tree.build();
	auto result = tree.find_longest_common_substring();
	std::cout << "longest common substring was: ";
	result.println();
	std::cout << "------------findLongestCommonSubstringTest finished ----------------\n\n";
}

template <typename Alphabet>
void lib_calvin_string::SuffixTreeTest<Alphabet>::printMatchResult(
		lib_calvin::abstract_string<Alphabet> const &pattern, std::string rightAnswer,
		lib_calvin::vector<std::pair<size_t, size_t>> const &result) const {
	std::cout << "Result for: ";
	pattern.print();
	std::cout << " rightAnswer: " << rightAnswer << "\n";
	for (auto iter = result.begin(); iter != result.end(); ++iter) {
		std::cout << "Match at text: " << iter->first << " position: " << iter->second << "\n";
	}
}