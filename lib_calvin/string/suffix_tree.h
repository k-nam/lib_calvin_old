#ifndef LIB_CALVIN__STRING__SUFFIX_TREE_H
#define LIB_CALVIN__STRING__SUFFIX_TREE_H

#include "abstract_string.h"
#include "simple_graph.h"
#include "stopwatch.h"
#include "sort.h"

namespace lib_calvin_string {
	template <typename Alphabet>
	class SuffixTreeTest;


	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph =
		lib_calvin_graph::simple_graph>
	void suffixTreeMatching(lib_calvin::abstract_string<Alphabet> const &text,
							lib_calvin::abstract_string<Alphabet> const &pattern,
							std::vector<size_t> &result);
}

namespace lib_calvin
{
	template <typename Alphabet,
		template <typename V, typename E, typename K, typename ExtractKey> class Graph = 
		lib_calvin_graph::simple_graph>
	class suffix_tree {
	private:
		// all negative value integer is only for internal use. 
		// users of this class should only use positive char or int value of Alphabet
		Alphabet dollor_ = '9';
	public:
		suffix_tree(lib_calvin::abstract_string<Alphabet> const &texts);
		suffix_tree(vector<lib_calvin::abstract_string<Alphabet>> const &texts);
		void build();
		// return (textId, startIndex) of match
		vector<std::pair<size_t, size_t>> find_pattern(abstract_string<Alphabet> const &pattern) const;
		abstract_string<Alphabet> find_longest_common_substring() const;
	public:
		friend class lib_calvin_string::SuffixTreeTest<Alphabet>;
	private:
		enum class NodeType {
			Internal = 0, Leaf = 1, Root = 2
		};

		struct NodeKey {
			NodeKey(size_t, size_t);
			NodeKey();
			size_t textId_;
			size_t id_; // identifies unique Node within given textId_; denotes the starting index of a suffix
			bool operator==(NodeKey const &rhs) const;
			bool operator!=(NodeKey const &rhs) const;
			bool operator<(NodeKey const &rhs) const;
			NodeKey & operator=(NodeKey const &rhs);
		};

		struct Node {
			Node(NodeKey key, NodeType type, NodeKey parent, NodeKey suffixLink);
			Node(Node const &);
			~Node();
			static void countThisObject();
			// For leaf nodes, id also denotes the starting index of corresponding Point
			NodeKey key_;
			NodeType type_;
			NodeKey parent_;
			NodeKey suffixLink_; // if this node represents ax, points to x. only for internal node.
			size_t substringLength_; // only for internal node; length of substring represented by this node
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
		// endIndex is the past-end index of that substring of c_string represented by the containing link
		// textId of a Point is determined by the edge between src and dest
		struct Point {
			Point(NodeKey src, NodeKey dest, size_t endIndex);
			Point &operator=(Point const &);
			bool operator==(Point const &) const;
			NodeKey src_;
			NodeKey dest_;
			// If matched to the end of link, Point is represented by the dest node
			size_t endIndex_;
		};
	private:
		void init(vector<abstract_string<Alphabet>> const &);
		void buildTree();
		void findAllCommmonSubstrings();
		size_t getLength(Link const &link) const;
		Node makeNewLeaf(NodeKey parentId);
		Node makeNewInternal(NodeKey parentId);
		// indices are based on entire text
		Point followPathDown(size_t textId, NodeKey src, size_t startIndex, size_t endIndex) const;
		abstract_string<Alphabet> readToPoint(Point const &) const;
		abstract_string<Alphabet> readToNode(NodeKey const &) const;
		Link const & getLink(NodeKey src, NodeKey dest) const;
		Link const & getLink(Point const &) const;
		Link const & getParenetLink(NodeKey key) const;
		Node & getNode(NodeKey key);
		Node const & getNode(NodeKey key)const;
		bool isLeaf(NodeKey const &) const;
		Point createPoint(NodeKey dest) const; // when the end of suffix is dest node
		NodeKey getNullKey() const;
		NodeKey getRootKey() const;
		NodeKey getNewInternalKey();
		Point getNullPoint() const; // denotes error value
		Point followSuffixLink(NodeKey src) const;
		bool isNode(Point const &) const;
		// returns (Point after advaning one char, didPathExist)
		std::pair<Point, bool> continuesWith(Point const &, Alphabet const &) const;
		std::pair<Point, bool> rootContinuesWith(Alphabet const &) const;
		std::pair<Point, bool> nodeContinuesWith(NodeKey key, Alphabet const &) const;
		// returns internal braching node (either created or not)
		NodeKey createBranch(Point const &, Alphabet const &);
		void createBranchAtRoot(Alphabet const &character);
		void printSuffix(NodeKey leaf) const;
		void printAllSuffix() const;
		void printAllCommonSubstring() const;
		void insertEdge(NodeKey src, NodeKey dest, Link const &link);
		// return false when pattern is not found
		std::pair<Point, bool> findPatternPoint(abstract_string<Alphabet> const &pattern) const;
		void findReachableLeaves(Point const &point, vector<NodeKey> &result) const;
		void findReachableLeaves(NodeKey key, vector<NodeKey> &result) const;
		void printPoint(Point const &) const;
		void printNode(NodeKey key) const;
	private:
		vector<abstract_string<Alphabet>> texts_;
		//lib_calvin_graph::simple_graph<Node, Link, NodeKey, NodeExtractKey> graph_;
		Graph<Node, Link, NodeKey, NodeExtractKey> graph_;

