package com.nhncorp.bowling.game.order;

import com.nhncorp.bowling.game.BowlingPlayer;
import com.nhncorp.bowling.room.BowlingLaneId;

public class FrameContinueOrder extends GameOrder {
	private int frameNumber;	

	public FrameContinueOrder(BowlingPlayer player, 
			BowlingLaneId laneId, int frameNumber) {
		super(player, laneId);		
		this.bowlingLaneId = laneId;
		this.frameNumber = frameNumber;		
	}
	
	public int getFrameNumber() {
		return frameNumber;
	}

	public boolean isFrameFinished() {
		return false;
	}
	
	public boolean isGameFinished() {		
		return false;
	}
}
