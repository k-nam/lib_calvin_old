package com.nhncorp.bowling.tournament;

import org.junit.*;
import static org.junit.Assert.*;
import com.nhncorp.bowling.driver.EmployeeId;

public class TournamentManagerTest {
	private TournamentManager tournamentManager;
	private EmployeeId[] playerIds;
	
	
	@Before
	public void setUp() throws Exception {
		tournamentManager = new TournamentManager();

	}

	@After
	public void tearDown() throws Exception {
	}

	public void registerAll(int numberOfPlayers) throws Exception {
		playerIds = new EmployeeId[numberOfPlayers];
	  for (int i = 0; i < numberOfPlayers; i++) {
	  	playerIds[i] = EmployeeId.getUniqueEmployeeId();
	  }
		for (int i = 0; i < numberOfPlayers; i++) {
			tournamentManager.registerPlayer(playerIds[i]);
		}
	}
	
	public boolean hasEmployeeId(EmployeeId id) {
		for (int i = 0; i < playerIds.length; i++) {
			if (playerIds[i] == id) {
				return true;
			}
		}
		return false;
	}
	
	@Test
	public void registerPlayerTest() throws Exception {
		registerAll(1000);
	}
	
	@Test (expected = Exception.class)
	public void registerPlayerTest2() throws Exception {
		registerAll(100);
		tournamentManager.registerPlayer(playerIds[0]);
	}
	
	@Test (expected = Exception.class)
	public void closeRegistrationTest() throws Exception {
		tournamentManager.closeRegistration();
		tournamentManager.registerPlayer(playerIds[0]);
	}

	@Test (expected = Exception.class)
	public void notifyWinnersTest() throws Exception {
		registerAll(100);
		tournamentManager.startTournament();
		GameSkeleton game1 = tournamentManager.getNextGame();		
		BowlingGameId gameId1 = game1.getGameId();
		tournamentManager.notifyGameWinners(gameId1, new EmployeeId[3]);
	}
	
	@Test
	public void isTournamentFinishedTest() throws Exception {
		registerAll(1);
		tournamentManager.startTournament();
		assertTrue(tournamentManager.isTournamentFinished());
	}
	
	@Test
	public void isTournamentFinishedTest2() throws Exception {
		registerAll(2);
		tournamentManager.startTournament();
		assertFalse(tournamentManager.isTournamentFinished());
		GameSkeleton game1 = tournamentManager.getNextGame();		
		assertEquals(2, game1.getNumberOfPlayers());
	}
	
	@Test
	public void isTournamentFinishedTest3() throws Exception {
		registerAll(3);
		tournamentManager.startTournament();
		assertFalse(tournamentManager.isTournamentFinished());
	}
	
	// 5
	// 8 8 8 1
	@Test
	public void matchLogicTest() throws Exception {
		registerAll(25); 
		tournamentManager.startTournament();
		GameSkeleton game1 = tournamentManager.getNextGame();		
		BowlingGameId gameId1 = game1.getGameId();
		EmployeeId[] gamePlayers = game1.getPlayers();
		for (int i = 0; i < gamePlayers.length; i++) {
			assertTrue(hasEmployeeId(gamePlayers[i]));
		}
		GameSkeleton game2 = tournamentManager.getNextGame();
		gamePlayers = game2.getPlayers();
		BowlingGameId gameId2 = game2.getGameId();
		for (int i = 0; i < gamePlayers.length; i++) {
			assertTrue(hasEmployeeId(gamePlayers[i]));
		}
		GameSkeleton game3 = tournamentManager.getNextGame();
		BowlingGameId gameId3 = game3.getGameId();
		for (int i = 0; i < gamePlayers.length; i++) {
			assertTrue(hasEmployeeId(gamePlayers[i]));
		}
		assertEquals(null, tournamentManager.getNextGame());
		tournamentManager.notifyGameWinners(gameId1, 
				GameSkeletonTest.getRandomWinnersFrom(game1));
		assertEquals(null, tournamentManager.getNextGame());
		tournamentManager.notifyGameWinners(gameId2, 
			GameSkeletonTest.getRandomWinnersFrom(game2));
		tournamentManager.notifyGameWinners(gameId3, 
				GameSkeletonTest.getRandomWinnersFrom(game3));
		GameSkeleton game4 = tournamentManager.getNextGame();
		BowlingGameId gameId4 = game4.getGameId();
		for (int i = 0; i < gamePlayers.length; i++) {
			assertTrue(hasEmployeeId(gamePlayers[i]));
		}
		assertTrue(game4.isFinalGame());
		assertFalse(tournamentManager.isTournamentFinished());
		tournamentManager.notifyGameWinners(gameId4, 
				GameSkeletonTest.getRandomWinnersFrom(game4));
		assertEquals(null, tournamentManager.getNextGame());
		assertTrue(tournamentManager.isTournamentFinished());
	}

	// 4
	// 8 2
	// 8 8 8 8 2
	@Test
	public void matchLogicTest2() throws Exception {
		registerAll(34); 
		tournamentManager.startTournament();
		GameSkeleton game1 = tournamentManager.getNextGame();		
		BowlingGameId gameId1 = game1.getGameId();
		GameSkeleton game2 = tournamentManager.getNextGame();
		BowlingGameId gameId2 = game2.getGameId();
		GameSkeleton game3 = tournamentManager.getNextGame();
		BowlingGameId gameId3 = game3.getGameId();
		GameSkeleton game4 = tournamentManager.getNextGame();
		BowlingGameId gameId4 = game4.getGameId();
		assertEquals(null, tournamentManager.getNextGame());
		tournamentManager.notifyGameWinners(gameId1, 
				GameSkeletonTest.getRandomWinnersFrom(game1));
		tournamentManager.notifyGameWinners(gameId2, 
			GameSkeletonTest.getRandomWinnersFrom(game2));
		tournamentManager.notifyGameWinners(gameId3, 
				GameSkeletonTest.getRandomWinnersFrom(game3));
		tournamentManager.notifyGameWinners(gameId4, 
				GameSkeletonTest.getRandomWinnersFrom(game4));
		GameSkeleton game5 = tournamentManager.getNextGame();
		BowlingGameId gameId5 = game5.getGameId();		
		assertEquals(null, tournamentManager.getNextGame());		
		tournamentManager.notifyGameWinners(gameId5, 
				GameSkeletonTest.getRandomWinnersFrom(game5));
		GameSkeleton game6 = tournamentManager.getNextGame();
		BowlingGameId gameId6 = game6.getGameId();
		assertEquals(null, tournamentManager.getNextGame());
		assertTrue(game6.isFinalGame());
		tournamentManager.notifyGameWinners(gameId6, 
				GameSkeletonTest.getRandomWinnersFrom(game6));
		assertEquals(null, tournamentManager.getNextGame());
		assertTrue(tournamentManager.isTournamentFinished());
	}
	
	
}
