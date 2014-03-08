package com.nhncorp.bowling.game.order;

import com.nhncorp.bowling.game.BowlingPlayer;

public class PlayerCheckOrder extends Order {

	private BowlingPlayer player;
	
	public PlayerCheckOrder (BowlingPlayer player) {
		this.player = player;
	}
	
	public boolean isFrameFinished() {
		return false;
	}

	public boolean isGameFinished() {
		return false;
	}

}
