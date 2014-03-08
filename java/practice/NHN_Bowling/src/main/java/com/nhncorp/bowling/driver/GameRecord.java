package com.nhncorp.bowling.driver;

import com.nhncorp.bowling.game.SinglePlayerControl;

public class GameRecord {
	private FrameRecord[] frameRecords;
	
	public GameRecord() {
		frameRecords = new FrameRecord[SinglePlayerControl.TOTAL_NUMBER_OF_FRAMES + 1];
		for (int i = 1; i < frameRecords.length; i++) {
			frameRecords[i] = new FrameRecord();
		}
	}
	
	public boolean isStrike(int frameNumber) {
		return frameRecords[frameNumber].isStrike();
	}
	
	public int getFrameRecord(int frameNumber, boolean isFirstTurn) {
		if (isFirstTurn) {
			return frameRecords[frameNumber].getFirst();
		} else {
			return frameRecords[frameNumber].getSecond();
		}
	}
	
	public void setFrameRecord(int frameNumber, boolean isFirstTurn, int knockedPins) {
		if (isFirstTurn) {
			frameRecords[frameNumber].setFirst(knockedPins);
		} else {
			frameRecords[frameNumber].setSecond(knockedPins);
		}
	}
	
	public static GameRecord getGameRecordFromIntArray(int[] intArray) {
		GameRecord gameRecord = new GameRecord();
		boolean isFirstTurn = true;
		int frameNumber = 1;
		int intArrayIndex = 0;
		while (intArrayIndex < intArray.length) {
			int knockedPins = intArray[intArrayIndex++];
			if (isFirstTurn == true) {				
				gameRecord.setFrameRecord(frameNumber, true, knockedPins);
				// Strike
				if (gameRecord.isStrike(frameNumber)) {
					gameRecord.setFrameRecord(frameNumber, false, 0);
					frameNumber++;
				} else {
					isFirstTurn = false;
				}
			} else {
				gameRecord.setFrameRecord(frameNumber, false, knockedPins);
				frameNumber++;
				isFirstTurn = true;
			}				
		}
		return gameRecord;
	}

	public static GameRecord[] getGameRecordsFromIntArrays(int[][] intArrays) {
		GameRecord[] gameRecords = new GameRecord[intArrays.length];
		for (int i = 0; i < intArrays.length; i++) {
			gameRecords[i] = getGameRecordFromIntArray(intArrays[i]);
		}
		return gameRecords;		
	}

	public static boolean checkEquality(GameRecord gameRecord, int[] intArray) {
		int frameNumber = 1;
		int arrayIndex = 0;
		boolean isFirstTurn = true;		
		while (arrayIndex < intArray.length) {
			int knockedPins;
			if (isFirstTurn == true) {				
				knockedPins = gameRecord.getFrameRecord(frameNumber, true); 
				if (gameRecord.isStrike(frameNumber)) {
					frameNumber++;
				} else {
					isFirstTurn = false;
				}
			} else {
				knockedPins = gameRecord.getFrameRecord(frameNumber, false); 
				frameNumber++;
				isFirstTurn = true;
			}
			if (knockedPins != intArray[arrayIndex++]) {
				return false;
			}
		}
		return true;
	}

}
