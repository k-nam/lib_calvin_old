package card;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public enum Card {
	ONE_LIGHT(Month.January, CardType.TrueLight, "1광"),
	ONE_HONGDAN(Month.January, CardType.RedFive, "1홍단"),
	ONE_SHELL_1(Month.January, CardType.NormalShell, "1피"),
	ONE_SHELL_2(Month.January, CardType.NormalShell, "1피"),
	
	TWO_GODORI(Month.February, CardType.FiveBirdsTen, "2고도리"),
	TWO_HONGDAN(Month.February, CardType.RedFive, "2홍단"),
	TWO_SHELL_1(Month.February, CardType.NormalShell, "2피"),
	TWO_SHELL_2(Month.February, CardType.NormalShell, "2피"),
	
	THREE_LIGHT(Month.March, CardType.TrueLight, "3광"),
	THREE_HONGDAN(Month.March, CardType.RedFive, "3홍단"),
	THREE_SHELL_1(Month.March, CardType.NormalShell, "3피"),
	THREE_SHELL_2(Month.March, CardType.NormalShell, "3피"),
	
	FOUR_GODORI(Month.April, CardType.FiveBirdsTen, "4고도리"),
	FOUR_CHODAN(Month.April, CardType.GrassFive, "4초단"),
	FOUR_SHELL_1(Month.April, CardType.NormalShell, "4피"),
	FOUR_SHELL_2(Month.April, CardType.NormalShell, "4피"),
	
	FIVE_TEN(Month.May, CardType.NormalTen, "5열끗"),
	FIVE_CHODAN(Month.May, CardType.GrassFive, "5초단"),
	FIVE_SHELL_1(Month.May, CardType.NormalShell, "5피"),
	FIVE_SHELL_2(Month.May, CardType.NormalShell, "5피"),
	
	SIX_TEN(Month.June, CardType.NormalTen, "6열끗"),
	SIX_CHUNGDAN(Month.June, CardType.BlueFive, "6청단"),
	SIX_SHELL_1(Month.June, CardType.NormalShell, "6피"),
	SIX_SHELL_2(Month.June, CardType.NormalShell, "6피"),
	
	SEVEN_TEN(Month.July, CardType.NormalTen, "7열끗"),
	SEVEN_CHODAN(Month.July, CardType.GrassFive, "7초단"),
	SEVEN_SHELL_1(Month.July, CardType.NormalShell, "7피"),
	SEVEN_SHELL_2(Month.July, CardType.NormalShell, "7피"),
	
	EIGHT_LIGHT(Month.August, CardType.TrueLight, "8광"),
	EIGHT_GODORI(Month.August, CardType.FiveBirdsTen, "8고도리"),
	EIGHT_SHELL_1(Month.August, CardType.NormalShell, "8피"),
	EIGHT_SHELL_2(Month.August, CardType.NormalShell, "8피"),	
	
	NINE_TEN(Month.September, CardType.NormalTen, "9열끗"),
	NINE_CHUNGDAN(Month.September, CardType.BlueFive, "9청단"),
	NINE_SHELL_1(Month.September, CardType.NormalShell, "9피"),
	NINE_SHELL_2(Month.September, CardType.NormalShell, "9피"),		
	
	TEN_TEN(Month.October, CardType.NormalTen, "10열끗"),
	TEN_CHUNGDAN(Month.October, CardType.BlueFive, "10청단"),
	TEN_SHELL_1(Month.October, CardType.NormalShell, "10피"),
	TEN_SHELL_2(Month.October, CardType.NormalShell, "10피"),		
	
	ELEVEN_LIGHT(Month.November, CardType.RainLight, "비광"),
	ELEVEN_TEN(Month.November, CardType.NormalTen, "비열끗"),
	ELEVEN_FIVE(Month.November, CardType.NormalFive, "비5끗"),
	ELEVEN_SSANGPI(Month.November, CardType.DoubleShell, "비쌍피"),	
	
	TWELVE_LIGHT(Month.December, CardType.TrueLight, "똥광"),
	TWELVE_SSANGPI(Month.December, CardType.DoubleShell, "똥쌍피"),
	TWELVE_SHELL_1(Month.December, CardType.NormalShell, "똥피"),
	TWELVE_SHELL_2(Month.December, CardType.NormalShell, "똥피"),
	
	EMPTY(Month.Null, CardType.EmptyCard, "빈카드");
	
	private final Month month;
	private final CardType cardType;
	private final String name;
	
	private Card(Month month, CardType cardType, String name) {
		this.month = month;
		this.cardType = cardType;
		this.name = name;
	}

	public String toString() {
		return this.name;
	}
	
	public CardType getCardType() {
		return this.cardType;
	}	
	
	public Month getMonth() {
		return month;
	}
	
	public boolean isSameMonthAs(Card card) {
		return this.month.equals(card.month);
	}
	
	// Return [floorCards, hiddenCards, first, second, third]
	public static InitialCardSet getRandomInitialCardSet() {
		ArrayList<Card> totalCards = new ArrayList<Card>();
		
		// Make card set
		totalCards.add(ONE_LIGHT);
		totalCards.add(ONE_HONGDAN);
		totalCards.add(ONE_SHELL_1);
		totalCards.add(ONE_SHELL_2);
		
		totalCards.add(TWO_GODORI);
		totalCards.add(TWO_HONGDAN);
		totalCards.add(TWO_SHELL_1);
		totalCards.add(TWO_SHELL_2);
		
		totalCards.add(THREE_LIGHT);
		totalCards.add(THREE_HONGDAN);
		totalCards.add(THREE_SHELL_1);
		totalCards.add(THREE_SHELL_2);
		
		totalCards.add(FOUR_CHODAN);
		totalCards.add(FOUR_GODORI);
		totalCards.add(FOUR_SHELL_1);
		totalCards.add(FOUR_SHELL_2);
		
		totalCards.add(FIVE_CHODAN);
		totalCards.add(FIVE_TEN);
		totalCards.add(FIVE_SHELL_1);
		totalCards.add(FIVE_SHELL_2);
		
		
		totalCards.add(SIX_CHUNGDAN);
		totalCards.add(SIX_TEN);
		totalCards.add(SIX_SHELL_1);
		totalCards.add(SIX_SHELL_2);
		
		totalCards.add(SEVEN_CHODAN);
		totalCards.add(SEVEN_TEN);
		totalCards.add(SEVEN_SHELL_1);
		totalCards.add(SEVEN_SHELL_2);
		
		totalCards.add(EIGHT_LIGHT);
		totalCards.add(EIGHT_GODORI);
		totalCards.add(EIGHT_SHELL_1);
		totalCards.add(EIGHT_SHELL_2);
		
		totalCards.add(NINE_TEN);
		totalCards.add(NINE_CHUNGDAN);
		totalCards.add(NINE_SHELL_1);
		totalCards.add(NINE_SHELL_2);
		
		totalCards.add(TEN_TEN);
		totalCards.add(TEN_CHUNGDAN);
		totalCards.add(TEN_SHELL_1);
		totalCards.add(TEN_SHELL_2);
		
		totalCards.add(ELEVEN_LIGHT);
		totalCards.add(ELEVEN_TEN);
		totalCards.add(ELEVEN_FIVE);
		totalCards.add(ELEVEN_SSANGPI);
		
		totalCards.add(TWELVE_LIGHT);
		totalCards.add(TWELVE_SSANGPI);
		totalCards.add(TWELVE_SHELL_1);
		totalCards.add(TWELVE_SHELL_2);
		
		assert(totalCards.size() == 48); // 4 * 12 = 48
		
		// Shuffle cards randomly: exchange n *10 times
		int exchangeCount = totalCards.size() * 10;
		Random randomGenerator = new Random();
		for (int i = 0; i < exchangeCount ; i++) {
			// Do exchange
			int firstIndex = randomGenerator.nextInt(totalCards.size());
			int secondIndex = randomGenerator.nextInt(totalCards.size());
			Card temp = totalCards.get(firstIndex);
			totalCards.set(firstIndex, totalCards.get(secondIndex));
			totalCards.set(secondIndex, temp);
		}		
		
		List<Card> floorCards = new ArrayList<Card>();
		List<Card> hiddenCards = new ArrayList<Card>();
		List<Card> firstPlayerHandCards = new ArrayList<Card>();
		List<Card> secondPlayerHandCards = new ArrayList<Card>();		
		List<Card> thirdPlayerHandCards = new ArrayList<Card>();
		
		for (int i = 0; i < 48; i++) {
			if (i < 6) { 
				floorCards.add(totalCards.get(i));
			} else if (i < 27) {
				hiddenCards.add(totalCards.get(i));
			} else if (i < 34) {
				firstPlayerHandCards.add(totalCards.get(i));
			} else if (i < 41) {
				secondPlayerHandCards.add(totalCards.get(i));
			} else { // last 7 cards
				thirdPlayerHandCards.add(totalCards.get(i));
			}
		}	
		
		return new InitialCardSet(floorCards, hiddenCards, firstPlayerHandCards,
				secondPlayerHandCards, thirdPlayerHandCards);
	}
	
}
