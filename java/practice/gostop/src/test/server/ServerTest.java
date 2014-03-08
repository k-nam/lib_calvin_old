package server;
import static org.junit.Assert.*;

import java.util.ArrayList;
import java.util.List;

import org.junit.*;

import card.Card;

public class ServerTest {
	
	// Normal case (stealPoint < number of normal shells)
	@Test 
	public void getCardsToBeStolenTest_01() {
		List<Card> shells = new ArrayList<Card>();
		shells.add(Card.ONE_SHELL_1);
		shells.add(Card.TWO_SHELL_2);
		shells.add(Card.TWELVE_SHELL_2);
		List<Card> cardsToBeStolen = Server.getCardsToBeStolen(shells, 2);
		assertEquals(2, cardsToBeStolen.size());
		assertEquals(Card.ONE_SHELL_1, cardsToBeStolen.get(0));
		assertEquals(Card.TWO_SHELL_2, cardsToBeStolen.get(1));
	}
	
	// Boundary case (stealPoint == number of normal shells)
	@Test 
	public void getCardsToBeStolenTest_02() {
		List<Card> shells = new ArrayList<Card>();
		shells.add(Card.ONE_SHELL_1);
		shells.add(Card.TWO_SHELL_2);
		shells.add(Card.TWELVE_SHELL_2);
		List<Card> cardsToBeStolen = Server.getCardsToBeStolen(shells, 3);
		assertEquals(3, cardsToBeStolen.size());
		assertEquals(Card.ONE_SHELL_1, cardsToBeStolen.get(0));
		assertEquals(Card.TWO_SHELL_2, cardsToBeStolen.get(1));
		assertEquals(Card.TWELVE_SHELL_2, cardsToBeStolen.get(2));
	}
	
	// Steal double shells case (stealPoint > number of normal shells)
	@Test 
	public void getCardsToBeStolenTest_03() {
		List<Card> shells = new ArrayList<Card>();
		shells.add(Card.ONE_SHELL_1);
		shells.add(Card.TWO_SHELL_2);
		shells.add(Card.TWELVE_SSANGPI);
		List<Card> cardsToBeStolen = Server.getCardsToBeStolen(shells, 3);
		assertEquals(2, cardsToBeStolen.size());
		assertEquals(Card.TWO_SHELL_2, cardsToBeStolen.get(0));
		assertEquals(Card.TWELVE_SSANGPI, cardsToBeStolen.get(1));
	}
	
	// Steal double shell + over-steal
	@Test 
	public void getCardsToBeStolenTest_04() {
		List<Card> shells = new ArrayList<Card>();
		shells.add(Card.ELEVEN_SSANGPI);
		shells.add(Card.TWELVE_SSANGPI);
		List<Card> cardsToBeStolen = Server.getCardsToBeStolen(shells, 3);
		assertEquals(2, cardsToBeStolen.size());
		assertEquals(Card.ELEVEN_SSANGPI, cardsToBeStolen.get(0));
		assertEquals(Card.TWELVE_SSANGPI, cardsToBeStolen.get(1));
	}
	
	// Steal double shell + over-steal
	@Test 
	public void getCardsToBeStolenTest_05() {
		List<Card> shells = new ArrayList<Card>();
		shells.add(Card.TWELVE_SSANGPI);
		List<Card> cardsToBeStolen = Server.getCardsToBeStolen(shells, 1);
		assertEquals(1, cardsToBeStolen.size());
		assertEquals(Card.TWELVE_SSANGPI, cardsToBeStolen.get(0));
	}
	
	// Steal only double shells
	@Test 
	public void getCardsToBeStolenTest_06() {
		List<Card> shells = new ArrayList<Card>();
		shells.add(Card.ELEVEN_SSANGPI);
		shells.add(Card.TWELVE_SSANGPI);
		List<Card> cardsToBeStolen = Server.getCardsToBeStolen(shells, 2);
		assertEquals(1, cardsToBeStolen.size());
		assertEquals(Card.ELEVEN_SSANGPI, cardsToBeStolen.get(0));
	}

}
