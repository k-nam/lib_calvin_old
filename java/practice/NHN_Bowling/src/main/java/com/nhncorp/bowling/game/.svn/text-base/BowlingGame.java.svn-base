package com.nhncorp.bowling.game;

import java.util.Calendar;
import java.util.Vector;

import com.nhncorp.bowling.driver.Employee;
import com.nhncorp.bowling.driver.EmployeeId;
import com.nhncorp.bowling.game.order.GameOrder;
import com.nhncorp.bowling.game.schedule.LaneScheduler;
import com.nhncorp.bowling.main.HappyBeanAccount;
import com.nhncorp.bowling.room.BowlingLaneId;
import com.nhncorp.bowling.tournament.BowlingGameId;
import com.nhncorp.bowling.tournament.GameSkeleton;


public class BowlingGame {
	private BowlingGameId bowlingGameId;
	private BowlingPlayer[] initialPlayers;
	private Vector<BowlingPlayer> players;
	private MultiPlayerControl multiPlayerControl;
	private BowlingLaneId[] bowlingLaneIds;
	private boolean isStarted = false;
	private boolean isFinished = false;
	private Calendar startDate = null;
	private Calendar finishDate = null;
	private BowlingPlayer firstWinner = null;
	private BowlingPlayer secondWinner = null;
	private HappyBeanAccount happyBeanAccount;
	private boolean isFinalGame;
	private double TOURNAMENT_WINNER_PRIZE_RATIO[];
	
	public static final double FIRST_WINNER_PRIZE_MONEY_RATIO = 0.7;
	public static final double SECOND_WINNER_PRIZE_MONEY_RATIO = 0.3;
	
	public BowlingGame(BowlingGameId gameId, BowlingPlayer[] players) throws Exception {
		if (players.length <= GameSkeleton.MAX_NUMBER_OF_WINNERS) {
			throw new Exception();
		}
		if (players.length > GameSkeleton.MAX_NUMBER_OF_PLAYERS) {
			throw new Exception();
		}
		this.bowlingGameId = gameId;
		this.initialPlayers = players;
		this.players = new Vector<BowlingPlayer>();
		for (BowlingPlayer player : players) {
			this.players.add(player);
		}
		isFinalGame = false;
		TOURNAMENT_WINNER_PRIZE_RATIO = new double[] { 0.2, 0.1, 0.05, 0.02 };
	}
	
	public BowlingGame(BowlingGameId gameId, BowlingPlayer[] players, boolean isFinalGame) throws Exception {
		System.out.println("starting bowling game" + gameId.getValue());

		this.bowlingGameId = gameId;
		this.initialPlayers = players;
		this.players = new Vector<BowlingPlayer>();
		for (BowlingPlayer player : players) {
			this.players.add(player);
		}
		this.isFinalGame = isFinalGame;
		TOURNAMENT_WINNER_PRIZE_RATIO = new double[] { 0.2, 0.1, 0.05, 0.02 };

	}
	
	public void setHappyBeanAccount(HappyBeanAccount happyBeanAccount) {
		this.happyBeanAccount = happyBeanAccount;
	}
	
	public BowlingGameId getBowlingGameId() {
		return bowlingGameId;
	}
	
	public int getNumberOfPlayers() {
		return initialPlayers.length;
	}
	
	public void setLaneIds(BowlingLaneId[] bowlingLaneIds) {
		this.bowlingLaneIds = bowlingLaneIds;
	}
	
	public void notifyAbsentPlayer(EmployeeId employeeId) throws Exception {
		if (isStarted()) {
			throw new Exception();
		}
		for (int i = 0; i < players.size(); i++) {
			if (players.get(i).getEmployeeId().equals(employeeId)) {
				players.remove(i);
				return;
			}
		}
		throw new Exception();
	}
	
	public boolean isStarted() {
		return isStarted;
	}
	
	public boolean isFinished() {
		return isFinished;
	}
	
