package com.nhncorp.bowling.driver;

import org.junit.*;

import com.nhncorp.bowling.game.SinglePlayerControlTest;

import static org.junit.Assert.*;


public class GameRecordTest {
	private GameRecord gameRecord;
	private GameRecord[] gameRecords;
	
	@Before
	public void setUp() throws Exception {
	}

	@After
	public void tearDown() throws Exception {
	}
	
	@Test
	public void getGameRecordFromIntArrayTest() {
		gameRecord = GameRecord.getGameRecordFromIntArray(
				SinglePlayerControlTest.testInput_1);		
		assertTrue(GameRecord.checkEquality(gameRecord,
				SinglePlayerControlTest.testInput_1));	
	}
	
	@Test
	public void getGameRecordsIntArraysTest() {
		int[][] intArrays = { SinglePlayerControlTest.testInput_1,
				SinglePlayerControlTest.testInput_2,
				SinglePlayerControlTest.testInput_3,
				SinglePlayerControlTest.testInput_4
		};
		gameRecords = GameRecord.getGameRecordsFromIntArrays(intArrays);
		for (int i = 0; i < 4; i++) {
			assertTrue(GameRecord.checkEquality(gameRecords[i], intArrays[i]));
		}
	}
	
	@Test
	public void checkEqualityTest() {
		gameRecord = 
			GameRecord.getGameRecordFromIntArray(SinglePlayerControlTest.testInput_1);
		assertFalse(GameRecord.checkEquality(
				gameRecord, SinglePlayerControlTest.testInput_2));
	}

}
