package sortingTest;


/**
 * Priority Queue for radix sort (specifically, bucket sort for ten buckets)
 * It should be stable sorting, so Queue is needed
 * It process only integers.
 * @author Calvin Caulfield
 *
 */
public class Queue {
	private Node first;
	private Node last;
	private int size;
	
	public Queue(){
		first=null;
		last=null;
		size=0;
	}
	
	public void enQueue(int number){
		if (last==null){
			Node a;
			a=new Node(number);
			first=last=a;
			size++;
			return;
		}
		Node newlast=new Node(number);
		last.setNext(newlast);
		last=newlast;
		size++;
	}
	
	public void enQueue(Queue nextQueue){
		if ( nextQueue.first==null){
			return;
		}
		if (this.first==null){
			first=nextQueue.first;
			last=nextQueue.last;
			size+=nextQueue.size;
			return;
		}
		last.setNext(nextQueue.first);
		last=nextQueue.last;
		size+=nextQueue.size;
	}
	
	// should not be called when this queue is empty
	public int deQueue(){
		if (first==null){
			System.out.println("error in Queue.deQueue, queue empty");
			return(0);
		}
		
		Node newFirst;
		newFirst=first.getNext();
		if(newFirst==null){
			last=null;
		}
		int temp=first.getNumber();
		first=newFirst;
		size--;
		return(temp);
		
		
	}
	
	
	

}
























