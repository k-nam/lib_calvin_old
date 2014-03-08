package com.nhncorp.bowling.tournament;

import java.util.ArrayDeque;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Vector;

import com.nhncorp.bowling.driver.EmployeeId;

public class TournamentManager {
	private HashSet<EmployeeId> playerIds;
	private boolean isRegistrationClosed = false;
	private ArrayDeque<GameSkeleton> gameQueue;
	private GameSkeleton finalGame = null;
	private HashMap<BowlingGameId, GameSkeleton> idToGameMap;
	private EmployeeId tournamentWinner;
	
	public TournamentManager() {
		playerIds = new HashSet<EmployeeId>();
		gameQueue = new ArrayDeque<GameSkeleton>();
		idToGameMap = new HashMap<BowlingGameId, GameSkeleton>();
	}
	
	public void registerPlayer(EmployeeId playerId) throws Exception {
			if (playerIds.contains(playerId) || isRegistrationClosed == true) {
				throw new Exception();
			}
			playerIds.add(playerId);
	}

	public void closeRegistration() {
		isRegistrationClosed = true;		
	}
	
	public void startTournament() throws Exception {
		this.closeRegistration();
		this.setUpMatches();
		this.startInitialGames();
	}
	
	public GameSkeleton getNextGame() {
		if (gameQueue.isEmpty()) {
			return null;
		} else {
			return gameQueue.poll();
		}
	}
	
	public EmployeeId getTournamentWinner() {
		return this.tournamentWinner;
	}
	
	public void notifyGameWinners(BowlingGameId gameId, EmployeeId[] winners) throws Exception {
		if (winners.length > GameSkeleton.MAX_NUMBER_OF_WINNERS 
				|| winners.length < 0) {
			throw new Exception();
		} else {
			GameSkeleton game = idToGameMap.get(gameId);
			game.notifyWinners(winners);
			if (game.getParent() != null && game.getParent().isReadyToStart()) {
				gameQueue.add(game.getParent());
			}
			if (game.isFinalGame()) {
				this.tournamentWinner = winners[0];
			}
		}
	}
	
	public boolean isTournamentFinished() {
		if (finalGame.isFinished()) {
			return true;
		} else {
			return false;
		}
	}
	
	private void setUpMatches() throws Exception {
		if (playerIds.size() < 1) {
			throw new Exception();
		}
		Vector<GameSkeleton> games = new Vector<GameSkeleton>(); 		
		EmployeeId[] playerIds = 
			this.playerIds.toArray(new EmployeeId[this.playerIds.size()]);
		GameSkeleton tempGame = null;
		for (int i = 0; i < playerIds.length; i++) {
			if (i % GameSkeleton.MAX_NUMBER_OF_PLAYERS == 0) {
				BowlingGameId gameId = BowlingGameId.getUniqueGameId();
				tempGame = new GameSkeleton(gameId);
				idToGameMap.put(gameId, tempGame);
				games.add(tempGame);
			}
			try {
				tempGame.addPlayer(playerIds[i]);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		GameSkeleton parentGame = null;
		Vector<GameSkeleton> parentGames = new Vector<GameSkeleton>();
		while (true) {
			for (int i = 0; i < games.size(); i++) {
				if (i % GameSkeleton.MAX_NUMBER_OF_CHILDERN == 0) {
					BowlingGameId gameId = BowlingGameId.getUniqueGameId();
					parentGame = new GameSkeleton(gameId);
					idToGameMap.put(gameId, parentGame);					
					parentGames.add(parentGame);
				}
				games.get(i).setParent(parentGame);
				try {
					parentGame.addChild(games.get(i));
				} catch (Exception e) {
					e.printStackTrace();
				} 
			}
			if (parentGames.size() == 1) {
				this.finalGame = parentGames.firstElement();
				return;
			} else {
				games = parentGames;
				parentGames = new Vector<GameSkeleton>(); 
			}
		}	
	}
	
	private void startInitialGames() {
		GameSkeleton[] allGames = 
			idToGameMap.values().toArray(new GameSkeleton[idToGameMap.size()]);
		for (int i = 0; i < allGames.length; i++) {
			if (allGames[i].isReadyToStart() == true 
					&& allGames[i].isFinished() == false) {
				if (allGames[i].isDefaultWin()) {
					try {
						GameSkeleton thisGame = allGames[i];
						while(thisGame.isDefaultWin() == true) { 
							if (thisGame.isFinalGame() == true 
									&& thisGame.getNumberOfPlayers() == GameSkeleton.MAX_NUMBER_OF_WINNERS) {
								this.gameQueue.add(thisGame);
							} else {
								thisGame.notifyWinners(thisGame.getPlayers());
							}
							if (thisGame.getParent() == null) {
								break;
							} else {
								thisGame = thisGame.getParent();
							}
						}
					} catch (Exception e) {
						e.printStackTrace();
					}
				} else {
					this.gameQueue.add(allGames[i]);
				}
			}
		}
	}		
}

