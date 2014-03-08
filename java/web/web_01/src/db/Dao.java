package db;

public class Dao {
	public Dao(String entryName) {
		this.entryName = entryName;
	}
	
	public String getEntryName() {
		return this.entryName;
	}
	
	private String entryName;
	
}
