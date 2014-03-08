// Calvin Caulfield,  java programming
// BigIntTest.java
// project launched  March 24, 2007 finished March 25, 2007.   Revisioned in March 27 (making the program run continuously unless "quit" has been entered)
// simple calculator that only accept two operands and only +, -, and *  as operators
// my first java developing... good luck

/* implementation plan:  
    1. process the input string as 
 *       1) remove any blanks among the input string
 *       2) extract: sign_0, operand_0, operator, sign_1, operand_1    from the input String
 *       3) two operands are converted to 200 digits integers and processed
 *       4) for multiplying, use the temporary storaging array (multi dimensional )
 
 
 operator is either +, -, or *
 *      primary functions to implement are:
 *          a. int [101]  sum  (  int[100],   int[100] )
 *          b. int[200]  multiply  ( int [100],  int[100])
 *                  {  
 *                          ... sum ( ) ....
 *                    }
 *          c. output maker:  eliminate all the zeroes in the left side of the output, attach the - sign if necessary
 */              
package bigInteger;

import java.io.*  ;    // for Input classes
import java.util.*;    // StringTokenizer

public class BigIntTest{
    

    public static void main(String[] args) throws IOException      {
       
     while(starter()!=0){         // if the input is legal and is not "quit",  repeat the process
         
         
     }
     
     
    }   
     
