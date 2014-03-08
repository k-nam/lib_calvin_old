package Dir_File_Structure;
 

public class Dir implements Dir_Interface {
    
    private String dirName;
    private Dir nextDir;
    private Dir parentDir;    
    private Dir_Node childDir;         // dummy for Dir structure...
    private int numOfSubDir;    
    private int numOfSubFile;   // num of Files in Dir
    private File childFile;           // dummy File..
    
    // below are basic methods.
    public Dir(Dir input_parentDir, String input_dirName) {
        dirName=input_dirName;
        nextDir=null;
        parentDir=input_parentDir;
        childDir=new Dir_Node();
        numOfSubDir=0;
        childFile=new File (this, "");
        numOfSubFile=0;
    }
    
    public String getName (){
        return (dirName);
    }
    
    public void setNext (Dir input_nextDir){
        nextDir=input_nextDir;
    }
    
    public Dir getNext (){
        return (nextDir);
    }

    public int num_Of_Sub_Dir() {
        return (numOfSubDir);
    }
    
    public boolean isEmpty (){
        if (numOfSubDir==0 && numOfSubFile==0)
            return (true);
        return(false);
    }
    
                
    
    
    
    // below are internal methods for Dir
    
    private Dir find_Dir (String input_Name){       // find a Dir by its name and return it.. return null if it does not exist.
        
        
        Dir tempDir;
        tempDir=childDir.getNext();
        
        for  (int i= 0 ; i < numOfSubDir  ; i++){
            if (tempDir.getName().equalsIgnoreCase(input_Name)==true) 
                return (tempDir);
            tempDir=tempDir.getNext();
        }
        return (null);
    }
    
    private Dir find_Dir (int index){
        if ( (index<1) || index >numOfSubDir){
            
            System.out.println("find_Dir internal error");
            return(null);
        }
        
        Dir tempDir;
        tempDir=childDir.getNext();
        
        for (int i = 1 ; i < index  ;  i++) {
            tempDir=tempDir.getNext();
        }
       return(tempDir); 
        
    }   // index should not be 0
    
    private int find_IndexOf_Dir (String input_Name){// find the index of given String.  
        if (numOfSubDir==0)
            return (1);
        
        Dir tempDir;
        tempDir=childDir.getNext();
        
        for (int i = 1 ; i < (numOfSubDir+1) ; i++) {
            if (tempDir.getName().compareToIgnoreCase(input_Name) >= 0)
                  
                return (i);
            tempDir=tempDir.getNext();
        }
        
        return (numOfSubDir+1);
    }
    
    
    private Dir make_Dir (String input_Name, int index) {//  make Dir by given name and given index and return it. if the name already exist, it is an fatal error (can not be detected)
        // this is the  only  method that actually makes Dir .  should not exist another way!
        if ( (index> numOfSubDir+1 ) || (index <1) ){
            System.out.println("make_Dir internal index error");
            return(null);
        }
        if (index==1){
            Dir tempDir;
            Dir curr;
            curr=childDir.getNext();
            tempDir=new Dir (this, input_Name);
            childDir.setNext(tempDir);
            tempDir.setNext(curr);
            System.out.println(input_Name+" has been created as the "+index+" th Dir of "+this.getName());
            
            numOfSubDir++;
            return(tempDir);
        }
        
        Dir prev;
        Dir curr;
        prev=this.find_Dir(index-1);
        curr=prev.getNext();
        Dir tempDir;
        tempDir= new Dir (this, input_Name);
        prev.setNext(tempDir);
        tempDir.setNext(curr);
        System.out.println(input_Name+" has been created as the "+index+" Dir of "+this.getName());
        
        numOfSubDir++;
        return(tempDir);
    }
    
    private Dir make_Dir_Sorted (String input_Name) {
        if ( this.find_Dir(input_Name)!=null){
            System.out.println("make_Dir_Sorted internal error");
            return (null);
        }
        
        int index = this. find_IndexOf_Dir (input_Name);
        return ( this. make_Dir (input_Name, index));
    }
    
    
    private void remove_Dir (int index){   // only method that actually remove a Dir
        if ((index<1) || ( index>numOfSubDir)){
            System.out.println("remove_Dir internal error 1 index is "+index);
            return;
        }
        
        if (index==1){
            Dir currDir;
            Dir nextDir;
            currDir=childDir.getNext();
            nextDir=currDir.getNext();
            childDir.setNext(nextDir);
            System.out.println(this.getName()+"'s "+index+" th Dir "+currDir.getName()+" has been removed");
            
            numOfSubDir--;
            return;
        }
        
        Dir prev;
        Dir curr;
        prev=this.find_Dir(index-1);
        curr=this.find_Dir(index);
        prev.setNext(curr.getNext());
        System.out.println(this.getName()+"'s "+index+" th Dir "+curr.getName()+" has been removed");
        
        numOfSubDir--;
        return;
        
    }
    
    private void remove_Dir (String input_Name){
        if (this.find_Dir(input_Name)==null){
            System.out.println("remove_Dir internal error");
            return;
        }
        
        int index;
        index=this.find_IndexOf_Dir (input_Name);
        this.remove_Dir(index);
    }
    
      
    // below are internal methods for File
    
