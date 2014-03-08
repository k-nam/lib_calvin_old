package com.nhncorp.bowling.game.FitNesse;

import fit.ActionFixture;

import com.nhncorp.bowling.game.BowlingPlayer;
import com.nhncorp.bowling.game.SinglePlayerControl;
import com.nhncorp.bowling.game.order.FrameContinueOrder;
import com.nhncorp.bowling.game.order.GameOrder;
import com.nhncorp.bowling.room.BowlingLane;
import com.nhncorp.bowling.room.BowlingLaneId;
import com.nhncorp.bowling.score.FrameScores;

public class BowlingFrameQATest extends ActionFixture {	
	private SinglePlayerControl singlePlayerControl;
	private BowlingPlayer bowlingPlayer;
	private BowlingLaneId bowlingLaneId;
	private FrameContinueOrder startOrder;
	private GameOrder order = null;	
	private int gameScore;
	
	public void prepareBowlingGame() throws Exception {		
		bowlingPlayer = BowlingPlayer.getUniqueBowlingPlayer();
		bowlingLaneId = BowlingLaneId.getUniqueBowlingLaneId();
		
		singlePlayerControl = 
			new SinglePlayerControl(bowlingPlayer, bowlingLaneId);
		singlePlayerControl.startFrame();
		startOrder = (FrameContinueOrder)singlePlayerControl.getOrder();
	}	
	
	public void gameStart() throws Exception{
			singlePlayerControl.startFrame();
	}
		
	public void knockPins(int record){
			singlePlayerControl.knockPins(record);
	}
	
	public boolean isFrameFinished() throws Exception {
			order = singlePlayerControl.getOrder();
			return order.isFrameFinished();
	}
	
	public boolean isGameFinished() throws Exception {
		order = singlePlayerControl.getOrder();
		return order.isGameFinished();
	}	
	
	public void calculateFrameScore(int frameNo) throws Exception {
		gameScore = singlePlayerControl.getFrameScoreAt(frameNo);
	}
		
	public int getFrameScore() throws Exception {
		return gameScore;
	}
	
	public int getBowlingGameScore() {
		return singlePlayerControl.getTotalScore();
	}
}
