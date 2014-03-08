package client;


import java.util.List;
import java.util.Set;

import status.GameStatus;
import card.Card;


public interface ClientManagerInterface {
	
	public void initialize(GameStatus initialStatus);
	
	// Local event (for single player, two way event)
	public Card chooseCard(ClientId player, List<Card> candidates);
	public Card chooseCard(ClientId player, Card card1, Card card2);
	
	public boolean decideGoStop(ClientId player); // true for go, false for stop
	
	// Global event (for all players, one way event)
	public void hittingCard(ClientId hitter, Card card);
	public void flippingCard(ClientId flipper, Card card);
	public void takingCards(ClientId taker, List<Card> cards);
	public void stealingCards(ClientId stealer, List<Card> cards);


}