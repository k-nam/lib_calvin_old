package com.nhncorp.bowling.driver;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Vector;

import com.nhncorp.bowling.main.HappyBeanAccount;

public class BettingManager {
	public final int BETTING_AMOUNT = 5000;
	private final HashSet<EmployeeId> totalEmployeeIds;
	private HashMap<EmployeeId, EmployeeId> bettingMap;
	private HashMap<EmployeeId, Vector<EmployeeId>> invertedBettingMap;
	private int currentBettingMoney = 0;
	private HappyBeanAccount happyBeanAccount;
	
	public BettingManager(EmployeeId[] totalEmployeeIds, 
			HappyBeanAccount happyBeanAccount) {
		this.totalEmployeeIds = new HashSet<EmployeeId>();
		for (int i = 0; i < totalEmployeeIds.length; i++) {
			this.totalEmployeeIds.add(totalEmployeeIds[i]);
		}
		bettingMap = new HashMap<EmployeeId, EmployeeId>();
		invertedBettingMap = new HashMap<EmployeeId, Vector<EmployeeId>>();
		this.happyBeanAccount = happyBeanAccount;
	}

	public boolean betting(EmployeeId bettorId, EmployeeId playerId) throws Exception {
		if (totalEmployeeIds.contains(bettorId) == false
				|| totalEmployeeIds.contains(playerId) == false) {
			throw new Exception();
		}
		if (bettingMap.containsKey(bettorId)) {
			return false;
		} else {
			bettingMap.put(bettorId, playerId);
			if (invertedBettingMap.get(playerId) == null) {
				invertedBettingMap.put(playerId, new Vector<EmployeeId>());
			}
			invertedBettingMap.get(playerId).add(bettorId);
			currentBettingMoney += BETTING_AMOUNT;
			return true;
		}
	}
	
	public void distributeMoney(EmployeeId firstWinnerId, 
			LogOnManager logOnManager) {
		if (firstWinnerId != null) { // There exists winner
			Vector<EmployeeId> winners = invertedBettingMap.get(firstWinnerId);
			if (winners == null) { // nobody bet right
			} else {
				int numberOfWinners = winners.size();
				int moneyPerWinner = this.currentBettingMoney / 2 / numberOfWinners;
				for (int i = 0; i < winners.size(); i++) {
					System.out.println("giving money");
					Employee winner = 
						logOnManager.getEmployeeFromEmployeeId(winners.get(i));
					winner.receiveBettingPrize(moneyPerWinner);
					this.currentBettingMoney -= moneyPerWinner;
				}
			}
		}
		try {
			this.happyBeanAccount.addMoney(this.currentBettingMoney);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
