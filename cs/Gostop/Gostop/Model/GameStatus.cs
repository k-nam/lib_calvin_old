using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Gostop.Model
{
	public class GameStatus
	{
		#region // Members
		private Player _currentTurn;

		private HashSet<int> _aHandCards = null;
		private HashSet<int> _bHandCards = null;
		private HashSet<int> _cHandCards = null;

		private HashSet<int> _aTakenCards = null;
		private HashSet<int> _bTakenCards = null;
		private HashSet<int> _cTakenCards = null;

		private HashSet<int> _floorCards = null;
		private HashSet<int> _hiddenCards = null;		

		private int _aGoCount = 0;
		private int _bGoCount = 0;
		private int _cGoCount = 0;

		private int _aPoint = 0;
		private int _bPoint = 0;
		private int _cPoint = 0;

		private int _aLastGoPoint = 0;
		private int _bLastGoPoint = 0;
		private int _cLastGoPoint = 0;

		private int _aShakeCount = 0;
		private int _bShakeCount = 0;
		private int _cShakeCount = 0;			
		
		private int _aFuckCount = 0;
		private int _bFuckCount = 0;
		private int _cFuckCount = 0;
		#endregion

		#region // Getters
		public Player CurrentPlayer
		{
			get
			{
				return _currentTurn;
			}
		}
		public HashSet<int> AHandCards
		{
			get
			{
				return _aHandCards;
			}
		}
		public HashSet<int> BHandCards
		{
			get
			{
				return _bHandCards;
			}
		}
		public HashSet<int> CHandCards
		{
			get
			{
				return _cHandCards;
			}
		}

		public HashSet<int> ATakenCards
		{
			get
			{
				return _aHandCards;
			}
		}
		public HashSet<int> BTakenCards
		{
			get
			{
				return _bHandCards;
			}
		}
		public HashSet<int> CTakenCards
		{
			get
			{
				return _cHandCards;
			}
		}

		public HashSet<int> HiddenCards
		{
			get
			{
				return _hiddenCards;
			}
		}
		public HashSet<int> FloorCards
		{
			get
			{
				return _floorCards;
			}
		}

		public int AGoCount
		{
			get
			{
				return _aGoCount;
			}
		}
		public int BGoCount
		{
			get
			{
				return _bGoCount;
			}
		}
		public int CGoCount
		{
			get
			{
				return _bGoCount;
			}
		}

		public int APoint
		{
			get
			{
				return _aPoint;
			}
		}
		public int BPoint
		{
			get
			{
				return _bPoint;
			}
		}
		public int CPoint
		{
			get
			{
				return _bPoint;
			}
		}

		public int ALastGoPoint
		{
			get
			{
				return _aLastGoPoint;
			}
		}
		public int BLastGoPoint
		{
			get
			{
				return _bLastGoPoint;
			}
		}
		public int CLastGoPoint
		{
			get
			{
				return _bLastGoPoint;
			}
		}

		public int AShakeCount
		{
			get
			{
				return _aShakeCount;
			}
		}
		public int BShakeCount
		{
			get
			{
				return _bShakeCount;
			}
		}
		public int CShakeCount
		{
			get
			{
				return _bShakeCount;
			}
		}

		public int AFuckCount
		{
			get
			{
				return _aFuckCount;
			}
		}
		public int BFuckCount
		{
			get
			{
				return _bFuckCount;
			}
		}
		public int CFuckCount
		{
			get
			{
				return _bFuckCount;
			}
		}
		#endregion


		// Create a random initial status
		public GameStatus()
		{
			_currentTurn = Player.A;
			_aHandCards = new HashSet<int>();
			_bHandCards = new HashSet<int>();
			_cHandCards = new HashSet<int>();
			_aTakenCards = new HashSet<int>();
			_bTakenCards = new HashSet<int>();
			_cTakenCards = new HashSet<int>();
			_floorCards = new HashSet<int>();
			_hiddenCards = new HashSet<int>();
			ShuffleCards(_aHandCards, _bHandCards, _cHandCards,
				_floorCards, _hiddenCards);
		}

		public GameStatus(HashSet<int> playerA_HandCards, HashSet<int> playerB_HandCards,
			HashSet<int> playerC_HandCards, HashSet<int> floorCards)
		{
			_currentTurn = Player.A;
			_aHandCards = playerA_HandCards;
			_bHandCards = playerB_HandCards;
			_cHandCards = playerC_HandCards;
			_aTakenCards = new HashSet<int>();
			_bTakenCards = new HashSet<int>();
			_cTakenCards = new HashSet<int>();
			_floorCards = floorCards;
			_hiddenCards = new HashSet<int>();
			// Automatically calculate hidden cards;
			for (int i = 0; i < Card.NumberOfTotalCards; i++)
			{
				_hiddenCards.Add(i);
			}
			HashSet<int>.Enumerator enumerator = playerA_HandCards.GetEnumerator();
			while (enumerator.MoveNext())
			{
				_hiddenCards.Remove(enumerator.Current);
			}
			enumerator = playerB_HandCards.GetEnumerator();
			{
				_hiddenCards.Remove(enumerator.Current);
			}
			enumerator = playerC_HandCards.GetEnumerator();
			{
				_hiddenCards.Remove(enumerator.Current);
			}
			enumerator = floorCards.GetEnumerator();
			{
				_hiddenCards.Remove(enumerator.Current);
			}
		}

		public bool isInitialStatus()
		{
			bool isInitial = true;
			if (_aHandCards.Count != Card.InitialHandCardsPerPlayer)
			{
				isInitial = false;
			}
			if (_bHandCards.Count != Card.InitialHandCardsPerPlayer)
			{
				isInitial = false;
			}
			if (_cHandCards.Count != Card.InitialHandCardsPerPlayer)
			{
				isInitial = false;
			}
			if (_floorCards.Count != Card.InitialFloorCardsNum)
			{
				isInitial = false;
			}
			if (_hiddenCards.Count != 0)
			{
				isInitial = false;
			}
			return isInitial;
		}

		public override string ToString()
		{
			StringBuilder builder = new StringBuilder();

			builder.AppendLine("Current turn: " + _currentTurn); 
			builder.AppendLine();

			builder.AppendLine("<Hand cards>");
			builder.Append("Player A: ");
			builder.AppendLine(Card.PrintCardSet(_aHandCards));
			builder.Append("Player B: ");
			builder.AppendLine(Card.PrintCardSet(_bHandCards));
			builder.Append("Player C: ");
			builder.AppendLine(Card.PrintCardSet(_cHandCards));
			builder.AppendLine();

			builder.AppendLine("<Taken cards>");
			builder.Append("Player A: ");
			builder.AppendLine(Card.PrintCardSet(_aTakenCards));
			builder.Append("Player B: ");
			builder.AppendLine(Card.PrintCardSet(_bTakenCards));
			builder.Append("Player C: ");
			builder.AppendLine(Card.PrintCardSet(_cTakenCards));
			builder.AppendLine();

			builder.AppendLine("<Floor cards>");
			builder.AppendLine(Card.PrintCardSet(_floorCards));
			builder.AppendLine();

			builder.AppendLine("<Hidden cards>");
			builder.AppendLine(Card.PrintCardSet(_hiddenCards));
			builder.AppendLine();

			builder.AppendLine("<GoCount>");
			builder.AppendLine("Player A: " + _aGoCount);
			builder.AppendLine("Player B: " + _bGoCount);
			builder.AppendLine("Player C: " + _cGoCount);
			builder.AppendLine();

			builder.AppendLine("<Point>");
			builder.AppendLine("Player A: " + _aPoint);
			builder.AppendLine("Player B: " + _bPoint);
			builder.AppendLine("Player C: " + _cPoint);
			builder.AppendLine();

			builder.AppendLine("<ShakeCount>");
			builder.AppendLine("Player A: " + _aShakeCount);
			builder.AppendLine("Player B: " + _bShakeCount);
			builder.AppendLine("Player C: " + _cShakeCount);
			builder.AppendLine();

			builder.AppendLine("<FuckCount>");
			builder.AppendLine("Player A: " + _aFuckCount);
			builder.AppendLine("Player B: " + _bFuckCount);
			builder.AppendLine("Player C: " + _cFuckCount);
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
