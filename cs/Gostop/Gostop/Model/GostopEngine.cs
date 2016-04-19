using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;

namespace Gostop.Model {
	public class GostopEngine : IGostopEngine {
		public GostopEngine() {
		}
		// Null will be returned for ShowNextStep if a player's turn has not finished
		private GameStatus _gameStatus;
		// Accumulated events to show
		private List<Action> _history = new List<Action>();
		private List<Action> _choices = new List<Action>();

		private List<int> _hitCardChooseOption = null;
		private List<int> _flippedCardChooseOption = null;

		private bool _isFourCardDeclared = false;

		int _moneyToTakeFromOppnent1 = 0;
		int _moneyToTakeFromOppnent2 = 0;

		public GameStep Initialize(GameStatus initialStatus) {
			Debug.Assert(initialStatus.IsInitialStatus());
			_gameStatus = initialStatus;
			// First turn is a new turn, so fill it with next turn actions
			GetNewTurnChoices(_gameStatus, _choices);
			return new GameStep(_history, _gameStatus, _choices);
		}

		// Below are all possible cases
		// voluntary actions (which may be options for choices) are marked by <>

		// StartTurn -> <Hit | Shake | Bomb | FourCard>
		// <FourCard> -> EndGame
		// <Shake> -> <Hit | Shake | Bomb | FourCard>
		// <Hit | Bomb> -> (UnFuck) -> FlipHit -> (Fuck, UnFuck, Kiss, Cleanup) -> Steal -> 
		//					EndTurn | <Choose | GoOrStop>
		// <Choose> -> <Choose | GoOrStop> | EndTurn
		// <GoOrStop> -> EndTurn | EndGame
		// EndTurn -> EndGame | StartTurn

		public GameStep ProceedWith(int action) {
			if (action >= _choices.Count) // invalid actionId
			{
				Console.WriteLine("ServerMachine::ProceedWith(), invalid actionId");
				return null;
			}

			Action chosenAction = _choices[action];
			_history.Clear();
			_history.Add(chosenAction);
			_choices.Clear();
			bool isCardPlayFinished = false;

			switch (chosenAction.Type) {
				case ActionType.HitAction: {
						int cardId = ((HitAction)chosenAction).Card;
						isCardPlayFinished = ProcessHitAction(cardId);
						break;
					}
				case ActionType.VoidHitAction: {
						isCardPlayFinished = ProcessVoidHitAction();
						break;
					}
				case ActionType.ChooseAction: {
						var cards = ((ChooseAction)chosenAction).Card;
						isCardPlayFinished = ProcessChooseAction(cards);
						break;
					}
				case ActionType.GoOrStopAction: {
						bool isGo = ((GoOrStopAction)chosenAction).IsGo;
						ProcessGoOrStopAction(isGo);
						return new GameStep(_history, _gameStatus, _choices);
					}
				case ActionType.BombAction: {
						var cards = ((BombAction)chosenAction).Cards;
						isCardPlayFinished = ProcessBombAction(cards);
						break;
					}
				default: {
						Debug.Assert(false);
						break;
					}
			}

			// Fill choice list in the case of end of a turn 
			var player = _gameStatus.CurrentPlayer;
			if (isCardPlayFinished) {
				if (_gameStatus.GetPoint(player)[0] > _gameStatus.LastGoPoint[(int)player]) { // earned point
					if (IsGameFinished()) { // forced to Stop and win

					} else {
						_choices.Add(new GoOrStopAction(player, true));
						_choices.Add(new GoOrStopAction(player, false));
						_gameStatus.LastGoPoint[(int)player] = _gameStatus.GetPoint(player)[0];
					}
				} else { // didn't earn point
					if (IsGameFinished()) { // draw game

					} else {
						_gameStatus.ChangeTurn();
						GetNewTurnChoices(_gameStatus, _choices);
					}
				}
			} else {
				// choices should have been filled
				// Shake ->  StartTurn
				// Hit, VoidHit, Choose -> Choose
			}

			return new GameStep(_history, _gameStatus, _choices);
		}