    private File find_File (String input_Name){       // find a File by its name and return it.. return null if it does not exist.
        
        
        File tempFile;
        tempFile=childFile.getNext();
        
        for  (int i= 0 ; i < numOfSubFile  ; i++){
            if (tempFile.getName().equalsIgnoreCase(input_Name)==true) 
                return (tempFile);
            tempFile=tempFile.getNext();
        }
        return (null);
    }
    
    private File find_File (int index){
        if ( (index<1) || index >numOfSubFile){
            
            System.out.println("find_File internal error");
            return(null);
        }
        
        File tempFile;
        tempFile=childFile.getNext();
        
        for (int i = 1 ; i < index  ;  i++) {
            tempFile=tempFile.getNext();
        }
       return(tempFile); 
        
    }   // index should not be 0
    
    private int find_IndexOf_File (String input_Name){// find the index of given String.  
        if (numOfSubFile==0)
            return (1);
        
        File tempFile;
        tempFile=childFile.getNext();
        
        for (int i = 1 ; i < (numOfSubFile+1) ; i++) {
            if (tempFile.getName().compareToIgnoreCase(input_Name) >= 0)
                  
                return (i);
            tempFile=tempFile.getNext();
        }
        
        return (numOfSubFile+1);
    }
    
    private File make_File (String input_Name, int index) {//  make File by given name and given index and return it. if the name already exist, it is an fatal error (can not be detected)
        // this is the  only  method that actually makes File .  should not exist another way!
        if ( (index> numOfSubFile+1 ) || (index <1) ){
            System.out.println("makeFile internal index error");
            return(null);
        }
        if (index==1){
            File tempFile;
            File curr;
            curr=childFile.getNext();
            tempFile=new File (this, input_Name);
            childFile.setNext(tempFile);
            tempFile.setNext(curr);
            System.out.println(input_Name+" has been created as the "+index+" th File of "+this.getName());
            
            numOfSubFile++;
            return(tempFile);
        }
        
        File prev;
        File curr;
        prev=this.find_File(index-1);
        curr=prev.getNext();
        File tempFile;
        tempFile= new File (this, input_Name);
        prev.setNext(tempFile);
        tempFile.setNext(curr);
        System.out.println(input_Name+" has been created as the "+index+" File of "+this.getName());
        
        numOfSubFile++;
        return(tempFile);
    }
    
    private File make_File_Sorted (String input_Name) {
        if ( this.find_File(input_Name)!=null){
            System.out.println("make_File_Sorted internal error");
            return (null);
        }
        
        int index = this. find_IndexOf_File (input_Name);
        return ( this. make_File (input_Name, index));
    }
    
    private void remove_File (int index){   // only method that actually remove a File
        if ((index<1) || ( index>numOfSubFile)){
            System.out.println("remove_File internal error 1 index is "+index);
            return;
        }
        
        if (index==1){
            File currFile;
            File nextFile;
            currFile=childFile.getNext();
            nextFile=currFile.getNext();
            childFile.setNext(nextFile);
            System.out.println(this.getName()+"'s "+index+" th File "+currFile.getName()+" has been removed");
            
            numOfSubFile--;
            return;
        }
        
        File prev;
        File curr;
        prev=this.find_File(index-1);
        curr=this.find_File(index);
        prev.setNext(curr.getNext());
        System.out.println(this.getName()+"'s "+index+" th File "+curr.getName()+" has been removed");
        
        numOfSubFile--;
        return;
        
    }
    
    private void remove_File (String input_Name){
        if (this.find_File(input_Name)==null){
            System.out.println("remove_File internal error");
            return;
        }
        
        int index;
        index=this.find_IndexOf_File (input_Name);
        this.remove_File(index);
    }
    
    
    
    
    
    // below are external methods of Dir
    
    
    public Dir make_Dir(String[]  input_Path) {
        int length=input_Path.length;
        switch (length){
            
            case 1:{
                if  ( this.find_Dir(input_Path[0])  !=null){  // the final Dir already exist
                    System.out.println("Dir "+input_Path[0]+" already exist in "+this.getName()+". Cannot make it");
                    return (null);
                }

                return ( this.make_Dir_Sorted(input_Path[0]));
                     
            }
            
            
            default:{
                Dir tempDir;
                if (this.find_Dir(input_Path[0])==null){
                    tempDir= this.make_Dir_Sorted( input_Path[0]);
                    return (tempDir.make_Dir(Func.reduceArray(input_Path)));
                }

                tempDir=this.find_Dir(input_Path[0]);
                return(tempDir.make_Dir(Func.reduceArray(input_Path)));

            }
        }
    }

