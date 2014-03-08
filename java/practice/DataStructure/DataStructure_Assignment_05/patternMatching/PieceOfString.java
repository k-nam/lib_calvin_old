package patternMatching;

public class PieceOfString {
	
	private final String key;
	// this is a pair of integers (they are NOT key)
	private final int[] pair;
	private PieceOfString next;
	
	
	// pair is a pair of integers, which indicates (row, column) of the location where this 
	// PieceOfString is from (in the input text or pattern)
	public PieceOfString(String inputKey, int[] inputPair) {
		if(inputPair.length!=2){
			System.out.println("Internal error in PieceOfString.constructor");
		}
		key=inputKey;
		pair=inputPair;
		next=null;
		// TODO Auto-generated constructor stub
	}
	public String getKey(){
		return(key);
	}
	
	public int[] getPair(){
		return(pair);
	}
	
	public void setNext(PieceOfString inputNext){
		next=inputNext;
	}
	
	public PieceOfString getNext(){
		return (next);
	}

}
