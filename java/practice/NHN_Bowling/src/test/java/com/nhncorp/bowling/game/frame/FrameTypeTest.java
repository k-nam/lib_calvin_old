package com.nhncorp.bowling.game.frame;

import org.junit.*;

import com.nhncorp.bowling.game.frame.BowlingFrame;
import com.nhncorp.bowling.game.frame.FrameType;
import com.nhncorp.bowling.game.frame.OpenFrame;
import com.nhncorp.bowling.game.frame.SpareFrame;
import com.nhncorp.bowling.game.frame.StrikeFrame;


public class FrameTypeTest {
	private FrameType spareFrame;
	private FrameType strikeFrame;
	private BowlingFrame frame;
	private BowlingFrame nextFrame;
	
	@Before
	public void setUp() throws Exception {
		frame = new BowlingFrame();
		nextFrame = new BowlingFrame();
		frame.setNextFrame(nextFrame);
		spareFrame = new SpareFrame(frame);
		strikeFrame = new StrikeFrame(frame);
	}

	@After
	public void tearDown() throws Exception {
	}
	
	@Test
	public void openFrameIsScoreFixedTest() {
		OpenFrame openFrame = new OpenFrame(null);
		openFrame.isScoreFixed();
	}

	@Test
	public void spareFrameIsScoreFixedTest() {
		SpareFrame spareFrame = new SpareFrame(null);
		spareFrame.isScoreFixed();
	}
	
	@Test
	public void spareFrameIsScoreFixedTest2() {
		frame.setNextFrame(null);
		spareFrame.getBonus();
	}
	
	@Test
	public void strkeFrameGetBonusTest() {
		frame.setNextFrame(null);
		strikeFrame.getBonus();
	}
	
	@Test
	public void strkeFrameGetBonusTest2() {
		strikeFrame.getBonus();
	}
	
	
}
