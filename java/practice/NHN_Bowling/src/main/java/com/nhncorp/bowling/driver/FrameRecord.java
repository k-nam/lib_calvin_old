package com.nhncorp.bowling.driver;



import com.nhncorp.bowling.room.BowlingLane;


public class FrameRecord {	
	private int first;
	private int second;
	
	public int getFirst() {
		return first;
	}
	
	public void setFirst(int first) {
		this.first = first;
	}
	
	public int getSecond() {
		return second;
	}
	
	public void setSecond(int second) {
		this.second = second;
	}
	public boolean isStrike() {
		return first == BowlingLane.BOWLING_INITIAL_NUM_PINS;
	}	
}
