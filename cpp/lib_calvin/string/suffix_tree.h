#ifndef LIB_CALVIN__STRING__SUFFIX_TEST_H
#define LIB_CALVIN__STRING__SUFFIX_TEST_H

#include "abstract_string.h"
#include "graph.h"

namespace lib_calvin
{
template <typename Alphabet>
class suffix_tree {
public:
	suffix_tree(abstract_string<Alphabet> text);

private:
	enum class NodeType {
		Internal = 0, Leaf = 1
	};
	struct Node {
		NodeType type_; 
		// For leaf nodes, id also denotes the starting index of corresponding suffix
		size_t id_; // 0 for the entire string
		size_t parentId_;
		size_t suffixLink_; // if this node represents ax, points to x
		bool operator==(Node const &rhs) const;
		bool operator<(Node const &rhs) const;
	};
	struct Link {
		Alphabet head_; // first letter of the substring represented by this link
		size_t startIndex_;
		size_t endIndex_; // 0 if this link points to leaf node
		bool operator==(Link const &rhs) const;
		bool operator<(Link const &rhs) const;
	};
private:
	lib_calvin::graph<Node, Link> graph_;
};




template <typename Alphabet>
suffix_tree<Alphabet>::suffix_tree(abstract_string<Alphabet> text) {
}

template <typename Alphabet>
bool suffix_tree<Alphabet>::Node::operator==(Node const &rhs) const {
	return id_ == rhs.id_;
}

template <typename Alphabet>
bool suffix_tree<Alphabet>::Node::operator<(Node const &rhs) const {
	return id_ < rhs.id_;
}

template <typename Alphabet>
bool suffix_tree<Alphabet>::Link::operator==(Link const &rhs) const {

}

template <typename Alphabet>
bool suffix_tree<Alphabet>::Link::operator<(Link const &rhs) const {

}

}

#endif