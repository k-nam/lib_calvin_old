package logic;

public class Planner {
	public String recommendSubject(int age) {
		if (age < 20) {
			return "algorithm";
		} else if (age < 40) {
			return "systems programming";		
		} else {
			return "web programming";
		}
	}
}
