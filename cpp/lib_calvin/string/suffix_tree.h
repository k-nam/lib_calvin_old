#ifndef LIB_CALVIN__STRING__SUFFIX_TREE_H
#define LIB_CALVIN__STRING__SUFFIX_TREE_H

#include "abstract_string.h"
#include "graph.h"
#include "stopwatch.h"

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
	vector<size_t> find_pattern(abstract_string<Alphabet> const &pattern) const;
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
	// endIndex is the past-end index of that substring of string represented by the containing link
	struct Point {
		Point(size_t src, size_t dest, size_t endIndex);
		bool operator==(Point const &) const;
		size_t const src_;
		size_t const dest_;
		// If matched to the end of link, Point is represented by the dest node
		size_t const endIndex_;
	};
private:
	void build();
	size_t getLength(Link const &link) const;
	// indices are based on entire text
	Point followPathDown(size_t src, size_t startIndex, size_t endIndex) const;
	abstract_string<Alphabet> readToPoint(Point const &) const;
	Link const & getLink(size_t src, size_t dest) const;
	Link const & getLink(Point const &) const;
	Link const & getParenetLink(size_t id) const;
	Node & getNode(size_t id);
	Node const & getNode(size_t id)const;
	Point createPoint(size_t src, size_t dest, size_t endIndex) const;
	Point createPoint(size_t dest) const; // when the end of suffix is dest node
	bool isNode(Point const &) const;
	bool isLeafNode(Point const &) const;
	bool continuesWith(Point const &, Alphabet const &) const;
	bool rootContinuesWith(Alphabet const &) const;
	bool nodeContinuesWith(size_t node, Alphabet const &) const;
	void createBranch(Point const &, Alphabet const &);
	void createBranchAtRoot(Alphabet const &character);
	void printSuffix(size_t leaf) const;
	void printAllSuffix() const;
	void insertEdge(size_t src, size_t dest, Link const &link);
	// return false when pattern is not found
	std::pair<Point, bool> findPatternPoint(abstract_string<Alphabet> const &pattern) const;
	void findReachableLeaves(Point const &point, vector<size_t> &result) const;
	void findReachableLeaves(size_t node, vector<size_t> &result) const;
private:
	abstract_string<Alphabet> text_;
	lib_calvin::graph<Node, Link, size_t, NodeExtractKey> graph_;
	//Node nodeOne_; // first node which represents the entire text
private:
	size_t phase_; // length of current substring
	size_t extension_; // starting index
	size_t internalNodeId_;
	size_t rootId_;
	vector<size_t> leafNodes_;
};


template <typename Alphabet>
suffix_tree<Alphabet>::suffix_tree(abstract_string<Alphabet> const &text): 
text_(text + abstract_string<Alphabet>(Alphabet('$'))), internalNodeId_(text_.size()) {
	lib_calvin::stopwatch watch;
	watch.start();
	build();
	watch.stop();
	std::cout << "Suffix with size: " << text.length() << " build took: " << watch.read() << "\n";
}

template <typename Alphabet>
void suffix_tree<Alphabet>::build() {
	bool wasNodeCreatedInLastExtension = false;
	size_t lastCreated = 0;
	bool wasPhasedEndedExplicitly = true;
	// inserting root
	rootId_ = internalNodeId_++;
	graph_.insert_vertex(Node(NodeType::Root, rootId_, 0, 0));
	extension_ = 0;
	// $ char at the tail for convenience
	for (phase_ = 1; phase_ <= text_.size(); phase_++) { // must add text[phase] to each suffix		
		wasPhasedEndedExplicitly = true;	
		Alphabet const &character = text_[phase_ - 1];
		//std::cout << "phase: " << phase_ << " char: " << character << "\n";
		for ( ; extension_ < phase_; extension_++) { 
			//std::cout << " ext: " << extension_  << " ";
			if (extension_ + 1 == phase_) { // last one char
				if (!rootContinuesWith(character)) {
					//std::cout << "  Rule A: " << "\n";
					createBranchAtRoot(character);
					leafNodes_.push_back(extension_);
				} else {
					//std::cout << "  Rule B: " << "\n";
					break;
				}
			} else {			
				// current suffix: [extension_, phase_)
				Point currentPoint = followPathDown(rootId_, extension_, phase_ - 1);
				if (!continuesWith(currentPoint, character)) { // Rule2
					//std::cout << "  Rule A': " << "\n";
					createBranch(currentPoint, character);
					leafNodes_.push_back(extension_);
				} else { // Rule3
					//std::cout << "  Rule B': " << "\n";
					wasPhasedEndedExplicitly = false;
					break; 
				}
			}
		}	
		//std::cout << "\n";
	}
	phase_ = text_.length();
}

