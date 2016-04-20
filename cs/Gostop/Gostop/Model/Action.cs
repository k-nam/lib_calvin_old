using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Gostop.Model {
	// Represents single action which a player can take in game
	// Some actions are voluntary (result of a choice), others are inevitable
	// But from the game engine's point of view, those two need not be distinguished
	public enum ActionType {
		HitAction,
		VoidHitAction, 
		FlipHitAction, // automatic
		FuckAction, // automatic
		UnFuckAction, // automatic
		DadakAction, // automatic
		KissAction, // automatic
		TakeAction, // automatic
		CleanupAction, // automatic
		StealAction, // automatic
		ChooseAction,
		GoOrStopAction,
		ShakeAction,
		BombAction,
		FourCardAction,
		EndTurnAction, // automatic
		TakeMoneyAction, // automatic
		EndGameAction, // automatic
	}

	public class Action {
		public Player Player { get; set; }
		public ActionType Type { get; set; }
		public int PlayCard { get; set; }
		public HashSet<int> PlayCards { get; set; }
		public HashSet<int> TakeCards { get; set; }
		public HashSet<int> StealCards { get; set; }
		public bool IsGo { get; set; }

		public Player Opponent1 { get; set; }
		public Player Opponent2 { get; set; }
		public int TakeMoney1 { get; set; }
		public int TakeMoney2 { get; set; }
		public bool LightCritical1 { get; set; }
		public bool LightCritical2 { get; set; }
		public bool ShellCritical1 { get; set; }
		public bool ShellCritical2 { get; set; }

		public Month FourCardMonth { get; set; }


		public static string PrintActionType(ActionType action) {
			switch (action) {
				case ActionType.HitAction:
					return "HitAction";
				case ActionType.VoidHitAction:
					return "VoidHitAction";
				case ActionType.FlipHitAction:
					return "FlipHitAction";
				case ActionType.FuckAction:
					return "FuckAction";
				case ActionType.UnFuckAction:
					return "UnFuckAction";
				case ActionType.DadakAction:
					return "DadakAction";
				case ActionType.KissAction:
					return "KissAction";
				case ActionType.TakeAction:
					return "TakeAction";
				case ActionType.CleanupAction:
					return "CleanupAction";
				case ActionType.StealAction:
					return "StealAction";
				case ActionType.ChooseAction:
					return "ChooseCardAction";
				case ActionType.GoOrStopAction:
					return "GoStopActions";
				case ActionType.ShakeAction:
					return "ShakeAction";
				case ActionType.BombAction:
					return "BombAction";
				case ActionType.FourCardAction:
					return "FourCardAction";
				case ActionType.EndTurnAction:
					return "EndTurnAction";
				case ActionType.TakeMoneyAction:
					return "TakeMoneyAction";
				case ActionType.EndGameAction:
					return "EndGameAction";
				default:
					return "error";
			}
		}

		public override string ToString() {
			var actionType = PrintActionType(Type);
			string aug = ": ";
			switch (Type) {
				case ActionType.HitAction: {
						aug += Card.PrintCard(PlayCard);
						break;
					}
				case ActionType.VoidHitAction:
					break;				
				case ActionType.FlipHitAction: 
				case ActionType.FuckAction: 
				case ActionType.UnFuckAction: 
				case ActionType.DadakAction:
				case ActionType.KissAction: {
						aug += Card.PrintCard(PlayCard);
						break;
					}
				case ActionType.TakeAction: {
						aug += Card.PrintCardSet(TakeCards);
						break;
					}
				case ActionType.CleanupAction:
					break;
				case ActionType.StealAction: {
						aug += Card.PrintCardSet(StealCards);
						break;
					}
				case ActionType.ChooseAction: {
						aug += Card.PrintCard(PlayCard);
						break;
					}
				case ActionType.GoOrStopAction: {
						aug += IsGo;
						break;
					}
				case ActionType.ShakeAction: 
				case ActionType.BombAction: {
						aug += Card.PrintCardSet(PlayCards);
						break;
					}
				case ActionType.FourCardAction: {
						aug += FourCardMonth;
						break;
					}
				case ActionType.EndTurnAction:
					break;
				case ActionType.TakeMoneyAction: {
						aug += (":\nFrom " + Opponent1 + ": " + TakeMoney1 + " 원. 광박: " +
							LightCritical1 + " 피박: " + ShellCritical1 + "\n" +
							"From " + Opponent2 + ": " + TakeMoney2 + " 원. 광박: " +
							LightCritical2 + " 피박: " + ShellCritical2);
						break;
					}
				case ActionType.EndGameAction:
					break;
				default:
					return "error";
			}

			return this.Player+ " => " + actionType + aug;
		}

		public static string PrintActionList(List<Action> actions) {
			StringBuilder builder = new StringBuilder();
			List<Action>.Enumerator enumerator = actions.GetEnumerator();
			while (enumerator.MoveNext()) {
				builder.AppendLine(enumerator.Current.ToString());
			}
			return builder.ToString();
		}
	}

	// hit with hand card
	public class HitAction : Action {
		public HitAction(Player player, int card) {
			Player = player;
			Type = ActionType.HitAction;
			PlayCard = card;
		}
	}

	// just flip (only possible after bomb)
	public class VoidHitAction : Action {
		public VoidHitAction(Player player) {
			Player = player;
			Type = ActionType.VoidHitAction;
		}
	}

	public class FlipHitAction : Action {
		public FlipHitAction(Player player, int card) {
			Player = player;
			Type = ActionType.FlipHitAction;
			PlayCard = card;
		}
	}

	public class FuckAction : Action {
		public FuckAction(Player player, int card) {
			Player = player;
			Type = ActionType.FuckAction;
			PlayCard = card;
		}
	}

	public class UnFuckAction : Action {
		public UnFuckAction(Player player, int card) {
			Player = player;
			Type = ActionType.UnFuckAction;
			PlayCard = card;
		}
	}

	public class DadakAction : Action {
		public DadakAction(Player player, int card) {
			Player = player;
			Type = ActionType.DadakAction;
			PlayCard = card;
		}
	}

	public class KissAction : Action {
		public KissAction(Player player, int card) {
			Player = player;
			Type = ActionType.KissAction;
			PlayCard = card;
		}
	}

	public class CleanupAction : Action {
		public CleanupAction(Player player) {
			Player = player;
			Type = ActionType.CleanupAction;
		}
	}

	public class TakeAction : Action {
		public TakeAction(Player player, HashSet<int> cards) {
			Player = player;
			Type = ActionType.TakeAction;
			TakeCards = cards;
		}
	}

	public class StealAction : Action {
		public StealAction(Player player, HashSet<int> cards) {
			Player = player;
			Type = ActionType.StealAction;
			StealCards = cards;
		}
	}

	public class ChooseAction : Action {
		public ChooseAction(Player player, int card) {
			Player = player;
			Type = ActionType.ChooseAction;
			PlayCard = card;
		}
	}

	public class GoOrStopAction : Action {
		public GoOrStopAction(Player player, bool isGo) {
			Player = player;
			Type = ActionType.GoOrStopAction;
			IsGo = isGo;
		}
	}

	public class ShakeAction : Action {
		public ShakeAction(Player player, HashSet<int> cards) {
			Player = player;
			Type = ActionType.ShakeAction;
			PlayCards = cards;
		}
	}

	public class BombAction : Action {
		public BombAction(Player player, HashSet<int> cards) {
			Player = player;
			Type = ActionType.BombAction;
			PlayCards = cards;
		}
	}

	public class FourCardAction : Action {
		public FourCardAction(Player player, Month month) {
			Player = player;
			Type = ActionType.FourCardAction;
			FourCardMonth = month;
		}
	}

	public class EndTurnAction : Action {
		public EndTurnAction(Player player) {
			Player = player;
			Type = ActionType.EndTurnAction;
		}
	}

	public class TakeMoneyAction : Action {
		public TakeMoneyAction(Player player, Player opponent1, Player opponent2,
														int money1, int money2,
														bool lightCritical1, bool lightCritical2,
														bool shellCritical1, bool shellCritical2) {
			Player = player;
			Type = ActionType.TakeMoneyAction;
			Opponent1 = opponent1;
			Opponent2 = opponent2;
			TakeMoney1 = money1;
			TakeMoney2 = money2;
			LightCritical1 = lightCritical1;
			LightCritical2 = lightCritical2;
			ShellCritical1 = shellCritical1;
			ShellCritical2 = shellCritical2;
		}
	}

	public class EndGameAction : Action {
		public EndGameAction(Player player) {
			Player = player;
			Type = ActionType.EndGameAction;
		}
	}
}


