package com.nhncorp.bowling.tournament;

import java.util.HashSet;
import com.nhncorp.bowling.driver.EmployeeId;

public class GameSkeleton {
	private final BowlingGameId gameId;
	private HashSet<EmployeeId> players;	
	private GameSkeleton parent = null;
	private GameSkeleton[] children;
	private boolean isFinished = false;
	
	public static final int MAX_NUMBER_OF_PLAYERS = 8;
	public static final int MAX_NUMBER_OF_WINNERS = 2;
	public static final int MAX_NUMBER_OF_CHILDERN = 4;
	
	public GameSkeleton(BowlingGameId gameId) {
		this.gameId = gameId;
		this.players = new HashSet<EmployeeId>();
		children = new GameSkeleton[MAX_NUMBER_OF_CHILDERN];
		for (int i = 0; i < children.length; i++) {
			children[i] = null;
		}
	}
	
	public BowlingGameId getGameId() {
		return gameId;
	}
	
	public EmployeeId[] getPlayers() {
		return players.toArray(new EmployeeId[players.size()]);
	}
	
	public int getNumberOfPlayers() {
		return players.size();
	}
	
	public void addChild(GameSkeleton child) throws Exception {
		for (int i = 0; i < children.length; i++) {
			if (children[i] == null) {
				children[i] = child;
				return;
			}
		}
		throw new Exception();
	}
	
	public void setParent(GameSkeleton parent) {
		this.parent = parent;
	}
	
	public GameSkeleton getParent() {
		return parent;
	}
	
	public boolean isFinished() {
		return isFinished;
	}
	
	public void notifyWinners(EmployeeId[] winners) throws Exception {
		if (this.isFinished == true) {
			throw new Exception();
		}
		this.isFinished = true;
		if (getParent() != null) {
			for (EmployeeId winner : winners) {
				if (players.contains(winner) == false) {
					throw new Exception();
				} else {
					getParent().addPlayer(winner);
				}
			}			
			getParent().cutOffChild(this);
		}
	}

	private void cutOffChild(GameSkeleton childToCutOff) throws Exception {
		for (int i = 0; i < children.length; i++) {
			if (children[i] == childToCutOff) {
				children[i] = null;
				return;
			}
		}
		throw new Exception();
	}

	public void addPlayer(EmployeeId winner) throws Exception {
		if (players.contains(winner) || players.size() >= MAX_NUMBER_OF_PLAYERS) {
			throw new Exception();
		} else {
			players.add(winner);
		}
	}
	
	public boolean isReadyToStart() {
		boolean isReady = true;
		for (int i = 0; i < children.length; i++) {
			if (children[i] != null) {
				isReady = false;
			}
		}
		return isReady;
	}
	
	public boolean isDefaultWin() {
		if (isReadyToStart() && getNumberOfPlayers() <= MAX_NUMBER_OF_WINNERS) {
			return true;
		} else {
			return false;
		}
			
	}
	
	public boolean isFinalGame() {
		if (parent == null) {
			return true;
		} else {
			return false;
		}
	}
	
}