template <typename Alphabet>
vector<size_t> 
suffix_tree<Alphabet>::find_pattern(abstract_string<Alphabet> const &pattern) const {
	stopwatch watch;
	watch.start();

	vector<size_t> result;
	auto point = findPatternPoint(pattern);
	if (point.second == false) {
	} else {
		findReachableLeaves(point.first, result);
	}
	watch.stop();
	std::cout << "pattern with size: " << pattern.length() << " match took: " << watch.read() << "\n";

	return result;
}

template <typename Alphabet>
std::pair<typename suffix_tree<Alphabet>::Point, bool>
suffix_tree<Alphabet>::findPatternPoint(abstract_string<Alphabet> const &pattern) const {
	size_t srcNode = 0;
	size_t destNode = rootId_;
	Link link(text_[0], 0, 0); // dummy
	bool atNode = true;
	size_t indexInText = 0;
	for (size_t i = 0; i < pattern.length(); i++) {
		if (atNode) {
			bool found = false;
			auto pairs = graph_.get_vertex_edge_pairs_from(destNode);
			for (auto iter = pairs.begin(); iter != pairs.end(); ++iter) {
				if (iter->second.head_ == pattern[i]) {
					srcNode = destNode;
					destNode = iter->first.id_;
					link = getLink(srcNode, destNode);
					indexInText = link.startIndex_;
					found = true;
					break;
				} 
			}
			// no way down
			if (!found) {
				return std::make_pair(Point(0, 0, 1), false); // dummy
			}
		}
		if (text_[indexInText] == pattern[i]) {
			if (indexInText + 1 < link.endIndex_) {
				atNode = false;
			} else {
				atNode = true;
			}
			indexInText++;
		} else {
			return std::make_pair(Point(0, 0, 1), false); // dummy
		}	
	}
	return std::make_pair(Point(srcNode, destNode, indexInText), true); 
}

template <typename Alphabet>
void
suffix_tree<Alphabet>::findReachableLeaves(Point const &point, vector<size_t> &result) const {
	findReachableLeaves(point.dest_, result);
}