	private:
		size_t textId_;
		size_t phase_; // length of current prefix
		size_t extension_; // starting index of operating substring
		size_t internalNodeId_;
		vector<NodeKey> leafNodes_;
		map<NodeKey, set<size_t>> internalNodeToTextId_;
	};

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	suffix_tree<Alphabet, Graph>::suffix_tree(abstract_string<Alphabet> const &text) {
		init(vector<abstract_string<Alphabet>>(1, text));
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	suffix_tree<Alphabet, Graph>::suffix_tree(vector<abstract_string<Alphabet>> const &texts) {
		init(texts);
		lib_calvin::stopwatch watch;
		watch.start();
		watch.stop();
		//std::cout << "Suffix with size: " << text.length() << " build took: " << watch.read() << "\n";
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	void suffix_tree<Alphabet, Graph>::init(vector<abstract_string<Alphabet>> const &texts) {
		texts_ = texts;
		// we need $ char at the tail for the correctness of the following routine
		// otherwise, some suffix may not have corresponding leaf node
		for (size_t i = 0; i < texts_.size(); i++) {
			texts_[i] += Alphabet(dollor_); // each c_string must have unique $
			dollor_--;
		}
		internalNodeId_ = 0;
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	void suffix_tree<Alphabet, Graph>::build() {
		buildTree();
		findAllCommmonSubstrings();
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	vector<std::pair<size_t, size_t>>
		suffix_tree<Alphabet, Graph>::find_pattern(abstract_string<Alphabet> const &pattern) const {
		stopwatch watch;
		watch.start();
		vector<NodeKey> reachableLeaves;
		auto pointOrNot = findPatternPoint(pattern);
		if (pointOrNot.second == false) {
		} else {
			findReachableLeaves(pointOrNot.first, reachableLeaves);
		}
		watch.stop();
		std::cout << "pattern with size: " << pattern.length() << " match took: " << watch.read() << "\n";
		vector<std::pair<size_t, size_t>> result;
		for (auto iter = reachableLeaves.begin(); iter < reachableLeaves.end(); iter++) {
			result.push_back(std::make_pair(iter->textId_, iter->id_));
		}
		lib_calvin::intro_sort(result.begin(), result.end());
		return result;
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	abstract_string<Alphabet>
		suffix_tree<Alphabet, Graph>::find_longest_common_substring() const {
		size_t maxLength = 0; // max length of substring common to all texts so far
		NodeKey keyForMaxLength = getNullKey(); // internal node corresponding to the above
		for (auto iter = internalNodeToTextId_.begin(); iter != internalNodeToTextId_.end(); ++iter) {
			if (iter->second.size() == texts_.size()) { // common to all
				size_t thisLength = getNode(iter->first).substringLength_;
				if (thisLength > maxLength) {
					maxLength = thisLength;
					keyForMaxLength = iter->first;
				}
			}
		}
		return readToNode(keyForMaxLength);
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	void suffix_tree<Alphabet, Graph>::buildTree() {
		// inserting root
		Node root(getRootKey(), NodeType::Root, getNullKey(), getNullKey());
		graph_.insert_vertex(root);

		for (textId_ = 0; textId_ < texts_.size(); textId_++) {
			bool wasInternalCreatedInLastExtension = false;
			NodeKey lastCreated;
			wasInternalCreatedInLastExtension = false;
			extension_ = 0;
			Point workingPoint = getNullPoint();
			for (phase_ = 1; phase_ <= texts_[textId_].size(); phase_++) { // must add text[phase-1] to each suffix		
				wasInternalCreatedInLastExtension = false;
				Alphabet const &character = texts_[textId_][phase_ - 1];
				//std::cout << "phase: " << phase_ << " char: " << character << "\n";
				for (; extension_ < phase_; extension_++) {
					//std::cout << "extension: " << extension_;
					if (extension_ + 1 == phase_) { // one last char left; deal with root only
						auto result = rootContinuesWith(character);
						if (!result.second) {
							//std::cout << "  Rule A: " << "\n";
							createBranchAtRoot(character);
						} else {
							//std::cout << "  Rule B: " << "\n";
							workingPoint = result.first;
							break;
						}
					} else {
						// current suffix: [extension_, phase_)
						auto result = continuesWith(workingPoint, character);
						if (!result.second) { // Rule2
											  //std::cout << "  Rule A': " << "\n";
							NodeKey branchNode = createBranch(workingPoint, character);
							if (wasInternalCreatedInLastExtension) {
								//std::cout << "setting suffix link from: " << lastCreated.id_ << "to : " <<  branchNode.id_ << "\n";
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
			}
			// currently, phase is length + 1 (due to the for loop)
			// because the length of a link is calculated based on phase_, this needs correction
			phase_ = texts_[textId_].length();
		}
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	void
		suffix_tree<Alphabet, Graph>::findAllCommmonSubstrings() {
		// for each and all of leaves, mark all ancestors (internal nodes) of it as
		// substring of a text[i], where i is the textId_ of that leaf node
		// as internal nodes can be a substring of multiple text, each key of internal node
		// will be mapped to a set of textIds
		for (auto iter = leafNodes_.begin(); iter != leafNodes_.end(); ++iter) {
			size_t textId = iter->textId_;
			NodeKey key = *iter;
			while (true) {
				Node const &node = getNode(key);
				if (node.type_ == NodeType::Root) {
					break;
				}
				auto result = internalNodeToTextId_[key].insert(textId);
				// its parent is already marked, so there is no need to go up further
				if (result.second == false) {
					break;
				}
				key = node.parent_;
			}
		}
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	std::pair<typename suffix_tree<Alphabet, Graph>::Point, bool>
		suffix_tree<Alphabet, Graph>::findPatternPoint(abstract_string<Alphabet> const &pattern) const {
		NodeKey srcNode;
		NodeKey destNode = getRootKey();
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

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	void
		suffix_tree<Alphabet, Graph>::findReachableLeaves(Point const &point, vector<NodeKey> &result) const {
		findReachableLeaves(point.dest_, result);
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	void
		suffix_tree<Alphabet, Graph>::findReachableLeaves(NodeKey key, vector<NodeKey> &result) const {
		if (getNode(key).type_ == NodeType::Leaf) {
			//std::cout << "findReachableLeaves adding: " << node << "\n";
			result.push_back(key);
		} else {
			auto pairs = graph_.get_vertex_edge_pairs_from(key);
			for (auto iter = pairs.begin(); iter != pairs.end(); ++iter) {
				findReachableLeaves(iter->first.key_, result);
			}
		}
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	void suffix_tree<Alphabet, Graph>::printPoint(Point const &point) const {
		readToPoint(point).print();
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	void suffix_tree<Alphabet, Graph>::printNode(NodeKey key) const {
		if (getNode(key).type_ == NodeType::Internal) {
			std::cout << "Internal, substring length is: " << getNode(key).substringLength_ << " ";
		}
		readToPoint(createPoint(key)).println();
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	typename suffix_tree<Alphabet, Graph>::NodeKey
		suffix_tree<Alphabet, Graph>::getNullKey() const {
		return NodeKey(1000, 0);
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	typename suffix_tree<Alphabet, Graph>::NodeKey
		suffix_tree<Alphabet, Graph>::getRootKey() const {
		return NodeKey(2000, 0);
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	typename suffix_tree<Alphabet, Graph>::NodeKey
		suffix_tree<Alphabet, Graph>::getNewInternalKey() {
		return NodeKey(3000, internalNodeId_++);
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	typename suffix_tree<Alphabet, Graph>::Point
		suffix_tree<Alphabet, Graph>::getNullPoint() const {
		return Point(getNullKey(), getNullKey(), 1);
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	suffix_tree<Alphabet, Graph>::NodeKey::NodeKey(size_t textId, size_t id) :
		textId_(textId), id_(id) {}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	suffix_tree<Alphabet, Graph>::NodeKey::NodeKey() : textId_(0), id_(0) {}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	bool suffix_tree<Alphabet, Graph>::NodeKey::operator==(NodeKey const &rhs) const {
		return textId_ == rhs.textId_ && id_ == rhs.id_;
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	bool suffix_tree<Alphabet, Graph>::NodeKey::operator!=(NodeKey const &rhs) const {
		return !(*this == rhs);
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	bool suffix_tree<Alphabet, Graph>::NodeKey::operator<(NodeKey const &rhs) const {
		return textId_ < rhs.textId_ ||
			(textId_ == rhs.textId_ && id_ < rhs.id_);
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	typename suffix_tree<Alphabet, Graph>::NodeKey &
		suffix_tree<Alphabet, Graph>::NodeKey::operator=(NodeKey const &rhs) {
		if (this != &rhs) {
			textId_ = rhs.textId_;
			id_ = rhs.id_;
		}
		return *this;
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	suffix_tree<Alphabet, Graph>::Node::Node(NodeKey key, NodeType type, NodeKey parent, NodeKey suffixLink) :
		key_(key), type_(type), parent_(parent), suffixLink_(suffixLink), substringLength_(0) {
		objectCount_++;
		//std::cout << "objectCount_ is: " << objectCount_ << "\n";
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	suffix_tree<Alphabet, Graph>::Node::Node(Node const &rhs) : key_(rhs.key_), type_(rhs.type_),
		parent_(rhs.parent_), suffixLink_(rhs.suffixLink_), substringLength_(rhs.substringLength_) {
		objectCount_++;
		//std::cout << "objectCount_ is: " << objectCount_ << "\n";
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	suffix_tree<Alphabet, Graph>::Node::~Node() {
		objectCount_--;
		if (objectCount_ < 0) {
			std::cout << "objectCount_ is: " << objectCount_ << "\n";
		}
		//std::cout << "objectCount_ is: " << objectCount_ << "\n";
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	void suffix_tree<Alphabet, Graph>::Node::countThisObject() {
		std::cout << "Node current count is: \"" << objectCount_ << "\": ";
		if (objectCount_ == 0) {
			std::cout << "O.K\n\n";
		} else {
			std::cout << "memory leak!!!\n";
			exit(0);
		}
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	int suffix_tree<Alphabet, Graph>::Node::objectCount_ = 0;

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	bool suffix_tree<Alphabet, Graph>::Node::operator==(Node const &rhs) const {
		return key_ == rhs.key_;
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	bool suffix_tree<Alphabet, Graph>::Node::operator<(Node const &rhs) const {
		return key_ < rhs.key_;
	}
	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	typename suffix_tree<Alphabet, Graph>::NodeKey
		suffix_tree<Alphabet, Graph>::NodeExtractKey::operator()(Node const &node) const {
		return node.key_;
	};

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	suffix_tree<Alphabet, Graph>::Link::Link(size_t textId, Alphabet head, size_t startIndex, size_t endIndex) :
		textId_(textId), head_(head), startIndex_(startIndex), endIndex_(endIndex) {
		if (startIndex >= endIndex && endIndex != 0) {
			std::cout << "Link error:  start: " << startIndex << " end: " << endIndex << "\n";
			exit(0);
		}
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	suffix_tree<Alphabet, Graph>::Link::Link() :
		textId_(0), head_(0), startIndex_(0), endIndex_(0) {}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	suffix_tree<Alphabet, Graph>::Point::Point(NodeKey src, NodeKey dest, size_t endIndex) :
		src_(src), dest_(dest), endIndex_(endIndex) {
		if (endIndex == 0) {
			std::cout << "Point ctor creating leaf\n";
			exit(0);
		}
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	typename suffix_tree<Alphabet, Graph>::Point &
		suffix_tree<Alphabet, Graph>::Point::operator=(Point const &rhs) {
		src_ = rhs.src_;
		dest_ = rhs.dest_;
		endIndex_ = rhs.endIndex_;
		return *this;
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	bool suffix_tree<Alphabet, Graph>::Point::operator==(Point const &rhs) const {
		return src_ == rhs.src_ && dest_ == rhs.dest_ && endIndex_ == rhs.endIndex_;
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	size_t
		suffix_tree<Alphabet, Graph>::getLength(Link const &link) const {
		if (link.endIndex_ != 0) {
			return link.endIndex_ - link.startIndex_;
		} else {
			if (textId_ == link.textId_) { // still working on the text
				return phase_ - link.startIndex_;
			} else { // done with the text
				return texts_[link.textId_].length() - link.startIndex_;
			}
		}
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	typename suffix_tree<Alphabet, Graph>::Node
		suffix_tree<Alphabet, Graph>::makeNewLeaf(NodeKey parentId) {
		leafNodes_.push_back(NodeKey(textId_, extension_));
		return Node(NodeKey(textId_, extension_), NodeType::Leaf, parentId, getNullKey());
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	typename suffix_tree<Alphabet, Graph>::Node
		suffix_tree<Alphabet, Graph>::makeNewInternal(NodeKey parentId) {
		return Node(getNewInternalKey(), NodeType::Internal, parentId, getNullKey());
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	typename suffix_tree<Alphabet, Graph>::Point
		suffix_tree<Alphabet, Graph>::followPathDown(size_t textId,
											  NodeKey src, size_t startIndex, size_t endIndex) const {
		vector<std::pair<Node, Link>> pairs = graph_.get_vertex_edge_pairs_from(src);
		for (auto iter = pairs.begin(); iter != pairs.end(); ++iter) {
			if (iter->second.head_ == texts_[textId][startIndex]) {
				size_t linkLength = getLength(iter->second);
				size_t linkStartIndex = iter->second.startIndex_;
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

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	abstract_string<Alphabet>
		suffix_tree<Alphabet, Graph>::readToPoint(Point const &point) const {
		Link const &link = getLink(point);
		if (!(link.startIndex_ < point.endIndex_)) {
			std::cout << "suffix_tree readToPoint error\n";
			std::cout << "link.startIndex_: " << link.startIndex_ << " link.endIndex_ " << link.endIndex_ <<
				" point.endIndex_ " << point.endIndex_ << "\n";
			exit(0);
		}
		size_t textId = link.textId_;
		abstract_string<Alphabet> subString = texts_[textId].substr(link.startIndex_, point.endIndex_);
		//std::cout << "start was: " << link.startIndex_ << " end: " << endIndex << " subs was: ";
		//subString.print();
		if (getNode(point.src_).type_ == NodeType::Root) {
			return subString;
		} else {
			return readToPoint(createPoint(point.src_)) + subString;
		}
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	abstract_string<Alphabet>
		suffix_tree<Alphabet, Graph>::readToNode(NodeKey const &key) const {
		return readToPoint(createPoint(key));
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	typename suffix_tree<Alphabet, Graph>::Link const &
		suffix_tree<Alphabet, Graph>::getLink(NodeKey src, NodeKey dest) const {
		Link const &link = graph_.get_edge(src, dest);
		if (!(link.startIndex_ < link.endIndex_) && link.endIndex_!= 0) {
			//if (true) {
			std::cout << "getLink error\n";
			std::cout << "startIndex_: " << link.startIndex_ << " endIndex_ " << link.endIndex_ << "\n";
			exit(0);
		}
		return link;
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	typename suffix_tree<Alphabet, Graph>::Link const &
		suffix_tree<Alphabet, Graph>::getLink(Point const &point) const {
		return getLink(point.src_, point.dest_);
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	typename suffix_tree<Alphabet, Graph>::Link const &
		suffix_tree<Alphabet, Graph>::getParenetLink(NodeKey key) const {
		return graph_.get_edge(getNode(key).parent_, key);
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	typename suffix_tree<Alphabet, Graph>::Node &
		suffix_tree<Alphabet, Graph>::getNode(NodeKey key) {
		return graph_.get_vertex(key);
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	typename suffix_tree<Alphabet, Graph>::Node const &
		suffix_tree<Alphabet, Graph>::getNode(NodeKey key) const {
		return graph_.get_vertex(key);
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	bool
		suffix_tree<Alphabet, Graph>::isLeaf(NodeKey const &key) const {
		return key.textId_ < texts_.size();
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	typename suffix_tree<Alphabet, Graph>::Point
		suffix_tree<Alphabet, Graph>::createPoint(NodeKey dest) const {
		if (getNode(dest).type_ == NodeType::Root) {
			std::cout << "suffix_tree createPoint error\n";
			exit(0);
		}
		Link const &link = getParenetLink(dest);
		Node const &node = getNode(dest);
		size_t endIndex = link.endIndex_;
		if (link.endIndex_ == 0) {
			if (link.textId_ == textId_) { // currently operaing on this text
				endIndex = phase_;
			} else { //this text has been done
				endIndex = texts_[link.textId_].size();
			}
		}
		return Point(node.parent_, dest, endIndex);
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	typename suffix_tree<Alphabet, Graph>::Point
		suffix_tree<Alphabet, Graph>::followSuffixLink(NodeKey key) const {
		if (getNode(key).suffixLink_ !=  getNullKey()) {
			//std::cout << "follow suffix\n";
			return createPoint(getNode(key).suffixLink_);
		} else {
			NodeKey parentNode = getNode(key).parent_;
			Link const &link = getLink(parentNode, key);
			//std::cout << "link start: " << link.startIndex_ << " link end: " << link.endIndex_ << "\n";
			if (getNode(parentNode).suffixLink_ != getNullKey()) {
				//std::cout << "follow suffix2\n";
				NodeKey linkOfParent = getNode(parentNode).suffixLink_;
				return followPathDown(link.textId_, linkOfParent, link.startIndex_, link.endIndex_);
			} else {
				if (parentNode == getRootKey()) {
					if (link.startIndex_ + 1 < link.endIndex_) {
						//std::cout << "follow suffix3\n";
						return followPathDown(link.textId_, getRootKey(), link.startIndex_ + 1, link.endIndex_);
					} else { // internal node of length 1 cannot have suffix link
							 //std::cout << "follow suffix4\n";
						return getNullPoint();
					}
				} else {
					NodeKey grandPa = getNode(parentNode).parent_;
					//std::cout << "follow suffix5, parent was: " << parentNode << " grandpd was: " << grandPa << "\n";		
					// parent is direct child of root. 
					return followPathDown(link.textId_, getRootKey(), link.startIndex_, link.endIndex_);
				}
			}
		}
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	bool suffix_tree<Alphabet, Graph>::isNode(Point const &point) const {
		return getLink(point.src_, point.dest_).endIndex_ == point.endIndex_;
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	std::pair<typename suffix_tree<Alphabet, Graph>::Point, bool>
		suffix_tree<Alphabet, Graph>::continuesWith(Point const &point, Alphabet const &character) const {
		//std::cout << "continue with : " << character << " \n";
		//readToPoint(point).print();
		if (isNode(point)) {
			return nodeContinuesWith(point.dest_, character);
		} else {
			//std::cout << "continue with4\n ";
			Link const &link = getLink(point);
			if (texts_[link.textId_][point.endIndex_] == character) {
				return std::make_pair(Point(point.src_, point.dest_, point.endIndex_ + 1), true);
			} else {
				return std::make_pair(getNullPoint(), false);
			}
		}
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	std::pair<typename suffix_tree<Alphabet, Graph>::Point, bool>
		suffix_tree<Alphabet, Graph>::nodeContinuesWith(NodeKey key, Alphabet const &character) const {
		//std::cout << "continue with2 : ";
		if (isLeaf(key)) { // wrong call
			std::cout << "continuesWith error1\n";
		}
		auto outLinks = graph_.get_vertex_edge_pairs_from(key);
		if (!(outLinks.size() > 1) && key != getRootKey()) {
			std::cout << "continuesWith error2\n";
			exit(0);
		}
		for (auto iter = outLinks.begin(); iter != outLinks.end(); ++iter) {
			if (iter->second.head_ == character) {
				//std::cout << "continue with3 : \n";
				return std::make_pair(Point(key, iter->first.key_,
									  getLink(key, iter->first.key_).startIndex_ + 1), true);
			}
		}
		return std::make_pair(getNullPoint(), false);
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	std::pair<typename suffix_tree<Alphabet, Graph>::Point, bool>
		suffix_tree<Alphabet, Graph>::rootContinuesWith(Alphabet const &character) const {
		return nodeContinuesWith(getRootKey(), character);
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	typename suffix_tree<Alphabet, Graph>::NodeKey
		suffix_tree<Alphabet, Graph>::createBranch(Point const &point, Alphabet const &character) {
		if (!isNode(point)) {
			NodeKey parentKey = point.src_;
			NodeKey childKey = point.dest_;
			//std::cout << "point src was: " << point.src_ << " point dest was: " << point.dest_<< "\n";
			//std::cout << "creating InternalId: " << newInternalId << "\n";
			Node newInternal = makeNewInternal(parentKey);
			NodeKey newInternalKey = newInternal.key_;
			getNode(childKey).parent_ = newInternalKey;
			Node newLeaf = makeNewLeaf(newInternalKey);
			Link oldLink(getLink(point));
			Link newLink1(oldLink.textId_, texts_[oldLink.textId_][point.endIndex_],
						  point.endIndex_, oldLink.endIndex_); // internal -> child
			Link newLink2(textId_, character, phase_ - 1, 0); // internal -> newLeaf
															  //std::cout << "changing old link end to: " << point.endIndex_ <<"\n";
			oldLink.endIndex_ = point.endIndex_;
			//std::cout << "old link start: " << oldLink.startIndex_ << "end " << oldLink.endIndex_ << "\n";
			// manage substringLength property
			size_t substringLengthOfParent = getNode(parentKey).substringLength_;
			newInternal.substringLength_ = substringLengthOfParent + getLength(oldLink);
			// graph operations must be performed last
			graph_.insert_vertex(newInternal);
			graph_.insert_vertex(newLeaf);
			graph_.remove_edge(parentKey, childKey);
			insertEdge(parentKey, newInternalKey, oldLink);
			insertEdge(newInternalKey, childKey, newLink1);
			insertEdge(newInternalKey, newLeaf.key_, newLink2);
			return newInternalKey;
		} else {
			NodeKey parentKey = getNode(point.dest_).key_;
			Node newLeaf = makeNewLeaf(parentKey);
			Link newLink(textId_, character, phase_ - 1, 0); // internal -> newLeaf
			graph_.insert_vertex(newLeaf);
			insertEdge(parentKey, newLeaf.key_, newLink);
			return parentKey;
		}
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	void suffix_tree<Alphabet, Graph>::createBranchAtRoot(Alphabet const &character) {
		Node newLeaf = makeNewLeaf(getRootKey());
		Link newLink(textId_, character, phase_ - 1, 0);
		graph_.insert_vertex(newLeaf);
		insertEdge(getRootKey(), newLeaf.key_, newLink);
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	void suffix_tree<Alphabet, Graph>::printSuffix(NodeKey leaf) const {
		std::cout << "suffix textId: " << leaf.textId_ << " starting index: " << leaf.id_ << " ";
		readToPoint(createPoint(leaf)).println();
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	void suffix_tree<Alphabet, Graph>::printAllSuffix() const {
		for (auto iter = leafNodes_.begin(); iter != leafNodes_.end(); ++iter) {
			printSuffix(*iter);
		}
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	void suffix_tree<Alphabet, Graph>::printAllCommonSubstring() const {
		for (auto iter = internalNodeToTextId_.begin(); iter != internalNodeToTextId_.end(); ++iter) {
			if (iter->second.size() == texts_.size()) {
				printNode(iter->first);
			}
		}
	}

	template <typename Alphabet, template <typename V, typename E, typename K, typename ExtractKey> class Graph>
	void suffix_tree<Alphabet, Graph>::insertEdge(NodeKey src, NodeKey dest, Link const &link) {
		graph_.insert_edge(src, dest, link);
		//std::cout << "inserting edge src: " << src << " dest: " << dest << " link start: " << link.startIndex_ <<
		//" link end: " << link.endIndex_ << "\n";
	}

}

template <typename Alphabet,
	template <typename V, typename E, typename K, typename ExtractKey> class Graph>
void lib_calvin_string::suffixTreeMatching(lib_calvin::abstract_string<Alphabet> const &text,
	lib_calvin::abstract_string<Alphabet> const &pattern,
	std::vector<size_t> &result) {

	lib_calvin::suffix_tree<Alphabet, Graph> tree(text);
	// return value of find_pattern is (textId, startIndex) pair
	tree.build();
	auto temp = tree.find_pattern(pattern);
	for (auto iter = temp.begin(); iter != temp.end(); ++iter) {
		result.push_back(iter->second);
	}
}


#endif