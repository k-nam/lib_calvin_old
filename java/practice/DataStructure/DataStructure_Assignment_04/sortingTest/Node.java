package sortingTest;

/**
 * This Node will contain only one integer
 * @author Calvin Caulfield
 *
 */
public class Node {
	private Node next;
	private int number;
	
	public Node(int inputNumber){
		number=inputNumber;
		next=null;
	}
	
	public void setNext(Node nextNode){
		next=nextNode;
	}
	
	public Node getNext(){
		return(next);
		
	}
	
	public int getNumber(){
		return(number);
	}

}
