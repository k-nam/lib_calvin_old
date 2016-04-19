using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Gostop.Model {
	public class GameStatus {
		#region // Members	

		HashSet<int>[] _handCards;
		HashSet<int>[] _takenCards;
		#endregion

		#region // Getters
		public Player CurrentPlayer {
			get;
			set;
		}
		public Player LastGoPlayer {
			get;
			set;
		}
		public HashSet<int> FloorCards {
			get;
			set;
		}
		public HashSet<int> HiddenCards {
			get;
			set;
		}

		public HashSet<int> HandCards(Player player) {
			return _handCards[((int)player)];
		}

		public HashSet<int> TakenCards(Player player) {
			return _takenCards[((int)player)];
		}

		public int[] GoCount {
			get;
			set;
		}

		public int[] LastGoPoint {
			get;
			set;
		}

		public int[] ShakeCount {
			get;
			set;
		}

		public int[] FuckCount {
			get;
			set;
		}

		public HashSet<Month> ShakenMonths {
			get;
			set;
		}
		#endregion

		// Create a random initial status
		public void Init() {
			CurrentPlayer = Player.A;
			LastGoPlayer = Player.A;
			FloorCards = new HashSet<int>();
			HiddenCards = new HashSet<int>();

			_handCards = new HashSet<int>[3];
			_takenCards = new HashSet<int>[3];
			_handCards[0] = new HashSet<int>();
			_handCards[1] = new HashSet<int>();
			_handCards[2] = new HashSet<int>();
			_takenCards[0] = new HashSet<int>();
			_takenCards[1] = new HashSet<int>();
			_takenCards[2] = new HashSet<int>();

			GoCount = new int[3];
			LastGoPoint = new int[3];
			ShakeCount = new int[3];
			FuckCount = new int[3];
			ShakenMonths = new HashSet<Month>();
		}

		public GameStatus() {
			Init();
			ShuffleCards(HandCards(Player.A), HandCards(Player.B), HandCards(Player.C),
				FloorCards, HiddenCards);
		}

		public GameStatus(HashSet<int> aHandCards, HashSet<int> bHandCards,
				HashSet<int> cHandCards, HashSet<int> floorCards) {

			Init();
			// Automatically calculate hidden cards;
			for (int i = 0; i < Card.NumberOfTotalCards; i++) {
				HiddenCards.Add(i);
			}
			HashSet<int>.Enumerator enumerator = aHandCards.GetEnumerator();
			while (enumerator.MoveNext()) {
				HiddenCards.Remove(enumerator.Current);
			}
			enumerator = bHandCards.GetEnumerator();
			{
				HiddenCards.Remove(enumerator.Current);
			}
			enumerator = cHandCards.GetEnumerator();
			{
				HiddenCards.Remove(enumerator.Current);
			}
			enumerator = floorCards.GetEnumerator();
			{
				HiddenCards.Remove(enumerator.Current);
			}
		}

		public void ChangeTurn() {
			if (CurrentPlayer == Player.A) {
				CurrentPlayer = Player.B;
			} else if (CurrentPlayer == Player.B) {
				CurrentPlayer = Player.C;
			} else {
				CurrentPlayer = Player.A;
			}
		}

		public bool IsInitialStatus() {
			bool isInitial = true;
			if (HandCards(Player.A).Count != Card.InitialHandCardsPerPlayer) {
				isInitial = false;
			}
			if (HandCards(Player.B).Count != Card.InitialHandCardsPerPlayer) {
				isInitial = false;
			}
			if (HandCards(Player.C).Count != Card.InitialHandCardsPerPlayer) {
				isInitial = false;
			}
			if (FloorCards.Count != Card.InitialFloorCardsNum) {
				isInitial = false;
			}
			if (HiddenCards.Count != 0) {
				isInitial = false;
			}
			return isInitial;
		}

		public int GetPoint(Player player) {
			var takenCards = TakenCards(player);
			return CalculatePoint(takenCards);

		}

		private int CalculatePoint(HashSet<int> cards) {
			var trueLights = Card.GetCardsOfType(cards, CardType.TrueLight);
			var rainLights = Card.GetCardsOfType(cards, CardType.RainLight);
			var fiveBirdsTens = Card.GetCardsOfType(cards, CardType.FiveBirdsTen);
			var normalTens = Card.GetCardsOfType(cards, CardType.NormalTen);
			var normalFives = Card.GetCardsOfType(cards, CardType.NormalFive);
			var redFives = Card.GetCardsOfType(cards, CardType.RedFive);
			var blueFives = Card.GetCardsOfType(cards, CardType.BlueFive);
			var grassFives = Card.GetCardsOfType(cards, CardType.GrassFive);
			var doubleShells = Card.GetCardsOfType(cards, CardType.DoubleShell);
			var normalShells = Card.GetCardsOfType(cards, CardType.NormalShell);

			int lightsPoint = 0;
			int fiveBirdsPoint = 0;
			int tensPoint = 0;
			int fivesPoint = 0;
			int redFivesPoint = 0;
			int blueFivesPoint = 0;
			int grassFivesPoint = 0;
			int shellsPoint = 0;

			bool tenCritical = false;

			int numLights = trueLights.Count + rainLights.Count;
			int numTens = fiveBirdsTens.Count + normalTens.Count;
			int numFives = normalFives.Count + redFives.Count + blueFives.Count + grassFives.Count;
			int shellCount = doubleShells.Count * 2 + normalShells.Count;

			if (numLights == 3) {
				if (rainLights.Count == 1) {
					lightsPoint = 2;
				} else {
					lightsPoint = 3;
				}
			} else if (numLights == 4) {
				lightsPoint = 4;
			} else if (numLights == 5) {
				lightsPoint = 15;
			}

			if (fiveBirdsTens.Count == 3) {
				fiveBirdsPoint = 5;
			}
			if (numTens >= 5) {
				tensPoint = numTens - 4;
			}
			if (numTens >= 7) {
				tenCritical = true;
			}

			if (numFives >= 5) {
				fivesPoint = numFives - 4;
			}
			if (redFives.Count == 3) {
				redFivesPoint = 3;
			}
			if (blueFives.Count == 3) {
				blueFivesPoint = 3;
			}
			if (grassFives.Count == 3) {
				grassFivesPoint = 3;
			}

			if (shellCount >= 10) {
				shellsPoint = shellCount - 9;
			}

			int totalPoint = lightsPoint + fiveBirdsPoint + tensPoint + fivesPoint +
												redFivesPoint + blueFivesPoint + grassFivesPoint + shellsPoint;
			if (tenCritical) {
				totalPoint *= 2;
			}

			return totalPoint;
		}

		public override string ToString() {
			StringBuilder builder = new StringBuilder();

			builder.AppendLine("Current turn: " + CurrentPlayer);
			builder.AppendLine();

			builder.AppendLine("<Hand cards>");
			builder.Append("Player A: ");
			builder.AppendLine(Card.PrintCardSet(HandCards(Player.A)));
			builder.Append("Player B: ");
			builder.AppendLine(Card.PrintCardSet(HandCards(Player.B)));
			builder.Append("Player C: ");
			builder.AppendLine(Card.PrintCardSet(HandCards(Player.C)));
			builder.AppendLine();

			builder.AppendLine("<Taken cards>");
			builder.Append("Player A: ");
			builder.AppendLine(Card.PrintCardSet(TakenCards(Player.A)));
			builder.Append("Player B: ");
			builder.AppendLine(Card.PrintCardSet(TakenCards(Player.B)));
			builder.Append("Player C: ");
			builder.AppendLine(Card.PrintCardSet(TakenCards(Player.C)));
			builder.AppendLine();

			builder.AppendLine("<Floor cards>");
			builder.AppendLine(Card.PrintCardSet(FloorCards));
			builder.AppendLine();

			builder.AppendLine("<Hidden cards>");
			builder.AppendLine(Card.PrintCardSet(HiddenCards));
			builder.AppendLine();

			builder.AppendLine("<GoCount>");
			builder.AppendLine("Player A: " + GoCount[(int)Player.A]);
			builder.AppendLine("Player B: " + GoCount[(int)Player.B]);
			builder.AppendLine("Player C: " + GoCount[(int)Player.C]);
			builder.AppendLine();

			builder.AppendLine("<Point>");
			builder.AppendLine("Player A: " + GetPoint(Player.A));
			builder.AppendLine("Player B: " + GetPoint(Player.B));
			builder.AppendLine("Player C: " + GetPoint(Player.C));
			builder.AppendLine();

			builder.AppendLine("<ShakeCount>");
			builder.AppendLine("Player A: " + ShakeCount[(int)Player.A]);
			builder.AppendLine("Player B: " + ShakeCount[(int)Player.B]);
			builder.AppendLine("Player C: " + ShakeCount[(int)Player.C]);
			builder.AppendLine();

			builder.AppendLine("<FuckCount>");
			builder.AppendLine("Player A: " + FuckCount[(int)Player.A]);
			builder.AppendLine("Player B: " + FuckCount[(int)Player.B]);
			builder.AppendLine("Player C: " + FuckCount[(int)Player.C]);
			builder.AppendLine();
			return builder.ToString();
		}

		// Among integers 0 ~ 47, randomly assigns 7, 7, 7, 6 cards to each set
		public static void ShuffleCards(HashSet<int> aHandCards,
			HashSet<int> bHandCards, HashSet<int> cHandCards,
				HashSet<int> floorCards, HashSet<int> hiddenCards) {
			List<int> cardSet = new List<int>();
			for (int i = 0; i < Card.NumberOfTotalCards; i++) {
				cardSet.Add(i);
			}
			Random random = new Random();
			for (int i = 0; i < 100; i++) {
				int first = random.Next(Card.NumberOfTotalCards);
				int second = random.Next(Card.NumberOfTotalCards);
				int temp = cardSet[first];
				cardSet[first] = cardSet[second];
				cardSet[second] = temp;
				//Console.WriteLine("Changing: " + first + " with " + second);
			}
			for (int i = 0; i < Card.NumberOfTotalCards; i++) {
				if (i < Card.InitialHandCardsPerPlayer) {
					aHandCards.Add(cardSet[i]);
				} else if (i < Card.InitialHandCardsPerPlayer * 2) {
					bHandCards.Add(cardSet[i]);
				} else if (i < Card.InitialHandCardsPerPlayer * 3) {
					cHandCards.Add(cardSet[i]);
				} else if (i < Card.InitialHandCardsPerPlayer * 3 + Card.InitialFloorCardsNum) {
					floorCards.Add(cardSet[i]);
				} else {
					hiddenCards.Add(cardSet[i]);
				}
			}
		}
	}
}
