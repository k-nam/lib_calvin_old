package com.nhncorp.bowling.main;

import java.util.ArrayDeque;

import com.nhncorp.bowling.game.BowlingGame;

public class GameScheduleBuffer {
	private ArrayDeque<BowlingGame> newGameQueue;
	private ArrayDeque<BowlingGame> finishedGameQueue;
	
	public GameScheduleBuffer() {
		newGameQueue = new ArrayDeque<BowlingGame>();
		finishedGameQueue = new ArrayDeque<BowlingGame>();
	}
	
	public synchronized void enqueNewGame(BowlingGame newGame) {
		//System.out.println("queueing new game");
		newGameQueue.add(newGame);
	}
	
	public synchronized BowlingGame getNewGame() {
		//System.out.println("polling new game");
		return newGameQueue.poll();
	}
	
	public synchronized void enqueFinishedGame(BowlingGame finishedGame) {
		//System.out.println("queueing finished game");
		finishedGameQueue.add(finishedGame);
	}
	
	public synchronized BowlingGame getFinishedGame() {
		//System.out.println("polling finished game");
		return finishedGameQueue.poll();
	}
	
}