		// Return values indicates whether this action ends the turn
		private bool ProcessHitAction(int cardId) {
			var player = _gameStatus.CurrentPlayer;
			Card hitCard = Card.GetCard(cardId);
			HashSet<int> floorCards = _gameStatus.FloorCards;
			HashSet<int> cardsToBeTaken = new HashSet<int>();
			int stealPoint = 0;

			// Hit
			_gameStatus.HandCards(player).Remove(cardId);
			floorCards.Add(cardId);

			var hitCardSet = Card.CardsWithSameMonth(floorCards, hitCard.Month);

			if (hitCardSet.Count == 2) {
				cardsToBeTaken.UnionWith(hitCardSet);
			} else if (hitCardSet.Count == 3) {
				// one more card will be choosed and taken
				cardsToBeTaken.Add(cardId);
			} else if (hitCardSet.Count == 4) {
				_history.Add(new UnFuckAction(player, cardId));
				stealPoint++;
				cardsToBeTaken.UnionWith(hitCardSet);
			}

			// Flip
			int flippedCardId = _gameStatus.HiddenCards.Last();
			Card flippedCard = Card.GetCard(flippedCardId);
			_gameStatus.HiddenCards.Remove(flippedCardId);
			floorCards.Add(flippedCardId);
			_history.Add(new FlipHitAction(player, flippedCardId));

			var flippedCardSet = Card.CardsWithSameMonth(floorCards, flippedCard.Month);

			if (hitCard.Month == flippedCard.Month) {
				if (flippedCardSet.Count == 2) {
					_history.Add(new KissAction(player, flippedCardId));
					stealPoint++;
					cardsToBeTaken.UnionWith(flippedCardSet);
				} else if (flippedCardSet.Count == 3) {
					_history.Add(new FuckAction(player, flippedCardId));
					_gameStatus.FuckCount[(int)Player.A]++;
					cardsToBeTaken.Clear();
				} else if (flippedCardSet.Count == 4) {
					_history.Add(new DadakAction(player, flippedCardId));
					stealPoint++;
					cardsToBeTaken.UnionWith(flippedCardSet);
				}
			} else // hit and flip card are different month
				{
				if (flippedCardSet.Count == 4) {
					_history.Add(new UnFuckAction(player, flippedCardId));
					stealPoint++;
					cardsToBeTaken.UnionWith(flippedCardSet);
				}
				if (hitCardSet.Count == 3) {
					// has to choose
					_hitCardChooseOption = hitCardSet.ToList();
					_hitCardChooseOption.Remove(cardId);
					cardsToBeTaken.Add(cardId);
				}
				if (flippedCardSet.Count == 3) {
					// has to choose
					_flippedCardChooseOption = flippedCardSet.ToList();
					_flippedCardChooseOption.Remove(flippedCardId);
					cardsToBeTaken.Add(flippedCardId);
				}
			}

			// Take
			if (cardsToBeTaken.Count != 0) {
				floorCards = SetSubtract(floorCards, cardsToBeTaken);
				_gameStatus.TakenCards(player).UnionWith(cardsToBeTaken);
				_history.Add(new TakeAction(player, cardsToBeTaken));
			}

			// Cleanup
			if (floorCards.Count == 0) {
				_history.Add(new CleanupAction(player));
				stealPoint++;
			}

			// Steal
			if (stealPoint > 0) {
				_history.Add(new StealAction(player, GetCardsToSteal(player, stealPoint)));
			}

			// Choose
			if (_hitCardChooseOption != null) {
				var chooseAction1 = new ChooseAction(player, _hitCardChooseOption[0]);
				var chooseAction2 = new ChooseAction(player, _hitCardChooseOption[1]);
				_choices.Add(chooseAction1);
				_choices.Add(chooseAction2);
				return false;
			} else if (_flippedCardChooseOption != null) {
				var chooseAction1 = new ChooseAction(player, _flippedCardChooseOption[0]);
				var chooseAction2 = new ChooseAction(player, _flippedCardChooseOption[1]);
				_choices.Add(chooseAction1);
				_choices.Add(chooseAction2);
				return false;
			} else {
				return true; // end turn
			}
		}

