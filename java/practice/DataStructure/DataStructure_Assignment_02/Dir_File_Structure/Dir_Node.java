package Dir_File_Structure;

import java.util.Vector;
import java.util.HashMap;

public class Dir_Node {
    
    private Dir nextDir;
    
    

    public Dir_Node() {
        nextDir=null;

    }
    
    
    public void setNext (Dir input_nextDir){
        nextDir=input_nextDir;
    }
    
    public Dir getNext(){
        return ( nextDir);
    }
    
}