	public void startGame() throws Exception {	
		// Mark start date
		this.startDate = Calendar.getInstance();		
		if (isStarted() == true) {
			throw new Exception();
		}
		isStarted = true;
		if (players.size() == 1) {
			firstWinner = players.get(0);
		}
		if (players.size() <= 1) {
			this.postGameProcess();
			return;
		}
		
		// Normal game flow
		multiPlayerControl = 
			new MultiPlayerControl(
					players.toArray(new BowlingPlayer[players.size()]),
					this.bowlingLaneIds, 
					new LaneScheduler(players.size(), bowlingLaneIds.length));
		multiPlayerControl.startFrame();
	}
	
	public Vector<GameOrder> getOrders() throws Exception {
		return multiPlayerControl.getOrders();
	}
	
	public void knockPins(BowlingLaneId laneId, int knockedPins) throws Exception {
		multiPlayerControl.knockPins(laneId, knockedPins);
		if (multiPlayerControl.isGameFinished() == true) {
			isFinished = true;
			this.decideWinners();
			this.postGameProcess();
		}
	}
	
	private void setFirstWinner(BowlingPlayer firstWinner) throws Exception {
		this.firstWinner = firstWinner;
	
	}
	
	private void setSecondWinner(BowlingPlayer secondWinner) throws Exception {
		this.secondWinner = secondWinner;
	
	}
	
	public void postGameProcess() throws Exception {

		isFinished = true;
		if (this.isFinalGame == true) {
			System.out.println("Final game");
			BowlingPlayer[] winners = multiPlayerControl.getWinners();
			for (int i = 0; i < winners.length && i < 4; i++) {
				Employee thisEmployee = winners[i].getEmployee();
				int moneyToGive = (int)(winners[i].getPrizeMoney() * 
						TOURNAMENT_WINNER_PRIZE_RATIO[i]);
				thisEmployee.receiveGamePrize(moneyToGive);
				winners[i].loseMoney(moneyToGive);
			}
			for (int i = 0; i < this.players.size(); i++) {
				int remaininMoney = players.get(i).getPrizeMoney();
				this.happyBeanAccount.addMoney(remaininMoney);
				players.get(i).loseMoney(remaininMoney);
			}
		} else {		
			int prizeMoney = 0;
			EmployeeId[] winners = getWinners();
			// Take prize money from the losers
			for (BowlingPlayer player : initialPlayers) {
				boolean isWinner = false;
				for (int i = 0; i < winners.length; i++) {
					if (player.getEmployeeId() == winners[i]) {
						isWinner = true;
					}
				}
				if (isWinner == false) {
					int hisMoney = player.getPrizeMoney();
					prizeMoney += hisMoney;
					player.loseMoney(hisMoney);
				}					
			}
			// Distribute prize money to winners
			if (winners.length == 0) {	
				if (happyBeanAccount != null) {
					happyBeanAccount.addMoney(prizeMoney);
				}
			
			} else if (winners.length == 1){
				firstWinner.gainMoney(prizeMoney);
			} else {
				firstWinner.gainMoney((int)(prizeMoney * FIRST_WINNER_PRIZE_MONEY_RATIO));
				secondWinner.gainMoney((int)(prizeMoney * SECOND_WINNER_PRIZE_MONEY_RATIO));
			}
		}
		
		// Mark finish date
		this.finishDate = Calendar.getInstance();
	}
	
	public EmployeeId[] getWinners() throws Exception {
		if (isFinished() == false) {
			throw new Exception();
		}
		if (firstWinner == null) {
			return new EmployeeId[0];
		}
		if (secondWinner == null) {
			EmployeeId[] winners = new EmployeeId[1];
			winners[0] = firstWinner.getEmployeeId();
			return winners;
		} else {
			EmployeeId[] winners = new EmployeeId[2];
			winners[0] = firstWinner.getEmployeeId();
			winners[1] = secondWinner.getEmployeeId();
			return winners;
		}
	}

	public void decideWinners() throws Exception {
		BowlingPlayer[] winners = multiPlayerControl.getWinners();
		setFirstWinner(winners[0]);
		setSecondWinner(winners[1]);
	
	}
	
}
