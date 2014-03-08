package com.nhncorp.bowling.game.frame;


public abstract class FrameType {
	protected BowlingFrame frame;		
	
	public FrameType(BowlingFrame frame) {
		this.frame = frame;
	}
	
	public boolean isStrike() {
		return false;
	}
	
	public abstract int getBonus();
	
	public int calculateScore() throws Exception {
		return frame.getTotalKnockedPins() + getBonus();
	}

	public abstract boolean isScoreFixed();
}
