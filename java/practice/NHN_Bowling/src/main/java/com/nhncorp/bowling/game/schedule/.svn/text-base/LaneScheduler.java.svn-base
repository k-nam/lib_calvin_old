package com.nhncorp.bowling.game.schedule;

import java.util.ArrayDeque;



public class LaneScheduler {
	private int numberOfPlayers;
	private int numberOfLanes;
	private int[] assignedPlayers;
	private int[] minimumPlayerNumberOfLanes;
	private int[] maximumPlayerNumberOfLanes;
	private boolean[] isPlayingPlayer;
	private ArrayDeque<Schedule> schedules;
	
	public LaneScheduler(int numberOfPlayers, int numberOfLanes) throws Exception {
		this.numberOfPlayers = numberOfPlayers;
		this.numberOfLanes = numberOfLanes;
		assignedPlayers = new int[numberOfLanes];
		minimumPlayerNumberOfLanes = new int[numberOfLanes];
		maximumPlayerNumberOfLanes = new int[numberOfLanes];
		schedules = new ArrayDeque<Schedule>(); 
		initialize();
	}
	
	public void initialize() throws Exception {
		if (numberOfPlayers < numberOfLanes) {
			throw new Exception();
		}
		int minimumNumberOfPlayersPerLane = numberOfPlayers / numberOfLanes;
		int residue = 
			numberOfPlayers - numberOfLanes * minimumNumberOfPlayersPerLane;
		int currentIndex = 0;
		int[] numberOfPlayersAtLanes = new int[numberOfLanes];		
		for (int i = 0; i < numberOfLanes; i++) {
			numberOfPlayersAtLanes[i] = minimumNumberOfPlayersPerLane;
			if (i < residue) {
				numberOfPlayersAtLanes[i]++;
			} 
			minimumPlayerNumberOfLanes[i] = currentIndex;
			assignedPlayers[i] = currentIndex;
			maximumPlayerNumberOfLanes[i] = 
				currentIndex + numberOfPlayersAtLanes[i] - 1;
			currentIndex = maximumPlayerNumberOfLanes[i] + 1;			
			schedules.add(new Schedule(assignedPlayers[i], i));
		}		
		isPlayingPlayer = new boolean[numberOfPlayers];
		for (int i = 0; i < numberOfPlayers; i++) {
			isPlayingPlayer[i] = true;
		}
	}
	
	public Schedule getNewSchedule() {
		if (schedules.isEmpty()) {
			return null;
		} else {
			return schedules.poll();
		}
	}
	
	public void setFree(int laneNumber) {		
		int nextPlayer;
		try {
			nextPlayer = getNextPlayerAtLane(laneNumber);
			assignedPlayers[laneNumber] = nextPlayer;
		} catch (Exception e) { // this lane has no more to do 
			e.getClass();
			return;
		}
		schedules.add(new Schedule(nextPlayer, laneNumber));
	}

	public void excludePlayer(int playerNumber) {
		isPlayingPlayer[playerNumber ] = false;
	}	
	
	// Throws when all players are finished
	private int getNextPlayerAtLane(int laneNumber) throws Exception {
		int candidatePlayer = 
			getNextNumberAtLane(assignedPlayers[laneNumber], laneNumber);
		while (candidatePlayer != assignedPlayers[laneNumber]) {			
			if (isPlayingPlayer[candidatePlayer] == true) {
				return candidatePlayer;
			} else {
				candidatePlayer = getNextNumberAtLane(candidatePlayer, laneNumber);
			}
		}
		if (isPlayingPlayer[candidatePlayer]) {
			return candidatePlayer;
		} else {
			throw new Exception();
		}
	}
	
	private int getNextNumberAtLane(int playerNumber, int laneNumber) {
		if (playerNumber !=	maximumPlayerNumberOfLanes[laneNumber]) {
			return playerNumber + 1;
		} else {
			return minimumPlayerNumberOfLanes[laneNumber];
		}
	}
}