template <typename Alphabet>
void
suffix_tree<Alphabet>::findReachableLeaves(size_t node, vector<size_t> &result) const {
	if (getNode(node).type_ == NodeType::Leaf) {
		std::cout << "findReachableLeaves adding: " << node << "\n";
		result.push_back(node);
	} else {
		auto pairs = graph_.get_vertex_edge_pairs_from(node);
		for (auto iter = pairs.begin(); iter != pairs.end(); ++iter) {
			findReachableLeaves(iter->first.id_, result);
		}
	}
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
	head_(head), startIndex_(startIndex), endIndex_(endIndex) { 
	if (startIndex >= endIndex && endIndex != 0) {
		std::cout << "Link error:  start: " << startIndex << " end: " << endIndex << "\n";
		exit(0);
	}
}

template <typename Alphabet>
suffix_tree<Alphabet>::Point::Point(size_t src, size_t dest, size_t endIndex):
	src_(src), dest_(dest), endIndex_(endIndex) { 
	if (endIndex == 0) {
		std::cout << "Point ctor creating leaf\n";
		exit(0);
	}
}

template <typename Alphabet>
bool suffix_tree<Alphabet>::Point::operator==(Point const &rhs) const {
	return src_ == rhs.src_ && dest_ == rhs.dest_ && endIndex_ == rhs.endIndex_;
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
	//std::cout << "graph size was: "<< graph_.number_of_vertex() << "numedge was: " << graph_.number_of_edge() << "\n";
	//std::cout << "start was: " << startIndex << "end was: " << endIndex << "\n";
	//std::cout << "num outlink was: " << pairs.size() << "\n";
	for (auto iter = pairs.begin(); iter != pairs.end(); ++iter) {
		//std::cout << "head was: " << iter->second.head_ << "\n";
		//std::cout << "startletter was: " << text_[startIndex] << "\n";
		if (iter->second.head_ == text_[startIndex]) {
			size_t linkLength = getLength(iter->second);
			size_t linkStartIndex = iter->second.startIndex_;
			//std::cout << "link src was: " << iter->second.startIndex_ << "link dest was: " << iter->second.endIndex_ << "\n";
			//std::cout << "linkLength was: " << linkLength << "\n";
			if (linkLength < endIndex - startIndex) {
				return followPathDown(iter->first.id_, startIndex + linkLength, endIndex);
			} else {
				return Point(src, iter->first.id_, linkStartIndex + endIndex - startIndex);
			}
		}
	}
	// error; path not found
	std::cout << "followPathDown error\n";
	exit(0);
}

template <typename Alphabet>
abstract_string<Alphabet> 
suffix_tree<Alphabet>::readToPoint(Point const &point) const {
	Link const &link = getLink(point);
	if (!(link.startIndex_ < point.endIndex_)) {
		std::cout << "suffix_tree readToPoint error\n";
		std::cout << "link.startIndex_: " << link.startIndex_ << " link.endIndex_ " << link.endIndex_ <<
			" point.endIndex_ " << point.endIndex_ << "\n";
		exit(0);
	}
	abstract_string<Alphabet> subString = text_.substr(link.startIndex_, point.endIndex_);
	//std::cout << "start was: " << link.startIndex_ << " end: " << endIndex << " subs was: ";
	//subString.print();
	if (getNode(point.src_).type_ == NodeType::Root) {
		return subString;
	} else {
		return readToPoint(createPoint(point.src_)) + subString;
	}
}

template <typename Alphabet>
typename suffix_tree<Alphabet>::Link const & 
suffix_tree<Alphabet>::getLink(size_t src, size_t dest) const {
	Link const &link = graph_.get_edge(src, dest);
	if (!(link.startIndex_ < link.endIndex_) && link.endIndex_!= 0) {
	//if (true) {
		std::cout << "getLink error\n";
		std::cout << "startIndex_: " << link.startIndex_ << " endIndex_ " << link.endIndex_ << "\n";
		exit(0);
	}
	return link;
}

template <typename Alphabet>
typename suffix_tree<Alphabet>::Link const & 
suffix_tree<Alphabet>::getLink(Point const &point) const {
	return getLink(point.src_, point.dest_);
}

template <typename Alphabet>
typename suffix_tree<Alphabet>::Link const & 
suffix_tree<Alphabet>::getParenetLink(size_t id) const {
	return graph_.get_edge(getNode(id).parentId_, id);
}

template <typename Alphabet>
typename suffix_tree<Alphabet>::Node & 
suffix_tree<Alphabet>::getNode(size_t id) {
	return graph_.get_vertex(id);
} 

template <typename Alphabet>
typename suffix_tree<Alphabet>::Node const & 
suffix_tree<Alphabet>::getNode(size_t id) const {
	return graph_.get_vertex(id);
} 

template <typename Alphabet>
typename suffix_tree<Alphabet>::Point 
suffix_tree<Alphabet>::createPoint(size_t src, size_t dest, size_t endIndex) const {
	return Point(src, dest, endIndex);
}

template <typename Alphabet>
typename suffix_tree<Alphabet>::Point 
suffix_tree<Alphabet>::createPoint(size_t dest) const {
	if (getNode(dest).type_ == NodeType::Root) {
		std::cout << "suffix_tree createPoint error\n";
		exit(0);
	}
	Link const &link = getParenetLink(dest);
	Node const &node = getNode(dest);
	size_t endIndex = link.endIndex_;
	if (link.endIndex_ == 0) {
		endIndex = phase_;
	}
	return Point(node.parentId_, dest, endIndex);
}


template <typename Alphabet>
bool suffix_tree<Alphabet>::isNode(Point const &point) const {
	return getLink(point.src_, point.dest_).endIndex_ == point.endIndex_;
}

template <typename Alphabet>
bool suffix_tree<Alphabet>::continuesWith(Point const &point, Alphabet const &character) const {
	//std::cout << "continue with : " << character << " \n";
	//readToPoint(point).print();
	if (isNode(point)) {
		return nodeContinuesWith(point.dest_, character);
	} else {
		//std::cout << "continue with4\n ";
		Link const &link = getLink(point);
		return text_[point.endIndex_] == character;
	}
}

template <typename Alphabet>
bool suffix_tree<Alphabet>::nodeContinuesWith(size_t node, Alphabet const &character) const {
	//std::cout << "continue with2 : ";
	auto outLinks = graph_.get_vertex_edge_pairs_from(node);
	if (!(outLinks.size() > 1) && node != rootId_) {
		std::cout << "continuesWith error2\n";
		exit(0);
	}
	for (auto iter = outLinks.begin(); iter != outLinks.end(); ++iter) {
		if (iter->second.head_ == character) {
			//std::cout << "continue with3 : \n";
			return true;
		}
	}
	return false;
}

template <typename Alphabet>
bool suffix_tree<Alphabet>::rootContinuesWith(Alphabet const &character) const {
	return nodeContinuesWith(rootId_, character);
}

template <typename Alphabet>
void suffix_tree<Alphabet>::createBranch(Point const &point, Alphabet const &character) {
	if (!isNode(point)) {
		Node &parent = getNode(point.src_);
		Node &child = getNode(point.dest_);
		size_t parentId = parent.id_;
		size_t childId = child.id_;
		//std::cout << "point src was: " << point.src_ << " point dest was: " << point.dest_<< "\n";
		size_t newInternalId =  internalNodeId_++;
		//std::cout << "newInternalId: " << newInternalId << "\n";
		Node newInternal(NodeType::Internal, newInternalId, parentId, 0);
		child.parentId_ = newInternalId;
		Node newLeaf(NodeType::Leaf, extension_, newInternalId, 0);
		//std::cout << "b\n";
		Link oldLink(getLink(point));
		Link newLink1(text_[point.endIndex_], 
											point.endIndex_, oldLink.endIndex_); // internal -> child
		Link newLink2(character, phase_ - 1, 0); // internal -> newLeaf
		//std::cout << "changing old link end to: " << point.endIndex_ <<"\n";
		oldLink.endIndex_ = point.endIndex_;
		//std::cout << "old link start: " << oldLink.startIndex_ << "end " << oldLink.endIndex_ << "\n";
		if (2 == oldLink.endIndex_) {
			//std::cout << "createBranch error: old start: " << oldLink.startIndex_ << " end: " << oldLink.endIndex_ << "\n";
		}
		//std::cout << "a\n";
		graph_.insert_vertex(newInternal);
		graph_.insert_vertex(newLeaf);	
		graph_.remove_edge(parentId, childId);
		insertEdge(parentId, newInternalId, oldLink);
		insertEdge(newInternalId, childId, newLink1);
		insertEdge(newInternalId, extension_, newLink2);
		//std::cout << "graph size: " << graph_.size() << "\n";
		//std::cout << "c\n";
	} else {
		size_t parentId = getNode(point.dest_).id_;
		Node newLeaf(NodeType::Leaf, extension_, parentId, 0);
		Link newLink(character, phase_ - 1, 0); // internal -> newLeaf
		graph_.insert_vertex(newLeaf);
		insertEdge(parentId, extension_, newLink);
	}
}

template <typename Alphabet>
void suffix_tree<Alphabet>::createBranchAtRoot(Alphabet const &character) {
	Node newLeaf(NodeType::Leaf, extension_, rootId_, 0);
	Link newLink(character, phase_ - 1, 0); 
	graph_.insert_vertex(newLeaf);
	insertEdge(rootId_, extension_, newLink);
}

template <typename Alphabet>
void suffix_tree<Alphabet>::printSuffix(size_t leaf) const {
	std::cout << "suffix starting index: " << leaf << " ";
	readToPoint(createPoint(leaf)).print();
}
template <typename Alphabet>
void suffix_tree<Alphabet>::printAllSuffix() const {
	for (auto iter = leafNodes_.begin(); iter != leafNodes_.end(); ++iter) {
		printSuffix(*iter);
	}
}

template <typename Alphabet>
void suffix_tree<Alphabet>::insertEdge(size_t src, size_t dest, Link const &link) {
	graph_.insert_edge(src, dest, link);
	//std::cout << "inserting edge src: " << src << " dest: " << dest << " link start: " << link.startIndex_ <<
		//" link end: " << link.endIndex_ << "\n";
}

}
#endif