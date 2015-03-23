#include "suffix_tree_test.h"
#include "suffix_tree.h"

void lib_calvin_string::suffixTreeTest() {
	SuffixTreeTest<char> a;
	a.suffixTreeTest();
}

template <typename Alphabet>
void lib_calvin_string::SuffixTreeTest<Alphabet>::suffixTreeTest() {
	std::cout << "------------suffixTreeTest start ----------------\n\n";
	lib_calvin::suffix_tree<char> a("aa");
	readSuffixTest();
	//followPathTest();
	std::cout << "------------suffixTreeTest finished ----------------\n\n";
}

template <typename Alphabet>
void lib_calvin_string::SuffixTreeTest<Alphabet>::prepareTree() {

}

template <typename Alphabet>
void lib_calvin_string::SuffixTreeTest<Alphabet>::readSuffixTest() {
	lib_calvin::suffix_tree<Alphabet> tree("s012s01234");
	tree.phase_ = 10;
	decltype(tree.graph_) &graph = tree.graph_;
	// root: 500, internal: 100, leaf: 0, 1
	graph.insert_vertex(Node(NodeType::Root, 500, 0, 0));
	graph.insert_vertex(Node(NodeType::Internal, 100, 500, 0));
	graph.insert_vertex(Node(NodeType::Leaf, 0, 100, 0));
	graph.insert_vertex(Node(NodeType::Leaf, 1, 100, 0));
	graph.insert_edge(500, 100, Link('s', 0, 4)); // "s012"
	graph.insert_edge(100, 0, Link('s', 4, 0)); // "-s01234"
	graph.insert_edge(100, 1, Link('3', 8, 0)); // "-34"

	Point s1 = Point(100, 0, 0);
	Point s2 = Point(100, 0, 9);
	Point s3 = Point(100, 0, 8);
	Point s4 = Point(100, 0, 7);
	Point s5 = Point(100, 0, 6);
	Point s6 = Point(100, 0, 5);
	Point s7 = Point(100, 1, 0);
	Point s8 = Point(100, 1, 9);
	Point s9 = Point(500, 100, 4);
	Point s10 = Point(500, 100, 3);
	Point s11 = Point(500, 100, 2);
	Point s12 = Point(500, 100, 1);

	tree.readToPoint(s1).print();
	tree.readToPoint(s2).print();
	tree.readToPoint(s3).print();
	tree.readToPoint(s4).print();
	tree.readToPoint(s5).print();
	tree.readToPoint(s6).print();
	tree.readToPoint(s7).print();
	tree.readToPoint(s8).print();
	tree.readToPoint(s9).print();
	tree.readToPoint(s10).print();
	tree.readToPoint(s11).print();
	tree.readToPoint(s12).print();

	Point t1 = tree.followPathDown(500, 0, 10);
	Point t2 = tree.followPathDown(500, 0, 9);
	Point t3 = tree.followPathDown(500, 0, 8);
	Point t4 = tree.followPathDown(500, 0, 7);
	Point t5 = tree.followPathDown(500, 0, 6);
	Point t6 = tree.followPathDown(500, 0, 5);
	Point t7 = tree.followPathDown(500, 0, 4);
	Point t8 = tree.followPathDown(500, 0, 3);
	Point t9 = tree.followPathDown(500, 0, 2);
	Point t10 = tree.followPathDown(500, 0, 1);

	tree.readToPoint(t1).print();
	tree.readToPoint(t2).print();
	tree.readToPoint(t3).print();
	tree.readToPoint(t4).print();
	tree.readToPoint(t5).print();
	tree.readToPoint(t6).print();
	tree.readToPoint(t7).print();
	tree.readToPoint(t8).print();
	tree.readToPoint(t9).print();
	tree.readToPoint(t10).print();
	std::cout << "\n";
}

template <typename Alphabet>
void lib_calvin_string::SuffixTreeTest<Alphabet>::followPathTest() {

}