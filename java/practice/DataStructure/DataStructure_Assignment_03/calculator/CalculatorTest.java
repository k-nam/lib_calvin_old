
// Starting to work on April 14th, 2007
// Finished on April 15th, 2007

package calculator;


import java.io.*;

/**
 *
 * @author Calvin Caulfield
 */
public class CalculatorTest {
    
  
    public CalculatorTest () {
    }
    
    
    public static void main (String[] args)  {
        
        //receive file input
        if (args.length==1){
            File file1;
            file1=new File(args[0]); 
            if ( file1==null|| (!file1.exists () ) ) {
                System.out.println("There is no file named \""+args[0]+"\"");
                return;
            }
            
          // process the text file
           FileReader fileReader1 = null;
           BufferedReader bufferedReader1;
            try {
                fileReader1=new FileReader (file1);
            } catch (FileNotFoundException ex) {
                ex.printStackTrace();
            }
           bufferedReader1=new BufferedReader (fileReader1);
           
           int status=1;
           do{
               String temp;
                try {
                    temp=bufferedReader1.readLine ();
                } catch (IOException ex) {
                    ex.printStackTrace();
                    System.out.println("An Error occured in reading the contents of the file");
                    return;
                }
             
               if(temp==null){
                   status=0;
                   continue;
               }
               //if(temp.equalsIgnoreCase("")==true){
            	//   continue;
               //}
                   
               Func.calculator (temp);
               
           }
           while(status==1);
           
           
            
            return;
        }
        
       
        
        
        if(args.length>1){
            System.out.println("Enter only one argument <Filename>");
            return;
        }
        
        
        
        if(args.length==0){
            System.out.println("There is no file input, keyboard input mode starting:");
            
            
             int status=1;   // If status is 0,  the main loop ends and the program finishes

            // setting BufferedReader
            BufferedReader input_0=new BufferedReader (new InputStreamReader(System.in));

            do {
                String input_1;
                try {
                    input_1 = input_0.readLine();
                } catch (IOException ex) {
                    ex.printStackTrace();
                    return;
                }

                if (input_1.equals("q")){
                    status=0;
                    continue;
                }
                Func.calculator (input_1);

            }
            while(status==1);
            return;
        }
   
    }
    
}