		private bool ProcessVoidHitAction() {
			// Flip
			int flippedCardId = _gameStatus.HiddenCards.Last();
			Card flippedCard = Card.GetCard(flippedCardId);
			var floorCards = _gameStatus.FloorCards;
			var player = _gameStatus.CurrentPlayer;
			int stealPoint = 0;

			_gameStatus.HiddenCards.Remove(flippedCardId);
			floorCards.Add(flippedCardId);
			_history.Add(new FlipHitAction(player, flippedCardId));

			var cardsToBeTaken = new HashSet<int>();
			var flippedCardSet = Card.CardsWithSameMonth(floorCards, flippedCard.Month);

			if (flippedCardSet.Count == 2) {
				cardsToBeTaken.UnionWith(flippedCardSet);
			} else if (flippedCardSet.Count == 3) {
				_flippedCardChooseOption = flippedCardSet.ToList();
				_flippedCardChooseOption.Remove(flippedCardId);
				cardsToBeTaken.Add(flippedCardId);
			} else if (flippedCardSet.Count == 4) {
				_history.Add(new UnFuckAction(player, flippedCardId));
				stealPoint++;
				cardsToBeTaken.UnionWith(flippedCardSet);
			}

			// Take
			if (cardsToBeTaken.Count != 0) {
				floorCards = SetSubtract(floorCards, cardsToBeTaken);
				_gameStatus.TakenCards(player).UnionWith(cardsToBeTaken);
				_history.Add(new TakeAction(player, cardsToBeTaken));
			}

			// Cleanup
			if (floorCards.Count == 0) {
				_history.Add(new CleanupAction(player));
				stealPoint++;
			}

			// Steal
			if (stealPoint > 0) {
				_history.Add(new StealAction(player, GetCardsToSteal(player, stealPoint)));
			}

			// Choose
			if (_flippedCardChooseOption != null) {
				var chooseAction1 = new ChooseAction(player, _flippedCardChooseOption[0]);
				var chooseAction2 = new ChooseAction(player, _flippedCardChooseOption[1]);
				_choices.Add(chooseAction1);
				_choices.Add(chooseAction2);
				return false;
			} else {
				return true;
			}
		}

		private bool ProcessChooseAction(int cardId) {
			Debug.Assert(_gameStatus.FloorCards.Contains(cardId));
			var player = _gameStatus.CurrentPlayer;
			_gameStatus.FloorCards.Remove(cardId);
			_gameStatus.HandCards(player).Add(cardId);

			if (_hitCardChooseOption != null) { // we just chose hit card option
				_hitCardChooseOption = null;
				if (_flippedCardChooseOption != null) { // we should choose one more time
					var chooseAction1 = new ChooseAction(player, _flippedCardChooseOption[0]);
					var chooseAction2 = new ChooseAction(player, _flippedCardChooseOption[1]);
					_choices.Add(chooseAction1);
					_choices.Add(chooseAction2);
					return false;
				} else {
					return true;
				}
			} else { // we just chose flipped card option
				Debug.Assert(_flippedCardChooseOption != null);
				_flippedCardChooseOption = null;
				return true;
			}
		}

		private void ProcessGoOrStopAction(bool isGo) {
			var player = _gameStatus.CurrentPlayer;
			_history.Add(new EndTurnAction(player));
			if (isGo) {
				_gameStatus.GoCount[(int)player]++;
				_gameStatus.ChangeTurn();
				GetNewTurnChoices(_gameStatus, _choices);
			} else {
				int[] cardPoint = _gameStatus.GetPoint(player);
				int goCount = _gameStatus.GoCount[(int)player];
				int goMultiplier = 1;
				if (goCount >= 3) {
					goMultiplier = (int)Math.Pow(2, goCount - 2);
				}
				int totalPoint = (cardPoint[0] + goCount) * goMultiplier;

				bool hasLightPoint = cardPoint[1] > 0 ? true : false;
				bool hasShellPoint = cardPoint[2] > 0 ? true : false;
				int lightMultiplier1 = 1;
				int lightMultiplier2 = 1;
				int shellMultipler1 = 1;
				int shellMultipler2 = 1;

				var opponent1 = GetOthers(player)[0];
				var opponent2 = GetOthers(player)[1];
				var cards1 = _gameStatus.TakenCards(opponent1);
				var cards2 = _gameStatus.TakenCards(opponent2);

				if (hasLightPoint && Card.GetLightsFrom(cards1).Count == 0) {
					lightMultiplier1 = 2;
				}
				if (hasLightPoint && Card.GetLightsFrom(cards2).Count == 0) {
					lightMultiplier2 = 2;
				}
				if (hasShellPoint && _gameStatus.GetPoint(opponent1)[3] <= 5) {
					shellMultipler1 = 2;
				}
				if (hasShellPoint && _gameStatus.GetPoint(opponent2)[3] <= 5) {
					shellMultipler2 = 2;
				}

				var moneyToTake1 = totalPoint * lightMultiplier1 * shellMultipler1;
				var moneyToTake2 = totalPoint * lightMultiplier2 * shellMultipler2;

				// somebody said Go before now; he has to pay all the money
				var lastGoPlayer = _gameStatus.LastGoPlayer;
				if (_gameStatus.GoCount[(int)lastGoPlayer] != 0) {		
					if (lastGoPlayer == opponent1) {
						moneyToTake1 += moneyToTake2;
						moneyToTake2 = 0;
					} else {
						moneyToTake2 += moneyToTake1;
						moneyToTake1 = 0;
					}
				}

				_history.Add(new TakeMoneyAction(player, opponent1, opponent2, moneyToTake1, moneyToTake2));
				_history.Add(new EndGameAction(player));
			}
		}

