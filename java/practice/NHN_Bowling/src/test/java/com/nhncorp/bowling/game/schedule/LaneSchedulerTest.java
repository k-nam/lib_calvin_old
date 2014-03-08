package com.nhncorp.bowling.game.schedule;


import org.junit.*;

import com.nhncorp.bowling.game.schedule.LaneScheduler;
import com.nhncorp.bowling.game.schedule.Schedule;

import static org.junit.Assert.*;

public class LaneSchedulerTest {
	private LaneScheduler scheduler;
	final int numberOfPlayers = 4;
	final int numberOfLanes = 2;

	
	@Before
	public void setUp() throws Exception {
		scheduler = new LaneScheduler(numberOfPlayers, numberOfLanes);
	}

	@After
	public void tearDown() throws Exception {
	}

	@Test
	public void testInitialize() throws Exception {
		scheduler = new LaneScheduler(numberOfPlayers, numberOfLanes);
		
	}
	
	@Test
	public void testGetNewSchedule() {
		Schedule schedule1 = scheduler.getNewSchedule();
		Schedule schedule2 = scheduler.getNewSchedule();
		assertEquals(0, schedule1.getLaneNumber());
		assertEquals(0, schedule1.getPlayerNumber());
		assertEquals(1, schedule2.getLaneNumber());
		assertEquals(2, schedule2.getPlayerNumber());
		scheduler.setFree(0);
		Schedule schedule3 = scheduler.getNewSchedule();
		assertEquals(0, schedule3.getLaneNumber());
		assertEquals(1, schedule3.getPlayerNumber());
		scheduler.setFree(0);
		Schedule schedule4 = scheduler.getNewSchedule();
		assertEquals(0, schedule4.getLaneNumber());
		assertEquals(0, schedule4.getPlayerNumber());


	}
	
	
	@Test 
	public void testGetNewSchedule2() {
		scheduler.getNewSchedule();
		scheduler.getNewSchedule();
		Schedule schedule = scheduler.getNewSchedule();
		assertEquals(null, schedule);
	}
}
