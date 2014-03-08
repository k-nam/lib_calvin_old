package com.nhncorp.bowling.game.schedule;

public class Schedule {
	private int playerNumber;
	private int laneNumber;

	public Schedule(int playerNumber, int laneNumber) {
		this.playerNumber = playerNumber;
		this.laneNumber = laneNumber;
	}
	
	public int getPlayerNumber() {
		return playerNumber;
	}

	public int getLaneNumber() {
		return laneNumber;
	}

}