		private bool ProcessShakeAction(HashSet<int> cards) {
			Debug.Assert(cards.Count == 3);
			var month = Card.GetCard(cards.First()).Month;
			Debug.Assert(Card.CardsWithSameMonth(cards, month).Count == 3);
			var player = _gameStatus.CurrentPlayer;

			_gameStatus.ShakeCount[(int)player]++;
			_gameStatus.ShakenMonths.Add(month);
			return false;
		}

		private bool ProcessBombAction(HashSet<int> cards) {
			var month = Card.GetCard(cards.First()).Month;
			Debug.Assert(Card.CardsWithSameMonth(_gameStatus.FloorCards, month).Count == 1);
			ProcessShakeAction(cards);

			// Reduce to ProcessHitAction by laying two of three cards to floor
			// and consider one card as hit card
			int hitCard = cards.First();
			cards.Remove(hitCard);
			SetSubtract(_gameStatus.HandCards(Player.A), cards);
			_gameStatus.FloorCards.UnionWith(cards);
			return ProcessHitAction(hitCard);
		}

		private bool IsGameFinished() {
			if (_isFourCardDeclared) {
				return true;
			} else if (_gameStatus.HiddenCards.Count == 0) {
				return true;
			} else {
				return false;
			}
		}

		private HashSet<int> GetCardsToSteal(Player player, int stealPoint) {
			var cardsToBeStolen = new HashSet<int>();
			var other1 = GetOthers(player)[0];
			var other2 = GetOthers(player)[1];
			var shell1 = Card.GetShellsFrom(_gameStatus.TakenCards(other1));
			var shell2 = Card.GetShellsFrom(_gameStatus.TakenCards(other2));
			var stolen1 = GetShellsToBeStolen(shell1, stealPoint);
			var stolen2 = GetShellsToBeStolen(shell2, stealPoint);
			cardsToBeStolen.UnionWith(stolen1);
			cardsToBeStolen.UnionWith(stolen2);
			return cardsToBeStolen;
		}
		public static HashSet<int> GetShellsToBeStolen(HashSet<int> shells,
			int stealPoint) {
			// Make sure all cards are shells
			Debug.Assert(AreAllShells(shells));
			// Strategy: sorts shells into normal shells and double shells, 
			// and return first 'stealCount' cards
			HashSet<int> normalShells = new HashSet<int>();
			HashSet<int> doubleShells = new HashSet<int>();
			HashSet<int>.Enumerator intSetIter = shells.GetEnumerator();
			while (intSetIter.MoveNext()) {
				if (Card.GetCard(intSetIter.Current).CardType == CardType.NormalShell) {
					normalShells.Add(intSetIter.Current);
				} else {
					doubleShells.Add(intSetIter.Current);
				}
			}
			List<int> shellsInOrder = new List<int>();
			intSetIter = normalShells.GetEnumerator();
			while (intSetIter.MoveNext()) {
				shellsInOrder.Add(intSetIter.Current); // normalShells
			}
			intSetIter = doubleShells.GetEnumerator();
			while (intSetIter.MoveNext()) {
				shellsInOrder.Add(intSetIter.Current); // normalShells ++ doubleShells
			}
			List<int>.Enumerator intListIter = shellsInOrder.GetEnumerator();
			HashSet<int> shellToBeStolen = new HashSet<int>();
			while (intListIter.MoveNext() && stealPoint > 0) {
				shellToBeStolen.Add(intListIter.Current);
				stealPoint--;
			}
			return shellToBeStolen;
		}

