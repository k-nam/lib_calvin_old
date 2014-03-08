package com.nhncorp.bowling.room;

public class BowlingLaneId {
	private static int currentId = 0;
	private final int id;
	
	public static BowlingLaneId getUniqueBowlingLaneId() {
		return new BowlingLaneId(currentId++);
	}
	
	public BowlingLaneId(int id) {
		this.id = id;
	}
	
	public int getValue() {
		return id;
	}
}
