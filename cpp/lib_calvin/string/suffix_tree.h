#ifndef LIB_CALVIN__STRING__SUFFIX_TREE_H
#define LIB_CALVIN__STRING__SUFFIX_TREE_H

#include "abstract_string.h"
#include "graph.h"

namespace lib_calvin_string {
template <typename Alphabet>
class SuffixTreeTest;
}

namespace lib_calvin
{
template <typename Alphabet>
class suffix_tree {
public:
	suffix_tree(abstract_string<Alphabet> const &text);
public:
	 friend class lib_calvin_string::SuffixTreeTest<Alphabet>;
public:
	enum class NodeType {
		Internal = 0, Leaf = 1, Root = 2
	};
	struct Node {
		Node(NodeType type, size_t id, size_t parentId, size_t suffixLink);
		NodeType type_; 
		// For leaf nodes, id also denotes the starting index of corresponding Point
		size_t id_; // 0 for the entire string
		size_t parentId_;
		size_t suffixLink_; // if this node represents ax, points to x. only for internal node.
		bool operator==(Node const &rhs) const;
		bool operator<(Node const &rhs) const;
	};
	struct NodeExtractKey {
		size_t operator()(Node const &) const;
	};
	struct Link {
		Link(Alphabet head, size_t startIndex, size_t endIndex);
		Alphabet head_; // first letter of the substring represented by this link
		size_t startIndex_;
		size_t endIndex_; // one past end notation; 0 if this link points to leaf node
		bool operator==(Link const &rhs) const;
		bool operator<(Link const &rhs) const;
	};
	// specifies a point in tree. point correspoinds to substring(s) of text.
	// endIndex is the past-end index of that substring
	struct Point {
		Point(size_t src, size_t dest, size_t endIndex);
		bool operator==(Point const &) const;
		bool isNode() const;
		size_t const src_;
		size_t const dest_;
		// index in text, not link. If matched to the end of link, Point is represented by a node
		size_t const endIndex_;
	};
private:
	size_t getLength(Link const &link) const;
	// indices are based on entire text
	Point followPathDown(size_t src, size_t startIndex, size_t endIndex) const;
	abstract_string<Alphabet> readToPoint(Point const &) const;
	Link const & getLink(size_t src, size_t dest) const;
	Link const & getLink(Point const &suffix) const;
	Link const & getParenetLink(size_t id) const;
	Node const & getNode(size_t id) const;
	Point createSuffix(size_t src, size_t dest, size_t endIndex) const;
	Point createSuffix(size_t dest) const; // when the end of suffix is dest node
private:
	abstract_string<Alphabet> text_;
	lib_calvin::graph<Node, Link, size_t, NodeExtractKey> graph_;
	//Node nodeOne_; // first node which represents the entire text
};


template <typename Alphabet>
suffix_tree<Alphabet>::suffix_tree(abstract_string<Alphabet> const &text): text_(text) {
	bool wasNodeCreatedInLastExtension = false;
	size_t lastCreated = 0;
	bool wasPhasedEndedExplicitly = true;
	for (size_t phase = 0; phase < text.size(); phase++) { // must add text[phase] to each suffix
		wasPhasedEndedExplicitly = true;
		size_t extensionToStart = 0;
		for (size_t extension = extensionToStart; extension <= phase; extension++) { 
			// current suffix: [extension, phase]
			if (true) { // Rule1

				extensionToStart = extension + 1;
			} else if (false) { // Rule2
				
				extensionToStart = extension + 1;
			} else { // Rule3
				wasPhasedEndedExplicitly = false;
				break; 
			}
		}	
	}
	//followPathDown(nodeOne_, 0, 3);
}

template <typename Alphabet>
suffix_tree<Alphabet>::Node::Node(NodeType type, size_t id, size_t parentId, size_t suffixLink):
	type_(type), id_(id), parentId_(parentId), suffixLink_(suffixLink) { }

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
suffix_tree<Alphabet>::NodeExtractKey::operator()(Node const &node) const {
	return node.id_;
};

template <typename Alphabet>
suffix_tree<Alphabet>::Link::Link(Alphabet head, size_t startIndex, size_t endIndex):
	head_(head), startIndex_(startIndex), endIndex_(endIndex) { }

template <typename Alphabet>
suffix_tree<Alphabet>::Point::Point(size_t src, size_t dest, size_t endIndex):
	src_(src), dest_(dest), endIndex_(endIndex) { }

template <typename Alphabet>
bool suffix_tree<Alphabet>::Point::operator==(Point const &rhs) const {
	return src_ == rhs.src_ && dest_ == rhs.dest_ && endIndex_ == rhs.endIndex_;
}

template <typename Alphabet>
bool suffix_tree<Alphabet>::Point::isNode() const {
	return getLink(src_, dest_).endIndex_ == endIndex_;
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
typename suffix_tree<Alphabet>::Point 
suffix_tree<Alphabet>::followPathDown(size_t src, size_t startIndex, size_t endIndex) const {
	vector<std::pair<Node, Link>> pairs = graph_.get_vertex_edge_pairs_from(src);
	for (auto iter = pairs.begin(); iter != pairs.end(); ++iter) {
		if (iter->second.head_ == text_[startIndex]) {
			size_t linkLength = getLength(iter->second);
			if (linkLength < endIndex - startIndex) {
				return followPathDown(iter->first.id_, startIndex + linkLength, endIndex);
			} else {
				return Point(src, iter->first.id_, endIndex);
			}
		}
	}
	// error; path not found
	std::cout << "followPathDown error\n";
	exit(0);
}

template <typename Alphabet>
abstract_string<Alphabet> 
suffix_tree<Alphabet>::readToPoint(Point const &suffix) const {
	Link const &link = getLink(suffix);
	size_t endIndex = suffix.endIndex_;
	if (endIndex == 0) {
		endIndex = phase_;
	}
	if (!(link.startIndex_ < endIndex)) {
		std::cout << "suffix_tree readToPoint error\n";
		exit(0);
	}
	abstract_string<Alphabet> subString = text_.substr(link.startIndex_, endIndex);
	//std::cout << "start was: " << link.startIndex_ << " end: " << endIndex << " subs was: ";
	//subString.print();
	if (getNode(suffix.src_).type_ == NodeType::Root) {
		return subString;
	} else {
		return readToPoint(createSuffix(suffix.src_)) + subString;
	}
}

template <typename Alphabet>
typename suffix_tree<Alphabet>::Link const & 
suffix_tree<Alphabet>::getLink(size_t src, size_t dest) const {
	return graph_.get_edge(src, dest);
}

template <typename Alphabet>
typename suffix_tree<Alphabet>::Link const & 
suffix_tree<Alphabet>::getLink(Point const &suffix) const {
	return graph_.get_edge(suffix.src_, suffix.dest_);
}

template <typename Alphabet>
typename suffix_tree<Alphabet>::Link const & 
suffix_tree<Alphabet>::getParenetLink(size_t id) const {
	return graph_.get_edge(getNode(id).parentId_, id);
}

template <typename Alphabet>
typename suffix_tree<Alphabet>::Node const & 
suffix_tree<Alphabet>::getNode(size_t id) const {
	return graph_.get_vertex(id);
} 

template <typename Alphabet>
typename suffix_tree<Alphabet>::Point 
suffix_tree<Alphabet>::createSuffix(size_t src, size_t dest, size_t endIndex) const {
	return Point(src, dest, endIndex);
}

template <typename Alphabet>
typename suffix_tree<Alphabet>::Point 
suffix_tree<Alphabet>::createSuffix(size_t dest) const {
	if (getNode(dest).type_ == NodeType::Root) {
		std::cout << "suffix_tree createSuffix error\n";
		exit(0);
	}
	Link const &link = getParenetLink(dest);
	return Point(getNode(dest).parentId_, dest, link.endIndex_);
}

}

#endif