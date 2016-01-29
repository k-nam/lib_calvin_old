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
private:
	// all negative value integer is only for internal use. 
	// users of this class should only use positive char or int value of Alphabet
	Alphabet dollor_ = -1;
public:
	suffix_tree(abstract_string<Alphabet> const &texts);
	suffix_tree(vector<abstract_string<Alphabet>> const &texts);
	vector<size_t> find_pattern(abstract_string<Alphabet> const &pattern) const;
public:
	 friend class lib_calvin_string::SuffixTreeTest<Alphabet>;
public:
	enum class NodeType {
		Internal = 0, Leaf = 1, Root = 2
	};

	struct NodeKey {
		NodeKey(size_t, size_t);
		NodeKey();
		size_t textId_;
		size_t id_; // identifies unique Node within given textId_; denotes the starting index of a suffix
		bool operator==(NodeKey const &rhs) const;
		bool operator<(NodeKey const &rhs) const;
	};

	struct Node {
		Node(NodeKey key, NodeType type, NodeKey parent, NodeKey suffixLink);
		Node(Node const &);
		~Node();
		static void countThisObject();
		NodeKey key_ const;
		// For leaf nodes, id also denotes the starting index of corresponding Point
		NodeKey key_;
		NodeType type_;
		NodeKey parent_;
		NodeKey suffixLink_; // if this node represents ax, points to x. only for internal node.
		bool operator==(Node const &rhs) const;
		bool operator<(Node const &rhs) const;
		static int objectCount_;
	};
	struct NodeExtractKey {
		NodeKey operator()(Node const &) const;
	};
	struct Link {
		Link(size_t textId, Alphabet head, size_t startIndex, size_t endIndex);
		Link();
		size_t textId_;
		Alphabet head_; // first letter of the substring represented by this link
		size_t startIndex_;
		size_t endIndex_; // one past end notation; 0 if this link points to leaf node
	};
	// specifies a point in tree. point correspoinds to substring(s) of text.
	// endIndex is the past-end index of that substring of string represented by the containing link
	// textId of a Point is determined by the edge between src and dest
	struct Point {
		Point(NodeKey src, NodeKey dest, size_t endIndex);
		Point &operator=(Point const &);
		bool operator==(Point const &) const;
		size_t src_;
		size_t dest_;
		// If matched to the end of link, Point is represented by the dest node
		size_t endIndex_;
	};
private:
	void build();
	size_t getLength(Link const &link) const;
	Node makeNewLeaf(NodeKey parentId, NodeKey suffixLink) const;
	Node makeNewInternal(NodeKey parentId, NodeKey suffixLink) const;
	// indices are based on entire text
	Point followPathDown(size_t textId, NodeKey src, size_t startIndex, size_t endIndex) const;
	abstract_string<Alphabet> readToPoint(Point const &) const;
	Link const & getLink(NodeKey src, NodeKey dest) const;
	Link const & getLink(Point const &) const;
	Link const & getParenetLink(size_t id) const;
	Node & getNode(NodeKey key);
	Node const & getNode(NodeKey key)const;
	Point createPoint(NodeKey dest) const; // when the end of suffix is dest node
	Point getNullPoint() const; // denotes error value
	Point followSuffixLink(NodeKey src) const;
	bool isNode(Point const &) const;
	bool isLeafNode(Point const &) const;
	// returns (Point after advaning one char, didPathExist)
	std::pair<Point, bool> continuesWith(Point const &, Alphabet const &) const;
	std::pair<Point, bool> rootContinuesWith(Alphabet const &) const;
	std::pair<Point, bool> nodeContinuesWith(NodeKey node, Alphabet const &) const;
	// returns internal braching node (either created or not)
	NodeKey createBranch(Point const &, Alphabet const &);
	void createBranchAtRoot(Alphabet const &character);
	void printSuffix(NodeKey leaf) const;
	void printAllSuffix() const;
	void insertEdge(NodeKey src, NodeKey dest, Link const &link);
	// return false when pattern is not found
	std::pair<Point, bool> findPatternPoint(abstract_string<Alphabet> const &pattern) const;
	void findReachableLeaves(Point const &point, vector<NodeKey> &result) const;
	void findReachableLeaves(NodeKey node, vector<NodeKey> &result) const;
	void printPoint(Point const &) const;
	void printNode(NodeKey node) const;
