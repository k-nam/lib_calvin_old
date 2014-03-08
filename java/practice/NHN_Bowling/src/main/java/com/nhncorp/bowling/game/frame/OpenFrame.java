package com.nhncorp.bowling.game.frame;

public class OpenFrame extends FrameType {	
	public OpenFrame(BowlingFrame frame) {
		super(frame);
	}
	
	public int getBonus() {
		return 0;
	}

	@Override
	public boolean isScoreFixed() {
		if (frame != null) {
			return frame.isFinished();
		} else {
			return false;
		}
	}
}