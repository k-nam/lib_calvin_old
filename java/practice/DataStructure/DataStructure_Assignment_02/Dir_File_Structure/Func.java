package Dir_File_Structure;



public class Func {
    
   
    public Func() {
    }
    
    
    public static String[] reduceArray ( String[] inputStringArray){
        
        String[] tempArray;
        int length= inputStringArray.length;
        tempArray= new String[length-1];
        for (int i = 0; i < length-1 ; i++) {
            tempArray[i]= inputStringArray[i+1];
        }
        return( tempArray);
}
    
    public static String[]  sumArray ( String[] inputStringArray, String inputString){
         
        String[] tempArray;
        int length= inputStringArray.length;
        tempArray= new String[length+1];
        
        for (int i = 0; i < length ; i++) {
            tempArray[i]= inputStringArray[i];
        }
        
        tempArray[length]= inputString;
        return( tempArray);
    }
    
    
    
    //show all the element with [ and ]
    public static void printArray (String[] inputStringArray){  // print all elements of a String[] in one line
        
        int length=inputStringArray.length;
        //System.out.print("(   ");
        for (int i = 2; i < length; i++) {
        System.out.printf("[ %1$13s  ]",inputStringArray[i]);
        }
        System.out.println();
    }
    
    // shows the last element without [ and ]
    public static void printArray_File (String[] inputStringArray){  // print all elements of a String[] in one line
        
        int length=inputStringArray.length;
        //System.out.print("(   ");
        for (int i = 2; i < length-1; i++) {
        System.out.printf( "[ %1$13s  ]",inputStringArray[i]);
        }
        
        System.out.printf("   %1$13s   ", inputStringArray[length-1]);
        System.out.println();
    }
    
    
    // all are shown just -  except for the last one
    public static void printArray_Last (String[] inputStringArray){  // print only the last elements of a String[] in one line
        
        int length=inputStringArray.length;
        String temp;
        temp=" - ";
        for (int i = 2; i < length-1; i++) {
        System.out.printf( "   %1$13s   ",temp);
        }
        System.out.printf( "   %1$13s   ",inputStringArray[length-1]);
        System.out.println();
    }
    
    
    
    // this search method searches through all the elements of the array;
    // that is, if the input string is included in a dir (path), that dir and its files wil be displyed
    public static void printArray_Search (String[] inputStringArray, String inputString){  // careful to ignore upper lower case. 
        
        int length=inputStringArray.length;
        int tester=0;
        String newInputString=inputString.toUpperCase();
        String newInputStringArray[];
        newInputStringArray= new String[length];
       
        
        for (int i = 0; i <length; i++) {
            newInputStringArray[i]= inputStringArray[i].toUpperCase();
            
            if ( newInputStringArray[i].indexOf(newInputString)!=-1){
                printArray(inputStringArray);
                 return;
            }
        }
        return;
       
    }
    
    
    // this one only searches for the last element of the Stirng[]  which has the input string in it. 
    // the last element will be shown without  [ and ].  ( supposed to be a file)
     public static void printArray_Search_Last (String[] inputStringArray, String inputString){  // careful to ignore upper lower case. 
        
        int length=inputStringArray.length;
        int tester=0;
        String newInputString=inputString.toUpperCase();
        String temp;
        temp=new String ( inputStringArray[length-1].toUpperCase());
        
        if ( temp.indexOf(newInputString)!=-1){
            printArray_File(inputStringArray);
             return;
        }
        
        return;
       
    }
     
     // same as the above, but all the elements are shown with [ and ] (all dirs)
     public static void printArray_Search_Last1 (String[] inputStringArray, String inputString){  // careful to ignore upper lower case. 
        
        int length=inputStringArray.length;
        int tester=0;
        String newInputString=inputString.toUpperCase();
        String temp;
        temp=new String ( inputStringArray[length-1].toUpperCase());
        
        if ( temp.indexOf(newInputString)!=-1){
            printArray(inputStringArray);
             return;
        }
        
        return;
       
    }
    
}

    