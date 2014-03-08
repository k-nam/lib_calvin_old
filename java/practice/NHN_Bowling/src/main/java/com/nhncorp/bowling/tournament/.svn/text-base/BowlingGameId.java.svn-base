package com.nhncorp.bowling.tournament;

public class BowlingGameId {
	private static int currentId = 0;
	private final int id;
	
	public static BowlingGameId getUniqueGameId() {
		return new BowlingGameId(currentId++);
	}
	
	public BowlingGameId(int id) {
		this.id = id;
	}
	
	public int getValue() {
		return id;
	}
}
