package status;

import java.util.*;
import card.Card;
import card.InitialCardSet;
import card.Month;
import client.ClientId;

// Stores every card and 'go' status of all players
public class GameStatus {
	public ClientId turn; // whose turn it is now
	
	public List<Card> floorCards;
	public List<Card> hiddenCards;

	public List<Card> firstPlayerHandCards;
	public List<Card> secondPlayerHandCards;
	public List<Card> thirdPlayerHandCards;
	
	public TakenCard firstPlayerTakenCards;
	public TakenCard secondPlayerTakenCards;
	public TakenCard thirdPlayerTakenCards;
	
	// Condition to go/stop call: score >= 3 && score > lastScore
	public int firstPlayerGoCount;
	public int secondPlayerGoCount;
	public int thirdPlayerGoCount;
	
	public int firstPlayerLastScore;
	public int secondPlayerLastScore;
	public int thirdPlayerLastScore;
	
	public ClientId lastGoPlayer;
	
	public List<Month> firstPlayerFuck;
	public List<Month> secondPlayerFuck;
	public List<Month> thirdPlayerFuck;
	
	public List<Month> firstPlayerShake;
	public List<Month> secondPlayerShake;
	public List<Month> thirdPlayerShake;

	
	// Constructor
	public GameStatus(List<Card> floorCards, List<Card> hiddenCards,
			List<Card> firstPlayerHandCards, List<Card> secondPlayerHandCards,
			List<Card> thirdPlayerHandCards) {
		
		this.floorCards = floorCards;
		this.hiddenCards = hiddenCards;
		
		this.firstPlayerHandCards = firstPlayerHandCards;
		this.secondPlayerHandCards = secondPlayerHandCards;
		this.thirdPlayerHandCards = thirdPlayerHandCards;
		
		this.firstPlayerTakenCards = new TakenCard();
		this.secondPlayerTakenCards = new TakenCard();
		this.thirdPlayerTakenCards = new TakenCard();
		
		firstPlayerGoCount = 0;
		secondPlayerGoCount = 0;
		thirdPlayerGoCount = 0;
		
		firstPlayerLastScore = 0;
		secondPlayerLastScore = 0;
		thirdPlayerLastScore = 0;
		
		lastGoPlayer = null;
		
		firstPlayerFuck = new ArrayList<Month>();
		secondPlayerFuck = new ArrayList<Month>();
		thirdPlayerFuck = new ArrayList<Month>();
		
		firstPlayerShake = new ArrayList<Month>();
		secondPlayerShake = new ArrayList<Month>();
		thirdPlayerShake = new ArrayList<Month>();		
	}
	
	public String toString() {
		StringBuilder outputString = new StringBuilder();
		outputString.append("--- Floor cards ---\n");
		outputString.append(floorCards.toString() + "\n");
		
		outputString.append("<<< Player 1 >>>\n");
		outputString.append("--- Hand cards- --\n" + 
				firstPlayerHandCards.toString() + "\n");
		outputString.append("--- Taken cards ---\n" + 
				firstPlayerTakenCards.toString() + "\n");
		outputString.append("--> Go: " + firstPlayerGoCount + "\n");
		
		outputString.append("<<< Player 2 >>>\n");
		outputString.append("--- Hand cards- --\n" + 
				secondPlayerHandCards.toString() + "\n");
		outputString.append("--- Taken cards ---\n" + 
				secondPlayerTakenCards.toString() + "\n");
		outputString.append("--> Go: " + secondPlayerGoCount + "\n");
		
		outputString.append("<<< Player 3 >>>\n");
		outputString.append("--- Hand cards- --\n" + 
				thirdPlayerHandCards.toString() + "\n");
		outputString.append("--- Taken cards ---\n" + 
				thirdPlayerTakenCards.toString() + "\n");
		outputString.append("--> Go: " + thirdPlayerGoCount + "\n");
		
		return outputString.toString();
	}
	
	static GameStatus getRandomInitialStatus() {
		InitialCardSet randomCardSet = Card.getRandomInitialCardSet();
		GameStatus randomInitialStatus = 
			new GameStatus(randomCardSet.floorCards, randomCardSet.hiddenCards,
					randomCardSet.firstPlayerHandCards, randomCardSet.secondPlayerHandCards,
					randomCardSet.thirdPlayerHandCards);		
		return randomInitialStatus;
	}
}
