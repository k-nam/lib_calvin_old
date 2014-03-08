package patternMatching;

public class AVL_Tree  {
	
	private TreeNode root;
	private int size;
	
	
	public AVL_Tree( ){
		root=null;
		size=0;
	}
	
	public void setRoot(TreeNode newRoot){
		root=newRoot;
		newRoot.setParent(null);
	}



	public void insert(PieceOfString newItem) {
		size++;
		
		if (root==null){
			ListOfPieceOfString newList;
			newList=new ListOfPieceOfString(newItem);
			TreeNode newRoot;
			newRoot=new TreeNode(this, null, newList); // the parent of root node is always null!!!
			root=newRoot;
			return;
		}
		root.insert(newItem);
		
		return;
		
	}







	public int getLength() {
	
		return (size);
	}



	public ListOfPieceOfString retrieve(String searchKey) {
		if (root==null){
			return(null);
		}
		else{
			return(root.retrieve(searchKey));
		}
	}
	
	

}












