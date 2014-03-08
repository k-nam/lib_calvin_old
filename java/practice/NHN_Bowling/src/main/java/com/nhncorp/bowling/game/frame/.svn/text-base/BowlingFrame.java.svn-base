package com.nhncorp.bowling.game.frame;

import com.nhncorp.bowling.room.BowlingLane;

public class BowlingFrame {	
	public enum Turns { Turns_NotBegun, 
		Turns_First, Turns_Second, Turns_Ended 
	}
	private Turns currentTurn;	
	private FrameType frameResult;	
	private BowlingFrame nextFrame;	
	private int[] knockedPins;
	
	public BowlingFrame() {
		currentTurn = Turns.Turns_NotBegun;
		knockedPins = new int[2];
		nextFrame = null;
		frameResult = null;
	}
	
	public BowlingFrame getNextFrame() {
		return nextFrame;
	}
	
	public boolean hasNextFrame() {
		return nextFrame != null;
	}
	public void setNextFrame(BowlingFrame nextFrame) {
		this.nextFrame = nextFrame;
	}
	
	public boolean isStarted() {
		return currentTurn != Turns.Turns_NotBegun;
	}
	
	public boolean isInFirstTurn() {
		return currentTurn == Turns.Turns_First;
	}
	
	public boolean isSecondTurn() {
		return currentTurn == Turns.Turns_Second;
	}
	
	public boolean isFinished() {
		return currentTurn == Turns.Turns_Ended;
	}
	
	public boolean isBowledAtLeastOnce() {
		return isSecondTurn() || isFinished();
	}
	
	public boolean isStrike() {
		if (currentTurn == Turns.Turns_Ended && frameResult.isStrike())
			return true;
		else {
			return false;
		}		 
	}
	
	public void start() {
		currentTurn = Turns.Turns_First;
	}
	
	public void knockPins(int numberOfKnockedPins) throws Exception {
		if (isStarted() != true) {
			throw new Exception();
		}
		if (currentTurn == Turns.Turns_First) {
			knockedPins[0] = numberOfKnockedPins;
			currentTurn = Turns.Turns_Second;
			if (numberOfKnockedPins == 
				BowlingLane.BOWLING_INITIAL_NUM_PINS) { // Strike
				currentTurn = Turns.Turns_Ended;
				frameResult = new StrikeFrame(this);				
			}
		} else if (currentTurn == Turns.Turns_Second){
			knockedPins[1] = numberOfKnockedPins;
			currentTurn = Turns.Turns_Ended;
			if (knockedPins[0] + knockedPins[1] == 
				BowlingLane.BOWLING_INITIAL_NUM_PINS) { // Spare
				frameResult = new SpareFrame(this);
			} else { // Open frame
				frameResult = new OpenFrame(this);
			}
		} else {
			throw new Exception();
		}
	}	

	public int getKnockedPins(Turns turn) throws Exception {
		if (isSecondTurn()) {
			if (turn == Turns.Turns_First) {
				return knockedPins[0];
			} 
		} else if (isFinished()) {
			if (turn == Turns.Turns_First) {
				return knockedPins[0];
			} else if (turn == Turns.Turns_Second && !isStrike()) {
				return knockedPins[1];
			}
		}
		throw new Exception();
	}	

	public int getTotalKnockedPins() throws Exception {
		if (isSecondTurn()) {
			return knockedPins[0];
		} else if (isFinished()) {
			return knockedPins[0] + knockedPins[1];
		}
		throw new Exception();
	}
	
	public int calculateScore() throws Exception {
		if (!isFinished()) {
			return getTotalKnockedPins();
		} else {
			return frameResult.calculateScore();
		}
	}
	
	public boolean isScoreFixed() {
		if (!isFinished()) {
			return false;
		} else {
			return frameResult.isScoreFixed();
		}
	}


	
	
}