		public static bool AreAllShells(HashSet<int> cardSet) {
			HashSet<int>.Enumerator i = cardSet.GetEnumerator();
			while (i.MoveNext()) {
				CardType thisType = Card.GetCard(i.Current).CardType;
				if (thisType != CardType.NormalShell && thisType != CardType.DoubleShell) {
					return false;
				}
			}
			return true;
		}


		// 2010-09-26: Should only be used when end of turn has come
		// e.g., this can not produce ChooseAction nor GoStopAction
		public static void GetNewTurnChoices(GameStatus curStatus, List<Action> choices) {
			choices.Clear();
			Player currentPlayer = curStatus.CurrentPlayer;
			HashSet<int> currentPlayerHandCards = curStatus.HandCards(currentPlayer);
			Dictionary<Month, HashSet<int>> sortedCards = Card.SortCards(currentPlayerHandCards);
			HashSet<int> floorCards = curStatus.FloorCards;
			Dictionary<Month, HashSet<int>>.Enumerator monthEnum = sortedCards.GetEnumerator();
			while (monthEnum.MoveNext()) {
				int numCards = monthEnum.Current.Value.Count;
				if (numCards < 3) // we have 1 or 2 cards in this month
				{
					HashSet<int>.Enumerator cardEnum =
						monthEnum.Current.Value.GetEnumerator();
					while (cardEnum.MoveNext()) {
						choices.Add(new HitAction(currentPlayer, cardEnum.Current));
					}
				} else if (numCards == 3) // Hit, Shake or Bomb
					{
					int cardsOnFloor =
						Card.CardsWithSameMonth(floorCards, monthEnum.Current.Key).Count;
					if (cardsOnFloor == 0) // hit or shake
					{
						HashSet<int>.Enumerator cardEnum =
							monthEnum.Current.Value.GetEnumerator();
						while (cardEnum.MoveNext()) {
							choices.Add(new HitAction(currentPlayer, cardEnum.Current));
						}
						choices.Add(new ShakeAction(currentPlayer, monthEnum.Current.Value));
					} else // Bomb
						{
						choices.Add(new BombAction(currentPlayer, monthEnum.Current.Value));
					}
				} else if (numCards == 4) // Fourcards
					{
					choices.Add(new FourCardAction(currentPlayer, monthEnum.Current.Key));
				} else {
					Console.WriteLine("ServerMachine::GetNextTurnActions error");
					return;
				}
			}
		}

