package card;

import java.util.List;

public class InitialCardSet {
	public List<Card> floorCards;
	public List<Card> hiddenCards;
	public List<Card> firstPlayerHandCards;
	public List<Card> secondPlayerHandCards;
	public List<Card> thirdPlayerHandCards;
	
	public InitialCardSet(List<Card> floorCards, List<Card> hiddenCards,
			List<Card> firstPlayerHandCards, List<Card> secondPlayerHandCards,
			List<Card> thirdPlayerHandCards) {
		
		this.floorCards = floorCards;
		this.hiddenCards = hiddenCards;
		this.firstPlayerHandCards = firstPlayerHandCards;
		this.secondPlayerHandCards = secondPlayerHandCards;
		this.thirdPlayerHandCards = thirdPlayerHandCards;
	}
	
	public String toString() {
		StringBuilder builder = new StringBuilder();
		builder.append("Floor cards: " + floorCards.toString() + "\n");
		builder.append("Hidden cards: " + hiddenCards.toString() + "\n");
		builder.append("Player 1 cards: " + firstPlayerHandCards.toString() + "\n");
		builder.append("Player 2 cards: " + secondPlayerHandCards.toString() + "\n");
		builder.append("Player 3 cards: " + thirdPlayerHandCards.toString());
		return builder.toString();
	}
}
