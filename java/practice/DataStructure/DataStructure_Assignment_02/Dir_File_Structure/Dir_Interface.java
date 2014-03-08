package Dir_File_Structure;



public interface Dir_Interface {
    
    public int num_Of_Sub_Dir ();
    
   
    
    
    public Dir make_Dir (String [] input_Path);
    
    public void remove_Dir (String[] input_Path);
    
    public Dir get_Dir (String [] input_Path);
    
    
    public File makeFile (String [] input_Path, String fileName);
    
    public void removeFile (String [] input_Path, String fileName);
    
    public File getFile (String [] input_Path, String fileName);
    
    
    
    
    
    
    public void print_All (String[] input_Path, boolean fileOption);    // whether to show its own 'dir' or show all of its sub dirs and files
    
    public void print_All_Search (String[] input_Path, String input_String, boolean fileOption);
    public void remove_All();
           
    
}
