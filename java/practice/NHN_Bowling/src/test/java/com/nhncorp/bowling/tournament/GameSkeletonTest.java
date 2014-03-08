package com.nhncorp.bowling.tournament;


import org.junit.*;

import com.nhncorp.bowling.driver.EmployeeId;

import static org.junit.Assert.*;

public class GameSkeletonTest {
	private GameSkeleton[] games;
	private GameSkeleton parentGame;
	private final int NUMBER_OF_CHILDREN = 4;
	private final int NUMBER_OF_PLAYERS = 8;
	
	@Before
	public void setUp() throws Exception {
		games = new GameSkeleton[NUMBER_OF_CHILDREN];
		parentGame = new GameSkeleton(BowlingGameId.getUniqueGameId());
		for (int i = 0; i < games.length; i++) {
			games[i] = new GameSkeleton(BowlingGameId.getUniqueGameId());
			for (int j = 0; j < NUMBER_OF_PLAYERS; j++) {
				games[i].addPlayer(EmployeeId.getUniqueEmployeeId());
			}
		}
	}


	@After
	public void tearDown() throws Exception {
	}
	
	@Test
	public void addChildTest() throws Exception {
		for (int i = 0; i < games.length; i++) {
			parentGame.addChild(games[i]);
		}
	}
	
	// Helper function
	public void setUpTree() throws Exception {
		for (int i = 0; i < games.length; i++) {
			parentGame.addChild(games[i]);		
			games[i].setParent(parentGame);
		}
	}
	
	// Helper function
	public static EmployeeId[] getRandomWinnersFrom(GameSkeleton game) {
		EmployeeId[] winners = new EmployeeId[2];
		winners[0] = game.getPlayers()[0];
		winners[1] = game.getPlayers()[1];	
		return winners;
	}
	
	@Test (expected = Exception.class)
	public void addChildTest2() throws Exception {
		for (int i = 0; i < games.length; i++) {
			parentGame.addChild(games[i]);			
		}
		parentGame.addChild(games[0]);
	}
	
	@Test
	public void setParentTest() {
		for (int i = 0; i < games.length; i++) {
			games[i].setParent(parentGame);
			assertEquals(parentGame, games[i].getParent());
		}
	}
	
	@Test
	public void isReadyToStartTest() {
		for (int i = 0; i < games.length; i++) {
			assertTrue(games[i].isReadyToStart());
		}
	}
	
	@Test
	public void notifyWinnersTest() throws Exception {
		setUpTree();
		games[0].notifyWinners(getRandomWinnersFrom(games[0]));
		assertTrue(games[0].isFinished());
		assertFalse(parentGame.isReadyToStart());
	}
	
	@Test
	public void notifyWinnersTest2() throws Exception {
		setUpTree();
		for (int i = 0; i < NUMBER_OF_CHILDREN; i++) {
			games[i].notifyWinners(getRandomWinnersFrom(games[i]));
			assertTrue(games[i].isFinished());
		}	
		assertTrue(parentGame.isReadyToStart());
	}
	
	@Test (expected = Exception.class)
	public void notifyWinnersTest3() throws Exception {
		setUpTree();
		games[0].notifyWinners(
				new EmployeeId[] { EmployeeId.getUniqueEmployeeId() });
	
	}
	
	@Test (expected = Exception.class)
	public void notifyWinnersTest4() throws Exception {
		setUpTree();
		games[0].notifyWinners(getRandomWinnersFrom(games[0]));
		games[0].notifyWinners(getRandomWinnersFrom(games[0]));
	}
	
	@Test (expected = Exception.class)
	public void cutOffChildTest() throws Exception {
		GameSkeleton parent = new GameSkeleton(BowlingGameId.getUniqueGameId());
		GameSkeleton child = new GameSkeleton(BowlingGameId.getUniqueGameId());
		child.setParent(parent);
		child.addPlayer(EmployeeId.getUniqueEmployeeId());
		child.addPlayer(EmployeeId.getUniqueEmployeeId());
		child.addPlayer(EmployeeId.getUniqueEmployeeId());
		child.notifyWinners(getRandomWinnersFrom(child));
	}
	
	@Test 
	public void isFinalGameTest() {
		assertTrue(parentGame.isFinalGame());
	}
	
	@Test 
	public void isFinalGameTest2() throws Exception {
		setUpTree();
		assertFalse(games[0].isFinalGame());
	}
	
}