    static int starter () throws IOException {               // startet function:  determine whether the input is "quit". compress the input string (eliminate blanks)  and pass the value to Calculator function.
      
        
      BufferedReader input_0 = new BufferedReader(new InputStreamReader(System.in));    //get input
        
      String input_1=input_0.readLine();   
         
         
       StringTokenizer input_2 ;
       StringBuffer input_3;
       
       input_2 = new StringTokenizer (input_1);
       input_3= new StringBuffer();
       
    
       while (input_2.hasMoreTokens() ==true)   {    //remove blanks
      
           input_3.append(input_2.nextToken());
      }                          //input_3 is the input without any blank,  type is ---StringBuffer---
       
                       
     
       String input_tester;                                  // quit the programm if the input is "quit"
       input_tester=new String(input_3);
       //System.out.println (input_tester);
       if(input_tester.equals ("quit")){
          
           
          return (0); 
       }
        
       
       else{
       if(input_3.indexOf ("a")!=-1||input_3.indexOf ("b")!=-1||input_3.indexOf ("c")!=-1||input_3.indexOf ("d")!=-1||input_3.indexOf ("e")!=-1||input_3.indexOf ("f")!=-1||input_3.indexOf ("g")!=-1||input_3.indexOf ("h")!=-1||input_3.indexOf ("i")!=-1||input_3.indexOf ("j")!=-1||input_3.indexOf ("k")!=-1||input_3.indexOf ("l")!=-1||input_3.indexOf ("m")!=-1||input_3.indexOf ("n")!=-1||input_3.indexOf ("o")!=-1||input_3.indexOf ("p")!=-1||input_3.indexOf ("q")!=-1||input_3.indexOf ("r")!=-1||input_3.indexOf ("s")!=-1||input_3.indexOf ("t")!=-1||input_3.indexOf ("u")!=-1||input_3.indexOf ("v")!=-1||input_3.indexOf ("w")!=-1||input_3.indexOf ("x")!=-1||input_3.indexOf ("y")!=-1||input_3.indexOf ("z")!=-1||input_3.indexOf (".")!=-1||input_3.indexOf (",")!=-1||input_3.indexOf ("/")!=-1||input_3.indexOf ("[")!=-1||input_3.indexOf ("]")!=-1||input_3.indexOf ("`")!=-1){
           System.out.println("your input is illegal;  enter only numbers and +,-,*\nenter again\n");
           return (1);
       }
       
           calculater (input_3);
       return (1);
       }
    }
    
    
    static void calculater (StringBuffer input_3)  {      //  factorize the input string into operands and operators.  pass those values to Sum function or Multiply function
        
        
       
      // StringBuffer input_3;
      // input_3=new StringBuffer(input);
       char sign_0;    //declaring 5 variables....
       char sign_1;
       int[] operand_0;
       int[] operand_1;
       char operator;
       
      operand_0=new int[100];   // make instances of two operands; arrays of 200 charactors each
      operand_1=new int[100];
      
   
  
          switch   (  input_3.charAt(0) ) {  //getting sign_0   and remove the first sign if it exists
         
              case '+': {
                  sign_0='+';  
                  input_3.deleteCharAt(0);
                  break;
             }
          
         
              case'-':  {
                   sign_0='-';   
                   input_3.deleteCharAt(0);
                     break;
              }
         
              default: {
                    sign_0='+';
             
               }
          }
    
     
          
          
          
          
  
      
     switch  (   input_3.indexOf("*",0)   ) {  //getting the operator and remove redundant operators,  gettin  sign_1 also.....
      
        case -1: {

                operator='+';

                if (    input_3.indexOf("-",0) <0   )  {

                        sign_1='+';
                        if ( input_3.charAt  (   input_3.indexOf("+",0)+1  ) == '+' )  {

                             input_3.deleteCharAt(input_3.indexOf("+",0));
                             input_3.replace(input_3.indexOf("+",0),input_3.indexOf("+",0)+1," ");
                           }
                              else 
                               input_3.replace(input_3.indexOf("+",0),input_3.indexOf("+",0)+1," ");
                 }       

                else   {

                            if ( input_3.charAt(  input_3.indexOf("-",0)+1   ) =='-'  )  {
                                     sign_1='+';
                                    input_3.deleteCharAt(input_3.indexOf("-",0));
                                    input_3.replace(input_3.indexOf("-",0),input_3.indexOf("-",0)+1," ");
                            }

                            else

                                if  ( input_3.charAt(  input_3.indexOf("-",0)+1   ) =='+'  )  {

                                            sign_1='-';
                                            input_3.deleteCharAt(input_3.indexOf("-",0));
                                             input_3.replace(input_3.indexOf("+",0),  input_3.indexOf("+",0)+1," ");
                                }
                                else
                                      if  ( input_3.charAt(  input_3.indexOf("-",0) - 1   ) =='+'  )  {

                                         sign_1='-';
                                         input_3.deleteCharAt(input_3.indexOf("-",0)-1);
                                           input_3.replace(input_3.indexOf("-",0),input_3.indexOf("-",0)+1," ");
                                      }
                                      else  {

                                          sign_1='-';
                                           input_3.replace(input_3.indexOf("-",0),input_3.indexOf("-",0)+1," ");
                                      }
               }  
                break;
      }
       
    
            default:    {
                
                      operator='*' ;
                    //  System.out.println(input_3.indexOf("*",0));          //testing


                      if(input_3.charAt(input_3.indexOf("*",0)+1)=='+' ||  input_3.charAt(input_3.indexOf("*",0)+1)=='-')  {

                              sign_1=input_3.charAt(input_3.indexOf("*",0)+1);
                              input_3.deleteCharAt(input_3.indexOf("*",0)+1);
                      }
                      else sign_1='+';

            }
 
     
  }   
      
      
     
     
     
     
     

   StringTokenizer temp;         //  setting temporary variables  to store each operand as Strings
   String op;  
   String op_0;
   String op_1;
    int length=input_3.length();
    op=new String(input_3.substring(0,length));  
     
     
    switch (operator)  {
 
            case '*':      {    // op_0  and op_1 is two String that stores the numbers without any operator.

                 temp=new StringTokenizer(op,"*");
                 op_0=new String(temp.nextToken());
                 op_1=new String(temp.nextToken());
                        break;
           }



            case '+': {   // op_0  and op_1 is two String that stores the numbers without any operator.

                 temp=new StringTokenizer(op);
                 op_0=new String(temp.nextToken());
                 op_1=new String(temp.nextToken());
                         break;
           } 


            default:  {
              op_0="0";
              op_1="0";

            }
   
    }
    
      
    
        String[] op_00;                              //setting temporary  'String arrays'  which take each digit as a String element
        String[] op_11;
        op_00= new String [100];
        op_11= new String [100];
 
        
        
      for ( int i = 0  ; i < 100  ; i ++)    {      //  setting two operands as interger arrays. 
    
       
        
                 if(  i <  op_0.length()  )  {

                     op_00[i]=""+op_0.charAt(op_0.length()-1-i);              //converting  a char into a single letter String
                     operand_0[i]=Integer.parseInt(op_00[i]);     //converting single digit String into a integer
                 }

                 else 
                     operand_0[i]=0;


                  if(  i <  op_1.length()    )   {

                     op_11[i]=""+op_1.charAt(op_1.length()-1-i);                //converting  a char into a single letter String
                     operand_1[i]=Integer.parseInt(op_11[i]);        //converting single digit String into a integer
                  } 

                   else 
                   operand_1[i]=0;
      }
    

    
      
      
 
  
      
      
       
         
         
         // System.out.println("operator is determined as"+operator);         // testing
       //   System.out.println("sign_0 is determined as"+sign_0);  
       //   System.out.println("sign_1 is determined as"+sign_1);  
       //   System.out.println("the resulting input_3 is"+ input_3);
    /*   for ( int i = 0  ; i < 100  ; i ++)
      {
         System.out.println(i+"th element of operand_0 is "+operand_0[i]);
        
      }
              
          for ( int i = 0  ; i < 100  ; i ++)
      {
         System.out.println(i+"th element of operand_1 is "+operand_1[i]);
        
      }  */
         
       
         
 
      if ( operator =='*')  {
      
           int[]  outMultiply;
          outMultiply= new int[201];                                // resulting   int array
           outMultiply= multiply ( operand_0, operand_1, sign_0, sign_1);         // calculate the sum
           String outString;                                              //String for final output
           outString=new String(outputMaker(outMultiply));   // process outputMaker
           System.out.println(outString);    
         //  System.out.println(outMultiply[0]);
      
      } 
      
          
     if ( operator=='+')    {  
    
          int[]  outSum;
           outSum= new int[102];                                // resulting   int array
           outSum= sum  ( operand_0, operand_1, sign_0, sign_1);         // calculate the sum
           String outString;                                              //String for final output
           outString=new String(outputMaker(outSum));   // process outputMaker
            
           
         
           System.out.println(outString);    
           
     }
          
    
          
          
          
    }
    
    
    
    
    static int[]  sum ( int[] a,  int[] b, char sign_a, char sign_b)     {    // summing function: taking two int[100]  and output the sum as int[102]  
   
    
      /* if ( sign_a=='-')                                         //  failed....  
       {
           for ( int i = 0 ;  i < 100  ; i  ++)
           {
               a[i]=-a[i];
           }
       }
        
        if ( sign_b=='-')
       {
           for ( int i = 0 ;  i < 100  ; i  ++)
           {
               b[i]=-b[i];
           }
       }  */
     //  System.out.println("the sign_a is   "+sign_a);   //test
       
      // System.out.println("the sign_b is   "+sign_b);
        int[] output; 
        output= new int [102] ;                 /////// 101 th entry is for the sign of the result :10 for +,  11 for -///////
        int carry=0;
           
        if ( (sign_a=='+' ) && (sign_b=='+') )    // 1  means + sign
            output[101]=10; 
        if(( sign_a=='-' ) && (sign_b=='-') )      // -1 means - sign
            output[101]=11;
        
        if ( sign_a==sign_b)         { // when ++   --    the operation is to sum, not subtract

                for ( int i = 0  ;  i <100 ;  i++)  {

                        output[i]= ( a[i] + b[i] + carry) % 10;
                        carry =  ( a[i] + b[i] + carry) /10 ;
                } 
              
                if ( carry==1) 
                      output[100]=1;
               else
                       output[100]=0;
        }
        
        
        
        
        
  if ( sign_a!=sign_b)       {       // when two signs are different,  I must first determine the final sign
         
            
         char theBigger='0';                     // theBigger indicates the bigger(absolute value) of a and b
         
            for ( int i = 99  ;  i >-1 ;  i--)  {

                     if ( a[i] > b[i])  {
                     
                         theBigger='a';
                         break;
                     }
                   
                     if ( a[i] <b[i])  { 
                     
                         theBigger='b';
                         break;
                     }

                     output[101]=10;           // when two operand are the same and only signs are opposite
             }
         
         
         
            if (theBigger=='a')    {    // a is bigger
            
                     if(sign_a=='+')  {
                     
                         output[101]=10;
                         //System.out.println(" sign +");
                     }
                     

                     if(sign_a=='-')  {
                     
                         output[101]=11;
                         //System.out.println(" sign - ");
                     }
                     

                    
                  
             }
            
          if (theBigger=='b')     {   // b is bigger

                     if(sign_b=='+')
                         output[101]=10;
                     if(sign_b=='-')
                        output[101]=11;
             
             }
            
        
         
         
         
        if  ( theBigger=='a')  {
        
                   carry=0;  
                   for ( int i = 0  ;  i <100 ;  i++)  {
                 

                                if (  ( a[i] - b[i] + carry )  >   -1 )  {
                                
                                    output[i]=a[i]-b[i]+carry;
                                    carry=0;
                                }
                                
                                else   {
                                
                                    output[i]=a[i]-b[i]+carry+10;
                                    carry=-1;
                                }
                   }
                
                   output[100]=0;
                   if (carry==-1)
                    System.out.println("error in function sum!! (in subtractin 1)");

         }
         
          if  ( theBigger=='b')  {

                     carry=0;
                     for ( int i = 0  ;  i <100 ;  i++)  {
                     
                                    if (  ( b[i] - a[i] + carry )  >   -1 )   {
                                    
                                        output[i]=b[i]-a[i]+carry;
                                        carry=0;
                                    }
                                    else  {
                                    
                                        output[i]=b[i]-a[i]+carry+10;
                                        carry=-1;
                                    }

                     }
                   
                     if (carry==-1)
                     System.out.println("error in function sum!! (in subtractin 2)");
                     output[100]=0;
         }
         
         
         //System.out.println(" the bigger is   "+theBigger);   //testing
    }
        
        
        
        return (output);
        
    }
    
    
    static int[]  simpleSum  (int[]a,  int[]b)   {  //take two int[200]  and output the sum of them (used in multiply)
   
       
               int[] output;
               output= new int [200];
               int carry=0;
               for ( int i= 0  ;  i < 200  ; i++)  {
               
                        output[i]=  (a[i]+ b[i] + carry)  %10;
                      //  System.out.println(output[i]);
                        carry=  (a[i]+ b[i] + carry)  /10;
                        //System.out.println(carry);
               }
               
               if ( carry!=0)  System.out.println("error  in simpleSum function");

               return (output);
       
       
       
   }
   
    
    
