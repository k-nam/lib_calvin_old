package com.nhncorp.bowling.score;

import static org.junit.Assert.*;

import org.junit.*;


import com.nhncorp.bowling.score.FrameScores;

public class BowlingScoreTest {

	private FrameScores score1;
	private FrameScores score2;
	private FrameScores score3;
	private FrameScores score4;
	private FrameScores[] array1;
	private FrameScores[] array2;
	private FrameScores[] array3;


	
	@Before
	public void setUp() throws Exception {
		score1 = new FrameScores();
		score2 = new FrameScores();
		score3 = new FrameScores();
		score4 = new FrameScores();
		array1 = new FrameScores[2];
		array2 = new FrameScores[2];
		array3 = new FrameScores[3];
		array1[0] = score1;
		array1[1] = score2;
		array2[0] = score3;
		array2[1] = score4;
		array3[0] = score1;
		array3[1] = score2;
	}

	@After
	public void tearDown() throws Exception {
	}
	
	@Test
	public void isEqualToTest() {
		score1.setFrameScore(1, 5);
		score2.setFrameScore(1, 5);
		assertTrue(score1.isEqualTo(score2));
		score2.setFrameScore(3, 5);
		assertFalse(score1.isEqualTo(score2));
	}
	
	@Test
	public void isEqualToTest2() {
		assertFalse(FrameScores.isEqual(array1, array3));
	}
	
	@Test
	public void isEqualToTest3() {
		score1.setFrameScore(1, 5);
		score3.setFrameScore(1, 5);
		assertTrue(FrameScores.isEqual(array1,	array2));		
		score4.setFrameScore(3, 5);
		assertFalse(FrameScores.isEqual(array1,	array2));		
	}
	

}
