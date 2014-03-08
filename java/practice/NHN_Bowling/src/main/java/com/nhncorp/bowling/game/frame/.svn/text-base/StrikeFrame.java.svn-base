package com.nhncorp.bowling.game.frame;

import com.nhncorp.bowling.game.frame.BowlingFrame.Turns;
import com.nhncorp.bowling.room.BowlingLane;


public class StrikeFrame extends FrameType {		
	public StrikeFrame(BowlingFrame frame) {
		super(frame);
	}
	
	public boolean isStrike() {
		return true;
	}
	
	public int getBonus() {
		BowlingFrame frame = super.frame;
		int bonus = 0;
		if (frame.hasNextFrame()) {
			BowlingFrame nextFrame = frame.getNextFrame();
			if (nextFrame.isStrike()) { // next frame is another strike
				bonus += BowlingLane.BOWLING_INITIAL_NUM_PINS;
				try {
					if (nextFrame.getNextFrame() != null
							&& nextFrame.getNextFrame().isBowledAtLeastOnce()) {
							bonus += nextFrame.getNextFrame().getKnockedPins(Turns.Turns_First);
					}
				} catch (Exception e){
					e.printStackTrace();
				}
			} else { // next frame is NOT a strike
				try {
					if (nextFrame.isBowledAtLeastOnce()) {
						bonus += nextFrame.getTotalKnockedPins();
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
		return bonus;
	}

	@Override
	public boolean isScoreFixed() {
		BowlingFrame frame = super.frame;
		BowlingFrame nextFrame = frame.getNextFrame();
		if (!frame.hasNextFrame() || !nextFrame.isFinished()) {
			return false;
		} else if (!nextFrame.isStrike()) {
			return true;
		} else if (nextFrame.hasNextFrame() 
				&& nextFrame.getNextFrame().isBowledAtLeastOnce()) {
				return true;
		} else {
			return false;
		}
	}
}
