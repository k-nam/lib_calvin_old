package Dir_File_Structure;



public class File {
    
    private Object item;      //each file has an target object
    private String fileName;
    private File nextFile;
    private Dir upperDir;
    
    
    
    
    
    
    public File(Dir inputDir, String inputName) {
        upperDir=inputDir;
        fileName=inputName;
    }
    
    
    public void setNext(File inputFile){
        nextFile=inputFile;
    }
    
    public File getNext (){
        return (nextFile);
    }
    
    public Object getItem (){
        return (item);
    }
    
    public String getName (){
        return (fileName);
    }
    
    
    
                
    
}
