using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Gostop.Model {
	public enum Month {
		January = 1,
		February = 2,
		March = 3,
		April = 4,
		May = 5,
		June = 6,
		July = 7,
		August = 8,
		September = 9,
		October = 10,
		November = 11,
		December = 12
	}

	public enum CardType {
		TrueLight,
		RainLight,
		FiveBirdsTen,
		NormalTen,
		RedFive,
		GrassFive,
		BlueFive,
		NormalFive,
		DoubleShell,
		NormalShell,
	}

	public class Card {
		public const int NumberOfTotalCards = 48;
		public const int InitialHandCardsPerPlayer = 7;
		public const int InitialFloorCardsNum = 6;
		public const int InitialHiddenCardsNum = 21;

		public Card(Month month, CardType cardType, string name) {
			_month = month;
			_cardType = cardType;
			_name = name;
		}
		public override string ToString() {
			return _name;
		}


		public Month Month {
			get {
				return _month;
			}
		}

		public CardType CardType {
			get {
				return _cardType;
			}
			set {
				_cardType = value;
			}
		}

		public string Name {
			get {
				return _name;
			}
		}

		private Month _month;
		private CardType _cardType;
		private string _name = null;
		private static List<Card> _cardSet = null;


		public static void CreateCards() {
			if (_cardSet != null) // already created
			{
				return;
			}
			_cardSet = new List<Card>();
			_cardSet.Add(new Card(Month.January, CardType.TrueLight, "1광"));
			_cardSet.Add(new Card(Month.January, CardType.RedFive, "1홍단"));
			_cardSet.Add(new Card(Month.January, CardType.NormalShell, "1피"));
			_cardSet.Add(new Card(Month.January, CardType.NormalShell, "1피*"));

			_cardSet.Add(new Card(Month.February, CardType.FiveBirdsTen, "2고도리"));
			_cardSet.Add(new Card(Month.February, CardType.RedFive, "2홍단"));
			_cardSet.Add(new Card(Month.February, CardType.NormalShell, "2피"));
			_cardSet.Add(new Card(Month.February, CardType.NormalShell, "2피*"));

			_cardSet.Add(new Card(Month.March, CardType.TrueLight, "3광"));
			_cardSet.Add(new Card(Month.March, CardType.RedFive, "3홍단"));
			_cardSet.Add(new Card(Month.March, CardType.NormalShell, "3피"));
			_cardSet.Add(new Card(Month.March, CardType.NormalShell, "3피*"));

			_cardSet.Add(new Card(Month.April, CardType.FiveBirdsTen, "4고도리"));
			_cardSet.Add(new Card(Month.April, CardType.GrassFive, "4초단"));
			_cardSet.Add(new Card(Month.April, CardType.NormalShell, "4피"));
			_cardSet.Add(new Card(Month.April, CardType.NormalShell, "4피*"));

			_cardSet.Add(new Card(Month.May, CardType.NormalTen, "5열끗"));
			_cardSet.Add(new Card(Month.May, CardType.GrassFive, "5초단"));
			_cardSet.Add(new Card(Month.May, CardType.NormalShell, "5피"));
			_cardSet.Add(new Card(Month.May, CardType.NormalShell, "5피*"));

			_cardSet.Add(new Card(Month.June, CardType.NormalTen, "6열끗"));
			_cardSet.Add(new Card(Month.June, CardType.BlueFive, "6청단"));
			_cardSet.Add(new Card(Month.June, CardType.NormalShell, "6피"));
			_cardSet.Add(new Card(Month.June, CardType.NormalShell, "6피*"));

			_cardSet.Add(new Card(Month.July, CardType.NormalTen, "7열끗"));
			_cardSet.Add(new Card(Month.July, CardType.GrassFive, "7초단"));
			_cardSet.Add(new Card(Month.July, CardType.NormalShell, "7피"));
			_cardSet.Add(new Card(Month.July, CardType.NormalShell, "7피*"));

			_cardSet.Add(new Card(Month.August, CardType.TrueLight, "8광"));
			_cardSet.Add(new Card(Month.August, CardType.FiveBirdsTen, "8고도리"));
			_cardSet.Add(new Card(Month.August, CardType.NormalShell, "8피"));
			_cardSet.Add(new Card(Month.August, CardType.NormalShell, "8피*"));

			_cardSet.Add(new Card(Month.September, CardType.NormalTen, "9열끗"));
			_cardSet.Add(new Card(Month.September, CardType.BlueFive, "9청단"));
			_cardSet.Add(new Card(Month.September, CardType.NormalShell, "9피"));
			_cardSet.Add(new Card(Month.September, CardType.NormalShell, "9피*"));

			_cardSet.Add(new Card(Month.October, CardType.NormalTen, "10열끗"));
			_cardSet.Add(new Card(Month.October, CardType.BlueFive, "10청단"));
			_cardSet.Add(new Card(Month.October, CardType.NormalShell, "10피"));
			_cardSet.Add(new Card(Month.October, CardType.NormalShell, "10피*"));

			_cardSet.Add(new Card(Month.November, CardType.RainLight, "비광"));
			_cardSet.Add(new Card(Month.November, CardType.NormalTen, "비열끗"));
			_cardSet.Add(new Card(Month.November, CardType.NormalFive, "비다섯끗"));
			_cardSet.Add(new Card(Month.November, CardType.DoubleShell, "비쌍피"));

			_cardSet.Add(new Card(Month.December, CardType.TrueLight, "똥광"));
			_cardSet.Add(new Card(Month.December, CardType.NormalShell, "똥피"));
			_cardSet.Add(new Card(Month.December, CardType.NormalShell, "똥피*"));
			_cardSet.Add(new Card(Month.December, CardType.DoubleShell, "똥쌍피"));
		}

		public static Card GetCard(int cardId) {
			if (_cardSet == null) {
				CreateCards();
			}
			if (!(cardId >= 0 && cardId < 48)) {
				Console.WriteLine("GetCard called with invalid cardId: " + cardId);
				return null;
			}
			return _cardSet[cardId];
		}

		public static int GetCardId(string name) {
			for (int i = 0; i < NumberOfTotalCards; i++) {
				if (GetCard(i).Name.Equals(name)) {
					return i;
				}
			}
			Console.WriteLine("GetCard(name) error");
			return -1; // This is not a proper card name!
		}

		// 2010-09-26
		public static HashSet<int> GetCardSet(string names) {
			if (names == "") {
				return new HashSet<int>();
			}
			HashSet<int> cardSet = new HashSet<int>();
			String[] separators = new String[1];
			separators[0] = " ";
			String[] nameArray = names.Split(separators,
				Card.InitialHandCardsPerPlayer, StringSplitOptions.None);
			for (int i = 0; i < nameArray.Count(); i++) {
				cardSet.Add(GetCardId(nameArray[i]));
			}
			return cardSet;
		}

		public static string PrintCard(int cardId) {
			return GetCard(cardId).ToString();
		}

		public static string PrintMonth(Month month) {
			switch (month) {
				case Month.January:
					return "1월";
				case Month.February:
					return "2월";
				case Month.March:
					return "3월";
				case Month.April:
					return "4월";
				case Month.May:
					return "5월";
				case Month.June:
					return "6월";
				case Month.July:
					return "7월";
				case Month.August:
					return "8월";
				case Month.September:
					return "9월";
				case Month.October:
					return "10월";
				case Month.November:
					return "11월";
				case Month.December:
					return "12월";
			}
			return null;
		}

		public static string PrintCardSet(HashSet<int> cards) {
			return PrintSortedCards(SortCards(cards));

		}

		// 2010-09-26
		public static string PrintSortedCards(
			Dictionary<Month, HashSet<int>> sortedCards) {
			StringBuilder builder = new StringBuilder();
			Dictionary<Month, HashSet<int>>.Enumerator monthEnum =
				sortedCards.GetEnumerator();
			while (monthEnum.MoveNext()) {
				HashSet<int>.Enumerator cardEnum = monthEnum.Current.Value.GetEnumerator();
				while (cardEnum.MoveNext()) {
					builder.Append(GetCard(cardEnum.Current) + " ");
				}
			}
			return builder.ToString();
		}


		public static bool IsSameMonth(int firstCard, int secondCard) {
			Console.WriteLine("issameMonth");
			return GetCard(firstCard).Month == GetCard(secondCard).Month;
		}

		// 2010-09-26
		public static HashSet<int> CardsWithSameMonth(HashSet<int> cards, Month month) {
			HashSet<int> cardsWithSameMonth = new HashSet<int>();
			HashSet<int>.Enumerator enumerator = cards.GetEnumerator();
			while (enumerator.MoveNext()) {
				if (GetCard(enumerator.Current).Month == month) {
					cardsWithSameMonth.Add(enumerator.Current);
				}
			}
			return cardsWithSameMonth;
		}

		public static HashSet<int> GetLightsFrom(HashSet<int> cards) {
			var trueLights = GetCardsOfType(cards, CardType.TrueLight);
			var rainLights = GetCardsOfType(cards, CardType.RainLight);
			trueLights.UnionWith(rainLights);
			return trueLights;
		}

		public static HashSet<int> GetShellsFrom(HashSet<int> cards) {
			var shells = GetCardsOfType(cards, CardType.NormalShell);
			var doubleShells = GetCardsOfType(cards, CardType.DoubleShell);
			shells.UnionWith(doubleShells);
			return shells;
		}

		public void ConvertSeptemberTenToDoubleShell() {
			GetCard(GetCardId("9열끗")).CardType = CardType.DoubleShell;
		}

		public static HashSet<int> GetCardsOfType(HashSet<int> cards, CardType type) {
			HashSet<int> result = new HashSet<int>();
			var iter = cards.GetEnumerator();
			while (iter.MoveNext()) {
				var thisType = Card.GetCard(iter.Current).CardType;
				if (thisType == type) {
					result.Add(iter.Current);
				}
			}
			return result;
		}

		// 2010-09-26
		public static Dictionary<Month, HashSet<int>> SortCards(HashSet<int> cards) {
			Dictionary<Month, HashSet<int>> sortedCards =
				new Dictionary<Month, HashSet<int>>();
			// Fill each month with empty HashSet
			for (int card = 0; card < Card.NumberOfTotalCards; card++) {
				Month month = GetCard(card).Month;
				if (sortedCards.ContainsKey(month) == false) {
					sortedCards.Add(month, new HashSet<int>());
				}
			}
			HashSet<int>.Enumerator enumerator = cards.GetEnumerator();
			while (enumerator.MoveNext()) {
				sortedCards[GetCard(enumerator.Current).Month].Add(enumerator.Current);
			}
			return sortedCards;
		}
	}
}


