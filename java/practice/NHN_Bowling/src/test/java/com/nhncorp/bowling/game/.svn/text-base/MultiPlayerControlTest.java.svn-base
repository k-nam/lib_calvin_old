package com.nhncorp.bowling.game;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import static org.junit.Assert.*;

import com.nhncorp.bowling.driver.BowlingGameDriver;
import com.nhncorp.bowling.driver.GameRecord;
import com.nhncorp.bowling.game.schedule.LaneScheduler;
import com.nhncorp.bowling.room.BowlingLaneId;
import com.nhncorp.bowling.score.FrameScores;

public class MultiPlayerControlTest {
	private static final int TEST_NUMBER_OF_PLAYERS = 4;
	private static final int TEST_NUMBER_OF_LANES = 2;
	
	MultiPlayerControl multiPlayerControl;
	BowlingPlayer[] bowlingPlayers;
	BowlingLaneId[] bowlingLaneIds;
	LaneScheduler laneScheduler;
	
	@Before
	public void setUp() throws Exception {	
		bowlingPlayers = new BowlingPlayer[TEST_NUMBER_OF_PLAYERS];
		for(int i = 0; i < bowlingPlayers.length; i++) {
			bowlingPlayers[i] = BowlingPlayer.getUniqueBowlingPlayer();
		}
		
		bowlingLaneIds = new BowlingLaneId[TEST_NUMBER_OF_LANES];
		for(int i = 0; i < bowlingLaneIds.length; i++) {
			bowlingLaneIds[i] = BowlingLaneId.getUniqueBowlingLaneId();
		}
		laneScheduler = 
			new LaneScheduler(TEST_NUMBER_OF_PLAYERS, TEST_NUMBER_OF_LANES);
		multiPlayerControl = new MultiPlayerControl(
				bowlingPlayers, bowlingLaneIds, laneScheduler);
	}

	@After
	public void tearDown() throws Exception {
	}
	
	@Test
	public void getPlayerAtTest() {
		for (int i = 0; i < bowlingPlayers.length; i++) {
			assertEquals(bowlingPlayers[i], multiPlayerControl.getPlayerAt(i));}
	}
	
	@Test
	public void getLaneAtTest() {
		for (int i = 0; i < bowlingLaneIds.length; i++) {
			assertEquals(bowlingLaneIds[i], multiPlayerControl.getLaneAt(i));
		}
	}
	 
	@Test
	public void getIndexOfTest() throws Exception {
		for (int i = 0; i < bowlingPlayers.length; i++) {
			assertEquals(i, 
					multiPlayerControl.getIndexOf(bowlingPlayers[i]));}
	}
	
	@Test (expected = Exception.class)
	public void getIndexOfTest2() throws Exception {
		multiPlayerControl.getIndexOf(
				BowlingPlayer.getUniqueBowlingPlayer());
	}
	
	@Test
	public void getIndexOfTest3() throws Exception {
		for (int i = 0; i < bowlingLaneIds.length; i++) {
			assertEquals(i, 
					multiPlayerControl.getIndexOf(bowlingLaneIds[i]));
		}
	}
	
	@Test (expected = Exception.class)
	public void getIndexOfTest4() throws Exception {
		multiPlayerControl.getIndexOf(BowlingLaneId.getUniqueBowlingLaneId());
	}
	
	@Test (expected = Exception.class)
	public void knockPinsTest() throws Exception {
		multiPlayerControl.knockPins(BowlingLaneId.getUniqueBowlingLaneId(), 10);		
	}
	
	@Test
	public void gamePlayLogicTest() throws Exception {
		int[][] testInputs = {SinglePlayerControlTest.testInput_1,
				SinglePlayerControlTest.testInput_2, 
				SinglePlayerControlTest.testInput_3,
				SinglePlayerControlTest.testInput_4};
		int[][] testScores = {SinglePlayerControlTest.testScore_1,
				SinglePlayerControlTest.testScore_2, 
				SinglePlayerControlTest.testScore_3,
				SinglePlayerControlTest.testScore_4};
			
		GameRecord[] gameRecords = 
			GameRecord.getGameRecordsFromIntArrays(testInputs);
		
		FrameScores[] expectedScores = 
			FrameScores.getBowlingScoresFromIntArrays(testScores);
		BowlingGameDriver.simulateBowlingGame(multiPlayerControl, gameRecords);
		assertTrue(FrameScores.isEqual(expectedScores, multiPlayerControl.getScores()));
	}
	
	
}
