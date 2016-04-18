using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Gostop.Model
{
	public class GameStatus
	{
		#region // Members	
		/*
		private int AGoCount = 0;
		private int BGoCount = 0;
		private int CGoCount = 0;

		private int APoint = 0;
		private int BPoint = 0;
		private int CPoint = 0;

		private int ALastGoPoint = 0;
		private int BLastGoPoint = 0;
		private int CLastGoPoint = 0;

		private int AShakeCount = 0;
		private int BShakeCount = 0;
		private int CShakeCount = 0;			
		
		private int AFuckCount = 0;
		private int BFuckCount = 0;
		private int CFuckCount = 0;
		*/
		#endregion

		#region // Getters
		public Player CurrentPlayer
		{
			get;
			set;
		}
		public HashSet<int> FloorCards
		{
			get;
			set;
		}
		public HashSet<int> HiddenCards
		{
			get;
			set;
		}
		public HashSet<int> AHandCards
		{
			get;
			set;
		}
		public HashSet<int> BHandCards
		{
			get;
			set;
		}
		public HashSet<int> CHandCards
		{
			get;
			set;
		}

		public HashSet<int> ATakenCards
		{
			get;
			set;
		}
		public HashSet<int> BTakenCards
		{
			get;
			set;
		}
		public HashSet<int> CTakenCards
		{
			get;
			set;
		}

		public int AGoCount
		{
			get;
			set;
		}
		public int BGoCount
		{
			get;
			set;
		}
		public int CGoCount
		{
			get;
			set;
		}

		public int APoint
		{
			get;
			set;
		}
		public int BPoint
		{
			get;
			set;
		}
		public int CPoint
		{
			get;
			set;
		}

		public int ALastGoPoint
		{
			get;
			set;
		}
		public int BLastGoPoint
		{
			get;
			set;
		}
		public int CLastGoPoint
		{
			get;
			set;
		}

		public int AShakeCount
		{
			get;
			set;
		}
		public int BShakeCount
		{
			get;
			set;
		}
		public int CShakeCount
		{
			get;
			set;
		}

		public int AFuckCount
		{
			get;
			set;
		}
		public int BFuckCount
		{
			get;
			set;
		}
		public int CFuckCount
		{
			get;
			set;
		}
		#endregion


		// Create a random initial status
		public GameStatus()
		{
			CurrentPlayer = Player.A;
			FloorCards = new HashSet<int>();
			HiddenCards = new HashSet<int>();
			AHandCards = new HashSet<int>();
			BHandCards = new HashSet<int>();
			CHandCards = new HashSet<int>();
			ATakenCards = new HashSet<int>();
			BTakenCards = new HashSet<int>();
			CTakenCards = new HashSet<int>();

			ShuffleCards(AHandCards, BHandCards, CHandCards,
				FloorCards, HiddenCards);
		}

		public GameStatus(HashSet<int> playerA_HandCards, HashSet<int> playerB_HandCards,
			HashSet<int> playerC_HandCards, HashSet<int> floorCards)
		{
			CurrentPlayer = Player.A;
			AHandCards = playerA_HandCards;
			BHandCards = playerB_HandCards;
			CHandCards = playerC_HandCards;
			ATakenCards = new HashSet<int>();
			BTakenCards = new HashSet<int>();
			CTakenCards = new HashSet<int>();
			FloorCards = floorCards;
			HiddenCards = new HashSet<int>();
			// Automatically calculate hidden cards;
			for (int i = 0; i < Card.NumberOfTotalCards; i++)
			{
				HiddenCards.Add(i);
			}
			HashSet<int>.Enumerator enumerator = playerA_HandCards.GetEnumerator();
			while (enumerator.MoveNext())
			{
				HiddenCards.Remove(enumerator.Current);
			}
			enumerator = playerB_HandCards.GetEnumerator();
			{
				HiddenCards.Remove(enumerator.Current);
			}
			enumerator = playerC_HandCards.GetEnumerator();
			{
				HiddenCards.Remove(enumerator.Current);
			}
			enumerator = floorCards.GetEnumerator();
			{
				HiddenCards.Remove(enumerator.Current);
			}
		}

		public bool isInitialStatus()
		{
			bool isInitial = true;
			if (AHandCards.Count != Card.InitialHandCardsPerPlayer)
			{
				isInitial = false;
			}
			if (BHandCards.Count != Card.InitialHandCardsPerPlayer)
			{
				isInitial = false;
			}
			if (CHandCards.Count != Card.InitialHandCardsPerPlayer)
			{
				isInitial = false;
			}
			if (FloorCards.Count != Card.InitialFloorCardsNum)
			{
				isInitial = false;
			}
			if (HiddenCards.Count != 0)
			{
				isInitial = false;
			}
			return isInitial;
		}

		public override string ToString()
		{
			StringBuilder builder = new StringBuilder();

			builder.AppendLine("Current turn: " + CurrentPlayer); 
			builder.AppendLine();

			builder.AppendLine("<Hand cards>");
			builder.Append("Player A: ");
			builder.AppendLine(Card.PrintCardSet(AHandCards));
			builder.Append("Player B: ");
			builder.AppendLine(Card.PrintCardSet(BHandCards));
			builder.Append("Player C: ");
			builder.AppendLine(Card.PrintCardSet(CHandCards));
			builder.AppendLine();

			builder.AppendLine("<Taken cards>");
			builder.Append("Player A: ");
			builder.AppendLine(Card.PrintCardSet(ATakenCards));
			builder.Append("Player B: ");
			builder.AppendLine(Card.PrintCardSet(BTakenCards));
			builder.Append("Player C: ");
			builder.AppendLine(Card.PrintCardSet(CTakenCards));
			builder.AppendLine();

			builder.AppendLine("<Floor cards>");
			builder.AppendLine(Card.PrintCardSet(FloorCards));
			builder.AppendLine();

			builder.AppendLine("<Hidden cards>");
			builder.AppendLine(Card.PrintCardSet(HiddenCards));
			builder.AppendLine();

			builder.AppendLine("<GoCount>");
			builder.AppendLine("Player A: " + AGoCount);
			builder.AppendLine("Player B: " + BGoCount);
			builder.AppendLine("Player C: " + CGoCount);
			builder.AppendLine();

			builder.AppendLine("<Point>");
			builder.AppendLine("Player A: " + APoint);
			builder.AppendLine("Player B: " + BPoint);
			builder.AppendLine("Player C: " + CPoint);
			builder.AppendLine();

			builder.AppendLine("<ShakeCount>");
			builder.AppendLine("Player A: " + AShakeCount);
			builder.AppendLine("Player B: " + BShakeCount);
			builder.AppendLine("Player C: " + CShakeCount);
			builder.AppendLine();

			builder.AppendLine("<FuckCount>");
			builder.AppendLine("Player A: " + AFuckCount);
			builder.AppendLine("Player B: " + BFuckCount);
			builder.AppendLine("Player C: " + CFuckCount);
			builder.AppendLine();
			return builder.ToString();
		}		

		// Among integers 0 ~ 47, randomly assigns 7, 7, 7, 6 cards to each set
		public static void ShuffleCards(HashSet<int> aHandCards,
			HashSet<int> bHandCards, HashSet<int> cHandCards,
			HashSet<int> floorCards, HashSet<int> hiddenCards)
		{
			List<int> cardSet = new List<int>();
			for (int i = 0; i < Card.NumberOfTotalCards; i++)
			{
				cardSet.Add(i);
			}
			Random random = new Random();
			for (int i = 0; i < 100; i++)
			{
				int first = random.Next(Card.NumberOfTotalCards);
				int second = random.Next(Card.NumberOfTotalCards);
				int temp = cardSet[first];
				cardSet[first] = cardSet[second];
				cardSet[second] = temp;
				//Console.WriteLine("Changing: " + first + " with " + second);
			}
			for (int i = 0; i < Card.NumberOfTotalCards; i++)
			{
				if (i < Card.InitialHandCardsPerPlayer)	
				{
					aHandCards.Add(cardSet[i]);
				}
				else if (i < Card.InitialHandCardsPerPlayer*2) 
				{
					bHandCards.Add(cardSet[i]);
				}
				else if (i < Card.InitialHandCardsPerPlayer*3) 
				{
					cHandCards.Add(cardSet[i]);
				}
				else if (i < Card.InitialHandCardsPerPlayer * 3 + Card.InitialFloorCardsNum)
				{
					floorCards.Add(cardSet[i]);
				}
				else
				{
					hiddenCards.Add(cardSet[i]);
				}
			}			
		}


	}
}
