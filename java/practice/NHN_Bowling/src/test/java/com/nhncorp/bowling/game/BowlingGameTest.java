package com.nhncorp.bowling.game;


import org.junit.*;
import static org.junit.Assert.*;
import com.nhncorp.bowling.driver.EmployeeId;
import com.nhncorp.bowling.room.BowlingLaneId;
import com.nhncorp.bowling.tournament.BowlingGameId;





public class BowlingGameTest {
	private BowlingGame bowlingGame;
	private BowlingPlayer[] players;
	
	@Before
	public void setUp() throws Exception {

	}

	@After
	public void tearDown() throws Exception {
	}
	
	public void makeGame(int numberOfPlayers, int numberOfLanes) throws Exception {
		players = new BowlingPlayer[numberOfPlayers];
		for (int i = 0; i < numberOfPlayers; i++) {
			players[i] = BowlingPlayer.getUniqueBowlingPlayer();			
		}
		bowlingGame = new BowlingGame(BowlingGameId.getUniqueGameId(), players);
		BowlingLaneId[] bowlingLaneIds = new BowlingLaneId[numberOfLanes];
		for (int i = 0; i < numberOfLanes; i++) {
			bowlingLaneIds[i] = BowlingLaneId.getUniqueBowlingLaneId();			
		}
		bowlingGame.setLaneIds(bowlingLaneIds);
	}
	
	@Test (expected = Exception.class)
	public void bowlingGameTest() throws Exception {
		makeGame(9, 2);
	}
	
	// Default by win game should not be created
	@Test (expected = Exception.class)
	public void bowlingGameTest2() throws Exception {
		makeGame(2, 2);
	}
	
	@Test
	public void getNumberOfPlayersTest() throws Exception {
		makeGame(8, 2);
		assertEquals(8, bowlingGame.getNumberOfPlayers());
	}

	@Test
	public void notifyAbsentPlayerTest() throws Exception {
		makeGame(8, 2);
		bowlingGame.notifyAbsentPlayer(players[0].getEmployeeId());
	}
 
	@Test
	public void notifyAbsentPlayerTest2() throws Exception {
		int numberOfPlayers = 4;
		makeGame(numberOfPlayers, 2);
		for (int i = 0; i < numberOfPlayers - 1; i++) {
			bowlingGame.notifyAbsentPlayer(players[i].getEmployeeId());
		}
		bowlingGame.startGame();
		assertTrue(bowlingGame.isFinished());
		assertEquals(1, bowlingGame.getWinners().length);

	}
	
	@Test
	public void notifyAbsentPlayerTest3() throws Exception {
		int numberOfPlayers = 4;
		makeGame(numberOfPlayers, 2);
		for (int i = 0; i < numberOfPlayers; i++) {
			bowlingGame.notifyAbsentPlayer(players[i].getEmployeeId());
		}
		bowlingGame.startGame();
		assertTrue(bowlingGame.isFinished());
		assertEquals(0, bowlingGame.getWinners().length);
	}
	
	@Test (expected = Exception.class)
	public void notifyAbsentPlayerTest4() throws Exception {
		makeGame(8, 2);
		bowlingGame.notifyAbsentPlayer(EmployeeId.getUniqueEmployeeId());
	}
	
	@Test (expected = Exception.class)
	public void notifyAbsentPlayerTest5() throws Exception {
		makeGame(8, 2);
		bowlingGame.startGame();
		bowlingGame.notifyAbsentPlayer(EmployeeId.getUniqueEmployeeId());
	}
	
	@Test (expected = Exception.class)
	public void startGameTest() throws Exception {
		makeGame(8, 2);
		bowlingGame.startGame();
		bowlingGame.startGame();
	}
	
	@Test (expected = Exception.class)
	public void getWinnersTest() throws Exception {
		makeGame(8, 2);
		bowlingGame.startGame();
		bowlingGame.getWinners();
	}
	
	@Test
	public void getOrdersTest() throws Exception {
		makeGame(8, 2);
		bowlingGame.startGame();
		assertEquals(2, bowlingGame.getOrders().size());
	}
	
	@Test
	public void aa() {
	}
	
}
