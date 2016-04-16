using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Gostop.Model
{
	public class ServerMachine: IServerMachine
	{
		public ServerMachine()
		{
		}
		// Null will be returned for ShowNextStep if a player's turn has not finished
		private GameStatus _currentStatus;
		// Accumulated events to show
		private List<Event> _events = new List<Event>();
		private List<Action> _choices = new List<Action>();

		public GameStep Initialize(GameStatus initialStatus)
		{
			if (!initialStatus.isInitialStatus())
			{
				Console.WriteLine("ServerMachine::Initialize(), initial status error");
			}
			_currentStatus = initialStatus;
			// First turn is a new turn, so fill it with next turn actions
			GetNewTurnActions(_currentStatus, _choices);
            return new GameStep(_events, _currentStatus, _choices);
		}

		public GameStep ProceedWith(int action)
		{
			if (action >= _choices.Count) // invalid actionId
			{
				Console.WriteLine("ServerMachine::ProceedWith(), invalid actionId");
				return null;
			}

			_events.Clear();

            bool isEndOfTurn = true;

			// Each case statement will create event, and modify game status
			Action chosenAction = _choices[action];
			switch (chosenAction.Type)
			{
				case ActionType.HitCardAction:
					{
						int cardId = ((HitAction)chosenAction).Card;
						isEndOfTurn = ProcessHitCardAction(cardId);
						break;
					}
				default:
					{
						Console.WriteLine("ServerMachine::ProceedWith(), " +
							"unhandled case in switch statement");
						return null;
					}
			}

			// Fill action list in the case of end of a turn 
			if (isEndOfTurn == true)
			{
				GetNewTurnActions(_currentStatus, _choices);
			}

            return new GameStep(_events, _currentStatus, _choices);
        }

		// Return values indicates whether this action ends the turn
		private bool ProcessHitCardAction(int cardId)
		{
			return true;
		}

		public static HashSet<int> GetShellsToBeStolen(HashSet<int> shells, 
			int stealCount)
		{
			// Make sure all cards are shells
			if (!AreAllShells(shells))
			{
				Console.WriteLine("ServerMachine::GetShellsToBeStolen(), " +
					"not all shells");
				return null;
			}
			// Strategy: sorts shells into normal shells and double shells, 
			// and return first 'stealCount' cards
			HashSet<int> normalShells = new HashSet<int>();
			HashSet<int> doubleShells = new HashSet<int>();
			HashSet<int>.Enumerator intSetIter = shells.GetEnumerator();
			while (intSetIter.MoveNext())
			{
				if (Card.GetCard(intSetIter.Current).CardType == CardType.NormalShell)
				{
					normalShells.Add(intSetIter.Current);
				}
				else
				{
					doubleShells.Add(intSetIter.Current);
				}
			}
			List<int> shellsInOrder = new List<int>(); 
			intSetIter = normalShells.GetEnumerator();
			while (intSetIter.MoveNext())
			{
				shellsInOrder.Add(intSetIter.Current); // normalShells
			}
			intSetIter = doubleShells.GetEnumerator();
			while (intSetIter.MoveNext())
			{
				shellsInOrder.Add(intSetIter.Current); // normalShells ++ doubleShells
			}
			List<int>.Enumerator intListIter = shellsInOrder.GetEnumerator();
			HashSet<int> shellToBeStolen = new HashSet<int>();
			while (intListIter.MoveNext() && stealCount > 0)
			{
				shellToBeStolen.Add(intListIter.Current);
				stealCount--;
			}			
			return shellToBeStolen;
		}

		public static bool AreAllShells(HashSet<int> cardSet)
		{
			HashSet<int>.Enumerator i = cardSet.GetEnumerator();
			while (i.MoveNext())
			{
				CardType thisType = Card.GetCard(i.Current).CardType;
				if (thisType != CardType.NormalShell && thisType != CardType.DoubleShell)
				{
					return false;
				}
			}
			return true;
		}


		// 2010-09-26: Should only be used when end of turn has come
		// e.g., this can not produce ChooseAction nor GoStopAction
		public static void GetNewTurnActions(GameStatus curStatus,
			List<Action> choices)
		{
			choices.Clear();
			Player currentPlayer = curStatus.CurrentPlayer;
			HashSet<int> currentPlayerHandCards = null;
			switch (currentPlayer)
			{
				case Player.A:
					{
						currentPlayerHandCards = curStatus.PlayerA_HandCards;
						break;
					}
				case Player.B:
					{
						currentPlayerHandCards = curStatus.PlayerB_HandCards;
						break;
					}
				case Player.C:
					{
						currentPlayerHandCards = curStatus.PlayerC_HandCards;
						break;
					}
			}
			Dictionary<Month, HashSet<int>> sortedCards =
				Card.SortCards(currentPlayerHandCards);
			HashSet<int> floorCards = curStatus.FloorCards;
			Dictionary<Month, HashSet<int>>.Enumerator monthEnum =
				sortedCards.GetEnumerator();
			while (monthEnum.MoveNext())
			{
				int numCards = monthEnum.Current.Value.Count;
				if (numCards < 3) // we have 1 or 2 cards in this month
				{
					HashSet<int>.Enumerator cardEnum = 
						monthEnum.Current.Value.GetEnumerator();
					while (cardEnum.MoveNext())
					{
						choices.Add(new HitAction(cardEnum.Current));
					}
				}
				else if (numCards == 3) // Hit, Shake or Bomb
				{
					int cardsOnFloor = 
						Card.CardsWithSameMonth(floorCards, monthEnum.Current.Key).Count;
					if (cardsOnFloor == 0) // hit or shake
					{
						HashSet<int>.Enumerator cardEnum =
							monthEnum.Current.Value.GetEnumerator();
						while (cardEnum.MoveNext())
						{
							choices.Add(new HitAction(cardEnum.Current));
						}
						choices.Add(new ShakeAction(monthEnum.Current.Value));
					}
					else // Bomb
					{
						choices.Add(new BombAction(monthEnum.Current.Value));
					}
				}
				else if (numCards == 4) // Fourcards
				{
					choices.Add(new FourCardAction(monthEnum.Current.Key));
				}
				else
				{
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
			ref int fuckPoint, int firstChoice, int secondChoice)
		{
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
					cardsWithHitCard.Count  > 2)
				{
					Console.WriteLine("ManageFloorCards error");
					return;
				}
				int numCardsOnFloor = cardsWithHitCard.Count;
				if (numCardsOnFloor == 0) // Kiss
				{
					stealPoint++;
					cardsToBeTaken.Add(hitCard);
					cardsToBeTaken.Add(flippedCard);
				}
				else if (numCardsOnFloor == 1) // fuck
				{
					fuckPoint++;
					floorCards.Add(hitCard);
					floorCards.Add(flippedCard);
				}
				else // cardsWithHitCard == 2, Dadak
				{
					stealPoint++;
					cardsToBeTaken.Add(hitCard);
					cardsToBeTaken.Add(flippedCard);
					HashSet<int>.Enumerator enumerator = cardsWithHitCard.GetEnumerator();
					while (enumerator.MoveNext())
					{
						cardsToBeTaken.Add(enumerator.Current);
						floorCards.Remove(enumerator.Current);
					}
				}
				#endregion
			}
			else // normal situation
			{
				#region
				if (cardsWithHitCard.Count == 0) // take nothing
				{
					floorCards.Add(hitCard);
				}
				else if (cardsWithHitCard.Count == 1)
				{
					cardsToBeTaken.Add(hitCard);
					HashSet<int>.Enumerator enumerator = cardsWithHitCard.GetEnumerator();
					while (enumerator.MoveNext())
					{
						cardsToBeTaken.Add(enumerator.Current);
						floorCards.Remove(enumerator.Current);
					}
				}
				else if (cardsWithHitCard.Count == 2)
				{
					if (firstChoice < 0)
					{
						Console.WriteLine("firstChoice value error");
						return;
					}
					cardsToBeTaken.Add(hitCard);
					cardsToBeTaken.Add(firstChoice);
					floorCards.Remove(firstChoice);
				}
				else if (cardsWithHitCard.Count == 3)
				{
					stealPoint++;
					cardsToBeTaken.Add(hitCard);
					HashSet<int>.Enumerator enumerator = cardsWithHitCard.GetEnumerator();
					while (enumerator.MoveNext())
					{
						cardsToBeTaken.Add(enumerator.Current);
						floorCards.Remove(enumerator.Current);
					}
				}
				else
				{
					Console.WriteLine("ManageFloorCards error2");
					return;
				}
				#endregion
				// Same routine as above for the flipped card
				#region
				if (cardsWithFlippedCard.Count == 0) // take nothing
				{
					floorCards.Add(flippedCard);
				}
				else if (cardsWithFlippedCard.Count == 1)
				{
					cardsToBeTaken.Add(flippedCard);
					HashSet<int>.Enumerator enumerator = cardsWithFlippedCard.GetEnumerator();
					while (enumerator.MoveNext())
					{
						cardsToBeTaken.Add(enumerator.Current);
						floorCards.Remove(enumerator.Current);
					}
				}
				else if (cardsWithFlippedCard.Count == 2)
				{
					if (secondChoice < 0)
					{
						Console.WriteLine("firstChoice value error");
						return;
					}
					cardsToBeTaken.Add(flippedCard);
					cardsToBeTaken.Add(secondChoice);
					floorCards.Remove(secondChoice);
				}
				else if (cardsWithFlippedCard.Count == 3)
				{
					stealPoint++;
					cardsToBeTaken.Add(flippedCard);
					HashSet<int>.Enumerator enumerator = cardsWithFlippedCard.GetEnumerator();
					while (enumerator.MoveNext())
					{
						cardsToBeTaken.Add(enumerator.Current);
						floorCards.Remove(enumerator.Current);
					}
				}
				else
				{
					Console.WriteLine("ManageFloorCards error2");
					return;
				}
				#endregion
			}
		}

		public static HashSet<int> SetSubtract(HashSet<int> setA, HashSet<int> setB)
		{
			HashSet<int> remain = new HashSet<int>(setA);
			HashSet<int>.Enumerator enumerator = setB.GetEnumerator();
			while (enumerator.MoveNext())
			{
				remain.Remove(enumerator.Current);
			}
			return remain;
		}
	}
}
