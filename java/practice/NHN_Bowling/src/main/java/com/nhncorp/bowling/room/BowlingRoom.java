package com.nhncorp.bowling.room;


public class BowlingRoom {
	private BowlingRoomId bowlingRoomId;
	private String name;
	private BowlingLane[] bowlingLanes;
	private boolean[] isLaneFree;
	public static final int MAXIMUM_NUMBER_OF_LANES_PER_ROOM = 20; 
	
	public BowlingRoom(BowlingRoomId roomId, String name, int numberOfLanes) throws Exception {
		this.bowlingRoomId = roomId;
		this.name = name;
		createBowlingLanes(numberOfLanes);
	}

	public BowlingRoomId getBowlingRoomId() {
		return bowlingRoomId;
	}
	
	public String getName() {
		return name;
	}
	
	private void createBowlingLanes(int numberOfLanes) throws Exception {
		if (numberOfLanes < 0 || numberOfLanes > MAXIMUM_NUMBER_OF_LANES_PER_ROOM) {
			throw new Exception();
		}
		this.bowlingLanes = new BowlingLane[numberOfLanes];
		isLaneFree = new boolean[numberOfLanes];
		for (int i = 0; i < numberOfLanes; i++) {
			isLaneFree[i] = true;
		}
	}

	public int getNumberOfLanes() {
		return bowlingLanes.length;
	}
	
	public BowlingLane getLaneAt(int laneNumber) {
		return bowlingLanes[laneNumber];
	}
	
	public boolean isLaneFree(int laneNumber) {
		return isLaneFree[laneNumber];
	}
	
	public void setLaneFree(int laneNumber) {
		isLaneFree[laneNumber] = true;
	}
	
	public void setLaneBusy(int laneNumber) {
		isLaneFree[laneNumber] = false;
	}
}
