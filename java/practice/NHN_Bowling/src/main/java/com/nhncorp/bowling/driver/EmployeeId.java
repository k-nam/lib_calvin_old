package com.nhncorp.bowling.driver;

public class EmployeeId {
	private static int currentId = 0;
	private final int id;
	
	public static EmployeeId getUniqueEmployeeId() {
		return new EmployeeId(currentId++);
	}
	
	public EmployeeId(int id) {
		this.id = id;
	}
	
	public int getValue() {
		return id;
	}
}
