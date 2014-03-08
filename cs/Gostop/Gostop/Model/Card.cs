using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Gostop.Model
{
	public enum Months
	{
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

	public enum CardTypes
	{
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

	public class Card
	{
		public const int NumberOfTotalCards = 48;
		public const int InitialHandCardsPerPlayer = 7;
		public const int InitialFloorCardsNum = 6;		

		public Card(Months month, CardTypes cardType, string name)
		{
			_month = month;
			_cardType = cardType;
			_name = name;
		}
		public override string ToString()
		{
			return _name;
		}

		
		public Months Month
		{
			get
			{
				return _month;
			}
		}

		public CardTypes CardType
		{
			get
			{
				return _cardType;
			}
		}

		public string Name
		{
			get
			{
				return _name;
			}
		}

		private Months _month;
		private CardTypes _cardType;
		private string _name = null;
		private static List<Card> _cardSet = null;


		public static void CreateCards()
		{
			if (_cardSet != null) // already created
			{
				return;
			}
			_cardSet = new List<Card>();
			_cardSet.Add(new Card(Months.January, CardTypes.TrueLight, "1광"));
			_cardSet.Add(new Card(Months.January, CardTypes.RedFive, "1홍단"));
			_cardSet.Add(new Card(Months.January, CardTypes.NormalShell, "1피"));
			_cardSet.Add(new Card(Months.January, CardTypes.NormalShell, "1피_"));

			_cardSet.Add(new Card(Months.February, CardTypes.FiveBirdsTen, "2고도리"));
			_cardSet.Add(new Card(Months.February, CardTypes.RedFive, "2홍단"));
			_cardSet.Add(new Card(Months.February, CardTypes.NormalShell, "2피"));
			_cardSet.Add(new Card(Months.February, CardTypes.NormalShell, "2피_"));

			_cardSet.Add(new Card(Months.March, CardTypes.TrueLight, "3광"));
			_cardSet.Add(new Card(Months.March, CardTypes.RedFive, "3홍단"));
			_cardSet.Add(new Card(Months.March, CardTypes.NormalShell, "3피"));
			_cardSet.Add(new Card(Months.March, CardTypes.NormalShell, "3피_"));

			_cardSet.Add(new Card(Months.April, CardTypes.FiveBirdsTen, "4고도리"));
			_cardSet.Add(new Card(Months.April, CardTypes.GrassFive, "4초단"));
			_cardSet.Add(new Card(Months.April, CardTypes.NormalShell, "4피"));
			_cardSet.Add(new Card(Months.April, CardTypes.NormalShell, "4피_"));

			_cardSet.Add(new Card(Months.May, CardTypes.NormalTen, "5열끗"));
			_cardSet.Add(new Card(Months.May, CardTypes.GrassFive, "5초단"));
			_cardSet.Add(new Card(Months.May, CardTypes.NormalShell, "5피"));
			_cardSet.Add(new Card(Months.May, CardTypes.NormalShell, "5피_"));

			_cardSet.Add(new Card(Months.June, CardTypes.NormalTen, "6열끗"));
			_cardSet.Add(new Card(Months.June, CardTypes.BlueFive, "6청단"));
			_cardSet.Add(new Card(Months.June, CardTypes.NormalShell, "6피"));
			_cardSet.Add(new Card(Months.June, CardTypes.NormalShell, "6피_"));

			_cardSet.Add(new Card(Months.July, CardTypes.NormalTen, "7열끗"));
			_cardSet.Add(new Card(Months.July, CardTypes.GrassFive, "7초단"));
			_cardSet.Add(new Card(Months.July, CardTypes.NormalShell, "7피"));
			_cardSet.Add(new Card(Months.July, CardTypes.NormalShell, "7피_"));

			_cardSet.Add(new Card(Months.August, CardTypes.TrueLight, "8광"));
			_cardSet.Add(new Card(Months.August, CardTypes.FiveBirdsTen, "8고도리"));
			_cardSet.Add(new Card(Months.August, CardTypes.NormalShell, "8피"));
			_cardSet.Add(new Card(Months.August, CardTypes.NormalShell, "8피_"));

			_cardSet.Add(new Card(Months.September, CardTypes.NormalTen, "9열끗"));
			_cardSet.Add(new Card(Months.September, CardTypes.BlueFive, "9청단"));
			_cardSet.Add(new Card(Months.September, CardTypes.NormalShell, "9피"));
			_cardSet.Add(new Card(Months.September, CardTypes.NormalShell, "9피_"));

			_cardSet.Add(new Card(Months.October, CardTypes.NormalTen, "10열끗"));
			_cardSet.Add(new Card(Months.October, CardTypes.BlueFive, "10청단"));
			_cardSet.Add(new Card(Months.October, CardTypes.NormalShell, "10피"));
			_cardSet.Add(new Card(Months.October, CardTypes.NormalShell, "10피_"));

			_cardSet.Add(new Card(Months.November, CardTypes.RainLight, "비광"));
			_cardSet.Add(new Card(Months.November, CardTypes.NormalTen, "비열끗"));
			_cardSet.Add(new Card(Months.November, CardTypes.NormalFive, "비다섯끗"));
			_cardSet.Add(new Card(Months.November, CardTypes.DoubleShell, "비쌍피"));

			_cardSet.Add(new Card(Months.December, CardTypes.TrueLight, "똥광"));
			_cardSet.Add(new Card(Months.December, CardTypes.NormalShell, "똥피"));
			_cardSet.Add(new Card(Months.December, CardTypes.NormalShell, "똥피_"));
			_cardSet.Add(new Card(Months.December, CardTypes.DoubleShell, "똥쌍피"));
		}

		public static Card GetCard(int cardId)
		{
			if (_cardSet == null)
			{
				CreateCards();
			}
			if (!(cardId >= 0 && cardId < 48))
			{
				Console.WriteLine("GetCard called with invalid cardId: " + cardId);
				return null;
			}
			return _cardSet[cardId];
		}

		public static int GetCardId(string name)
		{
			for (int i = 0; i < NumberOfTotalCards; i++)
			{
				if (GetCard(i).Name.Equals(name))
				{
					return i;
				}
			}
			Console.WriteLine("GetCard(name) error");
			return -1; // This is not a proper card name!
		}

		// 2010-09-26
		public static HashSet<int> GetCardSet(string names)
		{
			if (names == "")
			{
				return new HashSet<int>();
			}
			HashSet<int> cardSet = new HashSet<int>();
			String[] separators = new String[1];
			separators[0] = " ";
			String[] nameArray = names.Split(separators, 
				Card.InitialHandCardsPerPlayer, StringSplitOptions.None);
			for (int i = 0; i < nameArray.Count(); i++)
			{
				cardSet.Add(GetCardId(nameArray[i]));
			}
			return cardSet;
		}

		public static string PrintCard(int cardId)
		{
			return GetCard(cardId).ToString();
		}

		public static string PrintMonth(Months month)
		{
			switch (month)
			{
				case Months.January:
					return "1월";
				case Months.February:
					return "2월";
				case Months.March:
					return "3월";
				case Months.April:
					return "4월";
				case Months.May:
					return "5월";
				case Months.June:
					return "6월";
				case Months.July:
					return "7월";
				case Months.August:
					return "8월";
				case Months.September:
					return "9월";
				case Months.October:
					return "10월";
				case Months.November:
					return "11월";
				case Months.December:
					return "12월";
			}
			return null;
		}

		public static string PrintCardSet(HashSet<int> cards)
		{
			StringBuilder builder = new StringBuilder();
			builder.Append(" ");
			for (HashSet<int>.Enumerator i = cards.GetEnumerator(); i.MoveNext(); )
			{
				builder.Append(Card.PrintCard(i.Current));
				builder.Append(" ");
			}
			builder.Append(" ");
			return builder.ToString();
		}

		// 2010-09-26
		public static string PrintSortedCards(
			Dictionary<Months, HashSet<int>> sortedCards)
		{
			StringBuilder builder = new StringBuilder();
			Dictionary<Months, HashSet<int>>.Enumerator monthEnum =
				sortedCards.GetEnumerator();
			while (monthEnum.MoveNext())
			{
				builder.Append(PrintMonth(monthEnum.Current.Key) + ": ");
				HashSet<int>.Enumerator cardEnum = monthEnum.Current.Value.GetEnumerator();
				while (cardEnum.MoveNext())
				{
					builder.Append(GetCard(cardEnum.Current) + " ");
				}
				builder.AppendLine();
			}
			return builder.ToString();
		}


		public static bool IsSameMonth(int firstCard, int secondCard)
		{
			Console.WriteLine("issameMonth");
			return GetCard(firstCard).Month == GetCard(secondCard).Month;
		}

		// 2010-09-26
		public static HashSet<int> CardsWithSameMonth(HashSet<int> cards, Months month)
		{
			HashSet<int> cardsWithSameMonth = new HashSet<int>();
			HashSet<int>.Enumerator enumerator = cards.GetEnumerator();
			while (enumerator.MoveNext())
			{
				if (GetCard(enumerator.Current).Month == month)
				{
					cardsWithSameMonth.Add(enumerator.Current);
				}
			}
			return cardsWithSameMonth;
		}

		// 2010-09-26
		public static Dictionary<Months, HashSet<int>> SortCards(HashSet<int> cards)
		{
			Dictionary<Months, HashSet<int>> sortedCards = 
				new Dictionary<Months, HashSet<int>>();
			// Fill each month with empty HashSet
			for (int card = 0; card < Card.NumberOfTotalCards; card++)
			{
				Months month = GetCard(card).Month;
				if (sortedCards.ContainsKey(month) == false)
				{
					sortedCards.Add(month, new HashSet<int>());
				}
			}
			HashSet<int>.Enumerator enumerator = cards.GetEnumerator();
			while (enumerator.MoveNext())
			{
				sortedCards[GetCard(enumerator.Current).Month].Add(enumerator.Current);
			}			
			return sortedCards;
		}
	}
}


