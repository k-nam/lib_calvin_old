package com.nhncorp.bowling.room;

public class BowlingLane {
	private BowlingLaneId bowingLaneId;
	private int numberOfPins;
	public static final int BOWLING_INITIAL_NUM_PINS = 10;
	
	public BowlingLane(BowlingLaneId laneId) {
		this.bowingLaneId = laneId;
		numberOfPins = BOWLING_INITIAL_NUM_PINS;
	}
	public static BowlingLane getUniqueBowlingLane() {
		return new BowlingLane(BowlingLaneId.getUniqueBowlingLaneId());
	}
	
	public BowlingLaneId getLaneId() {
		return bowingLaneId;
	}
	
	public int getRemainingNumberOfPins() {
		return numberOfPins;
	}
	
	public void knockPins(int numberOfKnockedPins) throws Exception  {
		if (numberOfKnockedPins >= 0 
				&& numberOfKnockedPins <= numberOfPins) {
			numberOfPins -= numberOfKnockedPins;
		} else {
			throw (new Exception());
		}
	}
	
	public void reset() {
		numberOfPins = BOWLING_INITIAL_NUM_PINS;
	}
}
