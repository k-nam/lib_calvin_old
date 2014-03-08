package com.nhncorp.bowling.main;

import java.util.HashMap;

import com.nhncorp.bowling.driver.ApplyManager;
import com.nhncorp.bowling.driver.BettingManager;
import com.nhncorp.bowling.driver.Employee;
import com.nhncorp.bowling.driver.EmployeeId;
import com.nhncorp.bowling.driver.LogOnManager;
import com.nhncorp.bowling.game.BowlingGame;
import com.nhncorp.bowling.game.BowlingPlayer;
import com.nhncorp.bowling.tournament.GameSkeleton;
import com.nhncorp.bowling.tournament.TournamentManager;

public class BowlingServer {
	public static final int NUMBER_OF_EMPLOYEES = 10000;
	public static final int NUMBER_OF_BOWLING_PLAYERS = 3000;
	public static final int PLAYER_APPLICATION_FEE = 10000;
	
	private LogOnManager logOnManager;
	private ApplyManager applyManager;
	private BettingManager bettingManager;
	private TournamentManager tournamentManager;
	private HappyBeanAccount happyBeanAccount;
	private EmployeeId tournamentWinner = null;
	private GameScheduleBuffer gameScheduleBuffer;
	private HashMap<EmployeeId, BowlingPlayer> employeeIdToPlayerMap;
	
	public BowlingServer() throws Exception {
		initializeLogOnManager();
		EmployeeId[] totalEmployeeIds = logOnManager.getTotalEmployeeIds();
		applyManager = new ApplyManager(totalEmployeeIds);
		happyBeanAccount = new HappyBeanAccount();
		bettingManager = new BettingManager(totalEmployeeIds, happyBeanAccount);
		tournamentManager = new TournamentManager();
		gameScheduleBuffer = new GameScheduleBuffer();
		employeeIdToPlayerMap = new HashMap<EmployeeId, BowlingPlayer>();
	}
	
	public GameScheduleBuffer getGameScheduleBuffer() {
		return gameScheduleBuffer;
	}
	
	private void initializeLogOnManager() throws Exception {
		logOnManager = new LogOnManager();
		for (int i = 0; i < NUMBER_OF_EMPLOYEES; i++) {
			String id = "KR" + Integer.toString(i);
			String password = id;
			logOnManager.registerUser(id, password, Employee.getUniqueEmployee());
		}
	}
	
	public boolean logOn(String id, String passWord) {
		return logOnManager.logOn(id, passWord);
	}
	
	public boolean apply(String id) {
		if (logOnManager.isLoggedOnUser(id) == true) {
			try {
				applyManager.apply(
						logOnManager.getEmployeeFromId(id).getEmployeeId());
			} catch (Exception e) {				
				e.printStackTrace();
				return false;
			}
			return true;
		} else {
			return false;
		}
	}
	
	public boolean betting(String bettorId, String playerId) {
		if (logOnManager.isLoggedOnUser(bettorId) == true) {
			try {
				bettingManager.betting(
						logOnManager.getEmployeeFromId(bettorId).getEmployeeId(), 
						logOnManager.getEmployeeFromId(playerId).getEmployeeId());
			} catch (Exception e) {		
				return false;
			}
			return true;
		} else {
			return false;
		}
	}
	
	public void startBowlingTournament() throws Exception {
		EmployeeId[] totalPlayers = applyManager.getTotalPlayers();
		if (totalPlayers.length <= 1) {
			if (totalPlayers.length == 1) {
				tournamentWinner = totalPlayers[0];
			}
			this.distributeBettingMoney();
			return;
		}
		for (EmployeeId playerId : totalPlayers) {
			try {
				tournamentManager.registerPlayer(playerId);
				BowlingPlayer player = 
					new BowlingPlayer(logOnManager.getEmployeeFromEmployeeId(playerId),
							PLAYER_APPLICATION_FEE);
				this.employeeIdToPlayerMap.put(playerId, player);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		tournamentManager.startTournament();
		this.pollingOnGames();
	}
	
	public void distributeBettingMoney() {
		bettingManager.distributeMoney(tournamentWinner, logOnManager);
		this.happyBeanAccount.donateMoney();
	}

	public void pollingOnGames() {
		while (true) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			if (tournamentManager.isTournamentFinished()) {
				System.out.println("TOURNAMENT FINISHED");
				this.tournamentWinner = tournamentManager.getTournamentWinner();
				this.distributeBettingMoney();
				return;
			}
			while (true) {
				GameSkeleton skeleton = tournamentManager.getNextGame();
				if (skeleton == null) {
					break; 
				} else {
					EmployeeId[] playerIds = skeleton.getPlayers();
					BowlingPlayer[] players = new BowlingPlayer[skeleton.getNumberOfPlayers()];
					// Deal with win-by-default
					if (skeleton.getNumberOfPlayers() <= GameSkeleton.MAX_NUMBER_OF_WINNERS) {
						try {
							tournamentManager.notifyGameWinners(skeleton.getGameId(), playerIds);
						} catch (Exception e) {
							e.printStackTrace();
						}
						continue;
					}
					for (int i = 0; i < players.length; i++) {
						players[i] = this.employeeIdToPlayerMap.get(playerIds[i]);
					}
					BowlingGame newGame = null;
					try {
						newGame = new BowlingGame(skeleton.getGameId(), players, 
									skeleton.isFinalGame());
						newGame.setHappyBeanAccount(happyBeanAccount);
					} catch (Exception e) {
						e.printStackTrace();
					}
					gameScheduleBuffer.enqueNewGame(newGame);
					
				}
			}
			while (true) {
				BowlingGame finishedGame = gameScheduleBuffer.getFinishedGame();
				if (finishedGame == null) {
					break;
				}
				EmployeeId[] winners;
				try {
					winners = finishedGame.getWinners();
					tournamentManager.notifyGameWinners(
							finishedGame.getBowlingGameId(), winners);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
	}
}
