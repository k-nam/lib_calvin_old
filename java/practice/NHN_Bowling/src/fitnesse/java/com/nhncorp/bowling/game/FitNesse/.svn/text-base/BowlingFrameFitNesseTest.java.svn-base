package com.nhncorp.bowling.game.FitNesse;

import fit.ActionFixture;
import com.nhncorp.bowling.game.frame.BowlingFrame;

public class BowlingFrameFitNesseTest extends ActionFixture {
	private BowlingFrame frame;
	private BowlingFrame nextFrame;
	private BowlingFrame nextNextFrame;
	
	public void prepareBowlingFrame() throws Exception {
		frame = new BowlingFrame();
		nextFrame = new BowlingFrame();
		nextNextFrame = new BowlingFrame();		
		frame.setNextFrame(nextFrame);
		nextFrame.setNextFrame(nextNextFrame);
	}	
		
	public void startBowlingFrame() {
		frame.start();
	}
	
	public boolean isFrameStarted() {
		return frame.isStarted();
	}
	
	public boolean isFrameFinished() {
		return frame.isFinished();
	}
	
	public void knockPins (int numberOfKnockedPins) throws Exception {
		frame.knockPins(numberOfKnockedPins);
	}
	
	public boolean isInFirstTurn () {
		return frame.isInFirstTurn();
	}
	
