package com.nhncorp.bowling.main;

public class HappyBeanAccount {
	private int currentSum = 0;
	
	public void donateMoney() {
		int additionalDonation = (int)(currentSum * 0.5);
		System.out.println("NHN corp has donated " + additionalDonation 
				+ " to HappyBean!");
		currentSum += additionalDonation;
		System.out.println("I am HappyBean! Now, NHN Bowling donated: " + currentSum);
		currentSum = 0;
	}
	
	public void addMoney(int amount) throws Exception {
		if (amount < 0) {
			throw new Exception();
		} else {
			System.out.println("I am HappyBean! Adding: " + amount);
			currentSum += amount;
		}
	}
}
