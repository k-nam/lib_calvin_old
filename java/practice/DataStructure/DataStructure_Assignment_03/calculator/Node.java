

package calculator;


public class Node {
    
    private double operand;
    private char item='d';
    private int status;    // if it is a char   0,   if it is a double   1,
    private Node next;
    
    public Node (char inputItem) {
        item=inputItem;
        status=0;
    }
    
    public Node (double inputOperand){
        operand=inputOperand;
        status=1;
    }
    
    public void setNext(Node inputNode){
        next=inputNode;
    }
    
    public Node getNext (){
        return next;
    }
    
    public char getItem(){
        return (item);
    }
    
    public double getOperand(){
        return(operand);
    }
    
    public int status(){
        return (status);
    }
    
}
