package status;

import java.util.ArrayList;
import java.util.List;
import card.Card;
import card.CardType;

public class TakenCard {
	public List<Card> lightCards;
	public List<Card> tenCards;
	public List<Card> fiveCards;
	public List<Card> shellCards;
	
	public TakenCard() {
		lightCards = new ArrayList<Card>();
		tenCards = new ArrayList<Card>();
		fiveCards = new ArrayList<Card>();
		shellCards = new ArrayList<Card>();
	}
	
	public String toString() {
		StringBuilder outputString = new StringBuilder();
		
		outputString.append("Light: "); 
		for (Card card : lightCards) {
			outputString.append(card);
		}
		outputString.append(" / ");		
		outputString.append("Ten: ");
		for (Card card : tenCards) {
			outputString.append(card);
		}
		outputString.append(" / ");		
		outputString.append("Five: ");
		for (Card card : fiveCards) {
			outputString.append(card);
		}
		outputString.append(" / ");		
		outputString.append("Shell: ");
		for (Card card : shellCards) {
			outputString.append(card);
		}
		outputString.append("\n");
		
		Score score = calculateScore();
		outputString.append("Light: " + score.lightScore + " / ");
		outputString.append("Ten: " + score.tenScore + " / ");
		outputString.append("Five: " + score.fiveScore + " / ");
		outputString.append("Shell: " + score.shellScore + " / ");
		outputString.append("HongDan: " + score.hongDanScore + " / ");
		outputString.append("ChungDan: " + score.chungDanScore + " / ");
		outputString.append("ChoDan: " + score.choDanScore + " / ");
		outputString.append("Godori: " + score.godoriScore + " / ");
		outputString.append("Total score: " + score.getTotalScore());
		
		return outputString.toString();
	}
	
	public Score calculateScore() {
		int lightScore = 0;
		int tenScore = 0;
		int fiveScore = 0;
		int shellScore = 0;
		int godoriScore = 0;
		int hongDanScore = 0;
		int chungDanScore = 0;
		int choDanScore = 0;
		

	// Light cards score
		if (lightCards.size() < 3) {
			lightScore = 0;
		} else if (lightCards.size() == 3) {
			if (lightCards.contains(Card.ELEVEN_LIGHT)) {
				lightScore = 2;
			} else {
				lightScore = 3;
			}
		} else if (lightCards.size() == 4) {
			lightScore = 4;
		} else if (lightCards.size() == 5) {
			lightScore = 15;
		}
		
		// Ten cards score		
		if (tenCards.size() < 5) {
			tenScore = 0;
		} else {
			tenScore = tenCards.size() - 4;
		}
		
		// Five cards score
		if (fiveCards.size() < 5) {
			fiveScore = 0;
		} else {
			fiveScore = fiveCards.size() - 4;
		}
		
		// Shell score
		if (shellCards.size() < 10) {
			shellScore = 0;
		} else {
			shellScore = shellCards.size() - 9;
			// Double shell case
			if (shellCards.contains(Card.NINE_TEN)) {
				shellScore++;
			}
			if (shellCards.contains(Card.TWELVE_SSANGPI)) {
				shellScore++;
			}		
		}
		
		// Godori
		if (tenCards.contains(Card.TWO_GODORI) && 
				tenCards.contains(Card.FOUR_GODORI) &&
				tenCards.contains(Card.EIGHT_GODORI)) {
			godoriScore = 5;
		} else {
			godoriScore = 0;
		}
		
		// HongDan
		if (fiveCards.contains(Card.ONE_HONGDAN) && 
				fiveCards.contains(Card.TWO_HONGDAN) &&
				fiveCards.contains(Card.THREE_HONGDAN)) {
			hongDanScore = 3;
		} else {
			hongDanScore = 0;
		}
			
		// ChungDan
		if (fiveCards.contains(Card.SIX_CHUNGDAN) && 
				fiveCards.contains(Card.NINE_CHUNGDAN) &&
				fiveCards.contains(Card.TEN_CHUNGDAN)) {
			chungDanScore = 3;
		} else {
			chungDanScore = 0;
		}
		
		// ChoDan
		if (fiveCards.contains(Card.FOUR_CHODAN) && 
				fiveCards.contains(Card.FIVE_CHODAN) &&
				fiveCards.contains(Card.SEVEN_CHODAN)) {
			choDanScore = 3;
		}	else {
			choDanScore = 0;
		}
				
		return new Score(lightScore, tenScore, fiveScore, shellScore,
				godoriScore, hongDanScore, chungDanScore, choDanScore);
	}

	public void addCards(List<Card> cardsToBeAdded) {
		for (Card card : cardsToBeAdded) {
			CardType thisType = card.getCardType();
			if (thisType == CardType.TrueLight || thisType == CardType.RainLight) {
				lightCards.add(card);			
			} else if (thisType == CardType.NormalTen || 
					thisType == CardType.FiveBirdsTen) {
				tenCards.add(card);
			} else if (thisType == CardType.RedFive || thisType == CardType.BlueFive ||
					thisType == CardType.GrassFive || thisType == CardType.NormalFive) {
				fiveCards.add(card);
			} else if (thisType == CardType.NormalShell || 
					thisType == CardType.DoubleShell) {
				shellCards.add(card);
			} else {
				assert(false);
			}
		}
	}

	public void stealCards(List<Card> cardsToBeStolen) {
		for (Card card : cardsToBeStolen) {
			if (card.getCardType() != CardType.NormalShell &&
					card.getCardType() != CardType.DoubleShell) {
				assert(false);
			} else if (this.shellCards.contains(card) == false){
				assert(false);
			} else {
				this.shellCards.remove(card);
			}
		}
	}
}
