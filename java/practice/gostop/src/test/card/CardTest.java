package card;

import static org.junit.Assert.*;

import org.junit.*;

import card.Card;
import card.CardType;
import card.Month;

public class CardTest {
	


	@Test
	public void getRandomInitialCardSetTest() {
		System.out.println("testing");
		InitialCardSet cardSet = Card.getRandomInitialCardSet();
		
		assertEquals(6, cardSet.floorCards.size());
		assertEquals(21, cardSet.hiddenCards.size());
		assertEquals(7, cardSet.firstPlayerHandCards.size());
		assertEquals(7, cardSet.secondPlayerHandCards.size());
		assertEquals(7, cardSet.thirdPlayerHandCards.size());
		
		System.out.println(cardSet);
	}

}
