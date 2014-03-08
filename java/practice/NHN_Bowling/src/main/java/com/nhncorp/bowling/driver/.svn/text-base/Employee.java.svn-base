package com.nhncorp.bowling.driver;

public class Employee {
	private final EmployeeId employeeId;
	private final int age;
	public enum Gender { Male, Female };
	private final Gender gender;
	private final String name;
	
	public static Employee getUniqueEmployee() {
		return new Employee(EmployeeId.getUniqueEmployeeId(), 0, Gender.Male, "NHN Employee");
	}
	
	public Employee(EmployeeId employeeId, int age, Gender gender, String name) {
		this.employeeId = employeeId;
		this.age = age;
		this.gender = gender;
		this.name = name;
	}
	
	public Employee(Employee employee) {
		this.employeeId = employee.employeeId;
		this.age = employee.age;
		this.gender =employee. gender;
		this.name = employee.name;
	}
	
	public EmployeeId getEmployeeId() {
		return employeeId;
	}
	
	public int getAge() {
		return age;
	}
	
	public Gender getGender() {
		return gender;
	}
	
	public String getName() {
		return name;
	}

	public void receiveBettingPrize(int moneyPerWinner) {
		System.out.println("I am " + name + " I won betting and received: " +
				moneyPerWinner);		
	}
	
	public void receiveGamePrize(int prizeMoney) {
		System.out.println("I am " + name + " I won bowling tournament and received: " +
				prizeMoney);
	}
	
}
