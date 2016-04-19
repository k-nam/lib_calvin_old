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

		public int[] Point {
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
			Point = new int[3];
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

	public void changeTurn() {
		if (CurrentPlayer == Player.A) {
			CurrentPlayer = Player.B;
		} else if (CurrentPlayer == Player.B) {
			CurrentPlayer = Player.C;
		} else {
			CurrentPlayer = Player.A;
		}
	}

	public bool isInitialStatus() {
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
		builder.AppendLine("Player A: " + Point[(int)Player.A]);
		builder.AppendLine("Player B: " + Point[(int)Player.B]);
		builder.AppendLine("Player C: " + Point[(int)Player.C]);
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
