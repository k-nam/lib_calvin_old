package com.nhncorp.bowling.game;

import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;

import com.nhncorp.bowling.game.order.GameOrder;
import com.nhncorp.bowling.game.schedule.LaneScheduler;
import com.nhncorp.bowling.game.schedule.Schedule;
import com.nhncorp.bowling.room.BowlingLaneId;
import com.nhncorp.bowling.score.FrameScores;
import java.util.Vector;

public class MultiPlayerControl {
	private BowlingPlayer[] bowlingPlayers;
	private BowlingLaneId[] bowlingLaneIds;
	private SinglePlayerControl[] singlePlayerControls;
	private int currentFrameNumber;
	private HashMap<BowlingLaneId, SinglePlayerControl> laneToControlMap;
	private Vector<GameOrder> currentOrders;
	private LaneScheduler scheduler;
	private boolean isSpecialBowlingMode = false;
	
	public MultiPlayerControl(BowlingPlayer[] bowlingPlayers,
			BowlingLaneId[] blowlingLaneIds, LaneScheduler scheduler) {
		this.bowlingPlayers = bowlingPlayers;
		this.bowlingLaneIds = blowlingLaneIds;
		singlePlayerControls = new SinglePlayerControl[bowlingPlayers.length];
		for (int i = 0; i < bowlingPlayers.length; i++) {
			singlePlayerControls[i] = 
				new SinglePlayerControl(bowlingPlayers[i], null);
		}
		currentFrameNumber = 1;
		laneToControlMap = new HashMap<BowlingLaneId, SinglePlayerControl>();
		currentOrders = new Vector<GameOrder>();
		this.scheduler = scheduler;
	}
	
	public void setSpecialMode() {
		this.isSpecialBowlingMode = true;
	}
	
	public BowlingPlayer getPlayerAt(int index) {
		return bowlingPlayers[index];
	}
	
	public BowlingLaneId getLaneAt(int index) {
		return bowlingLaneIds[index];
	}
	
	public Vector<GameOrder> getOrders() {
		Vector<GameOrder> ordersToReturn = 
			new Vector<GameOrder>(currentOrders);
		currentOrders.clear();
		return ordersToReturn;
	}
	
	public int getIndexOf(BowlingPlayer bowlingPlayer) throws Exception {
	  for (int i = 0; i < bowlingPlayers.length; i++) {
	  	if (bowlingPlayers[i].equals(bowlingPlayer)) {
	  		return i;
	  	}
	  }
	  throw new Exception();
	}
	
	public int getIndexOf(BowlingLaneId laneId) throws Exception {
	  for (int i = 0; i < bowlingLaneIds.length; i++) {
	  	if (bowlingLaneIds[i].equals(laneId)) {
	  		return i;
	  	}
	  }
	  throw new Exception();
	}

	public boolean isGameFinished() {
		boolean isFinished = true;
		for (int i = 0; i < singlePlayerControls.length; i++) {
			if (singlePlayerControls[i].isGameFinished() == false) {
				isFinished = false;
			}
		}
		return isFinished;
	}
	

	public void startFrame() throws Exception {
		if (this.isGameFinished() == true) {
			throw new Exception();
		}
		if (this.currentFrameNumber == 1) {
			laneSchedule();
			return;
		} else {
			for (int i = 0; i < bowlingLaneIds.length; i++) {
				SinglePlayerControl control = laneToControlMap.get(bowlingLaneIds[i]);
				control.startFrame();
				currentOrders.add(control.getOrder());
			}
		}
	}
	
	private int getCurrentFrameNumber() {
		return currentFrameNumber;
	}
	
	public void laneSchedule() {
		if (isGameFinished()) {
			return;
		}
		Schedule schedule;
		while (true) {
			schedule = scheduler.getNewSchedule();
			if (schedule == null) {
				return;
			}
			int playerNumber = schedule.getPlayerNumber();
			int laneNumber = schedule.getLaneNumber();
			if (singlePlayerControls[playerNumber].isGameFinished()) {
				scheduler.excludePlayer(playerNumber);
				scheduler.setFree(laneNumber);
				continue;
			} else {
				singlePlayerControls[playerNumber].setLaneId(getLaneAt(laneNumber));
				laneToControlMap.put(getLaneAt(laneNumber), 
						singlePlayerControls[playerNumber]);
				try {
					if (singlePlayerControls[playerNumber].getCurrentNormalFrameNumber() ==
						this.getCurrentFrameNumber()) {
						singlePlayerControls[playerNumber].startFrame();
						currentOrders.add(singlePlayerControls[playerNumber].getOrder());
					} else {
						if (this.isFrameFinished() == true 
							&& this.isSpecialBowlingMode == false) { // trigger new frame
							currentFrameNumber++;
							startFrame();
						}
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}		
	}
	
	public void knockPins(BowlingLaneId lane, int numberOfKnockedPins) throws Exception {
		SinglePlayerControl singlePlayerControl = laneToControlMap.get(lane);
		if (singlePlayerControl == null) {			
			throw new Exception();
		}
		singlePlayerControl.knockPins(numberOfKnockedPins);
		if (singlePlayerControl.isNowPlaying()) {
			try {
				currentOrders.add(singlePlayerControl.getOrder());
			} catch (Exception e) {
				e.printStackTrace();
			}
		} else {
			try {
				scheduler.setFree(getIndexOf(lane));
			} catch (Exception e) {
				e.printStackTrace();
			}
			laneSchedule();
		}		
	}

	public FrameScores[] getScores() {
		FrameScores[] scoreArray = new FrameScores[bowlingPlayers.length];
		for (int i = 0; i < bowlingPlayers.length; i++) {
			scoreArray[i] = singlePlayerControls[i].getScore();
		}
		return scoreArray;
	}
	
	public int getTotalScoreOf(BowlingPlayer player) throws Exception {
		return singlePlayerControls[getIndexOf(player)].getTotalScore();
	}
	
	public int getFrameScoreOf(BowlingPlayer player, int frameNumber) throws Exception {
		return singlePlayerControls[getIndexOf(player)].getFrameScoreAt(frameNumber);
	}
	
	public boolean isFrameFinished() {
		boolean isFinished = true;
		for (SinglePlayerControl singlePlayerControl :singlePlayerControls) {
			if (singlePlayerControl.isGameFinished() == true 
					|| singlePlayerControl.getCurrentNormalFrameNumber() > 
							this.getCurrentFrameNumber()) {
				// This SinglePlayerControl is finished with current frame
			} else{
				isFinished = false;
			}
		}
		return isFinished;
	}
	
	public BowlingPlayer[] getWinners() throws Exception {
		PlayerScorePair[] pairs = new PlayerScorePair[bowlingPlayers.length];
		for (int i = 0; i < pairs.length; i++) {
			pairs[i] = new PlayerScorePair(bowlingPlayers[i],
					singlePlayerControls[i].getTotalScore(),
					singlePlayerControls[i].getFrameScoreAt(9) +
					singlePlayerControls[i].getFrameScoreAt(10));
		}
		Comparator<PlayerScorePair> comparator = new PlayerScorePairComparator();
		Arrays.sort(pairs, comparator);
		BowlingPlayer[] winners = new BowlingPlayer[pairs.length];
		for (int i = 0; i < pairs.length; i++) {
			winners[i] = pairs[i].getBowlingPlayer();
		}
		return winners;
	}
	
}
