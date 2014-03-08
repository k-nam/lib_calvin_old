package com.nhncorp.bowling.room;

public class BowlingRoomId {
	private static int currentId = 0;
	private final int id;
	
	public static BowlingRoomId getUniqueBowlingRoomId() {
		return new BowlingRoomId(currentId++);
	}
	
	public BowlingRoomId(int id) {
		this.id = id;
	}
	
	public int getValue() {
		return id;
	}
}
