package bean;

public class User {
	public User() {
		
	}
	
	public User(String id, String name) {
		this.id = id;
		this.name = name;
		this.hobbies = null;
	}
	
	public String getId() {
		return this.id;
	}
	
	public String getName() {
		return this.name;
	}
	
	public String[] getHobbies() {
		return this.hobbies;
	}
	
	public void setHobbies(String[] hobbies) {
		this.hobbies = hobbies;
	}
	
	private String id;
	private String name;
	private String[] hobbies;
}
