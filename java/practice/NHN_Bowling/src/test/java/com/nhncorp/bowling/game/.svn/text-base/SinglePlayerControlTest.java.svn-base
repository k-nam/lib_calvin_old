package com.nhncorp.bowling.game;


import org.junit.*;
import static org.junit.Assert.*;
import org.junit.After;
import org.junit.Before;

import com.nhncorp.bowling.driver.BowlingGameDriver;
import com.nhncorp.bowling.driver.GameRecord;
import com.nhncorp.bowling.game.BowlingPlayer;
import com.nhncorp.bowling.game.SinglePlayerControl;
import com.nhncorp.bowling.game.order.*;
import com.nhncorp.bowling.room.BowlingLaneId;
import com.nhncorp.bowling.score.FrameScores;



public class SinglePlayerControlTest {
	private SinglePlayerControl singlePlayerControl;
	private BowlingPlayer bowlingPlayer;
	private BowlingLaneId bowlingLaneId;
	
	public static int[] testInput_1 = { 4, 6, 	
		7, 2,  
		3, 6,  
		0, 5, 
		9, 0,  
		1, 2, 
		0, 0, 
		10,
		10, 
		10, 
		4, 5 };
	public static int[] testInput_2 = { 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 
		10, 10
	};
	public static int[] testInput_3 = { 1, 9,
		2,8,
		3,7,
		4,6,
		5,5,
		6,4,
		7,3,
		8,2,
		9,1,
		0,10,
		10		
	};
	public static int[] testInput_4 = { 1,9,
		2,8,
		3,7,
		4,6,
		5,5,
		6,4,
		7,3,
		8,2,
		9,1,
		3,5	
	};
	
	public static int[] testScore_1 = { 17,9,9,5,9,3,0,30,24,19 };
	public static int[] testScore_2 = { 30,30,30,30,30,30,30,30,30,30 };
	public static int[] testScore_3 = { 12,13,14,15,16,17,18,19,10,20 };
	public static int[] testScore_4 = { 12,13,14,15,16,17,18,19,13,8 };
	

	
	@Before
	public void setUp() throws Exception {
		bowlingPlayer = BowlingPlayer.getUniqueBowlingPlayer();
		bowlingLaneId =	BowlingLaneId.getUniqueBowlingLaneId();
		singlePlayerControl = 
			new SinglePlayerControl(bowlingPlayer, bowlingLaneId);
		
	}

	@After
	public void tearDown() throws Exception {
	}
	
	@Test	 
	public void startTest() throws Exception{		
		singlePlayerControl.startFrame();
		FrameContinueOrder startOrder = 
			(FrameContinueOrder)singlePlayerControl.getOrder();
		assertEquals(bowlingPlayer, startOrder.getPlayer());
		assertEquals(bowlingLaneId, startOrder.getLaneId());
		assertEquals(1, startOrder.getFrameNumber());
	}	
	
	@Test 
	public void getPlayerTest() {
		assertEquals(bowlingPlayer, singlePlayerControl.getPlayer());
	}
	
	@Test 
	public void getLaneTest() {
		assertEquals(bowlingLaneId, singlePlayerControl.getLaneId());
	}
	
	@Test	 (expected = Exception.class)
	public void getOrderTest() throws Exception {
		singlePlayerControl.startFrame();
		singlePlayerControl.getOrder();
		singlePlayerControl.getOrder();
	}	
	
	@Test
	public void knockPinsTest() throws Exception {	
		int[] record  = { 5 };
		FrameContinueOrder order = 
			(FrameContinueOrder)BowlingGameDriver.simulateBowlingGame(
					singlePlayerControl, record);
		assertEquals(bowlingPlayer, order.getPlayer());
		assertEquals(bowlingLaneId, order.getLaneId());
		assertEquals(1, order.getFrameNumber());
	}
	
	@Test
	public void knockPinsTest2() throws Exception {		
		int[] record  = { 5, 2 };
			BowlingGameDriver.simulateBowlingGame(singlePlayerControl, record);
		singlePlayerControl.startFrame();	
		FrameContinueOrder continueOrder = 
			(FrameContinueOrder)singlePlayerControl.getOrder();
		assertEquals(2, continueOrder.getFrameNumber());
	}
	
	@Test
	public void knockPinsTest3() throws Exception {
		int[] record  = { 10 };
		BowlingGameDriver.simulateBowlingGame(singlePlayerControl, record);
		singlePlayerControl.startFrame();
		FrameContinueOrder continueOrder = 
			(FrameContinueOrder)singlePlayerControl.getOrder();
		assertEquals(2, continueOrder.getFrameNumber());
	}
	
	@Test
	public void knockPinsTest4() throws Exception {		
		int[] record  = { 10, 2 };
		FrameContinueOrder order = 
			(FrameContinueOrder)BowlingGameDriver.simulateBowlingGame(
					singlePlayerControl, record);
		assertEquals(2, order.getFrameNumber());
	}
	
