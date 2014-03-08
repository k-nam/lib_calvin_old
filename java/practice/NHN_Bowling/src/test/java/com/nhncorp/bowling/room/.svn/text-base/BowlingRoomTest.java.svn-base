package com.nhncorp.bowling.room;

import org.junit.*;

import com.nhncorp.bowling.room.BowlingRoom;

import static org.junit.Assert.*;

public class BowlingRoomTest {
private BowlingRoom bowlingRoom;
private static final int NUMBER_OF_LANES = 8;
	@Before
	public void setUp() throws Exception {
		bowlingRoom = 
			new BowlingRoom(BowlingRoomId.getUniqueBowlingRoomId(), "room", NUMBER_OF_LANES);
	}

	@After
	public void tearDown() throws Exception {
	}

	@Test
	public void getNumberOfLanesTest() { 		
		assertEquals(NUMBER_OF_LANES, bowlingRoom.getNumberOfLanes());
	}
	
	@Test
	public void createLanesTest() throws Exception {
		bowlingRoom = 
			new BowlingRoom(BowlingRoomId.getUniqueBowlingRoomId(), "room", 5);
		assertEquals(5, bowlingRoom.getNumberOfLanes());
	}
	
	@Test (expected = Exception.class)
	public void createLanesTest2() throws Exception {
		bowlingRoom = 
			new BowlingRoom(BowlingRoomId.getUniqueBowlingRoomId(), "room", -1);
	}
	
	@Test
	public void isLaneFreeTest() {
		for (int i = 0; i < NUMBER_OF_LANES; i++) {
			assertTrue(bowlingRoom.isLaneFree(i));
		}
	}
	
	@Test
	public void setLaneBusyTest() {
		bowlingRoom.setLaneBusy(3);
		assertFalse(bowlingRoom.isLaneFree(3));
	}
	
	@Test
	public void setLaneFreeTest() {
		bowlingRoom.setLaneBusy(3);
		bowlingRoom.setLaneFree(3);
		assertTrue(bowlingRoom.isLaneFree(3));
	}	
}