		// 2010-09-28
		//  firstChoice and secondChoice is the choice of card by user when there are 
		// two cards with same month (as the card hit or flipped) on the floor
		// Normally, those values are negative to indicate there isn't room for choice
		public static void ManageFloorCards(HashSet<int> floorCards, int hitCard,
			int flippedCard, HashSet<int> cardsToBeTaken, ref int stealPoint,
			ref int fuckPoint, int firstChoice, int secondChoice) {
			stealPoint = 0;
			fuckPoint = 0;
			cardsToBeTaken.Clear();
			Dictionary<Month, HashSet<int>> sortedCards = Card.SortCards(floorCards);
			bool isSameMonth = Card.IsSameMonth(hitCard, flippedCard);
			HashSet<int> cardsWithHitCard = sortedCards[Card.GetCard(hitCard).Month];
			HashSet<int> cardsWithFlippedCard =
				sortedCards[Card.GetCard(flippedCard).Month];
			if (isSameMonth == true) // Kiss, Fuck, Dadak are possible
			{
				#region
				if (cardsWithHitCard.Count != cardsWithFlippedCard.Count ||
					cardsWithHitCard.Count > 2) {
					Console.WriteLine("ManageFloorCards error");
					return;
				}
				int numCardsOnFloor = cardsWithHitCard.Count;
				if (numCardsOnFloor == 0) // Kiss
				{
					stealPoint++;
					cardsToBeTaken.Add(hitCard);
					cardsToBeTaken.Add(flippedCard);
				} else if (numCardsOnFloor == 1) // fuck
					{
					fuckPoint++;
					floorCards.Add(hitCard);
					floorCards.Add(flippedCard);
				} else // cardsWithHitCard == 2, Dadak
					{
					stealPoint++;
					cardsToBeTaken.Add(hitCard);
					cardsToBeTaken.Add(flippedCard);
					HashSet<int>.Enumerator enumerator = cardsWithHitCard.GetEnumerator();
					while (enumerator.MoveNext()) {
						cardsToBeTaken.Add(enumerator.Current);
						floorCards.Remove(enumerator.Current);
					}
				}
				#endregion
			} else // normal situation
				{
				#region
				if (cardsWithHitCard.Count == 0) // take nothing
				{
					floorCards.Add(hitCard);
				} else if (cardsWithHitCard.Count == 1) {
					cardsToBeTaken.Add(hitCard);
					HashSet<int>.Enumerator enumerator = cardsWithHitCard.GetEnumerator();
					while (enumerator.MoveNext()) {
						cardsToBeTaken.Add(enumerator.Current);
						floorCards.Remove(enumerator.Current);
					}
				} else if (cardsWithHitCard.Count == 2) {
					if (firstChoice < 0) {
						Console.WriteLine("firstChoice value error");
						return;
					}
					cardsToBeTaken.Add(hitCard);
					cardsToBeTaken.Add(firstChoice);
					floorCards.Remove(firstChoice);
				} else if (cardsWithHitCard.Count == 3) {
					stealPoint++;
					cardsToBeTaken.Add(hitCard);
					HashSet<int>.Enumerator enumerator = cardsWithHitCard.GetEnumerator();
					while (enumerator.MoveNext()) {
						cardsToBeTaken.Add(enumerator.Current);
						floorCards.Remove(enumerator.Current);
					}
				} else {
					Console.WriteLine("ManageFloorCards error2");
					return;
				}
				#endregion
				// Same routine as above for the flipped card
				#region
				if (cardsWithFlippedCard.Count == 0) // take nothing
				{
					floorCards.Add(flippedCard);
				} else if (cardsWithFlippedCard.Count == 1) {
					cardsToBeTaken.Add(flippedCard);
					HashSet<int>.Enumerator enumerator = cardsWithFlippedCard.GetEnumerator();
					while (enumerator.MoveNext()) {
						cardsToBeTaken.Add(enumerator.Current);
						floorCards.Remove(enumerator.Current);
					}
				} else if (cardsWithFlippedCard.Count == 2) {
					if (secondChoice < 0) {
						Console.WriteLine("firstChoice value error");
						return;
					}
					cardsToBeTaken.Add(flippedCard);
					cardsToBeTaken.Add(secondChoice);
					floorCards.Remove(secondChoice);
				} else if (cardsWithFlippedCard.Count == 3) {
					stealPoint++;
					cardsToBeTaken.Add(flippedCard);
					HashSet<int>.Enumerator enumerator = cardsWithFlippedCard.GetEnumerator();
					while (enumerator.MoveNext()) {
						cardsToBeTaken.Add(enumerator.Current);
						floorCards.Remove(enumerator.Current);
					}
				} else {
					Console.WriteLine("ManageFloorCards error2");
					return;
				}
				#endregion
			}
		}

		public static HashSet<int> SetSubtract(HashSet<int> setA, HashSet<int> setB) {
			HashSet<int> remain = new HashSet<int>(setA);
			HashSet<int>.Enumerator enumerator = setB.GetEnumerator();
			while (enumerator.MoveNext()) {
				remain.Remove(enumerator.Current);
			}
			return remain;
		}

		private Player[] GetOthers(Player player) {
			if (player == Player.A) {
				return new Player[] { Player.B, Player.C };
			} else if (player == Player.B) {
				return new Player[] { Player.C, Player.A };
			} else {
				return new Player[] { Player.A, Player.B };
			}
		}
	}
}