	public int getFrameScore() throws Exception {
		return frame.calculateScore();
	}
/*
	public void isStartedTest() {
		assertFalse(frame.isStarted());
	}
	

	public void isStartTest2() {
		frame.start();
		assertTrue(frame.isStarted());
	}
	
 
	public void isInFirstTurnTest() {
		frame.start();
		assertTrue(frame.isInFirstTurn());
	}
	
 
	public void isInFirstTurnTest2() throws Exception {
		frame.start();
		frame.knockPins(5);
		assertFalse(frame.isInFirstTurn());
	}
	

	public void knockPinsTest() throws Exception {
		frame.start();
		frame.knockPins(5);
		frame.knockPins(5);
		assertTrue(frame.isFinished());
	}
	

	public void knockPinsdTest2() throws Exception {
		frame.start();
		frame.knockPins(10);
		assertTrue(frame.isFinished());
	}
	

	public void knockPinsTest3() throws Exception {
		frame.start();
		frame.knockPins(2);
		frame.knockPins(4);
		assertTrue(frame.isFinished());
	}
	
	//@Test (expected = Exception.class)
	public void knockPinsTest4() throws Exception {
		frame.knockPins(7);
	}
	
	//@Test (expected = Exception.class)
	public void knockPinsTest5() throws Exception {
		frame.start();
		frame.knockPins(7);
		frame.knockPins(3);
		frame.knockPins(3);
	}
	
	//@Test (expected = Exception.class)
	public void getKnockedPinsTest() throws Exception {
		frame.start();
		frame.getKnockedPins(Turns.Turns_First);
	}
	
  //@Test (expected = Exception.class)
	public void getKnockedPinsTest2() throws Exception {
		frame.start();
		frame.knockPins(7);
		frame.getKnockedPins(Turns.Turns_First);
		frame.getKnockedPins(Turns.Turns_Second);
	}
	
  //@Test (expected = Exception.class)
	public void getKnockedPinsTest3() throws Exception {
		frame.start();
		frame.knockPins(10);
		assertEquals(10, frame.getKnockedPins(Turns.Turns_Second));
	}
  
  //@Test
	public void getKnockedPinsTest4() throws Exception {
		frame.start();
		frame.knockPins(3);
		assertEquals(3, frame.getKnockedPins(Turns.Turns_First));
		frame.knockPins(5);
		assertEquals(3, frame.getKnockedPins(Turns.Turns_First));
		assertEquals(5, frame.getKnockedPins(Turns.Turns_Second));
	}
  
	//@Test
	public void getNextFrameTest() {
		frame.setNextFrame(nextFrame);
		assertEquals(nextFrame, frame.getNextFrame());
	}
	
	//@Test (expected = Exception.class)
	public void getTotalKnockedPinsTest() throws Exception {
		frame.start();
		frame.getTotalKnockedPins();
	}
	
  //@Test
  public void calculateScoreTest() throws Exception {
  	frame.start();  
  	frame.knockPins(5);
  	assertEquals(frame.calculateScore(), 5);
  }
	
  //@Test
  public void calculateScoreTest2() throws Exception {
  	frame.start();  
  	frame.knockPins(5);
   	frame.knockPins(3);
  	assertEquals(frame.calculateScore(), 8);
  }

  //@Test
  public void calculateScoreTest3() throws Exception {
  	frame.start();  
  	frame.knockPins(4);
   	frame.knockPins(6);
  	assertEquals(frame.calculateScore(), 10);
  }
  
  //@Test
  public void calculateScoreTest4() throws Exception {
  	frame.start();  
  	frame.knockPins(4);
   	frame.knockPins(6);
   	nextFrame.start();
   	nextFrame.knockPins(7);
  	assertEquals(frame.calculateScore(), 17);
  }
  
  //@Test
  public void calculateScoreTest5() throws Exception {
  	frame.start();  
  	frame.knockPins(4);
   	frame.knockPins(6);
   	nextFrame.start();
   	nextFrame.knockPins(7);
   	nextFrame.knockPins(3);
  	assertEquals(frame.calculateScore(), 17);
  }
  
  //@Test
  public void calculateScoreTest6() throws Exception {
  	frame.start();  
  	frame.knockPins(4);
   	frame.knockPins(6);
   	nextFrame.start();
   	nextFrame.knockPins(10);
  	assertEquals(frame.calculateScore(),20);
  }
  
  //@Test
  public void calculateScoreTest7() throws Exception {
  	frame.start();  
  	frame.knockPins(10);
   	nextFrame.start();
   	nextFrame.knockPins(8);
  	assertEquals(frame.calculateScore(),18);
  }
  
  //@Test
  public void calculateScoreTest8() throws Exception {
  	frame.start();  
  	frame.knockPins(10);
   	nextFrame.start();
   	nextFrame.knockPins(10);
  	assertEquals(frame.calculateScore(), 20);
  }
  
  //@Test
  public void calculateScoreTest9() throws Exception {
  	frame.start();  
  	frame.knockPins(10);
   	nextFrame.start();
   	nextFrame.knockPins(10);
   	nextNextFrame.start();
   	nextNextFrame.knockPins(3);
  	assertEquals(frame.calculateScore(), 23);
  }
  
  //@Test
  public void calculateScoreTes10() throws Exception {
  	frame.start();  
  	frame.knockPins(10);
   	nextFrame.start();
   	nextFrame.knockPins(10);
   	nextNextFrame.start();
   	nextNextFrame.knockPins(10);
  	assertEquals(frame.calculateScore(), 30);
  }
  
  //@Test
  public void isScoreFixedTest() throws Exception {
  	frame.start();
  	frame.knockPins(5);
  	assertFalse(frame.isScoreFixed());
  }
  
  //@Test
  public void isScoreFixedTest2() throws Exception {
  	frame.start();
  	frame.knockPins(5);
  	frame.knockPins(5);
  	assertFalse(frame.isScoreFixed());
  }
  
  //@Test
  public void isScoreFixedTest3() throws Exception {
  	frame.start();
  	frame.knockPins(5);
  	frame.knockPins(5);
  	nextFrame.start();
  	nextFrame.knockPins(7);
  	assertTrue(frame.isScoreFixed());
  	assertFalse(nextFrame.isScoreFixed());
  }
  
  //@Test
  public void isScoreFixedTest4() throws Exception {
  	frame.start();
  	frame.knockPins(10); // strike
  	assertFalse(frame.isScoreFixed());
  	nextFrame.start();
  	nextFrame.knockPins(7);
  	assertFalse(frame.isScoreFixed());
  	nextFrame.knockPins(3); // spare
  	assertTrue(frame.isScoreFixed());
  	assertFalse(nextFrame.isScoreFixed());
  }
  
  //@Test
  public void isScoreFixedTest5() throws Exception {
  	frame.start();
  	frame.knockPins(10); // strike
  	assertFalse(frame.isScoreFixed());
  	nextFrame.start();
  	nextFrame.knockPins(10); // strike again
  	assertFalse(frame.isScoreFixed());
  	nextNextFrame.start();
  	nextNextFrame.knockPins(10);
  	assertTrue(frame.isScoreFixed());
  	assertFalse(nextFrame.isScoreFixed());
  	assertFalse(nextNextFrame.isScoreFixed());
  }
}*/
}