    public void remove_Dir(String[] input_Path) {
        int length=input_Path.length;
        switch (length){
        case 1:{
        	if  ( this.find_Dir(input_Path[0])  ==null){  // the final Dir does not exist
                System.out.println("Dir "+input_Path[0]+" does not exist in "+this.getName()+". Cannot remove it");
                return ;
            }
            this.remove_Dir(input_Path[0]);
            return;
        }
        default:{
        	if (this.find_Dir(input_Path[0])==null){
                System.out.println("Dir "+input_Path[0]+" does not exist in "+this.getName()+". Cannot remove it");
                return;
            }
            Dir tempDir;
            tempDir=this.find_Dir(input_Path[0]);
            tempDir.remove_Dir(Func.reduceArray(input_Path));
            return;        
        }
        }   
    }

    public Dir get_Dir(String[] input_Path) {
        int length=input_Path.length;
        switch (length){
            
            case 0:{
                return (this);
            }
            
            
            case 1:{
	            if  ( this.find_Dir(input_Path[0]) ==null){  // the final Dir already exist
	                //System.out.println("Dir "+input_Path[0]+" does not exist in "+this.getName() );
	                return (null);
	            }
	
	            return ( this.find_Dir(input_Path[0]));
                     
            }
            
            
            default:{
                Dir tempDir;
                if (this.find_Dir(input_Path[0])==null){
                    //System.out.println("Dir "+input_Path[0]+" does not exist in "+this.getName( ));
                    return(null);
                }

                tempDir=this.find_Dir(input_Path[0]);
                return(tempDir.get_Dir(Func.reduceArray(input_Path)));

            }
        }
    }
    
    
    
    // below are public methods of File
    
    public File makeFile (String[] inputPath, String inputFileName){
        Dir tempDir;
        tempDir=this.get_Dir (inputPath);
        
        if (tempDir!=null){
            if ( tempDir.find_File(inputFileName)==null)   // if the file does not exist;
                   return (tempDir.make_File_Sorted(inputFileName)  );
            System.out.println("File "+inputFileName+" already exist in "+tempDir.getName()+". Cannot make it");
            return(null);
            }
        
        tempDir=this.make_Dir(inputPath);
        return (tempDir.make_File_Sorted(inputFileName)  );
    }
    
    public void removeFile (String [] inputPath, String inputFileName){
        
        Dir tempDir;
        File tempFile;
        tempDir = this.get_Dir(inputPath);
        
        if ( tempDir==null){
            System.out.println("removeFile Error; the path is invalid. Enter again. ");
            return;
        }
        
        if (   (   tempFile=tempDir.find_File(inputFileName)   )==null     ){
            System.out.println("removeFile Error; the File "+inputFileName+" does not exist in "+tempDir.getName());
            return;
        }
        tempDir.remove_File(inputFileName);
        return;
        
    }
    
    public File getFile  (String [] inputPath, String inputFileName){
        
        Dir tempDir;
        File tempFile;
        tempDir= this.get_Dir(inputPath);
        if (tempDir==null){
            System.out.println("getFile Error; the given path is invalid.");
            return(null);
        }
        tempFile=tempDir.find_File(inputFileName);
        return(tempFile);
        
    }
    
    
    
    
    // below are output methods 

    // show the list of all Dirs and Files in a Dir. recursively defined. 
    // if fileOption is ture,  show all the files also. if it is false, show only directories.
    public void print_All(String[] input_Path, boolean fileOption) {
        int length= input_Path.length;
        String[] tempStringArray;
        tempStringArray=Func.sumArray(input_Path, this.getName());
        
        Func.printArray( tempStringArray);
       
        
         if (fileOption==true){
                for (int i = 1; i < numOfSubFile+1; i++) {        // first display the files in the current directory...
                    String[] temp;

                    temp= Func.sumArray (tempStringArray, this.find_File(i).getName());
                    Func.printArray_Last( temp);
                }
         }
        
    
        for (int i = 1; i < (numOfSubDir+1); i++) {// give the command to all the sub Dir  for recursive action....
             this.find_Dir(i).print_All(tempStringArray, fileOption);
        }
        return;
        
    }
     
    
    // this one is a restriction of the print_All.  displays only the files that contain certatin String in it.
    // if fileOption is true, search only through the file names, 
    // if fileOption is false, search only through the dirctory names.  
    public void print_All_Search (String[] input_Path, String input_String, boolean fileOption){ 
        int length= input_Path.length;
        String[] tempStringArray;
        tempStringArray=Func.sumArray(input_Path, this.getName());
        
        if ( fileOption==true){
            for (int i = 1; i < numOfSubFile+1; i++) {        // first display the files in the current directory...
                String[] temp;

                temp= Func.sumArray (tempStringArray, this.find_File(i).getName());
                Func.printArray_Search_Last( temp, input_String);
            }
        }
        
        if (fileOption==false){
            Func.printArray_Search_Last1 (tempStringArray, input_String);
        }
      
        for (int i = 1; i < (numOfSubDir+1); i++) {// give the command to all the sub Dir  for recursive action....
            this.find_Dir(i).print_All_Search(tempStringArray, input_String, fileOption);
        }
       
        return;
    }
        
        
    
    
       

    public void remove_All() {
        childDir=new Dir_Node();
        numOfSubDir=0;
        childFile=new File (this, "");
        numOfSubFile=0;
    }
    
    
    
           
            
    
}
