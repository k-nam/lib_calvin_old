package com.nhncorp.bowling.game.order;

import com.nhncorp.bowling.game.BowlingPlayer;
import com.nhncorp.bowling.room.BowlingLaneId;

public class FrameFinishOrder extends GameOrder {
	public FrameFinishOrder(BowlingPlayer player, BowlingLaneId laneId) {
		super(player, laneId);
	}
	
	public boolean isFrameFinished() {
		return true;
	}
	
	public boolean isGameFinished() {
		return false;
	}
}
