
#ifdef USE_HEAD_NODE
#include "bin_plus_tree.h"
#define RB_TREE_BASE RbPTreeWithHead
#define BIN_TREE_BASE BinPlusTree
#else
#include "bin_tree.h"
#define RB_TREE_BASE RbTree
#define BIN_TREE_BASE BinTree
#endif

namespace lib_calvin_container
{

	template <typename T, typename K = T, typename Comp = std::less<K>, typename ExtractKey = Identity<T>>
	class RB_TREE_BASE : public BIN_TREE_BASE<T, K, Comp, ExtractKey> {
	public:
		typedef BIN_TREE_BASE<T, K, Comp, ExtractKey> BaseTreeType;
		typedef typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node BaseNodeType;
		typedef typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::RbColor RbColor;

		typedef typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Direction Direction;

		class Node : public BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node {
		public:
			using BIN_TREE_BASE<T, K, Comp, ExtractKey>::Node::color_;
			typedef typename  BIN_TREE_BASE<T, K, Comp, ExtractKey>::RbColor RbColor;
			
			// Node is created with its color set to red
			Node(T const &key);
			Node(T &&key);
			Node(T const &key, RbColor color);
			~Node();
			RbColor getColor() const { return color_; }
			void setColor(RbColor newColor) { color_ = newColor; }
			Node *getParent() const {
				return static_cast<Node *>(BaseNodeType::getParent());
			}
			Node *getLeftChild() const {
				return static_cast<Node *>(BaseNodeType::getLeftChild());
			}
			Node *getRightChild() const {
				return static_cast<Node *>(BaseNodeType::getRightChild());
			}
			Node *getChild(Direction direction) const {
				return static_cast<Node *>(BaseNodeType::getChild(direction));
			}
		private:
			
		};


	public:
		RB_TREE_BASE<T, K, Comp, ExtractKey>() : BIN_TREE_BASE<T, K, Comp, ExtractKey>() { }
		RB_TREE_BASE<T, K, Comp, ExtractKey>(RB_TREE_BASE<T, K, Comp, ExtractKey> const &rhs) :
			BIN_TREE_BASE<T, K, Comp, ExtractKey>() {
			copyFrom(rhs);
			//std::cout << "copy con: RB_TREE_BASE\n";
		}
		RB_TREE_BASE<T, K, Comp, ExtractKey>(RB_TREE_BASE<T, K, Comp, ExtractKey> &&rhs) :
			BIN_TREE_BASE<T, K, Comp, ExtractKey>() {
			//std::cout << "rb move ctor\n";
			BIN_TREE_BASE<T, K, Comp, ExtractKey>::swap(rhs);
		}
		RB_TREE_BASE<T, K, Comp, ExtractKey> &operator=(
			RB_TREE_BASE<T, K, Comp, ExtractKey> const &rhs) { 
			return static_cast<RB_TREE_BASE<T, K, Comp, ExtractKey> &>(BIN_TREE_BASE<T, K, Comp, ExtractKey>::operator=(rhs)); }
		RB_TREE_BASE<T, K, Comp, ExtractKey> &operator=(
			RB_TREE_BASE<T, K, Comp, ExtractKey> &&rhs) { 
			return static_cast<RB_TREE_BASE<T, K, Comp, ExtractKey> &>(BIN_TREE_BASE<T, K, Comp, ExtractKey>::operator=(std::forward<RB_TREE_BASE>(rhs))); }
		~RB_TREE_BASE() { }
		typedef typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::iterator iterator;
		std::pair<iterator, bool> insert(T const &);
		std::pair<iterator, bool> insert(T &&);
		template <typename InputIterator>
		void insert(InputIterator beg, InputIterator end);
		size_t erase(K const &);

	public:
		using BIN_TREE_BASE<T, K, Comp, ExtractKey>::empty;
	private:
		using BIN_TREE_BASE<T, K, Comp, ExtractKey>::root_;
		using BIN_TREE_BASE<T, K, Comp, ExtractKey>::copyFrom;

