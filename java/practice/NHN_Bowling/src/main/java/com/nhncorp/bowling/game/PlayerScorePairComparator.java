package com.nhncorp.bowling.game;

import java.util.Comparator;

public class PlayerScorePairComparator implements Comparator<PlayerScorePair> {

	public int compare(PlayerScorePair o1, PlayerScorePair o2) {
		if (o1.getAdjustedScore() < o2.getAdjustedScore()) {
			return -1;
		} else if (o1.getAdjustedScore() == o2.getAdjustedScore()) {
			return 0;
		} else {
			return 1;
		}
	}
	
}
