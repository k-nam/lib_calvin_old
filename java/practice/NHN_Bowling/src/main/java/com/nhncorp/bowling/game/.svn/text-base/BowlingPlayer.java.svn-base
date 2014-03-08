package com.nhncorp.bowling.game;

import com.nhncorp.bowling.driver.Employee;
import com.nhncorp.bowling.driver.EmployeeId;

public class BowlingPlayer {
	private Employee employee;
	private int prizeMoney;
	
	public static BowlingPlayer getUniqueBowlingPlayer() {
		return new BowlingPlayer(Employee.getUniqueEmployee(), 0);
	}
	
	public static BowlingPlayer getUniqueBowlingPlayer(int prizeMoney) {
		return new BowlingPlayer(Employee.getUniqueEmployee(), prizeMoney);
	}
	
	public BowlingPlayer(Employee applicant, int applyingFee) {
		this.employee = applicant;
		this.prizeMoney = applyingFee;		
	}
	
	public EmployeeId getEmployeeId() {
		return employee.getEmployeeId();
	}
	
	public Employee getEmployee() {
		return this.employee;
	}
	
	public int getPrizeMoney() {
		return prizeMoney;
	}

	public void loseMoney(int lostMoney) throws Exception {
		if (lostMoney <= prizeMoney && lostMoney >= 0) {
			prizeMoney -= lostMoney;
		} else {
			throw new Exception();
		}
	}

	public void gainMoney(int gainedMoney) throws Exception {
		if (gainedMoney >= 0) {
			prizeMoney += gainedMoney;
		} else {
			throw new Exception();
		}
	}
}