		template <typename T1>
		std::pair<typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::iterator, bool> insert_(T1 &&elem);
		BaseNodeType *makeNewNode(T const &elem) const override;
		BaseNodeType *makeNewNode(T &&elem) const override;
		BaseNodeType *makeNewNode(BaseNodeType const *rhs) const override;
		Node *rotate(Node *node, Direction direction);
		Node *rbRotate(Node *node, Direction direction);

		void rbInsertFix(Node *);
		void rbDeleteFix(Node *);

		// For debugging
		size_t RbTreeCheck(Node const *) const;
	};

	//-------------------------- Node public methods --------------------------//

	template <typename T, typename K, typename Comp, typename ExtractKey>
	RB_TREE_BASE<T, K, Comp, ExtractKey>::Node::Node(T const &key) :
		BaseNodeType(key) { 
		color_ = RbColor::Red;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	RB_TREE_BASE<T, K, Comp, ExtractKey>::Node::Node(T &&key) :
		BaseNodeType(std::forward<T>(key)) {
		color_ = RbColor::Red;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	RB_TREE_BASE<T, K, Comp, ExtractKey>::Node::Node(T const &key, RbColor color) :
		BaseNodeType(key) {
		color_ = color;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	RB_TREE_BASE<T, K, Comp, ExtractKey>::Node::~Node() {
		// std::cout << "rbnode dtored\n";
	}


	//-------------------------- RB_TREE_BASE public methods --------------------------//

	template <typename T, typename K, typename Comp, typename ExtractKey>
	std::pair<typename RB_TREE_BASE<T, K, Comp, ExtractKey>::iterator, bool>
		RB_TREE_BASE<T, K, Comp, ExtractKey>::insert(T const &elem) {
		return insert_(elem);
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	std::pair<typename RB_TREE_BASE<T, K, Comp, ExtractKey>::iterator, bool>
		RB_TREE_BASE<T, K, Comp, ExtractKey>::insert(T &&elem) {
		return insert_(std::forward<T>(elem));
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	template <typename T1>
	std::pair<typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::iterator, bool>
		RB_TREE_BASE<T, K, Comp, ExtractKey>::insert_(T1 &&elem) {
		//std::cout << "rb insert called\n";
		std::pair<BaseNodeType *, bool> result =
			BaseTreeType::insert_impl(std::forward<T1>(elem));
		BaseNodeType *insertedNode = result.first;
		bool isInserted = result.second;
		// RB_TREE_BASE maintenance
		if (isInserted == true) {
			rbInsertFix(static_cast<Node *>(insertedNode));
		}
		//RbTreeCheck(static_cast<Node *>(root_));
		return std::pair<iterator, bool>(iterator(insertedNode), isInserted);
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	size_t RB_TREE_BASE<T, K, Comp, ExtractKey>::erase(K const &elem) {
		Node *deleted = static_cast<Node *>(
			BIN_TREE_BASE<T, K, Comp, ExtractKey>::erase_impl(elem));
		if (deleted == nullptr) { // not deleted anything
			return 0;
		}
		if (deleted->getColor() == RbColor::Red) { // deleted a red node: OK
			delete deleted;
			return 1;
		}
		if (empty() == true) { // nothing is left
			delete deleted;
			return 1;
		}
		// deleted node acts as the NIL node (which is black) 
		if (!deleted->hasLeftChild() && !deleted->hasRightChild()) {
			rbDeleteFix(deleted);
		} else if (deleted->hasLeftChild()) {
			rbDeleteFix(deleted->getLeftChild());
		} else {
			rbDeleteFix(deleted->getRightChild());
		}
		//RbTreeCheck(static_cast<Node *>(root_));
		delete deleted;
		return 1;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	template <typename InputIterator>
	void RB_TREE_BASE<T, K, Comp, ExtractKey>::insert(InputIterator beg, InputIterator end)
	{
		while (beg != end) {
			this->insert(*beg);
			++beg;
		}
	}

	//--------------------------- RB_TREE_BASE private methods --------------------------//

	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename RB_TREE_BASE<T, K, Comp, ExtractKey>::BaseNodeType *
		RB_TREE_BASE<T, K, Comp, ExtractKey>::makeNewNode(T const &key) const {
		auto newNode = new Node(key);
#ifdef USE_HEAD_NODE
		BIN_TREE_BASE<T, K, Comp, ExtractKey>::prepareNode(newNode);
#endif	
		return newNode;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename RB_TREE_BASE<T, K, Comp, ExtractKey>::BaseNodeType *
		RB_TREE_BASE<T, K, Comp, ExtractKey>::makeNewNode(T &&key) const {
		auto newNode = new Node(std::move(key));
#ifdef USE_HEAD_NODE
		BIN_TREE_BASE<T, K, Comp, ExtractKey>::prepareNode(newNode);
#endif	
		return newNode;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename RB_TREE_BASE<T, K, Comp, ExtractKey>::BaseNodeType *
		RB_TREE_BASE<T, K, Comp, ExtractKey>::makeNewNode(BaseNodeType const *rhs) const {
		auto newNode = new Node(rhs->getKey(),
			static_cast<Node const *>(rhs)->getColor());
#ifdef USE_HEAD_NODE
		BIN_TREE_BASE<T, K, Comp, ExtractKey>::prepareNode(newNode);
#endif	
		return newNode;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename RB_TREE_BASE<T, K, Comp, ExtractKey>::Node *
		RB_TREE_BASE<T, K, Comp, ExtractKey>::rotate(Node *node, typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Direction direction) {
		typename BIN_TREE_BASE<T, K, Comp, ExtractKey>::Direction oppositeDirection = BaseNodeType::getOpposite(direction);
		Node *child = node->getChild(oppositeDirection);
		if (is_null<T, K, Comp, ExtractKey>(child)) {
			std::cout << "rotate error: child null\n";
			exit(0);
		}
		/* save link */
		Node *grandChild = child->getChild(direction);
		/* change root */
		if (node->isLeftInParent()) {
			node->getParent()->setLeftChild(child);
		} else {
			node->getParent()->setRightChild(child);
		}
		child->setParent(node->getParent());
		/* modify local tree */
		node->setParent(child);
		child->setChild(node, direction);
		node->setChild(grandChild, oppositeDirection);
		if (!is_null<T, K, Comp, ExtractKey>(grandChild)) {
			grandChild->setParent(node);
		}
		/* resetting global root */
		if (root_ == node) {
			root_ = child;
		}
		return child;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	typename RB_TREE_BASE<T, K, Comp, ExtractKey>::Node *
		RB_TREE_BASE<T, K, Comp, ExtractKey>::rbRotate(Node *node, Direction direction) {
		Direction oppositeDirection = BaseNodeType::getOpposite(direction);
		if (node->getColor() == RbColor::Black && node->hasChild(oppositeDirection) &&
			node->getChild(oppositeDirection)->getColor() == RbColor::Red &&
			(!node->hasChild(direction) ||
				node->getChild(direction)->getColor() == RbColor::Black)) { // OK...
		} else { // precondition not satisfied!
			std::cout << "rbRotateLeft pre error\n";
			exit(0);
		}
		node->setColor(RbColor::Red);
		node->getChild(oppositeDirection)->setColor(RbColor::Black);
		return rotate(node, direction);
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	void RB_TREE_BASE<T, K, Comp, ExtractKey>::rbInsertFix(Node *node) {
		// We are to make 'node' as black
		if (node->getColor() != RbColor::Red) { // job should have been finished before
			std::cout << "rb insert fix error: black node\n";
			exit(0);
		}
		if (node == root_) { // red node should be colored black; job done
			node->setColor(RbColor::Black);
			//std::cout << "setting root black\n";
			return;
		}
		if (node->getParent()->getColor() == RbColor::Black) { // if parent is black, job done
			return;
		}
		Node *parentNode = node->getParent();
		Node *grandPaNode = parentNode->getParent();
		Node *uncleNode = nullptr;
		if (is_null<T, K, Comp, ExtractKey>(grandPaNode)) {
			std::cout << "grandPa null\n";
			exit(0);
		}
		Direction direction = Direction::Left;
		Direction oppositeDirection = Direction::Right;
		if (parentNode == grandPaNode->getLeftChild()) {
		} else {
			direction = Direction::Right;
			oppositeDirection = Direction::Left;
		}
		uncleNode = grandPaNode->getChild(oppositeDirection);
		if (!is_null<T, K, Comp, ExtractKey>(uncleNode) &&
			uncleNode->getColor() == RbColor::Red) { /* both parent and uncle are red */
			parentNode->setColor(RbColor::Black);
			uncleNode->setColor(RbColor::Black);
			grandPaNode->setColor(RbColor::Red);
			rbInsertFix(grandPaNode); /* recursive call */
			return;
		}
		/* make suitable for red-black rotation */
		if (node == parentNode->getChild(oppositeDirection)) {
			node = parentNode;
			rotate(node, direction);
			parentNode = node->getParent();
		}
		/* red-black rotation! */
		/*parentNode->setColor(RbColor::Black); */
		/*grandPaNode->setColor(RbColor::Red); */
		rbRotate(grandPaNode, oppositeDirection);
		return;
	}

	template <typename T, typename K, typename Comp, typename ExtractKey>
	void RB_TREE_BASE<T, K, Comp, ExtractKey>::rbDeleteFix(Node *doubleBlack) {
		//std::cout << "rbDeleteFix start\n";
		if (doubleBlack == root_ || doubleBlack->getColor() == RbColor::Red) {
			doubleBlack->setColor(RbColor::Black);
			return;
		}
		Node *parentNode = doubleBlack->getParent(), *brotherNode = nullptr;
		// Symmetric code
		Direction direction = Direction::Left;
		Direction oppositeDirection = Direction::Right;
		if (doubleBlack == parentNode->getLeftChild()) { // leftChild
		} else {
			direction = Direction::Right;
			oppositeDirection = Direction::Left;
		}
		brotherNode = parentNode->getChild(oppositeDirection);
		if (is_null<T, K, Comp, ExtractKey>(brotherNode)) { /* NULL is considered black */
			rbDeleteFix(parentNode);
			return;
		} else if (brotherNode->getColor() == RbColor::Red) { /* unwanted situation -> change */
			rbRotate(parentNode, direction);
			rbDeleteFix(doubleBlack); /* redo with the same node */
			return;
		} else { /* brother is black*/
				 /* If no red nephew, go upward */
			if ((!brotherNode->hasLeftChild() ||
				brotherNode->getLeftChild()->getColor() == RbColor::Black) &&
				(!brotherNode->hasRightChild() ||
					brotherNode->getRightChild()->getColor() == RbColor::Black)) {
				brotherNode->setColor(RbColor::Red);
				rbDeleteFix(parentNode);
				return;
			} else if (brotherNode->hasChild(oppositeDirection) &&
				brotherNode->getChild(oppositeDirection)->getColor() == RbColor::Red) { /* can end! */
				brotherNode->getChild(oppositeDirection)->setColor(RbColor::Black);
				brotherNode->setColor(parentNode->getColor());
				parentNode->setColor(RbColor::Black);
				rotate(parentNode, direction);
				return;
			} else { /* brother has red left child */
				rbRotate(brotherNode, oppositeDirection);
				rbDeleteFix(doubleBlack);
				return;
			}
		}
	}

	// returns black height
	template <typename T, typename K, typename Comp, typename ExtractKey>
	size_t RB_TREE_BASE<T, K, Comp, ExtractKey>::RbTreeCheck(
		Node const *node) const {
		if (is_null<T, K, Comp, ExtractKey>(node)) {
			return 0;
		}
		// color check
		if (node->getColor() == RbColor::Red) {
			if (node->hasLeftChild() &&
				node->getLeftChild()->getColor() == RbColor::Red) {
				std::cout << "rb check: color error!\n";
				exit(0);
			}
			if (node->hasRightChild() &&
				node->getRightChild()->getColor() == RbColor::Red) {
				std::cout << "rb check: color error!\n";
				exit(0);
			}
		}
		size_t leftBlackHeight = RbTreeCheck(node->getLeftChild());
		size_t rightBlackHeight = RbTreeCheck(node->getRightChild());
		if (leftBlackHeight != rightBlackHeight) {
			std::cout << "rb check: black height error!\n";
			exit(0);
		}
		if (node->getColor() == RbColor::Red) {
			return leftBlackHeight;
		} else {
			return leftBlackHeight + 1;
		}
	}

} // lib_calvin_container


#undef RB_TREE_BASE
#undef BIN_TREE_BASE