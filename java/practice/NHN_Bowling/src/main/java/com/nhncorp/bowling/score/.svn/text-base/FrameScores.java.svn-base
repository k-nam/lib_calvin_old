package com.nhncorp.bowling.score;

import com.nhncorp.bowling.game.SinglePlayerControl;

public class FrameScores {
	private int[] frameScores;
	
	public FrameScores() {
		frameScores = 
			new int[SinglePlayerControl.NORMAL_FRAMES_PER_GAME + 1];
	}
		
	public static FrameScores getBowlingScoreFromIntArray(int[] inScore) {
		FrameScores score = new FrameScores();
		int frameNumber = 1;
		for (int i = 0; i < inScore.length; i++) {
			score.setFrameScore(frameNumber, inScore[i]);
			frameNumber++;
		}
		return score;
	}
	
	public static FrameScores[] getBowlingScoresFromIntArrays(int[][] inScores) {
		FrameScores[] bowlingScores = new FrameScores[inScores.length];
		for (int i = 0; i < inScores.length; i++) {
			bowlingScores[i] = getBowlingScoreFromIntArray(inScores[i]);
		}
		return bowlingScores;
	}
	
	public void setFrameScore(int frameNumber, int frameScore) {
		frameScores[frameNumber] = frameScore;
	}
	
	public boolean isEqualTo(FrameScores scoreToCompare) {
		boolean isEqual = true;
		for (int i = 1; 
			i < SinglePlayerControl.NORMAL_FRAMES_PER_GAME + 1; i++) {
			if (this.frameScores[i] != scoreToCompare.frameScores[i]) {
				isEqual = false;
			}
		}
		return isEqual;
	}

	public static boolean isEqual(FrameScores score1, FrameScores score2) {
		return score1.isEqualTo(score2);
	}
	
	public static boolean isEqual(FrameScores[] score1, FrameScores[] score2) {
		if (score1.length != score2.length) {
			return false;
		} else {
			for (int i = 0; i < score1.length; i++) {
				if (!isEqual(score1[i], score2[i])) {
					return false;
				}
			}
		}
		return true;
	}	
	
	// added
	public int getTotalScore() {
		int totalScore = 0;
		for (int i = 0; i < SinglePlayerControl.NORMAL_FRAMES_PER_GAME + 1; i++) {
			totalScore += frameScores[i];
		}		
		return totalScore;
	}
}
