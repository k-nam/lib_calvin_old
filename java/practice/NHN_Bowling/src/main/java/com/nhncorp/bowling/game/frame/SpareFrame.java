package com.nhncorp.bowling.game.frame;

import com.nhncorp.bowling.game.frame.BowlingFrame.Turns;

public class SpareFrame extends FrameType {	
	public SpareFrame(BowlingFrame frame) {
		super(frame);
	}
	
	public int getBonus() {
		BowlingFrame nextFrame = super.frame.getNextFrame();
		int bonus = 0;
		if (nextFrame != null) {
			try {
				if (nextFrame.isSecondTurn() || nextFrame.isFinished()) {
					bonus = nextFrame.getKnockedPins(Turns.Turns_First);
				}
			} catch (Exception e) {			
				e.printStackTrace();
			} 
		}
		return bonus;
	}

	@Override
	public boolean isScoreFixed() {
		if (frame == null) {
			return false;
		}
		if (!frame.hasNextFrame() 
				|| !frame.getNextFrame().isBowledAtLeastOnce()) {
			return false;
		} else {
			return true;
		}
	}	
	
}
