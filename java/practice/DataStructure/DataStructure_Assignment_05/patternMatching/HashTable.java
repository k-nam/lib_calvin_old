package patternMatching;

public class HashTable {
	
	private final static int SIZE_OF_HASH_TABLE=1;
	
	private AVL_Tree[] hashTable;
	
	private int size;
	
	
	
	
	public HashTable(){
		hashTable=new AVL_Tree[SIZE_OF_HASH_TABLE];
		for(int i=0; i<SIZE_OF_HASH_TABLE ; i++){
			hashTable[i]=new AVL_Tree();
		}
		size=0;
	}
	
	
	
	public void insert(PieceOfString newItem){
		size++;
		int hashKey;
		hashKey=this.hashFunction(newItem.getKey());
		hashTable[hashKey].insert(newItem);
		return;
		
	}
	
	
	
	public ListOfPieceOfString retrieve (String searchKey){
		int hashKey;
		hashKey=this.hashFunction(searchKey);
		ListOfPieceOfString out;
		out=hashTable[hashKey].retrieve(searchKey);
		
		return(out);
	}
	
	
	private int hashFunction(String inputString){
		
		int length=inputString.length();
		
		// sum is the sum of the ACSII values of all chars inthe inputString
		// this sum is used as the key for computing the hash key
		int sum=0;
		for (int i=0 ; i <length ; i++){
			char temp;
			temp=inputString.charAt(i);
			int a;
			a=(int)temp;
			sum+=a;
		}
		
		int hashKey=sum%SIZE_OF_HASH_TABLE;
		
		
		return(hashKey);
	}
	
	

}
