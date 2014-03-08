/*
 * Func.java
 *
 * Created on April 14, 2007, 11:40 AM
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */

package calculator;

import java.util.regex.*;



public class Func {
    

   public Func () {
   }
    
    
   public static boolean isInfix (String inputInfix){
    if (Pattern.matches ("(?x) \\s* ([\\+\\-]?\\s*\\()* \\s*  [\\+\\-]? \\s*\\d+(\\.\\d+)? \\s*  \\)*  " +  //first operacd
                "  ( \\s* [\\^\\*\\/\\%\\+\\-]  \\s* ([\\+\\-]?\\s*\\( )* \\s*" +         //oprator ,  parenthesis
                " [\\+\\-]?\\s*\\d+(\\.\\d+)? \\s*  \\)*  )*",inputInfix)==true){           // next operand
            //System.out.println("infix expression");
            return(true);
        }
    else return(false);
    }
    
   
   public static boolean isRightParenthesis(String inputInfix){
       Stack a;
       a= new Stack();
       for (int i = 0; i < inputInfix.length ();  i++) {
           if ( inputInfix.charAt (i)=='('  ) {
               a.push ('(');
               continue;
           }
           if (inputInfix.charAt (i)==')'){
               if (a.isEmpty ()==true){
                   return (false);
               }
               a.pop ();
               continue;
           }
           
       }
       
       if (a.isEmpty ()==false){
           return(false);
       }
       //System.out.println("Right parenthesis");
       return(true);
   }
   
   
   // before processing, remove all blanks in the string !!!
   // this method converts unary operators in an Infix expression to distinguishable characters
   // - is converted to ~,   + is converted to #
   // what is the iff condition for unary operator?  It is ;
   // 1. the operator is at the beginning of the expression
   // 2. the operator is right next to an opening parenthesis
   // 3. the operator is right next to another operator
   public static String unaryProcesser (String inputInfix){
       Stack a;
       a= new Stack();
       
       String b;
       b=inputInfix.replaceAll ("\\s*","");
       
       
       StringBuffer c;
       c=new StringBuffer(b);  // I will process this a to desirable output.
       
       
       for (int i = 0; i < c.length (); i++) {
           
           
           if ( (Pattern.matches("[\\+\\-]",""+c.charAt(i))==true)
                && ( (a.isEmpty ()==true) || (Pattern.matches("[\\(\\^\\*\\/\\%\\+\\-]",""+a.peek() )))){
               if (c.charAt (i)=='+'){
                   c.replace (i,i+1,"#");
               }
                if (c.charAt (i)=='-'){
                   c.replace (i,i+1,"~");
               }
               
           }
           
           a.push (c.charAt (i));
       }
       
      String d;
      d= new String(c);
      return(d);
       
   }
   
   
   
   // this method determines the priority of binary operators
   // more exactly, it determins whether or not the operator c2 should pop the operator c1
   // in the converting algorithm
   // so, the order is as follows
   // 0. (, )
   // 1. ~, #  
   // 2. ^   ( this is special. it is right associative, so if the same order operator comes in c2, don't pop
   // 3. *, /, %
   // 4. +,- '
   // 1 if pop,  0 if not to pop, 2 if ')' encounters '(''
   private static int priority(char c1, char c2){
       // if the stack is empty, nopop....
       if (c1=='t')
           return(0);
       
       if (c2==')'){
           if(c1=='(')
               return(2);
           if(c1=='~'||c1=='#'||c1=='^'||c1=='*'||c1=='/'||c1=='%'||c1=='+'||c1=='-'){
               return(1);
           }
       }
       
       if(c2=='~'||c2=='#'){
           //if(c1=='^'||c1=='*'||c1=='/'||c1=='%'||c1=='+'||c1=='-'){
           //    return(1);
           //}
           return(0);
       }
       
       if (c2=='^'){
           if ( c1=='~'||c1=='#'||c1=='~')
               return(1);
           return(0);
       }
       
       if(c2=='*'||c2=='/'||c2=='%'){
           if(c1=='~'||c1=='#'||c1=='^'||c1=='*'||c1=='/'||c1=='%'){
               return(1);
           }
           return(0);
       }
       
       
       
       if(c2=='+'||c2=='-'){
           if(c1=='(')
               return(0);
           return(1);
       }
       
       System.out.println("priority error");
       return(0);
   }
   
   
   public static String inFixToPostFix (String inputInfix){
       Stack a;
       a = new Stack();
       
       // first, convert unary operators....
       String c;
       c=new String(unaryProcesser (inputInfix));  // c is the unary processed String.
       
       // output is the final result
       StringBuffer output;
       output=new StringBuffer();         
       
       // Making ouput to be the wanted postfix expression       
       for (int i = 0; i < c.length ()  ; i++) {
           if (  (Pattern.matches ("[0-9\\.]", ""+c.charAt (i)) )==true)  {
               output.append (c.charAt (i));
               continue;
           } 
           
           // put in a blank to separate each operand..
             
           
           if ( c.charAt (i)=='('){
               a.push (c.charAt (i));
               output.append (" ");
               continue;
           }
           
           while ( priority (a.peek (), c.charAt(i) ) ==1 ) {
               output.append (" "+a.pop ()+" ");
           }
           
           if ( priority (a.peek (), c.charAt(i) ) ==2 ) {
               a.pop ();
               continue;
           }
           
           if (priority (a.peek (), c.charAt(i) ) ==0 ) {
               a.push (c.charAt(i));
               output.append (" ");
               continue;
           }
           
           
       }
       
       output.append (" ");
       while (a.isEmpty ()==false){
           output.append (" "+a.pop ()+" ");
       }
       
       String output1;
       output1=new String(output);
       return (output1); 
             
              
       
   }
   
   
   
