package com.nhncorp.bowling.game;

public class PlayerScorePair {
	private BowlingPlayer player;
	private int adjustedScore;
	
	public PlayerScorePair(BowlingPlayer player, int totalScore, int sumOfLastTwoFrames) {
		this.player = player;
		adjustedScore = totalScore * 100 + sumOfLastTwoFrames;
	}
	
	public BowlingPlayer getBowlingPlayer() {
		return player;
	}
	
	public int getAdjustedScore() {
		return adjustedScore;
	}
}
