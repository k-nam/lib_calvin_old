package server;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import status.GameStatus;

import client.ClientId;
import client.ClientManagerInterface;

import card.Card;
import card.CardType;

public class Server implements ServerInterface {

	@Override
	public void processTurn(GameStatus status, ClientManagerInterface clientManager) {
		ClientId player = status.turn;
		List<Card> floorCards = status.floorCards;
		
		// What does he hit?
		Card hitCard = clientManager.chooseCard(player, status.firstPlayerHandCards);
		
		// Flip a card
		assert(status.hiddenCards.size() > 0);
		Card flippedCard = status.hiddenCards.get(0);
		status.hiddenCards.remove(0);
		
		// Floor cards operations
		List<Card> cardsToBeTaken = new ArrayList<Card>();		
		int stealPoint = 0; // How many steals?
		
		List<Card> cardsSameMonthAsHit = 
			this.getCardsOfSameMonth(floorCards, hitCard);
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
				assert(false);
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
				cardsToBeTaken.add(clientManager.chooseCard(
						player, cardsSameMonthAsHit.get(0), cardsSameMonthAsHit.get(1)));
				break;
			case 3: // dung resolve
				cardsToBeTaken.add(hitCard);
				cardsToBeTaken.addAll(cardsSameMonthAsHit);
				stealPoint++;
				break;
			}
			List<Card> cardsSameMonthAsFlipped = 
				this.getCardsOfSameMonth(floorCards, flippedCard);
			switch (cardsSameMonthAsFlipped.size()) {
			case 0: // no event, no card will be taken
				break;
			case 1: // normal hit
				cardsToBeTaken.add(flippedCard);
				cardsToBeTaken.addAll(cardsSameMonthAsFlipped);
				break;
			case 2: // need choice
				cardsToBeTaken.add(flippedCard);
				cardsToBeTaken.add(clientManager.chooseCard(
						player, cardsSameMonthAsFlipped.get(0), 
						cardsSameMonthAsFlipped.get(1)));
			case 3: // dung resolve
				cardsToBeTaken.add(flippedCard);
				cardsToBeTaken.addAll(cardsSameMonthAsFlipped);
				stealPoint++;
				break;
			}
		}
		
		// Taking		
		floorCards.add(hitCard);
		floorCards.add(flippedCard);
		clientManager.takingCards(player, cardsToBeTaken);
		floorCards.removeAll(cardsToBeTaken);	
		switch (player) {
		case First:
			status.firstPlayerTakenCards.addCards(cardsToBeTaken);
			break;
		case Second:
			status.secondPlayerTakenCards.addCards(cardsToBeTaken);
			break;
		case Third:
			status.thirdPlayerTakenCards.addCards(cardsToBeTaken);
			break;
		}
		if (floorCards.isEmpty()) {
			stealPoint++;
		}
		
		// Stealing 
		switch (player) {
		case First:
			List<Card> cardsToBeStolenFromSecond = 
				getCardsToBeStolen(status.secondPlayerTakenCards.shellCards, stealPoint);
			
			break;
		case Second:
			status.secondPlayerTakenCards.addCards(cardsToBeTaken);
			break;
		case Third:
			status.thirdPlayerTakenCards.addCards(cardsToBeTaken);
			break;
		}		
		//clientManager.stealingCards(player, cardsToBeStolen);
		//clientManager.stealingCards(player, cards);
		// See score and decide go/stop if needed
		
	}	
	
	
	public static List<Card> getCardsToBeStolen(List<Card> shellCards, 
			int stealPoint) {
		List<Card> normalShells = new ArrayList<Card>();
		List<Card> doubleShells = new ArrayList<Card>();
		List<Card> cardsToBeStolen = new ArrayList<Card>();
		
		for (Card card : shellCards) {
			if (card.getCardType() == CardType.NormalShell) {
				normalShells.add(card);
			} else if (card.getCardType() == CardType.DoubleShell) {
				doubleShells.add(card);
			} else {
				assert(false);
			}
		}
		int curStealCount = 0;
		for (int index = 0 ; curStealCount < stealPoint && index < normalShells.size(); 
					index++, curStealCount++) {
			cardsToBeStolen.add(normalShells.get(index));
		}
		if (curStealCount < stealPoint ) { // we must steal double shells, too T.T
			for (int index = 0; curStealCount < stealPoint && index < doubleShells.size(); 
					index++, curStealCount += 2) { // double shell counts as 2 shells!
				cardsToBeStolen.add(doubleShells.get(index));
			}
		}
		if (curStealCount > stealPoint) { // Rescue over-stealing if possible
			if (cardsToBeStolen.get(0).getCardType() == CardType.NormalShell) {
				cardsToBeStolen.remove(0);
			} else {
				// We have to over-steal one shell at last
			}
		}
		return cardsToBeStolen;
	}
	
	public static List<Card> getCardsOfSameMonth(List<Card> floorCards, Card card) {
		List<Card> result = new ArrayList<Card>();
		for (Card floorCard : floorCards) {
			if (floorCard.isSameMonthAs(card)) {
				result.add(floorCard);
			}
		}
		return result;
	}
	
}
