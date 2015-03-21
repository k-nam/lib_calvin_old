#ifndef LIB_CALVIN__STRING__SUFFIX_TEST_H
#define LIB_CALVIN__STRING__SUFFIX_TEST_H

#include "abstract_string.h"
#include "graph.h"

namespace lib_calvin
{
template <typename Alphabet>
class suffix_tree {
public:
	suffix_tree(abstract_string<Alphabet> const &text);

private:
	enum class NodeType {
		Internal = 0, Leaf = 1, Root = 2
	};
	struct Node {
		NodeType type_; 
		// For leaf nodes, id also denotes the starting index of corresponding suffix
		size_t id_; // 0 for the entire string
		size_t parentId_;
		size_t suffixLink_; // if this node represents ax, points to x. only for internal node.
		bool operator==(Node const &rhs) const;
		bool operator<(Node const &rhs) const;
	};
	struct Link {
		Alphabet head_; // first letter of the substring represented by this link
		size_t startIndex_;
		size_t endIndex_; // one past end notation; 0 if this link points to leaf node
		bool operator==(Link const &rhs) const;
		bool operator<(Link const &rhs) const;
	};
	// specifies a point in tree
	struct suffix {
		Node node_;
		Link *link_;
		// index in text, not link. If matched to the end of link, suffix is represented by a node
		size_t lastMatchingIndex_; 
	};
private:
	size_t getLength(Link const &link) const;
	// indices are based on entire text
	suffix followPathDown(Node const &src, size_t startIndex, size_t endIndex) const;
private:
	abstract_string<Alphabet> text_;
	lib_calvin::graph<Node, Link> graph_;
	Node nodeOne_; // first node which represents the entire text
private:
	size_t phase_;
	size_t extension_;
};


template <typename Alphabet>
suffix_tree<Alphabet>::suffix_tree(abstract_string<Alphabet> const &text): text_(text) {
	followPathDown(nodeOne_, 0, 3);
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
size_t
suffix_tree<Alphabet>::getLength(Link const &link) const {
	if (link.endIndex_ != 0) {
		return link.endIndex_ - link.startIndex_;
	} else {
		return phase_ - link.startIndex_;
	}
}

template <typename Alphabet>
typename suffix_tree<Alphabet>::suffix 
suffix_tree<Alphabet>::followPathDown(Node const &src, size_t startIndex, size_t endIndex) const {
	vector<std::pair<Node, Link>> pairs = graph_.get_vertex_edge_pairs_from(src);
	for (auto iter = pairs.begin(); iter != pairs.end(); ++iter) {
		if (iter->second.head_ == text_[startIndex]) {
			size_t linkLength = getLength(iter->second);
			if (linkLength < endIndex - startIndex) {
				return followPathDown(iter->first, startIndex + linkLength, endIndex);
			} else {
				suffix result;
				result.node_ = iter->first;
				//result.link_ = 
			}
		}
	}
	// error; path not found
	std::cout << "followPathDown error\n";
	exit(0);
}

}

#endif