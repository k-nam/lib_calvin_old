
package calculator;

public class Stack {
    
private Node top;

    
    public Stack () {
        top=new Node ('p');
        top.setNext (null);
    }
    
    
    public boolean isEmpty(){
        if (top.getNext ()==null)
            return(true);
        else return(false);
    }
    
    public void push (char inputChar){
        Node temp;
        temp=new Node(inputChar);
        Node curr;
        curr=top.getNext ();
        top.setNext (temp);
        temp.setNext (curr);       
    }
    
    public void push (double inputDouble){
        Node temp;
        temp=new Node(inputDouble);
        Node curr;
        curr=top.getNext ();
        top.setNext (temp);
        temp.setNext (curr);       
    }
    
    
    public char pop (){
        if(top.getNext ()==null){
            System.out.println("Internal error in pop()");
            return('a');
        }
            
        
        Node temp;
        temp=top.getNext ();
        Node next;
        next=temp.getNext ();
        top.setNext (next);
        
        
        return(temp.getItem());
    
    }
    
    //return the top char  return d if it is a double
    public char peek(){
        if (top.getNext ()==null)
            return('t');
        return(top.getNext ().getItem ());
        
    }
    
    //return the previous char  return d if it is a double
    public char peek2(){
        if (top.getNext ()==null)
            return('t');
        if (top.getNext ().getNext ()==null)
            return('t');
        return(top.getNext ().getNext ().getItem ());
    }
    
    public char peek3(){
        if (top.getNext ()==null)
            return('t');
        if (top.getNext ().getNext ()==null)
            return('t');
        if (top.getNext ().getNext().getNext ()==null)
            return('t');
        return(top.getNext ().getNext ().getNext ().getItem ());
    }
    
    public double peekOperand(){
        if (top.getNext ()==null){
            System.out.println("error in peekOperand"); 
            return(0);
             
        }
           
        return(top.getNext ().getOperand ());
    }
    
    public double peekOperand2(){
        if (top.getNext ()==null){
            System.out.println("error in peekOperand"); 
            return(0);
        }
        if (top.getNext ().getNext ()==null){
            System.out.println("error in peekOperand"); 
            return(0);
        }
        
        return(top.getNext ().getNext().getOperand ());
    }
   
}