private:
	vector<abstract_string<Alphabet>> texts_;
	lib_calvin::graph<Node, Link, std::pair<size_t, size_t>, NodeExtractKey> graph_;
private:
	size_t textId_; 
	size_t phase_; // length of current prefix
	size_t extension_; // starting index of operating substring
	size_t internalNodeId_;
	NodeKey rootKey_;
	vector<NodeKey> leafNodes_;
};

template <typename Alphabet>
suffix_tree<Alphabet>::suffix_tree(abstract_string<Alphabet> const &text) {
	init(vector<abstract_string<Alphabet>>(text));
}

template <typename Alphabet>
suffix_tree<Alphabet>::suffix_tree(vector<abstract_string<Alphabet>> const &texts) {
	init(texts);
	lib_calvin::stopwatch watch;
	watch.start();
	build();
	watch.stop();
	//std::cout << "Suffix with size: " << text.length() << " build took: " << watch.read() << "\n";
}

template <typename Alphabet>
void suffix_tree<Alphabet>::init(vector<abstract_string<Alphabet>> const &texts) {
	texts_ = texts;
	// we need $ char at the tail for the correctness of the following routine
	// otherwise, some suffix may not have corresponding leaf node
	for (int i = 0; i < texts_.size(); i++) {
		texts_[i] += Alphabet(dollor_); // each string must have unique $
		dollor_--;
	}
	internalNodeId_ = 0;
}

