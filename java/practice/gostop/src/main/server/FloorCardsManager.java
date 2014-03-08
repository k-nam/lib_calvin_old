package server;


import java.util.*;

import client.ClientId;

import card.*;



public class FloorCardsManager {

	private Set<Card> floorCards = null;
	private Server server = null;
	
	private List<Card> getCardsOfSameMonth(Card card) {
		List<Card> result = new ArrayList<Card>();
		for (Card floorCard : this.floorCards) {
			if (floorCard.isSameMonthAs(card)) {
				result.add(floorCard);
			}
		}
		return result;
	}
	
	public FloorCardsManager(Set<Card> initialFloorCards,
			Server server) {
		this.floorCards = initialFloorCards;
		this.server= server;
	}

	public void processTurn(ClientId hitter, Card hitCard, Card flippedCard) {

		// Cards that will be taken by the hitter
		HashSet<Card> cardsToBeTaken = new HashSet<Card>();
		// How many steals?
		int stealPoint = 0;

		// 1. Hitting step
		List<Card> cardsSameMonthAsHit = this.getCardsOfSameMonth(hitCard);
		if (cardsSameMonthAsHit.size() == 3) { // resolved dung
			cardsToBeTaken.add(hitCard);
			cardsToBeTaken.addAll(cardsSameMonthAsHit);
			stealPoint++;
		}
		// 2. Flipping step
		if (flippedCard.isSameMonthAs(hitCard)) { // hitCard == flippedCard
			switch (cardsSameMonthAsHit.size()) {
			case 0: // kiss: take only two cards
				cardsToBeTaken.add(hitCard);
				cardsToBeTaken.add(flippedCard);
				stealPoint++;
				break;
			case 1: // fuck: take no cards
				break;
			case 2: // dadak: take all four cards of the month
				cardsToBeTaken.add(hitCard);
				cardsToBeTaken.add(flippedCard);
				cardsToBeTaken.addAll(cardsSameMonthAsHit); // two cards on the floor
				stealPoint++;
				break;
			default:
				System.out.println("Internal error in FloorCardsManager");
				break;
			}
		} else { // hitCard != flippedCard
			switch (cardsSameMonthAsHit.size()) {
			case 0: // no event, no card will be taken
				break;
			case 1: // normal hit
				cardsToBeTaken.add(hitCard);
				cardsToBeTaken.addAll(cardsSameMonthAsHit);
				break;
			case 2: // need choice
				cardsToBeTaken.add(hitCard);
			case 3: // already done above
				break;
			}
			List<Card> cardsSameMonthAsFlipped = this
					.getCardsOfSameMonth(flippedCard);
			switch (cardsSameMonthAsFlipped.size()) {
			case 0: // no event, no card will be taken
				break;
			case 1: // normal hit
				cardsToBeTaken.add(flippedCard);
				cardsToBeTaken.addAll(cardsSameMonthAsFlipped);
				break;
			case 2: // need choice
			case 3: // dung resolve
				cardsToBeTaken.add(flippedCard);
				cardsToBeTaken.addAll(cardsSameMonthAsFlipped);
				stealPoint++;
				break;
			}

		}
		// 3. Taking step
		this.floorCards.add(hitCard);
		this.floorCards.add(flippedCard);
		//processEvent(new CardTakingEvent(hitter, cardsToBeTaken));
		this.floorCards.removeAll(cardsToBeTaken);		
		if (this.floorCards.isEmpty()) {
			stealPoint++;
		}
		// 4. Stealing step

		return;

	}



}
