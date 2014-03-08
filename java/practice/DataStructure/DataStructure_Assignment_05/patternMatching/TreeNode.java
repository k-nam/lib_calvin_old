package patternMatching;


public class TreeNode {
		
	private AVL_Tree tree;
	private  ListOfPieceOfString item;
	private TreeNode parent;
	private TreeNode leftChild;  //left
	private TreeNode rightChild;  //right
	private int height;
	
	
	
	public TreeNode (AVL_Tree inputTree, TreeNode parentNode, ListOfPieceOfString newItem){
		tree=inputTree;
		parent=parentNode;
		item=newItem;
		leftChild=null;
		rightChild=null;
		height=1;
		
	
	}
	
	//returns true is this TreeNode is a leftChild of the parent
	private boolean isLeft(){
		if(parent.getLeft()==null){
			return(false);
		}
		if (parent.getLeft().equals(this)==true){
			return(true);
		}
		else
			return(false);
	}
	
	
	
	public int getHeight(){
		return(height);
	
	}	
	
	public int getLeftHeight(){
		if (leftChild==null){
			return(0);
		}
		return(leftChild.getHeight());
	}
	
	public int getRightHeight(){
		if (rightChild==null){
			return(0);
		}
		return(rightChild.getHeight());
	}
	
	
	public void resetHeight(){
		if(this.getLeftHeight()>=this.getRightHeight()){
			height=this.getLeftHeight()+1;
		}
		else{
			height=this.getRightHeight()+1;
		}
		if(parent==null){
			return;
		}
		parent.resetHeight();
	}
	
	
	
	
	
	public void turnLeft(){
		if (rightChild==null){
			System.out.println("Internal error in TreeNode.turnLeft()");
			return;
		}
		
		TreeNode originalParent;
		TreeNode originalRightChild;
		TreeNode originalRight_Left;
		originalParent=parent;
		originalRightChild=rightChild;
		originalRight_Left=rightChild.getLeft();
		
		//exception if this is root
		if(parent==null){
			tree.setRoot(originalRightChild);
			
			originalRightChild.setLeft(this);
			this.setRight(originalRight_Left);
			return;
		}
		
		if(this.isLeft()==true){
			originalParent.setLeft(originalRightChild);
		}
		else{
			originalParent.setRight(originalRightChild);
		}	
		originalRightChild.setLeft(this);
		this.setRight(originalRight_Left);
		
		return;		
	}
	
	
	public void turnRight(){
		if (leftChild==null){
			System.out.println("Internal error in TreeNode.turnRight()");
			return;
		}
		
		TreeNode originalParent;
		TreeNode originalLeftChild;
		TreeNode originalLeft_Right;
		originalParent=parent;
		originalLeftChild=leftChild;
		originalLeft_Right=leftChild.getRight();
		
		//exception if this is root
		if(parent==null){
			tree.setRoot(originalLeftChild);
			
			originalLeftChild.setRight(this);
			this.setLeft(originalLeft_Right);
			return;
		}
		
		if(this.isLeft()==true){
			originalParent.setLeft(originalLeftChild);
		}
		else{
			originalParent.setRight(originalLeftChild);
		}	
		originalLeftChild.setRight(this);
		this.setLeft(originalLeft_Right);
		
		return;	
		
	}
	
	
	
	
	
	
	
	
	
	
	

	
	
	
	
	
	
	

	public int compareTo(TreeNode comparingTreeNode) {
		
		return (item.getKey().compareTo(comparingTreeNode.item.getKey()));
	}
	
	public void setItem(ListOfPieceOfString newItem){
		item=newItem;
	}
	
	public ListOfPieceOfString getItem (){
		return(item);
	}
	
	public void setParent(TreeNode newParent){
		parent=newParent;
	}
	
	public void setLeft(TreeNode newLeft){
		
		leftChild=newLeft;
		if(newLeft!=null){
			newLeft.setParent(this);
		}
		this.resetHeight();
		
	}
	
	public void setRight(TreeNode newRight){
		
		rightChild=newRight;
		if(newRight!=null){
			newRight.setParent(this);
		}
		this.resetHeight();
		
	}
	
	public TreeNode getLeft (){
		return(leftChild);
	}
	
	public TreeNode getRight(){
		return(rightChild);
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	public void maintainBalance(){
		//When left subtree is too heavy
		if (this.getLeftHeight()>this.getRightHeight()+1){
			//When left subtree is left-heavy
			if ( leftChild.getLeftHeight()>leftChild.getRightHeight()){
				this.turnRight();				
			}
			//When left subtree is right-heavy (double turn)
			else{
				leftChild.turnLeft();
				this.turnRight();
			}
		}
		
		//When right subtree is too heavy
		if (this.getLeftHeight()+1<this.getRightHeight()){
			//when right subtree is right-heavy
			if (rightChild.getLeftHeight()<rightChild.getRightHeight()){
				this.turnLeft();				
			}
			//when right subtree is left-heavy (double turn)
			else{
				rightChild.turnRight();
				this.turnLeft();
			}
		}
		
		
		
		
		if(parent==null){
			return;
		}
		parent.maintainBalance();
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	public void insert (PieceOfString newString){
		
		// If the input String is this Node's string, just add the PieceOfString 
		// to this Node's list
		int temp=item.getKey().compareTo(newString.getKey());
		
		if(temp==0 ){
			item.insert(newString);
			return;
		}
		
		// When this Node comes after the input string ->go to left
		if(temp>0){
			if(leftChild==null){
				ListOfPieceOfString newList;
				newList=new ListOfPieceOfString(newString);
				TreeNode newLeft;
				newLeft=new TreeNode(tree, this, newList);
				this.setLeft(newLeft);

				this.maintainBalance();
				return;
			}
			leftChild.insert(newString);
		}
		
		// When this Node is antecedent to the input string -> go to right
		if(temp<0){
			if(rightChild==null){
				ListOfPieceOfString newList;
				newList=new ListOfPieceOfString(newString);
				TreeNode newRight;
				newRight=new TreeNode(tree, this, newList);
				this.setRight(newRight);
			
				this.maintainBalance();
				return;
			}
			rightChild.insert(newString);
			
		}
	}
	
		
		
	
	public ListOfPieceOfString retrieve (String inputString){
		int temp=item.getKey().compareTo(inputString);
		if( temp==0){
			return(item);
		}
		
		if(temp>0){
			if (leftChild==null){
				return(null);
			}
			return( leftChild.retrieve(inputString) );
		}
		
		if( temp<0){
			if (rightChild==null){
				return(null);
			}
			return( rightChild.retrieve(inputString) );
		}
		
		return(null);
	
	}
				
	
	
	
	
	
		
		
		
	
		
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

	
	

}
