package com.nhncorp.bowling.game;
import org.junit.*;

import static org.junit.Assert.*;


import com.nhncorp.bowling.game.BowlingPlayer;

public class BowlingPlayerTest {
	private BowlingPlayer player;
	int prizeMoney = 1000;
	
	@Before	
	public void setUp() throws Exception {
		
		player = BowlingPlayer.getUniqueBowlingPlayer(prizeMoney);
	}

	@After
	public void tearDown() throws Exception {	
	}
	
	@Test
	public void getPrizeMoneyTest() {		
		assertEquals(prizeMoney, player.getPrizeMoney());
	}
	
	@Test
	public void loseMoneyTest() throws Exception { 
		int lostMoney = 1000;
		player.loseMoney(lostMoney);
		assertEquals(prizeMoney - lostMoney, player.getPrizeMoney());
	}
	
	@Test(expected = Exception.class)
	public void loseMoneyTest2() throws Exception {
		int lostMoney = 1200;
		player.loseMoney(lostMoney);
	}
	
	@Test(expected = Exception.class)
	public void loseMoneyTest3() throws Exception {
		int lostMoney = -199;
		player.loseMoney(lostMoney);
	}
	
	@Test
	public void gainMoneyTest() throws Exception {
		int gainedMoney = 1000;
		player.gainMoney(gainedMoney);
		assertEquals(prizeMoney + gainedMoney, player.getPrizeMoney());
	}
	
	@Test(expected = Exception.class)
	public void gainMoneyTest2() throws Exception {
		int gainedMoney = -1000;
		player.gainMoney(gainedMoney);		
	}
}
