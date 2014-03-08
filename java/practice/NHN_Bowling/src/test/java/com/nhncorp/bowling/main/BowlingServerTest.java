package com.nhncorp.bowling.main;


import org.junit.*;

public class BowlingServerTest {
	private BowlingServer server;
	private FakeClient client;
	
	@Before
	public void setUp() throws Exception {
		server = new BowlingServer();
		for (int i = 0; i < BowlingServer.NUMBER_OF_EMPLOYEES; i++) {
			server.logOn("KR" + i, "KR" + i);
		}
		for (int i = 0; i < BowlingServer.NUMBER_OF_EMPLOYEES; i++) {
			server.betting("KR" + i, "KR" + i);
		}
		for (int i = 0; i < BowlingServer.NUMBER_OF_BOWLING_PLAYERS; i++) {
			server.apply("KR" + i);
		}
		client = new FakeClient(server.getGameScheduleBuffer());
	}

	@After
	public void tearDown() throws Exception {
	}
	
	@Test
	public void serverTest() throws Exception {
		client.start();
		server.startBowlingTournament();
		client.stop();
		
	}

}
