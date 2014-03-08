package Dir_File_Structure;


import java.io.*;
import java.util.*;

public class MovieDatabaseTest {
    

    public MovieDatabaseTest() {
    }
    
    static public void main ( String[] args ) throws IOException{
        
       
        Dir root;
        root= new Dir ( null, "Root");
        int status= 1;           // if this becomes 0,   terminate the program....
     
        BufferedReader input_0 = new BufferedReader(new InputStreamReader(System.in));    //get input
        
            
    
    
    
        do {
        	
        	String input_1=input_0.readLine();   

            StringTokenizer input_2 ;
            StringBuffer input_3;

            input_2 = new StringTokenizer (input_1);
            input_3= new StringBuffer();
            while (input_2.hasMoreTokens() ==true)   {    //remove blanks

                input_3.append(input_2.nextToken());
           }                          //input_3 is the input without any blank,  type is ---StringBuffer---



            String input_tester1;                                  // quit the programm if the input is "quit"
            input_tester1=new String(input_3);
            String input_tester;
            input_tester=new String(input_tester1.toUpperCase());  //input_tester is all UpperCase input without any blank!
            //System.out.println (input_tester);
            
            
            if(input_tester.equalsIgnoreCase ("QUIT")){
                status=0;
               continue;
            }
            
            
            
            if(input_tester.indexOf ("PRINTD") !=-1){
                if (root.isEmpty()==true){
                    
                    System.out.println("E M P T Y");
                    continue;
                }
                
                
                String [] a;
                a=new String[] {""};
                root.print_All(a, false);
                continue ; 
            }
            
            if(input_tester.indexOf ("PRINT") !=-1){
                if (root.isEmpty()==true){
                    
                    System.out.println("E M P T Y");
                    continue;
                }
                
                
                String [] a;
                a=new String[] {""};
                root.print_All(a, true);
                continue ; 
            }
            
            
            if(input_tester.indexOf("SEARCHD")!=-1){
                String [] c;
                c=new String[] {""};
                String temp;
                int a;
                int b;
                a=input_tester.indexOf("%");
                b=input_tester.indexOf("%", a+1);
                temp=input_tester.substring(a+1, b);
                
                
                if ( (temp.compareToIgnoreCase ("")==0)|| temp==null) {
                    System.out.println("invalid search command. enter again.");
                    continue;
                }
                
                root.print_All_Search(c, temp, false);
                continue; 
            }
            
            
            
            if(input_tester.indexOf("SEARCH")!=-1){
                String [] c;
                c=new String[] {""};
                String temp;
                int a;
                int b;
                a=input_tester.indexOf("%");
                b=input_tester.indexOf("%", a+1);
                temp=input_tester.substring(a+1, b);
                
                
                if ( (temp.compareToIgnoreCase ("")==0)|| temp==null) {
                    System.out.println("invalid search command. enter again.");
                    continue;
                }
                
                root.print_All_Search(c, temp, true);
                continue; 
            }
            
            
            
            
            
            
            
            if(input_tester.indexOf("INSERT")!=-1){
                int length=input_tester.length();
                int counter=0;        // the number of  % in the input !
                int a=5 ,b=5;
                       
                for (int i = 0; i < length; i++) {
                    if  ( ( b=input_tester.indexOf ("%", a+1 ) ) != -1 ) {
                        
                        a=b;
                        counter++;
                    }
                    else break;
                }
                
                if (counter%2 !=0){
                    System.out.println("INSERT input invalid. enter again");
                    continue;
                }
                
                
                String[] temp;        // becomes String[] path....
                String temp1;       // becomes fileName.
                 
                
        
                 
                temp=new String [counter/2];
                a=5; b=5;     // reseting a,b
                for (int i = 0; i < counter/2  ; i++) {
                    b= input_tester.indexOf("%", a+1);
                    a= input_tester.indexOf("%", b+1);
                    temp[i]=input_tester.substring(b+1, a);
                }
                temp1=new String(input_tester.substring(a+1));
                if (temp1.equalsIgnoreCase("")){
                    //System.out.println("null String can not be a file name. enter again");
                    root.make_Dir (temp);
                    continue;
                }
                 
                root.makeFile(temp, temp1);
                continue; 
            }
            
            if(input_tester.indexOf("DELETE")!=-1){
            	
                
                int length=input_tester.length();
                int counter=0;        // the number of  % in the input !
                int a=5 ,b=5;
                       
                for (int i = 0; i < length; i++) {
                    if  ( ( b=input_tester.indexOf ("%", a+1 ) ) != -1 ) {
                        
                        a=b;
                        counter++;
                    }
                    else break;
                }
                
                if (counter%2 !=0){
                    System.out.println("INSERT input invalid. enter again");
                    continue;
                }
                
                
                String[] temp;        // becomes String[] path....
                String temp1;       // becomes fileName.
                temp=new String [counter/2];
                a=5; b=5;     // reseting a,b
                for (int i = 0; i < counter/2  ; i++) {
                    b= input_tester.indexOf("%", a+1);
                    a= input_tester.indexOf("%", b+1);
                    temp[i]=input_tester.substring(b+1, a);
                }
                
                temp1=new String(input_tester.substring(a+1));
                 
                if (temp1.equalsIgnoreCase("")){
                    //System.out.println("null String can not be a file name. enter again");
                    root.remove_Dir (temp);
                    continue;
                }
                 
                root.removeFile(temp, temp1);

                continue; 
            }
            
            System.out.println("Your input is invalid. Type your cammand again.");
            continue;
            
        }
        while (status==1);

    }
    
}


   



    
    

