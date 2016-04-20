using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Gostop.Model {
	public class GameStatus {		

		HashSet<int>[] HandCards { get; set; }
		HashSet<int>[] TakenCards { get; set; }

		public Player CurrentPlayer { get; set; }
		public Player LastGoPlayer { get; set; }
		public HashSet<int> FloorCards { get; set; }
		public HashSet<int> HiddenCards { get; set; }

		public int[] GoCount { get; set; }
		public int[] LastGoPoint { get; set; }
		public int[] ShakeCount { get; set; }
		public int[] FuckCount { get; set; }

		public HashSet<Month> ShakenMonths { get; set; }


		public HashSet<int> GetHandCards(Player player) {
			return HandCards[(int)player];
		}

		public HashSet<int> GetTakenCards(Player player) {
			return TakenCards[(int)player];
		}

		// Create a random initial status
		public void Init() {
			CurrentPlayer = Player.A;
			LastGoPlayer = Player.A;
			FloorCards = new HashSet<int>();
			HiddenCards = new HashSet<int>();

			HandCards = new HashSet<int>[3];
			TakenCards = new HashSet<int>[3];
			HandCards[0] = new HashSet<int>();
			HandCards[1] = new HashSet<int>();
			HandCards[2] = new HashSet<int>();
			TakenCards[0] = new HashSet<int>();
			TakenCards[1] = new HashSet<int>();
			TakenCards[2] = new HashSet<int>();

			GoCount = new int[3];
			LastGoPoint = new int[3];
			ShakeCount = new int[3];
			FuckCount = new int[3];
			ShakenMonths = new HashSet<Month>();

			// at least 3 is needed to GoOrStop
			LastGoPoint[0] = 2;
			LastGoPoint[1] = 2;
			LastGoPoint[2] = 2;
		}

		public GameStatus() {
			Init();
			ShuffleCards(GetHandCards(Player.A), GetHandCards(Player.B), GetHandCards(Player.C),
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
			if (GetHandCards(Player.A).Count != Card.InitialHandCardsPerPlayer) {
				isInitial = false;
			}
			if (GetHandCards(Player.B).Count != Card.InitialHandCardsPerPlayer) {
				isInitial = false;
			}
			if (GetHandCards(Player.C).Count != Card.InitialHandCardsPerPlayer) {
				isInitial = false;
			}
			if (FloorCards.Count != Card.InitialFloorCardsNum) {
				isInitial = false;
			}
			if (HiddenCards.Count != Card.InitialHiddenCardsNum) {
				isInitial = false;
			}
			return isInitial;
		}

		public int[] GetPoint(Player player) {
			var takenCards = GetTakenCards(player);
			return CalculatePoint(takenCards);

		}

		// return { totalPoint, lightPoint, shellPoint, numShells }
		private int[] CalculatePoint(HashSet<int> cards) {
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
			int numShells = doubleShells.Count * 2 + normalShells.Count;

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

			if (numShells >= 10) {
				shellsPoint = numShells - 9;
			}

			int totalPoint = lightsPoint + fiveBirdsPoint + tensPoint + fivesPoint +
												redFivesPoint + blueFivesPoint + grassFivesPoint + shellsPoint;
			if (tenCritical) {
				totalPoint *= 2;
			}

			return new int[] { totalPoint, lightsPoint, shellsPoint, numShells };
		}

		public override string ToString() {
			StringBuilder builder = new StringBuilder();

			//builder.AppendLine("Current turn: " + CurrentPlayer);
			//builder.AppendLine();

			builder.AppendLine("----------------------------------------------------------------");

			builder.AppendLine("<Hand cards>");
			builder.Append("Player A: ");
			builder.AppendLine(Card.PrintCardSet(GetHandCards(Player.A)));
			builder.Append("Player B: ");
			builder.AppendLine(Card.PrintCardSet(GetHandCards(Player.B)));
			builder.Append("Player C: ");
			builder.AppendLine(Card.PrintCardSet(GetHandCards(Player.C)));
			builder.AppendLine();

			builder.AppendLine("<Taken cards>");
			builder.Append("Player A: ");
			builder.AppendLine(Card.PrintCardSet(GetTakenCards(Player.A)));
			builder.Append("Player B: ");
			builder.AppendLine(Card.PrintCardSet(GetTakenCards(Player.B)));
			builder.Append("Player C: ");
			builder.AppendLine(Card.PrintCardSet(GetTakenCards(Player.C)));
			builder.AppendLine();

			builder.AppendLine("<Floor cards>");
			builder.AppendLine(Card.PrintCardSet(FloorCards));
			builder.AppendLine();

			/*
			builder.AppendLine("<Hidden cards>");
			builder.AppendLine(Card.PrintCardSet(HiddenCards));
			builder.AppendLine();
			*/

			builder.AppendLine("<GoCount>");
			builder.Append("Player A: " + GoCount[(int)Player.A] + " ");
			builder.Append("Player B: " + GoCount[(int)Player.B] + " ");
			builder.Append("Player C: " + GoCount[(int)Player.C] + " ");
			builder.AppendLine();

			builder.AppendLine("<Point>");
			builder.Append("Player A: " + GetPoint(Player.A)[0] + " ");
			builder.Append("Player B: " + GetPoint(Player.B)[0] + " ");
			builder.Append("Player C: " + GetPoint(Player.C)[0] + " ");
			builder.AppendLine();

			builder.AppendLine("<ShakeCount>");
			builder.Append("Player A: " + ShakeCount[(int)Player.A] + " ");
			builder.Append("Player B: " + ShakeCount[(int)Player.B] + " ");
			builder.Append("Player C: " + ShakeCount[(int)Player.C] + " ");
			builder.AppendLine();

			builder.AppendLine("<FuckCount>");
			builder.Append("Player A: " + FuckCount[(int)Player.A] + " ");
			builder.Append("Player B: " + FuckCount[(int)Player.B] + " ");
			builder.Append("Player C: " + FuckCount[(int)Player.C] + " ");
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