	@Test
	public void knockPinsTest5() throws Exception {		
		int[] record  = { 10, 2, 2 };
		BowlingGameDriver.simulateBowlingGame(singlePlayerControl, record);
		singlePlayerControl.startFrame();
		FrameContinueOrder continueOrder = 
			(FrameContinueOrder)singlePlayerControl.getOrder();
		assertEquals(3, continueOrder.getFrameNumber());
	}
	
	@Test
	public void knockPinsTest6() throws Exception {		
		int[] record  = { 7, 3 };
		BowlingGameDriver.simulateBowlingGame(singlePlayerControl, record);
		singlePlayerControl.startFrame();
		FrameContinueOrder continueOrder = 
			(FrameContinueOrder)singlePlayerControl.getOrder();
		assertEquals(2, continueOrder.getFrameNumber());
	}
	
	@Test
	public void knockPinsTest7() throws Exception {		
		int[] record  = { 0, 0 };
		BowlingGameDriver.simulateBowlingGame(singlePlayerControl, record);
		singlePlayerControl.startFrame();		
		FrameContinueOrder continueOrder = 
			(FrameContinueOrder)singlePlayerControl.getOrder();
		assertEquals(2, continueOrder.getFrameNumber());
	}
	
	@Test
	public void knockPinsTest8() throws Exception {		
		int[] record  = { 10, 10, 10, 10, 10, 10 };
		BowlingGameDriver.simulateBowlingGame(singlePlayerControl, record);
		singlePlayerControl.startFrame();
		FrameContinueOrder continueOrder = 
			(FrameContinueOrder)singlePlayerControl.getOrder();
		assertEquals(7, continueOrder.getFrameNumber());
	}
	
	@Test
	public void knockPinsTest9() throws Exception {		
		int[] record  = { 10, 10, 10, 10, 10,
				10, 10, 10, 10, 10 };
		GameOrder order = 
			BowlingGameDriver.simulateBowlingGame(
					singlePlayerControl, record);
		assertEquals(11, ((FrameContinueOrder)order).getFrameNumber());
		assertEquals(10, singlePlayerControl.getCurrentNormalFrameNumber());
		singlePlayerControl.knockPins(3);
		singlePlayerControl.knockPins(3);
		order = singlePlayerControl.getOrder();		
		assertTrue(order.isGameFinished());
	}
	
	@Test 
	public void knockPinsTest10() throws Exception {		
		int[] record  = { 10, 10, 10, 10, 10,
				10, 10, 10, 10, 10, 
				10 };		
		GameOrder order = 
			(FrameContinueOrder)BowlingGameDriver.simulateBowlingGame(
					singlePlayerControl, record);
		assertTrue(singlePlayerControl.isNowPlaying());
		assertEquals(12, ((FrameContinueOrder)order).getFrameNumber());
		assertEquals(10, singlePlayerControl.getCurrentNormalFrameNumber());
		singlePlayerControl.knockPins(10);
		order = singlePlayerControl.getOrder();		
		assertTrue(order.isGameFinished());
	}
	
	@Test (expected = Exception.class)
	public void knockPinsTest11() throws Exception {		
		int[] record  = { 10, 10, 10, 10, 10,
				10, 10, 10, 10, 10, 
				10, 10 };
		BowlingGameDriver.simulateBowlingGame(singlePlayerControl, record);
		singlePlayerControl.startFrame();
	}
	
	@Test
	public void knockPinsTest12() throws Exception {		
		int[] record  = { 10, 10, 10, 10, 10,
				10, 10, 10, 10, 10, 
				10, 10, 10 };
		BowlingGameDriver.simulateBowlingGame(singlePlayerControl, record);
	}
		
	public boolean scoreLogicTestRoutine(int[] record, int[] score) throws Exception {
		GameRecord gameRecord = 
			GameRecord.getGameRecordFromIntArray(record);
		BowlingGameDriver.simulateBowlingGame(singlePlayerControl, gameRecord);
		FrameScores expectedScore = 
			FrameScores.getBowlingScoreFromIntArray(score);
		return singlePlayerControl.getScore().isEqualTo(expectedScore);
	}
	
	@Test
	// 46 72 36 05 90 12 00 X  X  X45
	// 17 9  9  5  9  3  0 30 24 19
	public void scoreLogicTest() throws Exception {
		assertTrue(scoreLogicTestRoutine(testInput_1, testScore_1));
	}
	
	@Test
	// X  X  X  X  X  X  X  X  X  X  X  X  
	// 30 30 30 30 30 30 30 30 30 30
	public void scoreLogicTest1() throws Exception {
		assertTrue(scoreLogicTestRoutine(testInput_2, testScore_2));
	}
	
	@Test
	// 1/ 2/ 3/ 4/ 5/ 6/ 7/ 8/ 9/ 0/ X 
	// 12 13 14 15 16 17 18 19 10 13 
	public void scoreLogicTest2() throws Exception {
		assertTrue(scoreLogicTestRoutine(testInput_3, testScore_3));

	}
	
	@Test
	// 1/ 2/ 3/ 4/ 5/ 6/ 7/ 8/ 9/ 35
	// 12 13 14 15 16 17 18 19 13 8 
	public void scoreLogicTest3() throws Exception {
		assertTrue(scoreLogicTestRoutine(testInput_4, testScore_4));
	}
}