   public static double postFixCalcultor(String inputPostFix){
       // this array a will contain each operand and operators in the postfix expression
       // as each element...
       String a[];
       int length;
       a=inputPostFix.split ("\\s+");
       length=a.length;
       if( a[0].equalsIgnoreCase ("")==true){
           for (int i = 0; i <length-1; i++) {
               a[i]=a[i+1];
           }
           length--;
          
       }
           
       
       
       //for (int i = 0; i < length; i++) {
       //    System.out.println(a[i]);
           
       //}
       
       Stack temp;
       temp=new Stack();
             
       for (int i = length-1; i >=0; i--) {
           // if it is unary operator, just push it to the stack
           if ( a[i].matches ("[\\~\\#]")==true){
               temp.push (a[i].charAt (0));
               continue;
           }
           // if it is binary operator, just push it to the stack
           if ( a[i].matches ("[\\^\\*\\/\\%\\+\\-]")==true){
               temp.push (a[i].charAt (0));
               continue;
           }
           
           // if it is an operand push it into the stack.
           Double op1;
           op1=new Double(a[i]);
           double op;
           op=op1.doubleValue();
           temp.push (op);
           
           
           //if the recent stacks are operatorable, do it repeatedly.
           
           while ( (temp.peek ()=='d'&&temp.peek2()=='~')||
                       (temp.peek()=='d'&&temp.peek2 ()=='#')||
                       temp.peek ()=='d'&&temp.peek2 ()=='d'){
               
               if (temp.peek ()=='d'&&temp.peek2()=='~'){
               double e;
               e=temp.peekOperand ();
               temp.pop();
               temp.pop();
               temp.push (-1*e);
               continue;
               }
               
               if (temp.peek ()=='d'&&temp.peek2()=='#'){
               double e;
               e=temp.peekOperand ();
               temp.pop();
               temp.pop();
               temp.push (e);
               continue;
               }

               if (temp.peek()=='d'&&temp.peek2()=='d'&&temp.peek3()=='^'){
                   
                   double e1;
                   double e2;
                   e1=temp.peekOperand ();
                   e2=temp.peekOperand2 ();
                   temp.pop ();
                   temp.pop ();
                   temp.pop ();
                   temp.push (Math.pow (e1,e2));
                   continue;
               }
               
               if (temp.peek()=='d'&&temp.peek2()=='d'&&temp.peek3()=='*'){
                   
                   double e1;
                   double e2;
                   e1=temp.peekOperand ();
                   e2=temp.peekOperand2 ();
                   temp.pop ();
                   temp.pop ();
                   temp.pop ();
                   temp.push (e1*e2);
                   continue;
               }
               
               if (temp.peek()=='d'&&temp.peek2()=='d'&&temp.peek3()=='/'){
                   
                   double e1;
                   double e2;
                   e1=temp.peekOperand ();
                   e2=temp.peekOperand2 ();
                   temp.pop ();
                   temp.pop ();
                   temp.pop ();
                   temp.push (e1/e2);
                   continue;
               }
               
               if (temp.peek()=='d'&&temp.peek2()=='d'&&temp.peek3()=='%'){
                   
                   double e1;
                   double e2;
                   e1=temp.peekOperand ();
                   e2=temp.peekOperand2 ();
                   temp.pop ();
                   temp.pop ();
                   temp.pop ();
                   temp.push (e1%e2);
                   continue;
               }
               
               if (temp.peek()=='d'&&temp.peek2()=='d'&&temp.peek3()=='+'){
                   
                   double e1;
                   double e2;
                   e1=temp.peekOperand ();
                   e2=temp.peekOperand2 ();
                   temp.pop ();
                   temp.pop ();
                   temp.pop ();
                   temp.push (e1+e2);
                   continue;
               }
               
               if (temp.peek()=='d'&&temp.peek2()=='d'&&temp.peek3()=='-'){
                   
                   double e1;
                   double e2;
                   e1=temp.peekOperand ();
                   e2=temp.peekOperand2 ();
                   temp.pop ();
                   temp.pop ();
                   temp.pop ();
                   temp.push (e1-e2);
                   continue;
               }
           }
       }
       
       double f;
       f=temp.peekOperand ();
       temp.pop ();
       if(temp.isEmpty ()==false){
           System.out.println("INTERNAL ERROR IN postFixCalcultor. ");
       }
           
       return(f);
   }
   
   public static void calculator (String input_1) {
             
           // [+-]?[1-9]\\d*(\\.\\d+)?   : real number
           // [+-]?     [+-]?[1-9]\\d*(\\.\\d+)?    : unary + real number
           // [^*/%+-]  : binary operator
           // [^*/%+-]  ([+-]?\\()?    [+-]?[1-9]\\d*(\\.\\d+)?  \\)?  :    a part of infix expression
            
            if (Func.isInfix(input_1)==false){
                System.out.println("Your input is not a legal Infix expression. Enter again");
                return;
            }
            
            if (Func.isRightParenthesis (input_1)==false){
                System.out.println("The parenthesis in your input does not match. Enter again");
                return;
            }
           
            String c=Func.unaryProcesser (input_1);
            //System.out.println(c);
            
            String d=Func.inFixToPostFix(c);
            System.out.println(d);
            
            System.out.println(Func.postFixCalcultor (d));
            System.out.println();
            return;
   }
   
    
}




























