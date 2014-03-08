package com.nhncorp.bowling.main;



import java.util.HashMap;
import java.util.Random;
import java.util.Vector;

import com.nhncorp.bowling.game.BowlingGame;
import com.nhncorp.bowling.game.order.GameOrder;
import com.nhncorp.bowling.room.BowlingLane;
import com.nhncorp.bowling.room.BowlingLaneId;




public class FakeClient extends Thread {
	private GameScheduleBuffer gameScheduleBuffer;
	private Random random;
	
	public FakeClient(GameScheduleBuffer buffer) {
		gameScheduleBuffer = buffer;
		random = new Random();
	}
	
	public void run() {
		while (true) {
			try {
				Thread.sleep(10);
			} catch (InterruptedException e1) {
				e1.printStackTrace();
			}
			
			BowlingGame newGame = gameScheduleBuffer.getNewGame();
			if (newGame == null) {
				continue;
			}
			try {
				BowlingLane[] bowlingLanes = new BowlingLane[2];
				BowlingLaneId[] bowlingLaneIds = new BowlingLaneId[2];
				bowlingLaneIds[0] = BowlingLaneId.getUniqueBowlingLaneId();
				bowlingLanes[0] = new BowlingLane(bowlingLaneIds[0]);
				bowlingLaneIds[1] = BowlingLaneId.getUniqueBowlingLaneId();
				bowlingLanes[1] = new BowlingLane(bowlingLaneIds[1]);
				HashMap<BowlingLaneId, BowlingLane> laneIdToLaneMap = 
					new HashMap<BowlingLaneId, BowlingLane>();
				laneIdToLaneMap.put(bowlingLaneIds[0], bowlingLanes[0]);
				laneIdToLaneMap.put(bowlingLaneIds[1], bowlingLanes[1]);
				newGame.setLaneIds(bowlingLaneIds);
				newGame.startGame();
				while(true) {
					Vector<GameOrder> currentOrders = newGame.getOrders();
					if (currentOrders.isEmpty() == true) {
						break;
					} else {
						for (int i = 0; i < currentOrders.size(); i++) {
							GameOrder order = currentOrders.get(i);
							if (order.isGameFinished() == true) {
								BowlingLaneId laneId = order.getLaneId();
								laneIdToLaneMap.get(laneId).reset();	
								return;
							} else if (order.isFrameFinished()) {
								BowlingLaneId laneId = order.getLaneId();
								laneIdToLaneMap.get(laneId).reset();							
							} else {
								BowlingLaneId laneId = order.getLaneId();
								int remainingPins = laneIdToLaneMap.get(laneId).getRemainingNumberOfPins();
								int knockedPins = random.nextInt(remainingPins);
								newGame.knockPins(laneId, knockedPins);
							}
						}
					}
				}			
			} catch (Exception e) {
				e.printStackTrace();
			}
			gameScheduleBuffer.enqueFinishedGame(newGame);
		}
	}
}
