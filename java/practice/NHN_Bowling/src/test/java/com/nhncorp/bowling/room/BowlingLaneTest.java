package com.nhncorp.bowling.room;


import org.junit.*;

import com.nhncorp.bowling.room.BowlingLane;

import static org.junit.Assert.*;


public class BowlingLaneTest {
	private BowlingLane lane;
	
	
	@Before	
	public void setUp() throws Exception {
		lane = new BowlingLane(BowlingLaneId.getUniqueBowlingLaneId());
	}

	@After
	public void tearDown() throws Exception {
	}
	
	@Test
	public void testInit() {
		assertEquals(lane.getRemainingNumberOfPins(), 10);
	}
	
	@Test 
	public void testThrowing() throws Exception {
		lane.knockPins(5);
		assertEquals(lane.getRemainingNumberOfPins(), 5);
	}	
	
	@Test (expected = Exception.class )
	public void testThrowingError() throws Exception {
		lane.knockPins(5);
		lane.knockPins(6);		
	}
}