template <typename Alphabet>
void suffix_tree<Alphabet>::build() {
	bool wasInternalCreatedInLastExtension = false;
	NodeKey lastCreated;
	bool wasPhaseEndedExplicitly = true;
	// inserting root
	rootKey_ = makeNewInternal(0, 0);
	graph_.insert_vertex(Node(rootKey_, NodeType::Root, 0, 0));
	Point workingPoint = getNullPoint();	

	for (textId_ = 0; textId_ < texts_.size(); textId_++) {
		size_t extension_ = 0;
		for (phase_ = 1; phase_ <= text_.size(); phase_++) { // must add text[phase - 1] to each suffix		
			wasInternalCreatedInLastExtension = false;
			Alphabet const &character = texts_[textId_][phase_ - 1];
			//std::cout << "phase: " << phase_ << " char: " << character << "\n";
			for (; extension_ < phase_; extension_++) {
				if (extension_ + 1 == phase_) { // last one char; deal with root only
					auto result = rootContinuesWith(character);
					if (!result.second) {
						//std::cout << "  Rule A: " << "\n";
						createBranchAtRoot(character);
						leafNodes_.push_back(NodeKey(textId_, extension_);
					} else {
						//std::cout << "  Rule B: " << "\n";
						workingPoint = result.first;
						break;
					}
				} else {
					// current suffix: [extension_, phase_)
				//	Point currentPoint = followPathDown(rootKey_, extension_, phase_ - 1);
				//	if (workingPoint == getNullPoint()){
						//std::cout << "wowrking point is dummy2\n";
					//	exit(0);
				//	}
				//	if (!(currentPoint == workingPoint)) {
					//	std::cout << "working point error\n";
					//	exit(0);
					//}
					auto result = continuesWith(workingPoint, character);
					if (!result.second) { // Rule2
						//std::cout << "  Rule A': " << "\n";
						NodeKey branchNode = createBranch(workingPoint, character);
						if (wasInternalCreatedInLastExtension) {
							//std::cout << "setting suffix link from: " << lastCreated<< "to : " <<  branchNode << "\n";
							getNode(lastCreated).suffixLink_ = branchNode;
						}
						wasInternalCreatedInLastExtension = true;
						lastCreated = branchNode;
						workingPoint = followSuffixLink(branchNode);
						leafNodes_.push_back(NodeKey(textId_, extension_));
					} else { // Rule3
						if (wasInternalCreatedInLastExtension) {
							// workingPoiont must be a node now
							getNode(lastCreated).suffixLink_ = workingPoint.dest_;
						}
						//std::cout << "  Rule B': " << "\n";
						workingPoint = result.first;
						break;
					}
				}
			}
			//std::cout << "\n";
		}
		// currently, phase is length + 1 (due to the for loop)
		// because the length of a link is calculated based on phase_, this needs correction
		phase_ = texts_[textId_].length();
	}
}

template <typename Alphabet>
vector<size_t>
suffix_tree<Alphabet>::find_pattern(abstract_string<Alphabet> const &pattern) const {
	stopwatch watch;
	watch.start();
	vector<NodeKey> reachableLeaves;
	auto point = findPatternPoint(pattern);
	if (point.second == false) {
	} else {
		findReachableLeaves(point.first, reachableLeaves);
	}
	watch.stop();
	std::cout << "pattern with size: " << pattern.length() << " match took: " << watch.read() << "\n";
	vector<size_t> result;
	for (auto i = reachableLeaves.begin(); i < reachableLeaves.end(); i++) {
		result2.push_back(reachableLeaves[i].second);
	}
	return result;
}

template <typename Alphabet>
std::pair<typename suffix_tree<Alphabet>::Point, bool>
suffix_tree<Alphabet>::findPatternPoint(abstract_string<Alphabet> const &pattern) const {
	NodeKey srcNode;
	NodeKey destNode = rootKey_;
	Link link;
	bool atNode = true;
	size_t textId = 0;
	size_t indexInText = 0;
	for (size_t i = 0; i < pattern.length(); i++) {
		if (atNode) {
			bool found = false;
			auto pairs = graph_.get_vertex_edge_pairs_from(destNode);
			for (auto iter = pairs.begin(); iter != pairs.end(); ++iter) {
				if (iter->second.head_ == pattern[i]) {
					srcNode = destNode;
					destNode = iter->first.key_;
					link = getLink(srcNode, destNode);
					textId = link.textId_;
					indexInText = link.startIndex_;
					found = true;
					break;
				} 
			}
			// no way down
			if (!found) {
				return std::make_pair(getNullPoint(), false); // dummy
			}
		}
		if (texts_[textId][indexInText] == pattern[i]) {
			if (link.endIndex_ == 0 || indexInText + 1 < link.endIndex_) {
				atNode = false;
			} else {
				atNode = true;
			}
			indexInText++;
		} else {
			return std::make_pair(getNullPoint(), false); // dummy
		}	
	}
	return std::make_pair(Point(srcNode, destNode, indexInText), true); 
}

template <typename Alphabet>
void
suffix_tree<Alphabet>::findReachableLeaves(Point const &point, vector<NodeKey> &result) const {
	findReachableLeaves(point.dest_, result);
}

template <typename Alphabet>
void
suffix_tree<Alphabet>::findReachableLeaves(NodeKey node, vector<NodeKey> &result) const {
	if (getNode(node).type_ == NodeType::Leaf) {
		//std::cout << "findReachableLeaves adding: " << node << "\n";
		result.push_back(node);
	} else {
		auto pairs = graph_.get_vertex_edge_pairs_from(node);
		for (auto iter = pairs.begin(); iter != pairs.end(); ++iter) {
			findReachableLeaves(iter->first.key_, result);
		}
	}
}

template <typename Alphabet>
void suffix_tree<Alphabet>::printPoint(Point const &point) const {
	readToPoint(point).print();
}

template <typename Alphabet>
void suffix_tree<Alphabet>::printNode(NodeKey node) const {
	readToPoint(createPoint(node)).print();
}

template <typename Alphabet>
Point suffix_tree<Alphabet>::getNullPoint() const {
	return Point(NodeKey(0, 0), NodeKey(0, 0), 1);
}

template <typename Alphabet>
suffix_tree<Alphabet>::NodeKey::NodeKey(size_t textId, sizt_t id):
	textId_(textId), id_(id) {}

template <typename Alphabet>
suffix_tree<Alphabet>::NodeKey::NodeKey(): textId_(0), id_(0) {}

template <typename Alphabet>
bool suffix_tree<Alphabet>::NodeKey::operator==(NodeKey const &rhs) const {
	return textId_ == rhs.textId_ && id_ == rhs.id_;
}

template <typename Alphabet>
bool suffix_tree<Alphabet>::NodeKey::operator<(NodeKey const &rhs) const {
	return textId_ < rhs.textId_ || 
		(textId_ == rhs.textId_ && id_ < rhs.id_);
}

template <typename Alphabet>
suffix_tree<Alphabet>::Node::Node(NodeKey key, NodeType type, NodeKey parent, NodeKey suffixLink):
	key_(key), type_(type), parent_(parent), suffixLink_(suffixLink) {
	objectCount_++; 
	//std::cout << "objectCount_ is: " << objectCount_ << "\n";
}

template <typename Alphabet>
suffix_tree<Alphabet>::Node::Node(Node const &rhs): key_(rhs.key_), type_(rhs.type_),
parent_(rhs.parent_), suffixLink_(rhs.suffixLink_) {
	objectCount_++; 
	//std::cout << "objectCount_ is: " << objectCount_ << "\n";
}

template <typename Alphabet>
suffix_tree<Alphabet>::Node::~Node() {
	objectCount_--;
	if (objectCount_ < 0) {
		std::cout << "objectCount_ is: " << objectCount_ << "\n";
	}
	//std::cout << "objectCount_ is: " << objectCount_ << "\n";
}

template <typename Alphabet>
void suffix_tree<Alphabet>::Node::countThisObject() {
	std::cout << "Node current count is: \"" << objectCount_ << "\": ";
	if (objectCount_ == 0) {
		std::cout << "O.K\n\n";
	} else {
		std::cout << "memory leak!!!\n";
		exit(0);
	}
}

template <typename Alphabet>
int suffix_tree<Alphabet>::Node::objectCount_ = 0;

template <typename Alphabet>
bool suffix_tree<Alphabet>::Node::operator==(Node const &rhs) const {
	return key_ == rhs.key_;
}

template <typename Alphabet>
bool suffix_tree<Alphabet>::Node::operator<(Node const &rhs) const {
	return key_ < rhs.key_;
}
template <typename Alphabet>
NodeKey
suffix_tree<Alphabet>::NodeExtractKey::operator()(Node const &node) const {
	return node.key_;
};

template <typename Alphabet>
suffix_tree<Alphabet>::Link::Link(size_t textId, Alphabet head, size_t startIndex, size_t endIndex):
	textId_(textId), head_(head), startIndex_(startIndex), endIndex_(endIndex) { 
	if (startIndex >= endIndex && endIndex != 0) {
		std::cout << "Link error:  start: " << startIndex << " end: " << endIndex << "\n";
		exit(0);
	}
}

template <typename Alphabet>
suffix_tree<Alphabet>::Link::Link(): 
	textId_(0), head_(0), startIndex_(0), endIndex_(0) {}

template <typename Alphabet>
suffix_tree<Alphabet>::Point::Point(NodeKey src, NodeKey dest, size_t endIndex):
	src_(src), dest_(dest), endIndex_(endIndex) { 
	if (endIndex == 0) {
		std::cout << "Point ctor creating leaf\n";
		exit(0);
	}
}

template <typename Alphabet>
typename suffix_tree<Alphabet>::Point &
suffix_tree<Alphabet>::Point::operator=(Point const &rhs) {
	src_ = rhs.src_;
	dest_ = rhs.dest_;
	endIndex_ = rhs.endIndex_;
	return *this;
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
		if (textId_ == link.textId_) { // still working on the text
			return phase_ - link.startIndex_;
		} else { // done with the text
			return texts_[link.textId].length() - link.startIndex_;
		}
	}
}

