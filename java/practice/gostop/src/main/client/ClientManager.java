package client;

import java.util.List;

import status.GameStatus;
import card.Card;

public class ClientManager implements ClientManagerInterface {

	@Override
	public void initialize(GameStatus initialStatus) {
		System.out.println("ClientManager is initialized with game status:\n"
				+ initialStatus);
	}
	
	@Override
	public Card chooseCard(ClientId player, List<Card> candidates) {
		System.out.println("Request: " + player + " to choose card.");
		System.out.println(player + " chose " + candidates.get(0));
		return candidates.get(0);
	}

	@Override
	public Card chooseCard(ClientId player, Card card1, Card card2) {
		System.out.println("Request: " + player + " to choose card between two.");
		System.out.println(player + " chose " + card1);
		return card1;
	}

	@Override
	public boolean decideGoStop(ClientId player) {
		System.out.println("Request: " + player + " to choose go/stop.");
		System.out.println(player + " chose <GO>");
		return true;
	}

	
	@Override
	public void hittingCard(ClientId hitter, Card card) {
		System.out.println(hitter + " hit card: " + card);
		return;
	}
	
	@Override
	public void flippingCard(ClientId flipper, Card card) {
		System.out.println(flipper + " flipped card: " + card);
		return;
	}

	@Override
	public void takingCards(ClientId taker, List<Card> cards) {
		System.out.println(taker + " took cards: " + cards);
		return;
	}
	
	@Override
	public void stealingCards(ClientId stealer, List<Card> cards) {
		System.out.println(stealer + " stole cards: " + cards);
		return;
	}
}
