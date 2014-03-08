package com.nhncorp.bowling.driver;

import java.util.Vector;

import com.nhncorp.bowling.game.MultiPlayerControl;
import com.nhncorp.bowling.game.SinglePlayerControl;
import com.nhncorp.bowling.game.order.*;

public class BowlingGameDriver {	
	public static void simulateBowlingGame(SinglePlayerControl control,
			GameRecord gameRecord) throws Exception {
		
		GameOrder order = null;
		int frameNumber = 1;
		while (true) {
			if (control.isNowPlaying() == false) {
				control.startFrame();
				order = control.getOrder();				
			}			
			control.knockPins(gameRecord.getFrameRecord(frameNumber, true));
			order = control.getOrder();
			if (gameRecord.isStrike(frameNumber) == false) {
				control.knockPins(gameRecord.getFrameRecord(frameNumber, false));
				order = control.getOrder();
			}
			if (order.isGameFinished()) {
				break;
			} else {
				frameNumber++;
			}
		}		
	}
	
	public static GameOrder simulateBowlingGame(SinglePlayerControl control,
			int[] playingRecord) throws Exception {
		
		GameOrder order = null;
		int recordIndex = 0;
		control.startFrame();
		order = control.getOrder();
		while (recordIndex < playingRecord.length) {
			if (order.isGameFinished()) {
				break;
			} else if (order.isFrameFinished()) {
				control.startFrame();
				control.knockPins(playingRecord[recordIndex++]);
				order = control.getOrder();
			} else {
				control.knockPins(playingRecord[recordIndex++]);
				order = control.getOrder();
			}
		}	
		return order;
	}
	
	public static void simulateBowlingGame(MultiPlayerControl control,
			GameRecord[] gameRecords) throws Exception {		

		
		boolean[][] isFirstTurn = new boolean[gameRecords.length][];
		for (int i = 0; i < gameRecords.length; i++) {
			isFirstTurn[i] = new boolean[SinglePlayerControl.TOTAL_NUMBER_OF_FRAMES + 1];
			for (int j = 0; j <= SinglePlayerControl.TOTAL_NUMBER_OF_FRAMES; j++) {
				isFirstTurn[i][j] = true;
			}
		}
		control.startFrame();
		Vector<GameOrder> currentOrders = control.getOrders();

		while (true) {
			for (int i = 0; i < currentOrders.size(); i++) {
				GameOrder currentOrder = currentOrders.get(i);	
				try {			
					int playerIndex = control.getIndexOf(currentOrder.getPlayer());
					int frameNumber = ((FrameContinueOrder)currentOrder).getFrameNumber();
					if (isFirstTurn[playerIndex][frameNumber] == true) {
						control.knockPins(((FrameContinueOrder)currentOrder).getLaneId(),
								gameRecords[playerIndex].getFrameRecord(frameNumber, true));
						isFirstTurn[playerIndex][frameNumber] = false;
					} else {
						control.knockPins(((FrameContinueOrder)currentOrder).getLaneId(),
								gameRecords[playerIndex].getFrameRecord(frameNumber, false));
					}				
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
			currentOrders = control.getOrders();
			if (currentOrders.isEmpty()) {
				if (control.isGameFinished()) {
					return;
				} 
			}
		}
	}
	
}