template <typename Alphabet>
Node 
suffix_tree<Alphabet>::makeNewLeaf(NodeKey parentId, NodeKey suffixLink) const {
	return Node(NodeKey(textId_, extension_), NodeType::Leaf, parentId, suffixLink);
}

template <typename Alphabet>
Node
suffix_tree<Alphabet>::makeNewInternal(NodeKey parentId, NodeKey suffixLink) const {
	return Node(NodeKey(textId_, internalNodeId_++), NodeType::Internal, parentId, suffixLink);
}

template <typename Alphabet>
typename suffix_tree<Alphabet>::Point 
suffix_tree<Alphabet>::followPathDown(size_t textId, 
		NodeKey src, size_t startIndex, size_t endIndex) const {
	vector<std::pair<Node, Link>> pairs = graph_.get_vertex_edge_pairs_from(src);
	//std::cout << "graph size was: "<< graph_.number_of_vertex() << "numedge was: " << graph_.number_of_edge() << "\n";
	//std::cout << "start was: " << startIndex << "end was: " << endIndex << "\n";
	//std::cout << "num outlink was: " << pairs.size() << "\n";
	for (auto iter = pairs.begin(); iter != pairs.end(); ++iter) {
		if (iter->second.head_ == texts_[textId][startIndex]) {
			size_t linkLength = getLength(iter->second);
			size_t linkStartIndex = iter->second.startIndex_;
			//std::cout << "link src was: " << iter->second.startIndex_ << "link dest was: " << iter->second.endIndex_ << "\n";
			//std::cout << "linkLength was: " << linkLength << "\n";	
			if (linkLength < endIndex - startIndex) {
				return followPathDown(textId, iter->first.key_, startIndex + linkLength, endIndex);
			} else {
				return Point(src, iter->first.key_, linkStartIndex + endIndex - startIndex);
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
suffix_tree<Alphabet>::getLink(NodeKey src, NodeKey dest) const {
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
suffix_tree<Alphabet>::getParenetLink(NodeKey key) const {
	return graph_.get_edge(getNode(key).parent_, key);
}

template <typename Alphabet>
typename suffix_tree<Alphabet>::Node & 
suffix_tree<Alphabet>::getNode(NodeKey key) {
	return graph_.get_vertex(key);
} 

template <typename Alphabet>
typename suffix_tree<Alphabet>::Node const & 
suffix_tree<Alphabet>::getNode(NodeKey key) const {
	return graph_.get_vertex(key);
} 

template <typename Alphabet>
typename suffix_tree<Alphabet>::Point 
suffix_tree<Alphabet>::createPoint(NodeKey dest) const {
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
	return Point(node.parent_, dest, endIndex);
}

template <typename Alphabet>
typename suffix_tree<Alphabet>::Point 
suffix_tree<Alphabet>::followSuffixLink(NodeKey node) const {
	if (getNode(node).suffixLink_ != 0) {
		//std::cout << "follow suffix\n";
		return createPoint(getNode(node).suffixLink_);
	} else {	
		NodeKey parentNode = getNode(node).parent_;
		Link const &link = getLink(parentNode, node);
		//std::cout << "link start: " << link.startIndex_ << " link end: " << link.endIndex_ << "\n";
		if (getNode(parentNode).suffixLink_ != 0) { 
			//std::cout << "follow suffix2\n";
			NodeKey linkOfParent = getNode(parentNode).suffixLink_;
			return followPathDown(linkOfParent, link.startIndex_, link.endIndex_);
		} else {
			if (parentNode == rootKey_) {		
				if (link.startIndex_ + 1 < link.endIndex_) {
					//std::cout << "follow suffix3\n";
					return followPathDown(rootKey_, link.startIndex_ + 1, link.endIndex_);
				} else { // internal node of length 1 cannot have suffix link
					//std::cout << "follow suffix4\n";
					return getNullPoint();
				}				
			} else {
				NodeKey grandPa = getNode(parentNode).parent_;
				//std::cout << "follow suffix5, parent was: " << parentNode << " grandpd was: " << grandPa << "\n";		
				// parent is direct child of root. 
				return followPathDown(rootKey_, link.startIndex_, link.endIndex_);
			}
		}
	}
}

template <typename Alphabet>
bool suffix_tree<Alphabet>::isNode(Point const &point) const {
	return getLink(point.src_, point.dest_).endIndex_ == point.endIndex_;
}

template <typename Alphabet>
std::pair<typename suffix_tree<Alphabet>::Point, bool>  
suffix_tree<Alphabet>::continuesWith(Point const &point, Alphabet const &character) const {
	//std::cout << "continue with : " << character << " \n";
	//readToPoint(point).print();
	if (isNode(point)) {
		return nodeContinuesWith(point.dest_, character);
	} else {
		//std::cout << "continue with4\n ";
		Link const &link = getLink(point);
		if (text_[point.endIndex_] == character) {
			return std::make_pair(Point(point.src_, point.dest_, point.endIndex_ + 1), true);
		}	else {
			return std::make_pair(getNullPoint(), false);
		}
	}
}

template <typename Alphabet>
std::pair<typename suffix_tree<Alphabet>::Point, bool>  
suffix_tree<Alphabet>::nodeContinuesWith(NodeKey node, Alphabet const &character) const {
	//std::cout << "continue with2 : ";
	auto outLinks = graph_.get_vertex_edge_pairs_from(node);
	if (!(outLinks.size() > 1) && node != rootKey_) {
		std::cout << "continuesWith error2\n";
		exit(0);
	}
	for (auto iter = outLinks.begin(); iter != outLinks.end(); ++iter) {
		if (iter->second.head_ == character) {
			//std::cout << "continue with3 : \n";
			return std::make_pair(Point(node, iter->first.key_, 
				getLink(node, iter->first.key_).startIndex_ + 1), true);
		}
	}
	return std::make_pair(getNullPoint(), false);
}

template <typename Alphabet>
std::pair<typename suffix_tree<Alphabet>::Point, bool> 
suffix_tree<Alphabet>::rootContinuesWith(Alphabet const &character) const {
	return nodeContinuesWith(rootKey_, character);
}

template <typename Alphabet>
NodeKey
suffix_tree<Alphabet>::createBranch(Point const &point, Alphabet const &character) {
	if (!isNode(point)) {
		NodeKey parentKey = getNode(point.src_).key_;
		NodeKey childKey = getNode(point.dest_).key_;
		//std::cout << "point src was: " << point.src_ << " point dest was: " << point.dest_<< "\n";
		NodeKey newInternalKey = getNewInternalKey();
		//std::cout << "creating InternalId: " << newInternalId << "\n";
		Node newInternal = makeNewInternal(parentKey, 0);
		NodeKey newInternalKey = newInternal.key_;
		getNode(childKey).parentKey_ = newInternal;
		Node newLeaf = makeNewLeaf(newInternalKey, 0);
		Link oldLink(getLink(point));
		Link newLink1(oldLink.textId_, texts_[oldLink.textId_][point.endIndex_],
			point.endIndex_, oldLink.endIndex_); // internal -> child
		Link newLink2(textId_, character, phase_ - 1, 0); // internal -> newLeaf
		//std::cout << "changing old link end to: " << point.endIndex_ <<"\n";
		oldLink.endIndex_ = point.endIndex_;
		//std::cout << "old link start: " << oldLink.startIndex_ << "end " << oldLink.endIndex_ << "\n";
		graph_.insert_vertex(newInternal);
		graph_.insert_vertex(newLeaf);	
		graph_.remove_edge(parentKey, childKey);
		insertEdge(parentKey, newInternalKey, oldLink);
		insertEdge(newInternalKey, childKey, newLink1);
		insertEdge(newInternalKey, newLeaf.key_, newLink2);
		return newInternalKey;
	} else {
		NodeKey parentKey = getNode(point.dest_).key_;
		Node newLeaf = makeNewLeaf(parentKey, 0);
		Link newLink(textId_, character, phase_ - 1, 0); // internal -> newLeaf
		graph_.insert_vertex(newLeaf);
		insertEdge(parentKey, newLeaf.key_, newLink);
		return parentKey;
	}
}

template <typename Alphabet>
void suffix_tree<Alphabet>::createBranchAtRoot(Alphabet const &character) {
	Node newLeaf = makeNewLeaf(rootKey_, 0);
	Link newLink(textId_, character, phase_ - 1, 0); 
	graph_.insert_vertex(newLeaf);
	insertEdge(rootKey_, newLeaf.key_, newLink);
}

template <typename Alphabet>
void suffix_tree<Alphabet>::printSuffix(NodeKey leaf) const {
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
void suffix_tree<Alphabet>::insertEdge(NodeKey src, NodeKey dest, Link const &link) {
	graph_.insert_edge(src, dest, link);
	//std::cout << "inserting edge src: " << src << " dest: " << dest << " link start: " << link.startIndex_ <<
		//" link end: " << link.endIndex_ << "\n";
}

}
#endif