package com.nhncorp.bowling.game.order;

import com.nhncorp.bowling.game.BowlingPlayer;
import com.nhncorp.bowling.room.BowlingLaneId;

public abstract class GameOrder extends Order {	
	protected BowlingPlayer bowlingPlayer;
	protected BowlingLaneId bowlingLaneId;
	
	public GameOrder(BowlingPlayer player, BowlingLaneId laneId) {
		bowlingPlayer = player;	
		bowlingLaneId = laneId;
	}	
	
	public BowlingPlayer getPlayer() {
		return bowlingPlayer;
	}
	
	public BowlingLaneId getLaneId() {
		return this.bowlingLaneId;
	}

	public abstract boolean isFrameFinished();
	public abstract boolean isGameFinished();

}
