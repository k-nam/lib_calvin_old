using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Gostop.Model
{
	public class GameStatus
	{
		#region // Members
		private Players _currentTurn;

		private HashSet<int> _playerA_HandCards = null;
		private HashSet<int> _playerB_HandCards = null;
		private HashSet<int> _playerC_HandCards = null;

		private HashSet<int> _playerA_TakenCards = null;
		private HashSet<int> _playerB_TakenCards = null;
		private HashSet<int> _playerC_TakenCards = null;

		private HashSet<int> _floorCards = null;
		private HashSet<int> _hiddenCards = null;		

		private int _playerA_GoCount = 0;
		private int _playerB_GoCount = 0;
		private int _playerC_GoCount = 0;

		private int _playerA_Point = 0;
		private int _playerB_Point = 0;
		private int _playerC_Point = 0;

		private int _playerA_LastGoPoint = 0;
		private int _playerB_LastGoPoint = 0;
		private int _playerC_LastGoPoint = 0;

		private int _playerA_ShakeCount = 0;
		private int _playerB_ShakeCount = 0;
		private int _playerC_ShakeCount = 0;			
		
		private int _playerA_FuckCount = 0;
		private int _playerB_FuckCount = 0;
		private int _playerC_FuckCount = 0;
		#endregion

		#region // Getters
		public Players CurrentTurn
		{
			get
			{
				return _currentTurn;
			}
		}
		public HashSet<int> PlayerA_HandCards
		{
			get
			{
				return _playerA_HandCards;
			}
		}
		public HashSet<int> PlayerB_HandCards
		{
			get
			{
				return _playerB_HandCards;
			}
		}
		public HashSet<int> PlayerC_HandCards
		{
			get
			{
				return _playerC_HandCards;
			}
		}

		public HashSet<int> PlayerA_TakenCards
		{
			get
			{
				return _playerA_HandCards;
			}
		}
		public HashSet<int> PlayerB_TakenCards
		{
			get
			{
				return _playerB_HandCards;
			}
		}
		public HashSet<int> PlayerC_TakenCards
		{
			get
			{
				return _playerC_HandCards;
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

		public int PlayerA_GoCount
		{
			get
			{
				return _playerA_GoCount;
			}
		}
		public int PlayerB_GoCount
		{
			get
			{
				return _playerB_GoCount;
			}
		}
		public int PlayerC_GoCount
		{
			get
			{
				return _playerB_GoCount;
			}
		}

		public int PlayerA_Point
		{
			get
			{
				return _playerA_Point;
			}
		}
		public int PlayerB_Point
		{
			get
			{
				return _playerB_Point;
			}
		}
		public int PlayerC_Point
		{
			get
			{
				return _playerB_Point;
			}
		}

		public int PlayerA_LastGoPoint
		{
			get
			{
				return _playerA_LastGoPoint;
			}
		}
		public int PlayerB_LastGoPoint
		{
			get
			{
				return _playerB_LastGoPoint;
			}
		}
		public int PlayerC_LastGoPoint
		{
			get
			{
				return _playerB_LastGoPoint;
			}
		}

		public int PlayerA_ShakeCount
		{
			get
			{
				return _playerA_ShakeCount;
			}
		}
		public int PlayerB_ShakeCount
		{
			get
			{
				return _playerB_ShakeCount;
			}
		}
		public int PlayerC_ShakeCount
		{
			get
			{
				return _playerB_ShakeCount;
			}
		}

		public int PlayerA_FuckCount
		{
			get
			{
				return _playerA_FuckCount;
			}
		}
		public int PlayerB_FuckCount
		{
			get
			{
				return _playerB_FuckCount;
			}
		}
		public int PlayerC_FuckCount
		{
			get
			{
				return _playerB_FuckCount;
			}
		}
		#endregion


		// Create a random initial status
		public GameStatus()
		{
			_currentTurn = Players.PlayerA;
			_playerA_HandCards = new HashSet<int>();
			_playerB_HandCards = new HashSet<int>();
			_playerC_HandCards = new HashSet<int>();
			_playerA_TakenCards = new HashSet<int>();
			_playerB_TakenCards = new HashSet<int>();
			_playerC_TakenCards = new HashSet<int>();
			_floorCards = new HashSet<int>();
			_hiddenCards = new HashSet<int>();
			ShuffleCards(_playerA_HandCards, _playerB_HandCards, _playerC_HandCards,
				_floorCards, _hiddenCards);
		}

		public GameStatus(HashSet<int> playerA_HandCards, HashSet<int> playerB_HandCards,
			HashSet<int> playerC_HandCards, HashSet<int> floorCards)
		{
			_currentTurn = Players.PlayerA;
			_playerA_HandCards = playerA_HandCards;
			_playerB_HandCards = playerB_HandCards;
			_playerC_HandCards = playerC_HandCards;
			_playerA_TakenCards = new HashSet<int>();
			_playerB_TakenCards = new HashSet<int>();
			_playerC_TakenCards = new HashSet<int>();
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
			if (_playerA_HandCards.Count != Card.InitialHandCardsPerPlayer)
			{
				isInitial = false;
			}
			if (_playerB_HandCards.Count != Card.InitialHandCardsPerPlayer)
			{
				isInitial = false;
			}
			if (_playerC_HandCards.Count != Card.InitialHandCardsPerPlayer)
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
			builder.AppendLine(Card.PrintCardSet(_playerA_HandCards));
			builder.Append("Player B: ");
			builder.AppendLine(Card.PrintCardSet(_playerB_HandCards));
			builder.Append("Player C: ");
			builder.AppendLine(Card.PrintCardSet(_playerC_HandCards));
			builder.AppendLine();

			builder.AppendLine("<Taken cards>");
			builder.Append("Player A: ");
			builder.AppendLine(Card.PrintCardSet(_playerA_TakenCards));
			builder.Append("Player B: ");
			builder.AppendLine(Card.PrintCardSet(_playerB_TakenCards));
			builder.Append("Player C: ");
			builder.AppendLine(Card.PrintCardSet(_playerC_TakenCards));
			builder.AppendLine();

			builder.AppendLine("<Floor cards>");
			builder.AppendLine(Card.PrintCardSet(_floorCards));
			builder.AppendLine();

			builder.AppendLine("<Hidden cards>");
			builder.AppendLine(Card.PrintCardSet(_hiddenCards));
			builder.AppendLine();

			builder.AppendLine("<GoCount>");
			builder.AppendLine("Player A: " + _playerA_GoCount);
			builder.AppendLine("Player B: " + _playerB_GoCount);
			builder.AppendLine("Player C: " + _playerC_GoCount);
			builder.AppendLine();

			builder.AppendLine("<Point>");
			builder.AppendLine("Player A: " + _playerA_Point);
			builder.AppendLine("Player B: " + _playerB_Point);
			builder.AppendLine("Player C: " + _playerC_Point);
			builder.AppendLine();

			builder.AppendLine("<ShakeCount>");
			builder.AppendLine("Player A: " + _playerA_ShakeCount);
			builder.AppendLine("Player B: " + _playerB_ShakeCount);
			builder.AppendLine("Player C: " + _playerC_ShakeCount);
			builder.AppendLine();

			builder.AppendLine("<FuckCount>");
			builder.AppendLine("Player A: " + _playerA_FuckCount);
			builder.AppendLine("Player B: " + _playerB_FuckCount);
			builder.AppendLine("Player C: " + _playerC_FuckCount);
			builder.AppendLine();
			return builder.ToString();
		}		

		// Among integers 0 ~ 47, randomly assigns 7, 7, 7, 6 cards to each set
		public static void ShuffleCards(HashSet<int> playerA_HandCards,
			HashSet<int> playerB_HandCards, HashSet<int> playerC_HandCards,
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
					playerA_HandCards.Add(cardSet[i]);
				}
				else if (i < Card.InitialHandCardsPerPlayer*2) 
				{
					playerB_HandCards.Add(cardSet[i]);
				}
				else if (i < Card.InitialHandCardsPerPlayer*3) 
				{
					playerC_HandCards.Add(cardSet[i]);
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
