package patternMatching;

public class ListOfPieceOfString {
	
	private String key;
	private PieceOfString first;
	private PieceOfString last;
	private int length;

	public ListOfPieceOfString(PieceOfString newString) {
		key=newString.getKey();
		first=newString;
		last=newString;
		length=0;
		
		// TODO Auto-generated constructor stub
	}
	
	public String getKey(){
		return(key);
	}
	
	public int getLength(){
		return(length);
	}
	
	public void insert(PieceOfString input){
		length++;
		last.setNext(input);
		last=input;
	}
	
	
	// See if the PieceOfString is actually in the given pair (row, column)
	public boolean isThere (int[] location){
		if(location.length!=2){
			System.out.println("Internal error in ListOfPieceOfString.isThere");
			return(false);
		}
		
		PieceOfString temp;
		temp=first;
		while (temp!=null){
			if (temp.getPair()[0]==location[0]&&temp.getPair()[1]==location[1]) {
				return(true);
			}
			temp=temp.getNext();
		}
		
		return(false);
	}
	
	

}
