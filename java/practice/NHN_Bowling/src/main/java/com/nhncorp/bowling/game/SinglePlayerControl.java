package com.nhncorp.bowling.game;


import com.nhncorp.bowling.game.frame.BowlingFrame;
import com.nhncorp.bowling.game.order.*;
import com.nhncorp.bowling.room.BowlingLaneId;
import com.nhncorp.bowling.score.FrameScores;

public class SinglePlayerControl {
	private BowlingPlayer 		bowlingPlayer;
	private BowlingLaneId			bowlingLaneId;
	private BowlingFrame[] 		bowlingFrames;
	public static final int 	TOTAL_NUMBER_OF_FRAMES = 12;
	public static final int		NORMAL_FRAMES_PER_GAME = 10;
	private int 							internalFrameNumber;
	private GameOrder 				currentOrder;
	
	public SinglePlayerControl(BowlingPlayer player,
			BowlingLaneId laneId) {
		bowlingPlayer = player;
		bowlingLaneId = laneId;
		bowlingFrames = new BowlingFrame[TOTAL_NUMBER_OF_FRAMES + 1];
		for (int i = 1; i < bowlingFrames.length; i++) {
			bowlingFrames[i] = new BowlingFrame();
		}
		for (int i = 1; i < bowlingFrames.length - 1; i++) {
			
			bowlingFrames[i].setNextFrame(bowlingFrames[i + 1]);
		}
		internalFrameNumber = 1;
	}
	
	public void startFrame() throws Exception {
		if (isGameFinished() == true || 
				getCurrentInternalFrame().isStarted() == true) {
			throw new Exception();
		} else {
			getCurrentInternalFrame().start();
			currentOrder = new FrameContinueOrder(bowlingPlayer, bowlingLaneId, 
					internalFrameNumber);
		}
	}

	public BowlingPlayer getPlayer() {
		return bowlingPlayer;
	}
	
	public BowlingLaneId getLaneId() {
		return bowlingLaneId;
	}
	
	public int getCurrentNormalFrameNumber() {
		if (internalFrameNumber < NORMAL_FRAMES_PER_GAME) {
			return internalFrameNumber;
		} else {
			return NORMAL_FRAMES_PER_GAME;
		}
	}
	 
	public void knockPins(int numberOfKnockedPins) {
		try {			
			getCurrentInternalFrame().knockPins(numberOfKnockedPins);
			if (getCurrentInternalFrame().isFinished() == true && 
					isGameFinished() == false) {
				internalFrameNumber++;
				if (internalFrameNumber > NORMAL_FRAMES_PER_GAME) {
					startFrame();
					return;
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}	
		if (isGameFinished()) {
			currentOrder = new GameFinishOrder(bowlingPlayer, bowlingLaneId);
		} else if (isNowPlaying()){
			currentOrder = new FrameContinueOrder(bowlingPlayer, bowlingLaneId,
					this.internalFrameNumber);
		}	else {
			currentOrder = new FrameFinishOrder(bowlingPlayer, bowlingLaneId);
		}
	}	 
	
	public GameOrder getOrder() throws Exception {
		if (currentOrder == null) {
			throw new Exception();
		}
		GameOrder orderToReturn = currentOrder;
		currentOrder = null;
		return orderToReturn;
	}
	
	public boolean isNowPlaying() {
		if (isGameFinished()) {
			return false;
		} else {
			return getCurrentNormalFrame().isStarted();
		}
	}
	
	public boolean isGameFinished() {
		return bowlingFrames[NORMAL_FRAMES_PER_GAME].isScoreFixed();
	}
	
	public FrameScores getScore() {
		FrameScores score = new FrameScores();
		for (int i = 1; i <= NORMAL_FRAMES_PER_GAME; i++ ) {
			try {
				score.setFrameScore(i, bowlingFrames[i].calculateScore());
			} catch (Exception e) {
				e.getCause();
			}
		}
		return score;
	}
	
	private BowlingFrame getCurrentInternalFrame() {
		return bowlingFrames[internalFrameNumber];
	}
	
	private BowlingFrame getCurrentNormalFrame() {
		return bowlingFrames[getCurrentNormalFrameNumber()];
	}
	
	public int getFrameScoreAt(int frameNumber) throws Exception {
		return bowlingFrames[frameNumber].calculateScore();
	}
	
	public int getTotalScore() {
		int totalScore = 0;
		for (int frameNumber = 1; frameNumber <= NORMAL_FRAMES_PER_GAME;
			frameNumber++) {
			try {
				totalScore += bowlingFrames[frameNumber].calculateScore();
			} catch (Exception e) {
				break;
			}
		}
		return totalScore;
	}
	
	public void setLaneId(BowlingLaneId laneId) {
		bowlingLaneId = laneId;
	}
}