    static String outputMaker ( int[] a)      {       // make final output String from the result of calculation. 
                                                          //first extract the sign from the first element. Then  remove all zeros of the left side. 
    
        String sign;
        //System.out.println("the sign of the output is    "+a[101]);
                
        int length= a.length;
        
        switch (a[length-1])  {
            
                case 10:  {
                sign= new String("");
                break;
                }
                
                case 11:  {
                sign = new String ("-");
                break;
                }
                   
            default:   sign=new String("abc2rer");
        }
         
        
        
        int startingPoint=length-2;
      
        for  ( int i  =  length-2  ; i>-1  ;  i--)  {
        
            if( a[i]==0)
            startingPoint--;
            else break;
        }
         
         StringBuffer outStringBuffer;
         String outString;
         outStringBuffer=new StringBuffer();
        
         outStringBuffer.append(""+sign);
         
         for  ( int i  =  startingPoint  ; i>-1  ;  i--)  {
         
             outStringBuffer.append(a[i]);
         }
         
         if  (outStringBuffer.length()==0)                   // prevent output null string
             return ("0");
         outString=new String ( outStringBuffer.substring(0) );
         
      ///  if(outString!="")
        
            return (outString);
       //  if(outString=="")
           //  return("1110");
        // else return("errer");
    }
    
    
    static int[] multiply   (int[] a,  int[]b , char sign_a, char sign_b)  {  // take two input and sign, output result
    
        int[] output;
        output=new int[201];                     // 201 th  element is sign   ;   10for + ,  11for -
        
        if (sign_a==sign_b)
            output[200]=10;
        else 
            output[200]=11;
        
        
        
        
        
        int [] [] []  temp;
        temp=new int[100] [100][200];        // i,  j,  200 ... multiplying temporary values
       
        
        for  (  int i = 0  ;   i<  100 ;  i ++)   { //initialize all elements to zero
        
            for  (int j= 0  ; j <100  ;  j++)  {
           
                    for (int k=0 ; k<200  ;  k++)       // calculate simple one bit * one bit   multiplication for each and every pair of bits of two operands. 
                    temp[i][j][k]=0;
            }
        }
        
        
                
        
        for  (  int i = 0  ;   i<  100 ;  i ++)    {  //  assign each partial sums to array
        
            for  (int j= 0  ; j <100  ;  j++)  {
            
                int temp_0= i+j;
                temp[i][j][temp_0]=(a[i]*b[j] )%10;
                temp[i][j][temp_0+1]=(a[i]*b[j]) /10;
                
            }
        }
        
        
        int[]temp_sum;
        int[]temp_to_add;
        temp_sum=new int[200];
        temp_to_add=new int[200];
        for (int k=0 ; k<200  ;  k++)  {
        
            temp_sum[k]=0;
            temp_to_add[k]=0;
        }
        
        
        
        for  (  int i = 0  ;   i<  100 ;  i ++)   {   //
        
            for  (int j= 0  ; j <100  ;  j++)  {
           
                for (int k=0 ; k<200  ;  k++)  {
                
                    temp_to_add[k]= temp[i][j][k];
                 }
                temp_sum=simpleSum(temp_sum, temp_to_add);      // sum all the temporary arrays
             
            }
        }
     
        for (int i  = 0;   i<200  ; i++)  {
        
                    output[i]=temp_sum[i];
        }
        
        return(output);
        
    }

    
    }    
    

















